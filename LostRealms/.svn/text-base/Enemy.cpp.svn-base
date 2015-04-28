/*
 *  Enemy.cpp
 *  MapEditor
 *
 *  Created by Scott Scaccianoce on 12/29/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */


#include "Enemy.h"
#include "game.h"
Enemy::Enemy(SPMesh *ptr) : SPEntity(ptr) {
	starting_dir[0] = 0.0f;
	starting_dir[1] = 0.0f;
	starting_dir[2] = 1.0f;
	
	players_threat[0] = 0;
	players_threat[1] = 0;
	players_threat[2] = 0;
	players_threat[3] = 0;
	
	lastAttack = 0; // elapsed time since last attack
	attackSpeed = 3.0;
	attackRangeSquared = 5.2;
	attackMode = false;
	
	speed = 3.0f;
}

Enemy::~Enemy() {}

bool Enemy::isCloseForPlayerToAttack(M3DVector3f &player_pos, float PlayerDistSqrd) {
	M3DVector3f enemyPos;
	GetOrigin(enemyPos);
	float dist = m3dGetDistanceSquared3(player_pos,enemyPos);
	return PlayerDistSqrd >= dist;
	
}

bool Enemy::TakeDamage(float amount, int player_id) {
	
	
	if (hp == hp_max) {
		//FIrst attack we want to initailize our attack
		lastAttack = 0;
		StartAnimation(true);
	}
	
	if (mezed) {
		GamePacket g;
		g.packet_id =27;
		g.data2 = 0; //not used
		for (int i =0; i < enemy_objects.size(); ++i) {
			if (enemy_objects[i] == this) {
				g.player_id = i;
				break;
			}
		}		
		g.data1 = 0;
		nHost->SendBroadcast(g);
		gPacketManager.AddPacket(g);
	}
	
	hp -= amount;
	if (hp <= 0) {
		hp = 0;
		respawn_timer = 0.0f;
		StopAnimation();
		return true;
	} 
	
	Taunt(player_id,amount);
	
	return false;
}

void Enemy::Taunt(int player_id, int amount) {
	players_threat[player_id] += amount;
	attackMode = true;
}


void Enemy::AnimationUpdate(float dt) {
	if (stun || mezed) {
		return;
	}
	
	if (attackMode) {
		
		M3DVector3f pPos, ePos, result;
		GetOrigin(ePos);
		players[FindHighestThreat()].object->GetOrigin(pPos);
		m3dSubtractVectors3(result, pPos, ePos);
		m3dNormalizeVector3(result);
		SetForwardVector(result);
		
		if(!players[FindHighestThreat()].isCloseForEnemyToAttack(ePos, attackRangeSquared)) {
			MoveForward(speed * dt);
			Update(dt);
		}
		
	}
	attackingObject->Update(dt);
}
void Enemy::AIUpdate(int index, float dt) {
	
	//Check stun
	if (stun) {
		stun_elapsed += dt;
		if (stun_elapsed < stun_duration) {
			return;
		} else {
			//unstun
			stun = false;
			GamePacket g;
			g.packet_id = 26;
			g.player_id = index;
			g.data1 = 0;
			nHost->SendBroadcast(g);
		}

		
	}
	
	//Check Mez
	if (mezed) {
		mez_elapsed += dt;
		if (mez_elapsed < mez_duration) {
			return;
		} else {
			//unmez
			mezed = false;
			GamePacket g;
			g.packet_id = 27;
			g.player_id = index;
			g.data1 = 0;
			nHost->SendBroadcast(g);
		}
		
		
	}
	
	if (hp <= 0 && respawn_timer < respawn) {
		respawn_timer += dt;
		if (respawn_timer >= respawn ) {
			
			//We don't send a packet to ourselves so the server doesn't spam itself with packages
			RespawnEnemy();
			//tell everyone that the guy has respaawned;
			GamePacket g;
			g.packet_id = 22;
			g.data1 = index;
			nHost->SendBroadcast(g);
		}
		return;
	}
	
	if (attackMode) {
		//We are being attcked
		if (lastAttack < attackSpeed) {
			lastAttack += dt;
		}
		if (lastAttack >= attackSpeed) {
		
			int mostHatedPlayer = FindHighestThreat();
			M3DVector3f pos;
			GetOrigin(pos);
			
			if (players[mostHatedPlayer].isCloseForEnemyToAttack(pos, attackRangeSquared)) {
				//Send attack to player but no damage is done.  We let the player update everyone on how much damage is done.
				
				GamePacket g;
				g.packet_id = 19;
				g.player_id = mostHatedPlayer;
				g.data1 = attack; 
				g.data2 = index;
				strcpy(g.name , name.c_str());
				nHost->SendBroadcast(g);
				gPacketManager.AddPacket(g);
			} else {
				
				//Too far away.  Send that we want to attack him
				GamePacket g;
				g.packet_id = 21;
				g.player_id = mostHatedPlayer;
				g.data1 = index; 
				nHost->SendBroadcast(g);
				gPacketManager.AddPacket(g);
			}

			
			lastAttack = 0.0f;
		}
		
	}
	
}

void Enemy::RespawnEnemy() {
	SetOrigin(starting_pos);
	respawn_timer = 0.0f;
	hp = hp_max;
	players_threat[0] = 0;
	players_threat[1] = 0;
	players_threat[2] = 0;
	players_threat[3] = 0;
	attackMode = false;
	mezed = false;
	stun = false;
	
	
	
}

int Enemy::FindHighestThreat() {
	
	int id = 0;
	int value = 0;
	
	for (int i =0; i < 4; ++i) {
		if (value < players_threat[i]) {
			id = i;
			value = players_threat[i];
		}
	}
	return id;
}
void Enemy::EnemyRender() {
	
	if (!attackMode) {
		Render();
	} else {
				
		M3DVector3f mPos, mForward, mUp;
		GetOrigin(mPos);
		GetForwardVector(mForward);
		GetUpVector(mUp);
		
		attackingObject->SetForwardVector(mForward);
		attackingObject->SetUpVector(mUp);
		attackingObject->SetOrigin(mPos);
		
		if (attackingObject->Animating()) {
			attackingObject->Render();
		} else {
			Render();
		}

		
	}

}

void Enemy::UpdateStun(float dur, bool value) {
	stun = value;
	stun_elapsed = 0;
	stun_duration = dur;
	
}
void Enemy::UpdateMez(float dur, bool value) {
	mezed = value;
	mez_elapsed = 0;
	mez_duration = dur;
}
