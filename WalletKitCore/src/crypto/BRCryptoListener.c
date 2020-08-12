//
//  BRCryptoListener.c
//  BRCrypto
//
//  Created by Ed Gamble on 8/11/20
//  Copyright © 2020 breadwallet. All rights reserved.
//
//  See the LICENSE file at the project root for license information.
//  See the CONTRIBUTORS file at the project root for a list of contributors.

#include "BRCryptoListenerP.h"
#include "support/BROSCompat.h"

IMPLEMENT_CRYPTO_GIVE_TAKE (BRCryptoListener, cryptoListener)

#define cryptoWalletManagerTake(x)   (x)
#define cryptoWalletTake(x)    (x)
#define cryptoTransferTake(x)  (x)
#define cryptoNetworkTake(x)   (x)
#define cryptoSystemTake(x)    (x)

// MARK: - Generate Transfer Event

typedef struct {
    BREvent base;
    BRCryptoListener listener;
    BRCryptoWalletManager manager;
    BRCryptoWallet wallet;
    BRCryptoTransfer transfer;
    BRCryptoTransferEvent event;
} BRListenerSignalTransferEvent;

static void
cryptoListenerSignalTransferEventDispatcher (BREventHandler ignore,
                                             BRListenerSignalTransferEvent *event) {
    event->listener->transferCallback (event->listener->context,
                                       event->manager,
                                       event->wallet,
                                       event->transfer,
                                       event->event);
}

static BREventType handleListenerSignalTransferEventType = {
    "CWM: Handle Listener Transfer EVent",
    sizeof (BRListenerSignalTransferEvent),
    (BREventDispatcher) cryptoListenerSignalTransferEventDispatcher
};

extern void
cryptoListenerGenerateTransferEvent (const BRCryptoTransferListener *listener,
                                     BRCryptoTransfer transfer,
                                     BRCryptoTransferEvent event) {
    BRListenerSignalTransferEvent listenerEvent =
    { { NULL, &handleListenerSignalTransferEventType},
        listener->listener,
        cryptoWalletManagerTake (listener->manager),
        cryptoWalletTake(listener->wallet),
        (CRYPTO_TRANSFER_EVENT_DELETED != event.type
         ? cryptoTransferTake(transfer)
         : transfer),
        event };

    eventHandlerSignalEvent(listener->listener->handler, (BREvent *) &listenerEvent);
}

// MARK: - Generate Wallet Event

typedef struct {
    BREvent base;
    BRCryptoListener listener;
    BRCryptoWalletManager manager;
    BRCryptoWallet wallet;
    BRCryptoWalletEvent event;
} BRListenerSignalWalletEvent;

static void
cryptoListenerSignalWalletEventDispatcher (BREventHandler ignore,
                                           BRListenerSignalWalletEvent *event) {
    event->listener->walletCallback (event->listener->context,
                                     event->manager,
                                     event->wallet,
                                     event->event);
}

static BREventType handleListenerSignalWalletEventType = {
    "CWM: Handle Listener Wallet Event",
    sizeof (BRListenerSignalWalletEvent),
    (BREventDispatcher) cryptoListenerSignalWalletEventDispatcher
};

extern void
cryptoListenerGenerateWalletEvent (const BRCryptoWalletListener *listener,
                                   BRCryptoWallet wallet,
                                   BRCryptoWalletEvent event) {
    BRListenerSignalWalletEvent listenerEvent =
    { { NULL, &handleListenerSignalWalletEventType},
        listener->listener,
        cryptoWalletManagerTake (listener->manager),
        (CRYPTO_WALLET_EVENT_DELETED != event.type
         ? cryptoWalletTake(wallet)
         : wallet),
        event };

    eventHandlerSignalEvent(listener->listener->handler, (BREvent *) &listenerEvent);
}

// MARK: - Generate Manager Event

typedef struct {
    BREvent base;
    BRCryptoListener listener;
    BRCryptoWalletManager manager;
    BRCryptoWalletManagerEvent event;
} BRListenerSignalManagerEvent;

static void
cryptoListenerSignalManagerEventDispatcher (BREventHandler ignore,
                                            BRListenerSignalManagerEvent *event) {
    event->listener->managerCallback (event->listener->context,
                                      event->manager,
                                      event->event);
}

static BREventType handleListenerSignalManagerEventType = {
    "CWM: Handle Listener Manager Event",
    sizeof (BRListenerSignalManagerEvent),
    (BREventDispatcher) cryptoListenerSignalManagerEventDispatcher
};

extern void
cryptoListenerGenerateManagerEvent (const BRCryptoWalletManagerListener *listener,
                                    BRCryptoWalletManager manager,
                                    BRCryptoWalletManagerEvent event) {
    BRListenerSignalManagerEvent listenerEvent =
    { { NULL, &handleListenerSignalManagerEventType},
        listener->listener,
        (CRYPTO_WALLET_MANAGER_EVENT_DELETED != event.type
         ? cryptoWalletManagerTake(manager)
         : manager),
        event };

    eventHandlerSignalEvent (listener->listener->handler, (BREvent *) &listenerEvent);
}

// MARK: - Generate Network Event

typedef struct {
    BREvent base;
    BRCryptoListener listener;
    BRCryptoNetwork network;
    BRCryptoNetworkEvent event;
} BRListenerSignalNetworkEvent;

static void
cryptoListenerSignalNetworkEventDispatcher (BREventHandler ignore,
                                            BRListenerSignalNetworkEvent *event) {
    event->listener->networkCallback (event->listener->context,
                                      event->network,
                                      event->event);
}

static BREventType handleListenerSignalNetworkEventType = {
    "CWM: Handle Listener Network Event",
    sizeof (BRListenerSignalNetworkEvent),
    (BREventDispatcher) cryptoListenerSignalNetworkEventDispatcher
};

extern void
cryptoListenerGenerateNetworkEvent (const BRCryptoNetworkListener *listener,
                                    BRCryptoNetwork network,
                                    BRCryptoNetworkEvent event) {
    BRListenerSignalNetworkEvent listenerEvent =
    { { NULL, &handleListenerSignalNetworkEventType},
        listener->listener,
        (CRYPTO_WALLET_MANAGER_EVENT_DELETED != event.type
         ? cryptoNetworkTake(network)
         : network),
        event };

    eventHandlerSignalEvent (listener->listener->handler, (BREvent *) &listenerEvent);
}

// MARK: - Event Type

static const BREventType *
cryptoListenerEventTypes[] = {
    &handleListenerSignalNetworkEventType,
    &handleListenerSignalTransferEventType,
    &handleListenerSignalWalletEventType,
    &handleListenerSignalManagerEventType,
};

static const unsigned int
cryptoListenerEventTypesCount = (sizeof (cryptoListenerEventTypes) / sizeof(BREventType*)); //  11




extern BRCryptoListener
cryptoListenerCreate (BRCryptoListenerContext context,
                      BRCryptoListenerNetworkCallback networkCallback,
                      BRCryptoListenerWalletManagerCallback managerCallback,
                      BRCryptoListenerWalletCallback walletCallback,
                      BRCryptoListenerTransferCallback transferCallback) {
    
    BRCryptoListener listener = calloc (1, sizeof (struct BRCryptoListenerRecord));

    listener->ref = CRYPTO_REF_ASSIGN (cryptoListenerRelease);
    pthread_mutex_init_brd (&listener->lock, PTHREAD_MUTEX_NORMAL);

    listener->networkCallback  = networkCallback;
    listener->managerCallback  = managerCallback;
    listener->walletCallback   = walletCallback;
    listener->transferCallback = transferCallback;

    listener->handler = eventHandlerCreate ("SYS Listener",
                                            cryptoListenerEventTypes,
                                            cryptoListenerEventTypesCount,
                                            &listener->lock);

    return listener;
}

static void
cryptoListenerRelease (BRCryptoListener listener) {
    eventHandlerStop (listener->handler);
    eventHandlerDestroy (listener->handler);

    pthread_mutex_destroy (&listener->lock);

    memset (listener, 0, sizeof(*listener));
    free (listener);
}

extern void
cryptoListenerStart (BRCryptoListener listener) {
    eventHandlerStart (listener->handler);
}

extern void
cryptoListenerStop (BRCryptoListener listener) {
    eventHandlerStop (listener->handler);
}

#undef cryptoSystemTake
#undef cryptoNetworkTake
#undef cryptoTransferTake
#undef cryptoWalletTake
#undef cryptoWalletManagerTake