/*
 *  Player.cpp
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 12/4/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "Player.h"
#include "game.h"
#include "PacketManager.h"


Player::Player() {
	object = NULL;
	moving_forward = false;
	moving_back = false;
	turning_right = false;
	turning_left = false;
	speed = 2.3f;
	buff_speed = 0;
	IsCurrentPlayer = false;
	hp = 0;
	hp_max = 0;
	mp = 0;
	mp_max = 100;
	
	bladeturn = false;
	haste = false;
	torns = false;
	torn_duration = 10;
	torn_elapsed = 0.0f;
	sucuplant = false;
	baseInt = baseStr = baseAgi = baseWis = baseVit = baseDef  = 1;
	
	WisLevelIncreaseAmount = 25;
	VitLevelIncreaseAmount = 20;
	
	attacking = false;
	attackSpeed = 3.0f;
	lastAttack = 0.0f;
	attackRangeSquared = 5.0f;
	
	lastRestore = 0;
	restoreRate = 4;
	
	lastSave = 0;
	saveRate = 30;
}

Player::~Player() {
	delete object;
	delete attackingObject;
	terrain = NULL;
	
}

int Player::CalculateMeleeDamage() {
	UpdateStats();
	return equipment[8].attack + Str;
	
}

int Player::CalculateSpellDamage() {
	return 0;
}



void Player::SetName(const std::string &n) {
	
}

void Player::ElapseBuffs(float dt){
	
}

void Player::SetObject(SPMesh *mesh) {
	object = new SPEntity(mesh);
	
}
void Player::SetAttackingObject(SPMesh *mesh) {
	attackingObject = new SPEntity(mesh);
	
}
void Player::Render() {
	if (connected) {
	//	if (!attacking) {
		//	object->Render();
		//	//object->RenderBB();
		//} else {
			
			
			if (attackingObject->Animating()) {
				M3DVector3f mPos, mForward, mUp;
				object->GetOrigin(mPos);
				object->GetForwardVector(mForward);
				object->GetUpVector(mUp);
				
				attackingObject->SetForwardVector(mForward);
				attackingObject->SetUpVector(mUp);
				attackingObject->SetOrigin(mPos);
				attackingObject->Render();
			} else {
				object->Render();
			}
			
		//}

	}
	
}



bool Player::UpdateObject(float dt) {
	if (!connected) {
		return true;
	}
	
	if (torn_elapsed < torn_duration) {
		torn_elapsed += dt;
		if (torn_elapsed >= torn_duration) {
			torns = false;
		}
	}
	
	float haste_speed = (haste) ? (float)attackSpeed / 2.0 : 0.0f;
	if (!attacking) {
		lastSave += dt;
		if (lastSave > saveRate) {
			lastSave = 0;
			SaveData();
		}
	}
	
	if (!attacking && IsCurrentPlayer) {
		lastRestore += dt;
		
		if (lastRestore >= restoreRate) {
			//heal hp and mp
			if (hp < hp_max) {
				RestHp();
			}
			
			if (mp < mp_max) {
				RestMp();
			}
			lastRestore = 0.0f;
		}
	}
	
	if (lastAttack < attackSpeed && IsCurrentPlayer && castingSpell == NULL) {
		lastAttack += dt;
	}
	
	if (attacking && TargetEnemy != NULL) {
		if (lastAttack + haste_speed >= attackSpeed && IsCurrentPlayer) {
			
			
			
			//Send an attack
			M3DVector3f pos;
			object->GetOrigin(pos);
			
			if (TargetEnemy != NULL && TargetEnemy->isCloseForPlayerToAttack(pos,attackRangeSquared)) {
				//check if we miss
				if (MissAbility(TargetEnemy)) {
					GamePacket g;
					g.packet_id = 29;
					g.player_id = player_id;
					
					nHost->SendBroadcast(g);
					gPacketManager.AddPacket(g);
					combatLog->AddItem("Your attack misses");
					
				} else {
				
					//Its in attack range we actually get an attack off.
					int damage = CalculateMeleeDamage();
					GamePacket g;
					g.packet_id = 18;
					g.player_id = player_id;
					g.data1 = damage; 
					g.data2 = TargetIndex;
					nHost->SendBroadcast(g);
					gPacketManager.AddPacket(g);
					combatLog->AddItem("You damage " + TargetEnemy->name + " for " + IntToString(damage) + " damage");
				}
			} else {
				combatLog->AddItem("Too far away to attack");
				
			}
			lastAttack = 0.0f;
		}
	}
	
	bool moved = false;
	
	if (moving_forward) {
		object->MoveForward((speed + buff_speed) * dt);
		moved = true;
		
		if (IsCurrentPlayer) {
			if (CollidedWithObjects() ) {
				moved = false;
				object->MoveForward(-(speed + buff_speed) * dt);
				return false;
			}
		}
	} else if (moving_back) {
		object->MoveForward(-(speed + buff_speed) * dt);
		moved = true;
		if (IsCurrentPlayer) {
			if (CollidedWithObjects() ) {
				moved = false;
				object->MoveForward((speed + buff_speed) * dt);
				return false;
			}
		}	
	}
	if (turning_left) {
		object->RotateLocalY(1.9 * dt);
		moved = true;
				
	} else if (turning_right) {
		object->RotateLocalY(-1.9 * dt);
		moved = true;
	}
	
	if (moved) {
		
		float height = 0;
		terrain->GetHeightAtPos(object->GetOriginX(), object->GetOriginY(), object->GetOriginZ(), height );
		object->SetOriginY(height);
	}
	object->Update(dt);
	attackingObject->Update(dt);
	return true;
}

void Player::SetTerrain(QuadTerrain *land) {
	terrain = land;
	
}

void Player::UpdatePacket(const GamePacket &packet ) {
	pos[0] = packet.pos[0];
	pos[1] = packet.pos[1];
	pos[2] = packet.pos[2];
	level = packet.data1;
	class_id = packet.data2;
	name = packet.name;
//	std::string class_id = IntToString(packet.data2);
	
	dbGame->Query("Select mesh_id, attack_mesh_id from Classes where class_id = " + IntToString(class_id));
	int mesh_id = atoi(dbGame->results[0][0].c_str());
	int attack_mesh_id = atoi(dbGame->results[0][1].c_str());
	
	SetObject(meshes[mesh_id -1]);
	SetAttackingObject(meshes[attack_mesh_id -1]);
	attackingObject->GetAnimationInfo();
	attackingObject->ShouldRepeat(false);
	object->SetOrigin(pos);
	SetStartingStats();
	
	
	
	
}
void Player::SetDatabases(Database * p, Database *g) {
	dbPlayers = p;
	dbGame = g;

}

bool Player::CollidedWithObjects() {
	int size = objects.size();
	for (int i =0; i < size; ++i ) {
		if (object->TestCollision(*objects[i])) {
			return true;
		}
		
	}
	
	return false;
}

bool Player::UpdateStats() {
	
	int old_hp_max = hp_max;
	
	GetStatsFromItems();
	
	//Calculate Max HP
	hp_max = VitLevelIncreaseAmount * Vit;
	if (hp > hp_max) {
		hp = hp_max;
	}
	
	if (mp  > mp_max) {
		mp = mp_max;
	}
	
	//Send pack for players to update their Max values
	if (hp_max != old_hp_max ) {
		return true;
	}
	
	return false;

}


void Player::GetStatsFromItems() {
	int EQUIP_MAX = 50;
	
	itemInt = itemStr = itemAgi = itemDef = itemWis = itemVit = 0;
	buffInt = buffStr = buffAgi = buffDef = buffWis = buffVit = 0;
	
	for (int i = 0; i < 9; ++i) {
		if (equipment[i].db_id != "0") {
			itemInt += equipment[i].mInt;
			itemStr += equipment[i].mStr;
			itemAgi += equipment[i].mAgi;
			itemVit += equipment[i].mVit;
			itemWis += equipment[i].mWis;
			itemDef += equipment[i].mDef;
			
		}
	}
	
	//items have soft cap
	if (itemInt > EQUIP_MAX) {
		itemInt = EQUIP_MAX;
	}
	if (itemStr > EQUIP_MAX) {
		itemStr = EQUIP_MAX;
	}
	if (itemAgi > EQUIP_MAX) {
		itemAgi = EQUIP_MAX;
	}
	if (itemVit > EQUIP_MAX) {
		itemVit = EQUIP_MAX;
	}
	if (itemWis > EQUIP_MAX) {
		itemWis = EQUIP_MAX;
	}
	if (itemDef > EQUIP_MAX) {
		itemDef = EQUIP_MAX;
	}
	
	
	std::map < int, Buff>::iterator iter;
	for (iter = gBuffs.begin(); iter != gBuffs.end(); ++iter) {
		buffInt += iter->second.GetInt();	
		buffStr += iter->second.GetStr();
		buffAgi += iter->second.GetAgi();
		buffVit += iter->second.GetVit();
		buffWis += iter->second.GetWis();
		buffDef += iter->second.GetDef();

	
	}
	
	//Get Level stats
	
	int levelInt, levelStr, levelAgi, levelVit, levelWis, levelDef;
	
	levelInt = baseInt + ((level - 1) * growthInt);
	levelStr = baseStr + ((level - 1) * growthStr);
	levelAgi = baseAgi + ((level - 1) * growthAgi);
	levelVit = baseVit + ((level - 1) * growthVit);
	levelWis = baseWis + ((level - 1) * growthWis);
	levelDef = baseDef + ((level - 1) * growthDef);
	
	
	Int = levelInt + itemInt + buffInt;
	Str = levelStr + itemStr + buffStr;
	Agi = levelAgi + itemAgi + buffAgi;
	Vit = levelVit + itemVit + buffVit;
	Wis = levelWis + itemWis + buffWis;
	Def = levelDef + itemDef + buffDef;
	
}



void Player::ToggleAttack() {
	if (attacking) {
		attacking = false;
		lastAttack = 0;
	} else {
		//We need to start attacking
		attacking = true;
	}


}

void Player::GainExp(int enemylevel, int baf_bonus) {
	int amount;
	int level_diff = enemylevel - players[PlayerId].level;
	
	switch (level_diff) {
		case -2:
			amount = 1;
			break;
		case -1:
			amount = 2;
			break;
		case 0:
			amount = 3;
			break;
		case 1:
			amount = 5;
			break;
		case 2:
			amount = 7;
			break;
		case 3:
			amount = 9;
			break;
		case 4:
			amount = 11;
			break;
		case 5:
			amount = 13;
			break;
		default:
			amount = 0;
			break;
	}
	
	//add modifiers if not zero
	if (amount != 0) {
		//loop through all connected players that aren't ourselves
		for (int i =0; i < 4; ++i) {
			if (players[i].connected && i != PlayerId) {
				amount--;
			}
		}
		
		//add baf_bonus
		amount += baf_bonus;
	}
	
	//check in case we went negative
	if (amount < 0) {
		amount = 0;
	}
	
	combatLog->AddItem("You Gain " + IntToString(amount) + " exp", YELLOW);
	exp += amount;
	
	if (exp >= exp_max) {
		//Gain level;
		level += 1;
		exp -= exp_max;
		combatLog->AddItem("You are now level " + IntToString(level), YELLOW);
		exp_max = gLevels[level];
		UpdateStats(); //get new hp
		Heal(10000, false);
		GainMp(100);
		SaveData();
	}
}

bool Player::isCloseForEnemyToAttack(M3DVector3f &enemy_pos, float PlayerDistSqrd) {
	M3DVector3f playerPos;
	object->GetOrigin(playerPos);
	float dist = m3dGetDistanceSquared3(enemy_pos,playerPos);
	return PlayerDistSqrd >= dist;
	
}

int Player::CalculateDefenseDamage(int amount, int enemyIndex) {
	if (bladeturn) {
		bladeturn = false;
		combatLog->AddItem("Bladeturn absorbs damage.", RED);
		return 0;
	}
	
	//check if you dodge
	float dodgeRate = (float)((Agi / 5.0)) / 100.0f;
	float num = unifRand(0,1);
	if (num < dodgeRate) {
		combatLog->AddItem("Dodged Attack", RED);
		return 0;
	}
	
	if (torns) {
		//Deal damage back to the monster
		GamePacket g;
		g.packet_id = 18;
		g.player_id = player_id;
		g.data1 = torns_amount;
		g.data2 = enemyIndex;
		nHost->SendBroadcast(g);
		gPacketManager.AddPacket(g);
		combatLog->AddItem("Torns deals " + IntToString(torns_amount) + " damage", TEAL);
	}
	
	UpdateStats();
	
	//Return amount after applying resistance
	return amount - ((float)amount * (Def/4)/100);
}

void Player::TakeDamage(int amount, int enemyIndex) {
	
	if (IsCurrentPlayer) {
		combatLog->AddItem(enemy_objects[enemyIndex]->name + " attacks you for " + IntToString(amount) + " damage", RED);	
		lastRestore = 0.0f;
	}
	
	
	hp -= amount;
	if (castingSpell != NULL) {
		castingSpell->elapsed = 0;
		castingSpell = NULL;
		
	}
	
	if (sucuplant) {
		//Heal a bit of the damage;
		GamePacket g;
		g.packet_id = 24;
		g.player_id = player_id;
		g.hp = sucuplant_amount;
		g.mp = 0;
		nHost->SendBroadcast(g);
		gPacketManager.AddPacket(g);
	}

}
void Player::CostMp(int amount) {
	mp -= amount;
	if (mp < 0) {
		mp = 0;
	}
	
}
void Player::Heal(int amount, bool printmsg) {
	hp +=amount;
	if (hp > hp_max) {
		hp = hp_max;
	}
	
	if (IsCurrentPlayer && amount > 0 && printmsg) {
		combatLog->AddItem("You are Healed for " + IntToString(amount), GREEN);
	}
}
void Player::GainMp(int amount) {
	mp +=amount;
	if (mp > mp_max) {
		mp = mp_max;
	}
}

void Player::RestHp() {
	float amount = 0.04 * (float)hp_max;
	amount = (amount < 1.0f) ? 1.0f : amount;
	Heal(amount, false);
}
void Player::RestMp() {
	//2 sec for every 1 mana
	//restore rate is 4 sec , thats why we have 2 mp
	float amount = 2.0f;
	mp +=amount;
	if (mp > mp_max) {
		mp = mp_max;
	}
}

void Player::SaveData() {
	string d_posx, d_posy, d_posz, d_level, d_hp, d_mp, d_exp, d_exp_max;
	M3DVector3f origin;
	object->GetOrigin(origin);
	d_posx = FloatToString(object->GetOriginX());
	d_posy = FloatToString(object->GetOriginY());
	d_posz = FloatToString(object->GetOriginZ());
	d_level = IntToString(level);
	d_hp = IntToString(hp);
	d_mp = IntToString(mp);
	d_exp = IntToString(exp);
	d_exp_max = IntToString(exp_max);
	
	std::string sql = "update Players set posx = "+d_posx+ ",";
				sql += "posy = "+d_posy +",";
				sql += "posz = "+d_posz +",";
				sql += "level = "+d_level+",";
				sql += "hp = "+d_hp+"," ;
				sql += "mp = "+d_mp+",";
				sql += "exp = "+d_exp+",";
				sql += "exp_max = "+d_exp_max;
				sql += " where player_id = " + db_id;
	
	dbPlayers->Query(sql);
}

bool Player::IsInDist(SPEntity *obj, float distSquared) {
	M3DVector3f selfOrigin, objOrigin;
	object->GetOrigin(selfOrigin);
	obj->GetOrigin(objOrigin);
	float dist = m3dGetDistanceSquared3(selfOrigin,objOrigin);
	return distSquared >= dist;

}

void Player::SetStartingStats() {
	growthAgi = growthInt = growthStr = growthDef = 0;
	growthVit = 1;
	growthAgi = 1;
	baseWis = 1;
	
	switch (class_id) {
		case 1: //Pally
			growthStr = 1;
			growthVit +=1;
			growthDef = 1;
			
			baseInt = 1;
			baseStr = 4;
			baseAgi = 2;
			baseVit = 3;
			baseDef = 2;
			
			break;
			
		case 2: //Wizard
			growthInt = 3;
			
			baseInt = 3;
			baseStr = 2;
			baseAgi = 1;
			baseVit = 1;
			baseDef = 2;
			
			break;

		case 3: //Bard
			growthAgi = 2;
			growthInt = 1;
			
			baseInt = 2;
			baseStr = 4;
			baseAgi = 2;
			baseVit = 2;
			baseDef = 2;
			
			break;
			
		case 4: // Druid
			growthInt = 2;
			growthAgi = 1;
			
			baseInt = 2;
			baseStr = 3;
			baseAgi = 1;
			baseVit = 2;
			baseDef = 2;
			
			break;
			
		default:
			break;
	}
		
}

bool Player::MissAbility(Enemy *enemy) {
	UpdateStats();
	int levelDiff = enemy->level - Wis;
	if (levelDiff < 0) {
		return false;
	}
	
	//they are higher level so we need to figure out if we miss or not.
	float diffRate = (levelDiff+1) * 0.15;
	float num = unifRand(0,1);
	if (num < diffRate) {
		return true;
	}
	
	return false;
}
