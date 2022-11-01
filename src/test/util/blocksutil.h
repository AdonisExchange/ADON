// Copyright (c) 2021-2021 The PIVX developers
// Copyright (c) 2022 The ADON developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.

#ifndef ADON_BLOCKSUTIL_H
#define ADON_BLOCKSUTIL_H

#include "primitives/block.h"
#include <memory>

// Process block and boost_check for the specific rejection reason.
void ProcessBlockAndCheckRejectionReason(std::shared_ptr<CBlock>& pblock,
                                         const std::string& blockRejectionReason,
                                         int expectedChainHeight);

CBlock getBlock13b8a();

#endif //ADON_BLOCKSUTIL_H
