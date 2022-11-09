// Copyright (c) 2020-2021 The PIVX developers
// Copyright (c) 2022 The ADON developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.
#include "legacy/validation_zerocoin_legacy.h"

#include "libzerocoin/CoinSpend.h"
#include "wallet/wallet.h"
#include "zadon/zadonmodule.h"

bool DisconnectZerocoinTx(const CTransaction& tx)
{
    /** UNDO ZEROCOIN DATABASING
         * note we only undo zerocoin databasing in the following statement, value to and from ADON
         * addresses should still be handled by the typical bitcoin based undo code
         * */
    if (tx.ContainsZerocoins()) {
        libzerocoin::ZerocoinParams *params = Params().GetConsensus().Zerocoin_Params(false);
        if (tx.HasZerocoinSpendInputs()) {
            //erase all zerocoinspends in this transaction
            for (const CTxIn &txin : tx.vin) {
                bool isPublicSpend = txin.IsZerocoinPublicSpend();
                if (txin.scriptSig.IsZerocoinSpend() || isPublicSpend) {
                    CBigNum serial;
                    if (isPublicSpend) {
                        PublicCoinSpend publicSpend(params);
                        CValidationState state;
                        if (!ZADONModule::ParseZerocoinPublicSpend(txin, tx, state, publicSpend)) {
                            return error("Failed to parse public spend");
                        }
                        serial = publicSpend.getCoinSerialNumber();
                    } else {
                        libzerocoin::CoinSpend spend = ZADONModule::TxInToZerocoinSpend(txin);
                        serial = spend.getCoinSerialNumber();
                    }

                    if (!zerocoinDB->EraseCoinSpend(serial))
                        return error("failed to erase spent zerocoin in block");
                }

            }
        }
    }
    return true;
}
