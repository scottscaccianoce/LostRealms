/*
 *  Network.h
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 7/8/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifndef __NETWORK__
#define __NETWORK__
#include "enet/enet.h"
#include <vector>
#include "PacketManager.h"

enum NetworkType {
	nServer=0, nClient
};

class NetworkHost {
public:
	NetworkHost(NetworkType type, int connections = 1, int port = 1234 );
	~NetworkHost();
	void Update();
	void SendBroadcast(GamePacket packet);
	NetworkType GetType();
	bool connected;
	
private:
	
	ENetHost *host;
	ENetAddress address;
	NetworkType nType;
	ENetPeer *peer;
	std::vector<ENetPeer*> clients;
};


#endif

