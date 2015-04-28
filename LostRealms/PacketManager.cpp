/*
 *  PacketManager.cpp
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 12/3/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "PacketManager.h"

PacketManager gPacketManager;

bool PacketManager::GetPacket(GamePacket &p){
	bool retVal = false;
	if(packets.size()  > 0)
	{
		p = packets[0];
		packets.erase(packets.begin ());
		retVal = true;
	}
	return retVal;
}
void PacketManager::AddPacket(GamePacket &packet) {
	packets.push_back(packet);
}
