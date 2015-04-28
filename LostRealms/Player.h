/*
 *  Player.h
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 12/4/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifndef __PLAYER_H
#define __PLAYER_H

#include "math3d.h"
#include <string>
//#include "Buff.h"
//#include "Spell.h"
#include <vector>
#include "SPEntity.h"
#include "quadterrain.h"
#include "PacketManager.h"
#include "util.h"
#include "Database.h"
#include "spMesh.h"


enum stats{kInt=0,kStr,kAgi,kVit,kWis,kDef,kAtk};

class Player {
public:
	Player();
	~Player();
	
	int CalculateMeleeDamage();
	int CalculateSpellDamage();
	bool MissAbility(Enemy *enemy);
	
	void UpdatePacket(const GamePacket &packet );
	bool UpdateObject(float dt);
	void SetName(const std::string &n);
	void SetDatabases(Database * p, Database *g);
	void ElapseBuffs(float dt);
	void SetObject(SPMesh *mesh);
	void SetAttackingObject(SPMesh *mesh);
	void SetTerrain(QuadTerrain *land);
	void Render();
	bool UpdateStats(); //Return true means we changed maxed values and need to update the players
	void GetStatsFromItems();
	void SetStartingStats();
	
	bool moving_forward;
	bool moving_back;
	bool turning_right;
	bool turning_left;
	bool connected;
	
	SPEntity *object;
	SPEntity *attackingObject;
	
	float speed;
	float buff_speed;
	int hp;
	int hp_max;
	int mp;
	int mp_max;
	int exp;
	int exp_max;
	int level;
	int class_id;
	std::string name;
	std::string db_id;
	
	M3DVector3f pos;
	M3DVector3f dir;
	
	//Stats
	int Str,baseStr, itemStr, buffStr, growthStr;
	int Int,baseInt, itemInt, buffInt, growthInt;
	int Def,baseDef, itemDef, buffDef, growthDef;
	int Vit,baseVit, itemVit, buffVit, growthVit;
	int Wis,baseWis, itemWis, buffWis, growthWis;
	int Agi,baseAgi, itemAgi, buffAgi, growthAgi;
	
	bool IsCurrentPlayer;
	int player_id;
	
	bool CollidedWithObjects();
	
	//Attacking variables
	bool attacking;
	float lastAttack; // elapsed time since last attack
	float attackSpeed;
	float attackRangeSquared;
	void ToggleAttack();
	void GainExp(int level, int baf_bonus);
	bool isCloseForEnemyToAttack(M3DVector3f &enemy_pos, float PlayerDistSqrd);
	int CalculateDefenseDamage(int amount, int enemyIndex);
	void TakeDamage(int amount, int enemyIndex);
	void CostMp(int amount);
	void Heal(int amount, bool printmsg = true);
	void GainMp(int amount);
	
	
	void SaveData();
	float lastSave, saveRate;
	
	
	//Range Distance
	bool IsInDist(SPEntity *obj, float distSquared);
	
	bool bladeturn;
	bool sucuplant;
	int sucuplant_amount;
	int torns_amount;
	bool torns;
	float torn_duration;
	float torn_elapsed;
	bool haste;
	float restoreRate, lastRestore;
	
private:
	
	
	Database *dbPlayers;
	Database *dbGame;
	

	void RestHp();
	void RestMp();
	
	//std::vector<Buff> buffs;
	//std::vector<Spell> spells;
	
	
	
	
	QuadTerrain *terrain;
	
	//Used For Stat forumulas
	int WisLevelIncreaseAmount;
	int VitLevelIncreaseAmount;

};

#endif