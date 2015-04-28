/*
 *  Enemy.h
 *  MapEditor
 *
 *  Created by Scott Scaccianoce on 12/29/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "SPEntity.h"
#include "spMesh.h"

class Enemy : public SPEntity {
public:
	
	Enemy(SPMesh *mesh_ptr);
	~Enemy();
	void AIUpdate(float dt);
	void AnimationUpdate(float dt);
	
	void RespawnEnemy();
	int FindHighestThreat();
	
	int hp;
	int hp_max;
	int attack;
	int level;
	bool hostile;
	int baf_radius;
	int respawn;
	int enemy_id;
	int exp;
	int players_threat[4];
	
	float respawn_timer;
	
	std::string name;
	M3DVector3f starting_pos;
	M3DVector3f starting_dir;
	
	//Attacking
	float lastAttack; // elapsed time since last attack
	float attackSpeed;
	float attackRangeSquared;
	bool attackMode;
	
	bool isCloseForPlayerToAttack(M3DVector3f &player_pos, float PlayerDistSqrd);
	bool CalculateDefenseDamage(float amount, int player_id); // true on death of enemy
	bool TakeDamage(float amount, int player_id);
	
	//Pointer to attacking mesh
	SPEntity *attackingObject;
private:
	
	
};

#endif