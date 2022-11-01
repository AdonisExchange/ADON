// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Copyright (c) 2021-2021 The PIVX developers
// Copyright (c) 2022 The ADON developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ADON_UTIL_VALIDATION_H
#define ADON_UTIL_VALIDATION_H

#include <string>

class CValidationState;

/** Convert CValidationState to a human-readable message for logging */
std::string FormatStateMessage(const CValidationState& state);

extern const std::string strMessageMagic;

#endif // ADON_UTIL_VALIDATION_H
