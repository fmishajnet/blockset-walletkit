/*
 * Created by Michael Carrara <michael.carrara@breadwallet.com> on 5/31/18.
 * Copyright (c) 2018 Breadwinner AG.  All right reserved.
 *
 * See the LICENSE file at the project root for license information.
 * See the CONTRIBUTORS file at the project root for a list of contributors.
 */
package com.breadwallet.corecrypto;

import com.breadwallet.crypto.TransferState;
import com.breadwallet.crypto.Unit;
import com.breadwallet.crypto.Wallet;

import java.util.concurrent.atomic.AtomicReference;

/* package */
abstract class Transfer implements com.breadwallet.crypto.Transfer {

    protected final Wallet owner;
    protected final Unit defaultUnit;
    protected final AtomicReference<TransferState> state;

    protected Transfer(Wallet owner, Unit defaultUnit) {
        this.owner = owner;
        this.defaultUnit = defaultUnit;
        this.state = new AtomicReference<>(TransferState.CREATED());
    }

    @Override
    public Wallet getWallet() {
        return owner;
    }

    @Override
    public com.breadwallet.crypto.Amount getAmountDirected() {
        switch (getDirection()) {
            case RECOVERED:
                return Amount.create(0L, defaultUnit).get();
            case SENT:
                return getAmount().negate();
            case RECEIVED:
                return getAmount();
            default:
                throw new IllegalStateException("Invalid transfer direction");
        }
    }

    @Override
    public TransferState getState() {
        return state.get();
    }

    /* package */
    TransferState setState(TransferState newState) {
        return state.getAndSet(newState);
    }
}
