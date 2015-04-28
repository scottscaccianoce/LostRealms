/*
 *  Network.cpp
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 7/8/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "Network.h"
#include <iostream>

using namespace std;

NetworkHost::NetworkHost(NetworkType t, int connections, int port) {
	nType = t;
	connected = false;
	
	if (t == nServer) {
		address.host = ENET_HOST_ANY;
		address.port = port;
		host = enet_host_create(&address, connections, 2, 0, 0);
		if (host == NULL) {
			cout << "failed making host";
			enet_host_destroy(host);
		} else {
			char buffer[20];
			enet_address_get_host_ip(&address, &buffer[0], 20);
			int x = 0;
			
		}
		
		
	} else {
		address.host = ENET_HOST_BROADCAST;
		address.port = port;
		host = enet_host_create(NULL, 1, 2, 0, 0);
		if (host == NULL) {
			cout << "failed making host";
			enet_host_destroy(host);
		}
		
		//enet_address_set_host(&address, ENET_HOST_BROADCAST);
		//enet_address_set_host(&address, "192.168.1.5");
		
		//enet_address_set_host(&address, "192.168.0.197");
		
		address.port = port;
		
		peer = enet_host_connect(host,&address, 2, 0);
		
		if (peer == NULL) {
			cout << "Failed to connect";
		}
		ENetEvent event;
		if (enet_host_service(host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
			//connected
			connected = true;
			cout << "CONNECTED";
		} else {
			enet_peer_reset(peer);
			
		}
		
	}
}

NetworkHost::~NetworkHost() {
	enet_host_destroy(host);

}

void NetworkHost::SendBroadcast(GamePacket game_packet) {
	/* Create a reliable packet of size 7 containing "packet\0" */
    ENetPacket * packet = enet_packet_create ((char*)&game_packet, 
                                              sizeof(game_packet), 
                                              ENET_PACKET_FLAG_RELIABLE);
   
    enet_host_broadcast (host, 0, packet);       
	
    /* One could just use enet_host_service() instead. */
    enet_host_flush (host);
}

void NetworkHost::Update() {
	ENetEvent event;
	//if (nType == nServer) {
		GamePacket g;
	
		while (enet_host_service(host, &event, 0.1) > 0) {
			switch (event.type) {
				case ENET_EVENT_TYPE_CONNECT:
					cout << "New Client Connected" << event.peer->address.host;
					if ( nType == nServer ) {
						clients.push_back(peer);
					}
						
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					memcpy(&g, event.packet->data, sizeof(GamePacket));
					gPacketManager.AddPacket(g);
					enet_packet_destroy (event.packet);
					break;
				default:
					break;
			}
			 
		}
	
	//}
}

NetworkType NetworkHost::GetType() {
	return nType;
}