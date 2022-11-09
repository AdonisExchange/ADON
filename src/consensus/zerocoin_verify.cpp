// Copyright (c) 2020-2021 The PIVX developers
// Copyright (c) 2022 The ADON developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "zerocoin_verify.h"

#include "chainparams.h"
#include "consensus/consensus.h"
#include "invalid.h"
#include "script/interpreter.h"
#include "txdb.h" // for zerocoinDb
#include "utilmoneystr.h"        // for FormatMoney
#include "../validation.h"
#include "zadon/zadonmodule.h"


static bool CheckZerocoinSpend(const CTransactionRef _tx, CValidationState& state)
{
    const CTransaction& tx = *_tx;
    //max needed non-mint outputs should be 2 - one for redemption address and a possible 2nd for change
    if (tx.vout.size() > 2) {
        int outs = 0;
        for (const CTxOut& out : tx.vout) {
            if (out.IsZerocoinMint())
                continue;
            outs++;
        }
        if (outs > 2 && !tx.IsCoinStake())
            return state.DoS(100, error("%s: over two non-mint outputs in a zerocoinspend transaction", __func__));
    }

    //compute the txout hash that is used for the zerocoinspend signatures
    CMutableTransaction txTemp;
    for (const CTxOut& out : tx.vout) {
        txTemp.vout.push_back(out);
    }
    uint256 hashTxOut = txTemp.GetHash();

    bool fValidated = false;
    const Consensus::Params& consensus = Params().GetConsensus();
    std::set<CBigNum> serials;
    CAmount nTotalRedeemed = 0;
    for (const CTxIn& txin : tx.vin) {

        //only check txin that is a zcspend
        bool isPublicSpend = txin.IsZerocoinPublicSpend();
        if (!txin.IsZerocoinSpend() && !isPublicSpend)
            continue;

        libzerocoin::CoinSpend newSpend;
        CTxOut prevOut;
        if (isPublicSpend) {
            if(!GetOutput(txin.prevout.hash, txin.prevout.n, state, prevOut)){
                return state.DoS(100, error("%s: public zerocoin spend prev output not found, prevTx %s, index %d", __func__, txin.prevout.hash.GetHex(), txin.prevout.n));
            }
            libzerocoin::ZerocoinParams* params = consensus.Zerocoin_Params(false);
            PublicCoinSpend publicSpend(params);
            if (!ZADONModule::parseCoinSpend(txin, tx, prevOut, publicSpend)){
                return state.DoS(100, error("%s: public zerocoin spend parse failed", __func__));
            }
            newSpend = publicSpend;
        } else {
            newSpend = ZADONModule::TxInToZerocoinSpend(txin);
        }

        //check that the denomination is valid
        if (newSpend.getDenomination() == libzerocoin::ZQ_ERROR)
            return state.DoS(100, error("%s: Zerocoinspend does not have the correct denomination", __func__));

        //check that denomination is what it claims to be in nSequence
        if (newSpend.getDenomination() != txin.nSequence)
            return state.DoS(100, error("%s: Zerocoinspend nSequence denomination does not match CoinSpend", __func__));

        //make sure the txout has not changed
        if (newSpend.getTxOutHash() != hashTxOut)
            return state.DoS(100, error("%s: Zerocoinspend does not use the same txout that was used in the SoK", __func__));

        if (isPublicSpend) {
            libzerocoin::ZerocoinParams* params = consensus.Zerocoin_Params(false);
            PublicCoinSpend ret(params);
            if (!ZADONModule::validateInput(txin, prevOut, tx, ret)){
                return state.DoS(100, error("%s: public zerocoin spend did not verify", __func__));
            }
        }

        if (serials.count(newSpend.getCoinSerialNumber()))
            return state.DoS(100, error("%s: Zerocoinspend serial is used twice in the same tx", __func__));
        serials.insert(newSpend.getCoinSerialNumber());

        //make sure that there is no over redemption of coins
        nTotalRedeemed += libzerocoin::ZerocoinDenominationToAmount(newSpend.getDenomination());
        fValidated = true;
    }

    if (!tx.IsCoinStake() && nTotalRedeemed < tx.GetValueOut()) {
        LogPrintf("%s: redeemed = %s , spend = %s \n", __func__, FormatMoney(nTotalRedeemed), FormatMoney(tx.GetValueOut()));
        return state.DoS(100, error("%s: Transaction spend more than was redeemed in zerocoins", __func__));
    }

    return fValidated;
}

bool CheckPublicCoinSpendEnforced(int blockHeight, bool isPublicSpend)
{
    return error("%s: failed to add block, public spend enforcement not activated", __func__);
}

bool ContextualCheckZerocoinTx(const CTransactionRef& tx, CValidationState& state, const Consensus::Params& consensus, int nHeight, bool isMined)
{
    // zerocoin enforced via block time. First block with a zc mint is 863735
    const bool fZerocoinEnforced = false;
    const bool fPublicSpendEnforced = false;
    const bool fRejectMintsAndPrivateSpends = !isMined || !fZerocoinEnforced || fPublicSpendEnforced;
    const bool fRejectPublicSpends = !isMined || !fPublicSpendEnforced || consensus.NetworkUpgradeActive(nHeight, Consensus::UPGRADE_V5_0);

    const bool hasPrivateSpendInputs = !tx->vin.empty() && tx->vin[0].IsZerocoinSpend();
    const bool hasPublicSpendInputs = !tx->vin.empty() && tx->vin[0].IsZerocoinPublicSpend();
    const std::string& txId = tx->GetHash().ToString();

    int nSpendCount{0};
    for (const CTxIn& in : tx->vin) {
        if (in.IsZerocoinSpend()) {
            if (fRejectMintsAndPrivateSpends)
                return state.DoS(100, error("%s: zerocoin spend tx %s not accepted at height %d",
                                            __func__, txId, nHeight), REJECT_INVALID, "bad-txns-zc-private-spend");
            if (!hasPrivateSpendInputs)
                return state.DoS(100, error("%s: zerocoin spend tx %s has mixed spend inputs",
                                            __func__, txId), REJECT_INVALID, "bad-txns-zc-private-spend-mixed-types");

        } else if (in.IsZerocoinPublicSpend()) {
            if (fRejectPublicSpends)
                return state.DoS(100, error("%s: zerocoin public spend tx %s not accepted at height %d",
                                            __func__, txId, nHeight), REJECT_INVALID, "bad-txns-zc-public-spend");
            if (!hasPublicSpendInputs)
                return state.DoS(100, error("%s: zerocoin spend tx %s has mixed spend inputs",
                                            __func__, txId), REJECT_INVALID, "bad-txns-zc-public-spend-mixed-types");

        } else {
            // this is a transparent input
            if (hasPrivateSpendInputs || hasPublicSpendInputs)
                return state.DoS(100, error("%s: zerocoin spend tx %s has mixed spend inputs",
                                            __func__, txId), REJECT_INVALID, "bad-txns-zc-spend-mixed-types");
        }
    }

    if (hasPrivateSpendInputs || hasPublicSpendInputs) {
        if (!CheckZerocoinSpend(tx, state))
            return false;   // failure reason logged in validation state
    }

    for (const CTxOut& o : tx->vout) {
        if (o.IsZerocoinMint() && fRejectMintsAndPrivateSpends) {
            return state.DoS(100, error("%s: zerocoin mint tx %s not accepted at height %d",
                                        __func__, txId, nHeight), REJECT_INVALID, "bad-txns-zc-mint");
        }
    }

    return true;
}

bool IsSerialInBlockchain(const CBigNum& bnSerial, int& nHeightTx)
{
    uint256 txHash;
    // if not in zerocoinDB then its not in the blockchain
    if (!zerocoinDB->ReadCoinSpend(bnSerial, txHash))
        return false;

    // Now get the chain tx
    CTransactionRef tx;
    uint256 hashBlock;
    if (!GetTransaction(txHash, tx, hashBlock, true))
        return false;

    if (hashBlock.IsNull() || !mapBlockIndex.count(hashBlock)) {
        return false;
    }

    CBlockIndex* pindex = mapBlockIndex[hashBlock];
    if (!chainActive.Contains(pindex)) {
        return false;
    }

    nHeightTx = pindex->nHeight;
    return true;
}

bool ContextualCheckZerocoinSpend(const CTransaction& tx, const libzerocoin::CoinSpend* spend, int nHeight)
{
    return false;
}

bool ParseAndValidateZerocoinSpends(const Consensus::Params& consensus,
                                    const CTransaction& tx, int chainHeight,
                                    CValidationState& state,
                                    std::vector<std::pair<CBigNum, uint256>>& vSpendsRet)
{
    for (const CTxIn& txIn : tx.vin) {
        bool isPublicSpend = txIn.IsZerocoinPublicSpend();
        bool isPrivZerocoinSpend = txIn.IsZerocoinSpend();
        if (!isPrivZerocoinSpend && !isPublicSpend)
            continue;

        // Check enforcement
        if (!CheckPublicCoinSpendEnforced(chainHeight, isPublicSpend)) {
            return false;
        }

        if (isPublicSpend) {
            libzerocoin::ZerocoinParams* params = consensus.Zerocoin_Params(false);
            PublicCoinSpend publicSpend(params);
            if (!ZADONModule::ParseZerocoinPublicSpend(txIn, tx, state, publicSpend)) {
                return false;
            }
            //queue for db write after the 'justcheck' section has concluded
            if (!ContextualCheckZerocoinSpend(tx, &publicSpend, chainHeight)) {
                state.DoS(100, error("%s: failed to add block %s with invalid public zc spend", __func__,
                                     tx.GetHash().GetHex()), REJECT_INVALID);
                return false;
            }
            vSpendsRet.emplace_back(publicSpend.getCoinSerialNumber(), tx.GetHash());
        } else {
            libzerocoin::CoinSpend spend = ZADONModule::TxInToZerocoinSpend(txIn);
            //queue for db write after the 'justcheck' section has concluded
            if (!ContextualCheckZerocoinSpend(tx, &spend, chainHeight)) {
                return state.DoS(100, error("%s: failed to add block %s with invalid zerocoinspend", __func__,
                                     tx.GetHash().GetHex()), REJECT_INVALID);
            }
            vSpendsRet.emplace_back(spend.getCoinSerialNumber(), tx.GetHash());
        }
    }
    return !vSpendsRet.empty();
}
