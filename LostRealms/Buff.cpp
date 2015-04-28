/*
 *  Buff.cpp
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 12/5/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "Buff.h"
#include "game.h"

std::map<int, Buff> gBuffs;

Buff::Buff() {
	alive = false;	
}
Buff::Buff(int sIndex) {
	elapsed = 0;
	tick_elapsed = 0;
	duration = 0;
	tick_duration = 0;
	
	mInt = 0;
	mStr = 0;
	mAgi = 0;
	mWis = 0;
	mDef = 0;
	mVit = 0;
	mSpeed = 0;
	mHp = 0;
	mMp = 0;
	spellIndex = sIndex;
	alive = true;
}
void Buff::Init(int buff_duration, int t_duration, bool lasting, bool pass , int a ) {
	elapsed = 0;
	tick_elapsed = 0;
	duration = buff_duration;
	tick_duration = t_duration;
	
	mInt = 0;
	mStr = 0;
	mAgi = 0;
	mWis = 0;
	mDef = 0;
	mVit = 0;
	mSpeed = 0;
	mHp = 0;
	mMp = 0;
	alive = true;
	forever = lasting;
	passive = pass;
	amount =a;
	mPulse = false;
	mPulseAmount = 0;
}
void Buff::SetSpeed(int value) {
	GamePacket g;
	g.packet_id = 25;
	g.data1 = value;
	g.player_id = PlayerId;
	nHost->SendBroadcast(g);
	gPacketManager.AddPacket(g);
}

void Buff::SetHaste() {
	haste = true;
	players[PlayerId].haste = true;
}

Buff::~Buff() {
	
}

void Buff::Update(float dt) {
	//we only have to update if this isn't going to be on forever
	if (alive && !passive) {
		
		if (mHp > 0 || mMp > 0 || mPulse) {
			tick_elapsed += dt;
			if ( tick_elapsed >= tick_duration) {
				if (mPulse) {
					DoPulse();
				} else {
					tick_elapsed = 0;
					GamePacket g;
					g.packet_id = 24;
					g.hp = mHp;
					g.mp = mMp;
					g.player_id = PlayerId;
					nHost->SendBroadcast(g);
					gPacketManager.AddPacket(g);
				}
			}
		}
		
		elapsed += dt;
		if (elapsed > duration) {
			if (mSpeed > 0) {
				//Send packet that we lost speed
				GamePacket g;
				g.packet_id = 25;
				g.data1 = 0;
				g.player_id = PlayerId;
				nHost->SendBroadcast(g);
				gPacketManager.AddPacket(g);
			}
			
			if (forever) {
				GamePacket g;
				g.packet_id = 23;
				g.player_id = PlayerId;
				g.data1 = spells[spellIndex].db_id;
				g.hp = amount;
				g.mp = 0;
				
				//Not used
				g.data2 = 0;
				g.data3 = 0;
				nHost->SendBroadcast(g);
				gPacketManager.AddPacket(g);
				//combatLog->AddItem("Reapply");
				
			}
			// check if haste
			if (haste) {
				players[PlayerId].haste = false;
			}
			
			alive = false;
		}
	}
}

void Buff::DoPulse() {
	tick_elapsed = 0;
	
	if (spells[spellIndex].db_id == 34) {
		
		for(int i =0; i < 4; ++i) {
			if (players[i].connected) {
				M3DVector3f pos;
				players[i].object->GetOrigin(pos);
				
				if(m3dGetDistance3(pos, mPulsePos) <= mPulseRange) {
					//In range so kick off heal packet
					GamePacket g;
					g.packet_id = 24;
					g.hp = mPulseAmount;
					g.mp = 0;
					g.player_id = i;
					nHost->SendBroadcast(g);
					gPacketManager.AddPacket(g);
					
				}
			}
		}
		
	} else if (spells[spellIndex].db_id == 35) {
		for(int i =0; i < 4; ++i) {
			if (players[i].connected) {
				M3DVector3f pos;
				players[i].object->GetOrigin(pos);
				
				if(m3dGetDistance3(pos, mPulsePos) <= mPulseRange) {
					//In range so kick off heal packet
					GamePacket g;
					g.packet_id = 28;
					g.data1 = mPulseAmount;
					g.player_id = i;
					nHost->SendBroadcast(g);
					gPacketManager.AddPacket(g);
					
				}
			}
		}
	} else if (spells[spellIndex].db_id == 36) {
		if (PlayerId == mPulseOwner) {
			Enemy *treeTargetEnemy = NULL;
			int eID = 0;
			float enemyDist = 100;
			int size = enemy_objects.size();
			
			for(int i =0; i < size; ++i) {
				
				M3DVector3f pos;
				enemy_objects[i]->GetOrigin(pos);
				float dist = m3dGetDistance3(pos, mPulsePos);
				if( dist <= mPulseRange && dist < enemyDist) {
					if (enemy_objects[i]->hp > 0 && !enemy_objects[i]->mezed) {
						treeTargetEnemy = enemy_objects[i];
						enemyDist = dist;
						eID = i;
					}
				}
			
			}
			
			if (treeTargetEnemy != NULL) {
				GamePacket g;
				g.packet_id = 18;
				g.player_id = PlayerId;
				g.data1 = mPulseAmount; 
				g.data2 = eID;
				nHost->SendBroadcast(g);
				gPacketManager.AddPacket(g);
				combatLog->AddItem("Oak Tree Hits " + treeTargetEnemy->name + " for " + IntToString(mPulseAmount) + " damage", TEAL);
			}
			
		}
	} 
}