/*
 *  ProcessPackets.cpp
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 12/3/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */


#include "game.h"
#include "PacketManager.h"
#include <string>

/*
 
 Packet ID
 
 0 = Chat message
 1 = Client Request Player_id
 2 = Server is assigning you a player_id
 3 = player update
 
 10 = player is starting forward
 11 = player is stoping forward
 12 = player is starting backward
 13 = player is stoping backward
 14 = player is starting turning left
 15 = player is stoping turning left
 16 = player is starting turning right
 17 = player is stoping turning right
 
 18 = player attacked
 19 = Mob is attacking a player, sends a packet with targeted player and how much gross damage
 20 = Player got attacked by mob.   He needs to figure out how much net damage and report to group
 21 = Enemy is engaging a player, but they out of range.  Letting everyone know
 22 = Respawn enemy
 23 = spell was casted
 24 = Player got healed by a buff (hp or mp).  So adjust hp mp values for player
 25 = Player got speed Buff data1 has value could be 0 or run song
 26 = Set Enemy Stun status
 27 = Set Enemy Mez status
 28 = Set Player Torns status
 29 = Player Missed Autoattack
 
 
 */





void Game::ProcessPackets() {

	int max_packets = 10;
	for(int i =0; i < max_packets; ++i) {
		
		GamePacket packet;
		bool gotPacket = gPacketManager.GetPacket(packet);
		
		if(!gotPacket) {
			break;
		}
		std::string pname;
		std::string pmsg;
		switch (packet.packet_id) {
			
			//Chat Message;
			case 0:
				pname = packet.name;
				pmsg =packet.msg;
				log->AddItem(pname + ": " + pmsg );
				
				//Cheats
				if (pmsg == "fullhp") {
					players[PlayerId].Heal(10000);
				}
				if (pmsg == "fullmp") {
					players[PlayerId].GainMp(100);
				}
				
				break;
				
			//Requesting player_id
			case 1:
			{
				//Find next connection that is closed
				int id = 0;
				for (id = 0; id < 4; ++id) {
					if (players[id].connected == false) {
						players[id].connected = true;
						break;
					}
				}
				
				GamePacket return_packet;
				return_packet.packet_id = 2;
				return_packet.player_id = id;
				strcpy(return_packet.name , packet.name);
				return_packet.pos[0] = packet.pos[0];
				return_packet.pos[1] = packet.pos[1];
				return_packet.pos[2] = packet.pos[2];
				return_packet.data2 = packet.data2; //level
				return_packet.data1 = packet.data1; //class
				return_packet.hp = packet.hp;
				return_packet.mp = packet.mp;
				nHost->SendBroadcast(return_packet);
				players[id].UpdatePacket(return_packet);
				players[id].object->GetAnimationInfo();
				
				
				for (int i =0; i < 4; ++i) {
					if (i != id && players[i].connected) {
						GamePacket oPlayer;
						oPlayer.packet_id = 4;
						oPlayer.player_id = i;
						strcpy(oPlayer.name , players[i].name.c_str());
						oPlayer.pos[0] = players[i].pos[0];
						oPlayer.pos[1] = players[i].pos[1];
						oPlayer.pos[2] = players[i].pos[2];
						oPlayer.data1 = players[i].level;
						oPlayer.data2 = players[i].class_id;
						strcpy(oPlayer.msg , return_packet.name);
						nHost->SendBroadcast(oPlayer);
						
					}
				}
			}	
				break;
			
				//If you sent the id request then you should assign yourself the player information
			case 2:
			{
				if (strcmp(player_name.c_str(), packet.name) == 0) {
					PlayerId = packet.player_id;
					players[packet.player_id].IsCurrentPlayer = true;
					players[packet.player_id].db_id = db_player_id;
					
					//Get last remaining data from Players table in the database;
					if (dbPlayers->Query("Select exp, exp_max,hp,mp from Players where player_id = " + db_player_id)) {
						players[packet.player_id].exp = dbPlayers->GetInteger(0, 0);
						players[packet.player_id].exp_max= dbPlayers->GetInteger(0, 1);
						players[packet.player_id].hp= dbPlayers->GetInteger(0, 2);
						players[packet.player_id].mp= dbPlayers->GetInteger(0, 3);
						
						GetEquipFromDatabase();
						UpdatePlayerStats();
						
					}
				}
				players[packet.player_id].connected = true;
				players[packet.player_id].UpdatePacket(packet);
				players[packet.player_id].object->GetAnimationInfo();
				players[packet.player_id].player_id = packet.player_id;
				GamePacket return_packet;
				return_packet.packet_id = 0;
				strcpy(return_packet.name, "Server");
				sprintf(return_packet.msg, "%s has connected with id %i", packet.name, packet.player_id);
				strcpy(return_packet.name, "Server");
				return_packet.pos[0] = packet.pos[0];
				return_packet.pos[1] = packet.pos[1];
				return_packet.pos[2] = packet.pos[2];
				return_packet.data1 = packet.data1;
				return_packet.data2 = packet.data2;
				nHost->SendBroadcast(return_packet);
				gPacketManager.AddPacket(return_packet);
				
			}	
				break;
			case 3:
			{
				if (PlayerId != packet.player_id && players[packet.player_id].connected) {
					players[packet.player_id].object->SetOrigin(packet.pos);
					players[packet.player_id].object->SetForwardVector(packet.dir);
					players[packet.player_id].hp = packet.hp;
					players[packet.player_id].mp = packet.mp;
					players[packet.player_id].hp_max = packet.hpmax;
					players[packet.player_id].mp_max = packet.mpmax;
					
					
				}
				
			}	
				break;
			case 4:
			{
				if (strcmp(player_name.c_str(), packet.msg) == 0) {
					players[packet.player_id].connected = true;
					players[packet.player_id].UpdatePacket(packet);
					players[packet.player_id].object->GetAnimationInfo();
					
													
				}
				
				
			}	
				break;
				
				
			case 10:
				players[packet.player_id].moving_forward = true;
				players[packet.player_id].object->StartAnimation(true);
				
				break;
			case 11:
				players[packet.player_id].moving_forward = false;
				players[packet.player_id].object->StopAnimation();
				break;
			case 12:
				players[packet.player_id].moving_back = true;
				players[packet.player_id].object->StartAnimation(true);
				break;
			case 13:
				players[packet.player_id].moving_back = false;
				players[packet.player_id].object->StopAnimation();
				break;
			case 14:
				players[packet.player_id].turning_left = true;
				break;
			case 15:
				players[packet.player_id].turning_left = false;
				break;
			case 16:
				players[packet.player_id].turning_right = true;
				break;
			case 17:
				players[packet.player_id].turning_right = false;
				break;
					
			case 18:
				players[packet.player_id].attackingObject->StartAnimation();
				
				if (enemy_objects[packet.data2]->hp > 0) { // Make sure monster is alive, might have been killed by someone else
					if (enemy_objects[packet.data2]->TakeDamage(packet.data1, packet.player_id)) {
						if (TargetEnemy ==enemy_objects[packet.data2]) {
							TargetEnemy = NULL;
							players[packet.player_id].ToggleAttack();
						}
						
						//if close enough you get exp
						float PlayerDistSqrd = 50 * 50;
						M3DVector3f player_pos;
						players[PlayerId].object->GetOrigin(player_pos);
						if (enemy_objects[packet.data2]->isCloseForPlayerToAttack(player_pos, PlayerDistSqrd)) {
							players[PlayerId].GainExp(enemy_objects[packet.data2]->level, 0);
						}
					}
				}
				break;
				
			case 19:
				if (PlayerId == packet.player_id) {
					//we got hit, so lets calculate damage
					int damage = players[PlayerId].CalculateDefenseDamage(packet.data1, packet.data2);
					if (damage > 0) {
						GamePacket hurt;
						hurt.player_id = packet.player_id;
						hurt.packet_id = 20;
						hurt.data1 = damage;
						hurt.data2 = packet.data2;
						strcpy(hurt.name,packet.name);
						gPacketManager.AddPacket(hurt);
						nHost->SendBroadcast(hurt);
					}
				}
				enemy_objects[packet.data2]->player_target = packet.player_id;
				enemy_objects[packet.data2]->attackingObject->StartAnimation();
				break;
			case 20:
				players[packet.player_id].TakeDamage(packet.data1, packet.data2);
				if (packet.player_id == PlayerId) {
					std::string name = packet.name;
					
					castBar->Hide();
				}	
				
				
				break;
			case 21:
				enemy_objects[packet.data1]->player_target = packet.player_id;
				enemy_objects[packet.data1]->attackMode = true;
				break;
			case 22:
				enemy_objects[packet.data1]->respawn;
				break;
			case 23:
			{
				//Casted a spell
				//Get spell id in vector
				int spellIndex = 0;
				int tIndex = packet.data2;
				int aIndex = packet.data3;
				int amount = packet.hp;
				for (i = 0; i < 36; ++i) {
					if (spells[i].db_id == packet.data1) {
						spellIndex = i;
						break;
					}
				}
				
				if (spells[spellIndex].spellType == 3) {
					aIndex = packet.player_id;
				}
				
				players[packet.player_id].CostMp(packet.mp );
				CreateSpellEmitter(packet.data1,tIndex,aIndex, spellIndex );
				
				//Now check if we have to do something with it.  
				//If it's enemy we need to do damage/spell effect   type = 1
				//If it's ally we need to do heal					type = 2
				//if it's group we need to check if					type = 3
				
				if (spells[spellIndex].spellType == 1 ) {
					//calculate and send damage packet
					HandleTargetEnemySpell(spellIndex, enemy_objects[tIndex], packet.player_id, amount, tIndex);
				} else if (spells[spellIndex].spellType == 2) {
					//Call Do Buff function to actually handle this spell
					HandleTargetAllySpell(spellIndex, &players[aIndex], amount);
				} else if (spells[spellIndex].spellType == 3){
					//check if you are in range of this to get the effect
					if (packet.player_id == PlayerId || players[packet.player_id].IsInDist(players[PlayerId].object, spells[spellIndex].range) ) {
						//Call Do Buff function to actually handle 
						HandleTargetGroupSpell(spellIndex, packet.player_id, amount);
					}
					//Need on check on every other player in range for just the emitters
					for (int i =0; i < 4; ++i) {
						if (players[i].connected) {
							if (i != packet.player_id && players[packet.player_id].IsInDist(players[i].object, spells[spellIndex].range)
								|| (PlayerId != packet.player_id && i == packet.player_id)
								) 
							
							{
								CreateSpellEmitter(packet.data1,tIndex,i, spellIndex );
							}
							
						}
					}
				} 


				
				
				break;
			}
			case 24:
				
				players[packet.player_id].Heal(packet.hp);
				players[packet.player_id].GainMp(packet.mp);
				
				break;
				
			case 25:
				players[packet.player_id].buff_speed = packet.data1;
				break;
				
			case 26:
				enemy_objects[packet.player_id]->UpdateStun(packet.data2,(packet.data1 == 1));
				break;
			case 27:
				enemy_objects[packet.player_id]->UpdateMez(packet.data2,(packet.data1 == 1));
				break;
			case 28:
				players[packet.player_id].torns = true;
				players[packet.player_id].torns_amount = packet.data1;
				players[packet.player_id].torn_elapsed = 0.0f;
				break;
			case 29:
				//we do nothing but show animation since we missed
				players[packet.player_id].attackingObject->StartAnimation();
				break;
				
				break;

			default:
				break;
		}
	}
}