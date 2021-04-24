//
//  BRBSVParams.h
//  WalletKitCore
//
//  Created by Ehsan Rezaie on 2020-06-04.
//  Copyright © 2019 Breadwinner AG. All rights reserved.
//
//  See the LICENSE file at the project root for license information.
//  See the CONTRIBUTORS file at the project root for a list of contributors.

#ifndef BRBSVParams_h
#define BRBSVParams_h

#include "bitcoin/BRBitcoinChainParams.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define BSV_FORKID 0x40

extern const BRBitcoinChainParams *bsvMainNetParams;
extern const BRBitcoinChainParams *bsvTestNetParams;

static inline int btcChainParamsIsBSV (const BRBitcoinChainParams *params) {
    return bsvMainNetParams == params || bsvTestNetParams == params;
}

#ifdef __cplusplus
}
#endif

#endif // BRSVParams_h
