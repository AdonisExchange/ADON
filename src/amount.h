// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin developers
// Copyright (c) 2017-2021 The PIVX developers
// Copyright (c) 2022 The ADON developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ADON_AMOUNT_H
#define ADON_AMOUNT_H

#include <stdint.h>

/** Amount in ADON (Can be negative) */
typedef int64_t CAmount;

static const CAmount COIN = 100000000;
static const CAmount CENT = 1000000;

#endif //  ADON_AMOUNT_H
