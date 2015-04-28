/*
 *  PacketManager.h
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 12/3/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */



#ifndef GAME_PACKET_H
#define GAME_PACKET_H


#include <vector>
#include "util.h"
#include "math3d.h"

struct GamePacket {
	int packet_id;
	int player_id;
	M3DVector3f pos;
	M3DVector3f dir;
	int hpmax;
	int hp;
	int mpmax;
	int mp;
	char name[10];
	char msg[100];
	int data1;
	int data2;
	int data3;
};


class PacketManager {

public:
	bool GetPacket(GamePacket &p);
	void AddPacket(GamePacket &packet);
	
private:
	
	std::vector<GamePacket> packets;


};
#endif
extern PacketManager gPacketManager;