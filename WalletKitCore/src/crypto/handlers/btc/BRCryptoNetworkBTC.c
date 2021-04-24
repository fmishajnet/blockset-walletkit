//
//  BRCryptoNetworkBTC.c
//  WalletKitCore
//
//  Created by Ed Gamble on 05/07/2020.
//  Copyright © 2019 Breadwinner AG. All rights reserved.
//
//  See the LICENSE file at the project root for license information.
//  See the CONTRIBUTORS file at the project root for a list of contributors.
//
#include "BRCryptoBTC.h"
#include "bitcoin/BRBitcoinChainParams.h"
#include "bcash/BRBCashParams.h"
#include "bsv/BRBSVParams.h"
#include "crypto/BRCryptoHashP.h"

static BRCryptoNetworkBTC
cryptoNetworkCoerce (BRCryptoNetwork network, BRCryptoNetworkType type) {
    assert (type == network->type);
    return (BRCryptoNetworkBTC) network;
}

static BRCryptoNetworkBTC
cryptoNetworkCoerceANY (BRCryptoNetwork network) {
    assert (CRYPTO_NETWORK_TYPE_BTC == network->type ||
            CRYPTO_NETWORK_TYPE_BCH == network->type ||
            CRYPTO_NETWORK_TYPE_BSV == network->type);
    return (BRCryptoNetworkBTC) network;
}

extern const BRBitcoinChainParams *
cryptoNetworkAsBTC (BRCryptoNetwork network) {
    return cryptoNetworkCoerce(network, network->type)->params;
}

typedef struct {
    const BRBitcoinChainParams *params;
} BRCryptoNetworkCreateContextBTC;

static void
cryptoNetworkCreateCallbackBTC (BRCryptoNetworkCreateContext context,
                                BRCryptoNetwork network) {
    BRCryptoNetworkCreateContextBTC *contextBTC = (BRCryptoNetworkCreateContextBTC*) context;
    BRCryptoNetworkBTC networkBTC = cryptoNetworkCoerceANY (network);

    networkBTC->params = contextBTC->params;
}

static BRCryptoNetwork
cryptoNetworkCreateBTC (BRCryptoNetworkListener listener,
                        const char *uids,
                        const char *name,
                        const char *desc,
                        bool isMainnet,
                        uint32_t confirmationPeriodInSeconds,
                        BRCryptoAddressScheme defaultAddressScheme,
                        BRCryptoSyncMode defaultSyncMode,
                        BRCryptoCurrency nativeCurrency) {
    assert (0 == strcmp (desc, (isMainnet ? "mainnet" : "testnet")));

    BRCryptoNetworkCreateContextBTC contextBTC = {
        (isMainnet ? btcMainNetParams : btcTestNetParams)
    };

    return cryptoNetworkAllocAndInit (sizeof (struct BRCryptoNetworkBTCRecord),
                                      CRYPTO_NETWORK_TYPE_BTC,
                                      listener,
                                      uids,
                                      name,
                                      desc,
                                      isMainnet,
                                      confirmationPeriodInSeconds,
                                      defaultAddressScheme,
                                      defaultSyncMode,
                                      nativeCurrency,
                                      &contextBTC,
                                      cryptoNetworkCreateCallbackBTC);
}

static BRCryptoNetwork
cryptoNetworkCreateBCH (BRCryptoNetworkListener listener,
                        const char *uids,
                        const char *name,
                        const char *desc,
                        bool isMainnet,
                        uint32_t confirmationPeriodInSeconds,
                        BRCryptoAddressScheme defaultAddressScheme,
                        BRCryptoSyncMode defaultSyncMode,
                        BRCryptoCurrency nativeCurrency) {
    assert (0 == strcmp (desc, (isMainnet ? "mainnet" : "testnet")));

    BRCryptoNetworkCreateContextBTC contextBTC = {
        (isMainnet ? bchMainNetParams : bchTestNetParams)
    };

    return cryptoNetworkAllocAndInit (sizeof (struct BRCryptoNetworkBTCRecord),
                                      CRYPTO_NETWORK_TYPE_BCH,
                                      listener,
                                      uids,
                                      name,
                                      desc,
                                      isMainnet,
                                      confirmationPeriodInSeconds,
                                      defaultAddressScheme,
                                      defaultSyncMode,
                                      nativeCurrency,
                                      &contextBTC,
                                      cryptoNetworkCreateCallbackBTC);
}

static BRCryptoNetwork
cryptoNetworkCreateBSV (BRCryptoNetworkListener listener,
                        const char *uids,
                        const char *name,
                        const char *desc,
                        bool isMainnet,
                        uint32_t confirmationPeriodInSeconds,
                        BRCryptoAddressScheme defaultAddressScheme,
                        BRCryptoSyncMode defaultSyncMode,
                        BRCryptoCurrency nativeCurrency) {
    assert (0 == strcmp (desc, (isMainnet ? "mainnet" : "testnet")));

    BRCryptoNetworkCreateContextBTC contextBTC = {
        (isMainnet ? bsvMainNetParams : bsvTestNetParams)
    };

    return cryptoNetworkAllocAndInit (sizeof (struct BRCryptoNetworkBTCRecord),
                                      CRYPTO_NETWORK_TYPE_BSV,
                                      listener,
                                      uids,
                                      name,
                                      desc,
                                      isMainnet,
                                      confirmationPeriodInSeconds,
                                      defaultAddressScheme,
                                      defaultSyncMode,
                                      nativeCurrency,
                                      &contextBTC,
                                      cryptoNetworkCreateCallbackBTC);
}

static void
cryptoNetworkReleaseBTC (BRCryptoNetwork network) {
    BRCryptoNetworkBTC networkBTC = cryptoNetworkCoerceANY (network);
    (void) networkBTC;
}

static BRCryptoAddress
cryptoNetworkCreateAddressBTC (BRCryptoNetwork network,
                                const char *addressAsString) {
    BRCryptoNetworkBTC networkBTC = cryptoNetworkCoerce (network, CRYPTO_NETWORK_TYPE_BTC);
    assert (btcChainParamsIsBitcoin (networkBTC->params));
    return cryptoAddressCreateFromStringAsBTC (networkBTC->params->addrParams, addressAsString);
}

static BRCryptoAddress
cryptoNetworkCreateAddressBCH (BRCryptoNetwork network,
                                const char *addressAsString) {
    BRCryptoNetworkBTC networkBTC = cryptoNetworkCoerce (network, CRYPTO_NETWORK_TYPE_BCH);
    assert (!btcChainParamsIsBitcoin (networkBTC->params));
    return cryptoAddressCreateFromStringAsBCH (networkBTC->params->addrParams, addressAsString);
}

static BRCryptoAddress
cryptoNetworkCreateAddressBSV (BRCryptoNetwork network,
                                const char *addressAsString) {
    BRCryptoNetworkBTC networkBTC = cryptoNetworkCoerce (network, CRYPTO_NETWORK_TYPE_BSV);
    assert (btcChainParamsIsBSV (networkBTC->params));
    return cryptoAddressCreateFromStringAsBSV (networkBTC->params->addrParams, addressAsString);
}

static BRCryptoBlockNumber
cryptoNetworkGetBlockNumberAtOrBeforeTimestampBTC (BRCryptoNetwork network,
                                                   BRCryptoTimestamp timestamp) {
    BRCryptoNetworkBTC networkBTC = cryptoNetworkCoerce (network, network->type);
    const BRBitcoinCheckPoint *checkpoint = btcChainParamsGetCheckpointBefore (networkBTC->params, (uint32_t) timestamp);
    return (NULL == checkpoint ? 0 : checkpoint->height);
}

static BRCryptoBoolean
cryptoNetworkIsAccountInitializedBTC (BRCryptoNetwork network,
                                      BRCryptoAccount account) {
    BRCryptoNetworkBTC networkBTC = cryptoNetworkCoerceANY (network);
    (void) networkBTC;

    return CRYPTO_TRUE;
}


static uint8_t *
cryptoNetworkGetAccountInitializationDataBTC (BRCryptoNetwork network,
                                              BRCryptoAccount account,
                                              size_t *bytesCount) {
    BRCryptoNetworkBTC networkBTC = cryptoNetworkCoerceANY (network);
    (void) networkBTC;

    return NULL;
}

static void
cryptoNetworkInitializeAccountBTC (BRCryptoNetwork network,
                                   BRCryptoAccount account,
                                   const uint8_t *bytes,
                                   size_t bytesCount) {
    BRCryptoNetworkBTC networkBTC = cryptoNetworkCoerceANY (network);
    (void) networkBTC;
    
    return;
}

static BRCryptoHash
cryptoNetworkCreateHashFromStringBTC (BRCryptoNetwork network,
                                      const char *string) {
    assert(64 == strlen (string));
    UInt256 hash = uint256(string);
    return cryptoHashCreateAsBTC (hash);
}

static char *
cryptoNetworkEncodeHashBTC (BRCryptoHash hash) {
    return cryptoHashStringAsHex (hash, false);
}

// MARK: - Network Fee

extern uint64_t
cryptoNetworkFeeAsBTC (BRCryptoNetworkFee networkFee) {
    BRCryptoBoolean overflow;
    uint64_t value = cryptoAmountGetIntegerRaw (networkFee->pricePerCostFactor, &overflow);
    assert (CRYPTO_FALSE == overflow);
    return value;
}

BRCryptoNetworkHandlers cryptoNetworkHandlersBTC = {
    cryptoNetworkCreateBTC,
    cryptoNetworkReleaseBTC,
    cryptoNetworkCreateAddressBTC,
    cryptoNetworkGetBlockNumberAtOrBeforeTimestampBTC,
    cryptoNetworkIsAccountInitializedBTC,
    cryptoNetworkGetAccountInitializationDataBTC,
    cryptoNetworkInitializeAccountBTC,
    cryptoNetworkCreateHashFromStringBTC,
    cryptoNetworkEncodeHashBTC
};

BRCryptoNetworkHandlers cryptoNetworkHandlersBCH = {
    cryptoNetworkCreateBCH,
    cryptoNetworkReleaseBTC,
    cryptoNetworkCreateAddressBCH,
    cryptoNetworkGetBlockNumberAtOrBeforeTimestampBTC,
    cryptoNetworkIsAccountInitializedBTC,
    cryptoNetworkGetAccountInitializationDataBTC,
    cryptoNetworkInitializeAccountBTC,
    cryptoNetworkCreateHashFromStringBTC,
    cryptoNetworkEncodeHashBTC
};

BRCryptoNetworkHandlers cryptoNetworkHandlersBSV = {
    cryptoNetworkCreateBSV,
    cryptoNetworkReleaseBTC,
    cryptoNetworkCreateAddressBSV,
    cryptoNetworkGetBlockNumberAtOrBeforeTimestampBTC,
    cryptoNetworkIsAccountInitializedBTC,
    cryptoNetworkGetAccountInitializationDataBTC,
    cryptoNetworkInitializeAccountBTC,
    cryptoNetworkCreateHashFromStringBTC,
    cryptoNetworkEncodeHashBTC
};


//static BRCryptoNetwork
//cryptoNetworkCreateAsBTC (const char *uids,
//                          const char *name,
//                          const BRBitcoinChainParams *params) {
//    BRCryptoNetwork network = cryptoNetworkCreate (uids, name, CRYPTO_NETWORK_TYPE_BTC);
//    network->type = BLOCK_CHAIN_TYPE_BTC;
//    network->u.btc = params;
//
//    return network;
//}
//
//static BRCryptoNetwork
//cryptoNetworkCreateAsBCH (const char *uids,
//                          const char *name,
//                          const BRBitcoinChainParams *params) {
//    BRCryptoNetwork network = cryptoNetworkCreate (uids, name, CRYPTO_NETWORK_TYPE_BCH);
//    network->type = BLOCK_CHAIN_TYPE_BTC;
//    network->u.btc = params;
//
//    return network;
//}
//
//static BRCryptoNetwork
//cryptoNetworkCreateAsETH (const char *uids,
//                          const char *name,
//                          BREthereumNetwork net) {
//    BRCryptoNetwork network = cryptoNetworkCreate (uids, name, CRYPTO_NETWORK_TYPE_ETH);
//    network->type = BLOCK_CHAIN_TYPE_ETH;
//    network->u.eth = net;
//
//    return network;
//}
//
//static BRCryptoNetwork
//cryptoNetworkCreateAsGEN (const char *uids,
//                          const char *name,
//                          uint8_t isMainnet,
//                          BRCryptoNetworkCanonicalType canonicalType) {
//    BRCryptoNetwork network = cryptoNetworkCreate (uids, name, canonicalType);
//    network->type = BLOCK_CHAIN_TYPE_GEN;
//    network->u.gen = genNetworkCreate(canonicalType, isMainnet);
//    return network;
//}

