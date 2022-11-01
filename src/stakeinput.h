// Copyright (c) 2017-2021 The PIVX developers
// Copyright (c) 2022 The ADON developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ADON_STAKEINPUT_H
#define ADON_STAKEINPUT_H

#include "chain.h"
#include "streams.h"
#include "uint256.h"

class CKeyStore;
class CWallet;
class CWalletTx;

class CStakeInput
{
protected:
    const CBlockIndex* pindexFrom = nullptr;

public:
    CStakeInput(const CBlockIndex* _pindexFrom) : pindexFrom(_pindexFrom) {}
    virtual ~CStakeInput(){};
    virtual const CBlockIndex* GetIndexFrom() const = 0;
    virtual bool GetTxOutFrom(CTxOut& out) const = 0;
    virtual CAmount GetValue() const = 0;
    virtual bool IsZADON() const = 0;
    virtual CDataStream GetUniqueness() const = 0;
};


class CAdonStake : public CStakeInput
{
private:
    const CTxOut outputFrom;
    const COutPoint outpointFrom;

public:
    CAdonStake(const CTxOut& _from, const COutPoint& _outPointFrom, const CBlockIndex* _pindexFrom) :
            CStakeInput(_pindexFrom), outputFrom(_from), outpointFrom(_outPointFrom) {}

    static CAdonStake* NewAdonStake(const CTxIn& txin, int nHeight, uint32_t nTime);

    const CBlockIndex* GetIndexFrom() const override;
    bool GetTxOutFrom(CTxOut& out) const override;
    CAmount GetValue() const override;
    CDataStream GetUniqueness() const override;
    CTxIn GetTxIn() const;
    bool IsZADON() const override { return false; }
};


#endif //ADON_STAKEINPUT_H
