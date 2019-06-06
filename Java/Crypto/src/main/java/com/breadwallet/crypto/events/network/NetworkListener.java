/*
 * Created by Michael Carrara <michael.carrara@breadwallet.com> on 5/31/18.
 * Copyright (c) 2018 Breadwinner AG.  All right reserved.
 *
 * See the LICENSE file at the project root for license information.
 * See the CONTRIBUTORS file at the project root for a list of contributors.
 */
package com.breadwallet.crypto.events.network;

import com.breadwallet.crypto.System;
import com.breadwallet.crypto.Network;

public interface NetworkListener {

    void handleNetworkEvent(System system, Network network, NetworkEvent event);
}
