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
	attackSpeed = 2.9;
	attackRangeSquared = 2;
	attackMode = false;
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
	}
	
	hp -= amount;
	if (hp <= 0) {
		hp = 0;
		respawn_timer = 0.0f;
		return true;
	} 
	
	players_threat[player_id] += amount;
	attackMode = true;
	return false;
}
void Enemy::AnimationUpdate(float dt) {
	if (attackMode) {
		Update(dt);
	}
}
void Enemy::AIUpdate(float dt) {
	
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