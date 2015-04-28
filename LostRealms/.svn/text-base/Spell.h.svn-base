/*
 *  Spell.h
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 12/5/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifndef SPELL_H
#define SPELL_H

#include "texture.h"
#include "Enemy.h"
#include "Player.h"

class Spell {
	
public:
	Spell();
	~Spell();
	
	void LoadTexture(const std::string &filename);
	bool waitingOnRecast();
	bool CanCast(Enemy *TargetEnemy, Player *TargetAlly, int mp_amount);
	
	
	sTexture *icon;
	bool loadedIcon;
	std::string school;
	int class_id;
	int db_id;
	std::string name;
	
	float elapsed;
	float recast_elapased;
	float recast;
	float cost;
	float cast_time;
	float range;
	float base_dmg;
	int spellType;
	
	
	int duration;
	int tick;
	bool forever;
	bool passive;
};



extern 	Spell spells[36];




#endif