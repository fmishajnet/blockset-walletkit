/*
 * Created by Michael Carrara <michael.carrara@breadwallet.com> on 5/31/18.
 * Copyright (c) 2018 Breadwinner AG.  All right reserved.
 *
 * See the LICENSE file at the project root for license information.
 * See the CONTRIBUTORS file at the project root for a list of contributors.
 */
package com.breadwallet.corenative.bitcoin;

import com.breadwallet.corenative.CryptoLibrary;
import com.google.common.primitives.UnsignedInteger;
import com.google.common.primitives.UnsignedLong;

import java.util.List;

/* package */
class OwnedBRWalletManager implements CoreBRWalletManager {

    private final BRWalletManager core;

    /* package */
    OwnedBRWalletManager(BRWalletManager core) {
        this.core = core;
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        if (null != core) {
            CryptoLibrary.INSTANCE.BRWalletManagerFree(core);
        }
    }

    @Override
    public BRWallet getWallet() {
        return core.getWallet();
    }

    @Override
    public void generateUnusedAddrs(UnsignedInteger limit) {
        core.generateUnusedAddrs(limit);
    }

    @Override
    public List<String> getAllAddrs() {
        return core.getAllAddrs();
    }

    @Override
    public List<String> getAllAddrsLegacy() {
        return core.getAllAddrsLegacy();
    }

    @Override
    public void connect() {
        core.connect();
    }

    @Override
    public void disconnect() {
        core.disconnect();
    }

    @Override
    public void scan() {
        core.scan();
    }

    @Override
    public void submitTransaction(CoreBRTransaction tx, byte[] seed) {
        core.submitTransaction(tx, seed);
    }

    @Override
    public boolean matches(BRWalletManager o) {
        return core.matches(o);
    }

    @Override
    public void announceBlockNumber(int rid, UnsignedLong blockNumber) {
        core.announceBlockNumber(rid, blockNumber);
    }

    @Override
    public void announceSubmit(int rid, CoreBRTransaction transcation, int error) {
        core.announceSubmit(rid, transcation, error);
    }

    @Override
    public void announceTransaction(int rid, CoreBRTransaction transaction) {
        core.announceTransaction(rid, transaction);
    }

    @Override
    public void announceTransactionComplete(int rid, boolean success) {
        core.announceTransactionComplete(rid, success);
    }
}
