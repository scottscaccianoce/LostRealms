/*
 *  Spell.cpp
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 12/5/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "Spell.h"
#include "game.h"

Spell spells[36];

Spell::Spell(){

	recast_elapased = recast;
	elapsed = 0;
	loadedIcon = false;
}

Spell::~Spell(){
	if (loadedIcon) {
		delete icon;
	}
}

void Spell::LoadTexture(const std::string &filename ) {
	if (!loadedIcon) {
		icon = new sTexture();
		icon->loadTexture(filename);
		loadedIcon = true;
	}
	

}

bool Spell::waitingOnRecast() {
	return recast_elapased < recast;
}

bool Spell::CanCast(Enemy *TargetEnemy, Player* tAlly, int mp_amount) {
	if (spellType == 1) {
		if (  TargetEnemy == NULL) {
			castingSpell = NULL;
			return false;
		}
		M3DVector3f enemyPos, playerPos;
		TargetEnemy->GetOrigin(enemyPos);
		players[PlayerId].object->GetOrigin(playerPos);
		return mp_amount >= cost && m3dGetDistanceSquared3(enemyPos, playerPos) <= (range * range);
	} else if (spellType ==2) {
		
		//If we didn't select an ally we return if casting rez
		if (tAlly == NULL && db_id == 12) {
			return false;
		}
		
		//if not rez then we target ourself
		if (tAlly == NULL ) {
			tAlly = &players[PlayerId];
		}
		
		M3DVector3f AllyPos, playerPos;
		players[PlayerId].object->GetOrigin(playerPos);
		tAlly->object->GetOrigin(AllyPos);
		return mp_amount >= cost && m3dGetDistanceSquared3(AllyPos, playerPos) <= (range * range);
		
	}else if (spellType ==3) {
		return mp_amount >= cost;
	}
	return false;
}
//GAME

void Game::ResetCurrentCasting() {
	if (castingSpell != NULL) {
		castingSpell->elapsed = 0;
		castingSpell = NULL;
		castBar->Hide();
	}

}

void Game::StartCasting() {
	if (castingSpell != NULL) {
		if (castingSpell->spellType == 1 && TargetEnemy == NULL) {
			castingSpell = NULL;
			return;
		}
		
		
		castingSpell->elapsed = 0;
		castBar->SetPercentage(0.0f);
		
		castBar->Show();
	}
	
}

void Game::UpdateCasting(float dt) {
	if (castingSpell != NULL) {
		castingSpell->elapsed += dt;
		//We don't want to get mana while casting or wizard is op
		players[PlayerId].lastRestore = 0;
		
		bool valid = true;
		if (castingSpell->spellType ==1 && enemy_objects[currentCastingEnemyIndex]->hp <= 0) {
			castingSpell = NULL;
			valid = false;
		}
		//check rez
		if (castingSpell->db_id == 12 && currentCastingPlayerIndex == PlayerId) {
			valid = false;
			castingSpell = NULL;
		}
		if (valid) {
			if (castingSpell->elapsed >= castingSpell->cast_time) {
				
				if (castingSpell->spellType == 1 && players[PlayerId].MissAbility(enemy_objects[currentCastingPlayerIndex])) {
				
					combatLog->AddItem("Your spell fizzles", TEAL);
				} else {
					combatLog->AddItem("You Cast " + castingSpell->name, TEAL);
					

					//send packet that spell was casted successfully
					GamePacket g;
					g.packet_id = 23;
					g.player_id = PlayerId;
					g.data1 = castingSpell->db_id;
					g.data2 = currentCastingEnemyIndex;
					g.data3 = currentCastingPlayerIndex;
					
					players[PlayerId].UpdateStats();
					if (castingSpell->spellType == 1 || castingSpell->spellType == 2) {
						g.hp = castingSpell->base_dmg * players[PlayerId].level + players[PlayerId].Int * 2;
					} else {
						
						//check for oak tree or group heal or pine tree or heal chant pally
						if (castingSpell->db_id == 36 || castingSpell->db_id == 29 || castingSpell->db_id == 15 || castingSpell->db_id == 34 ) {
							g.hp = castingSpell->base_dmg * players[PlayerId].level + players[PlayerId].Int * 2;
						} else {
							if (castingSpell->base_dmg == 5) {
								g.hp = players[PlayerId].level / 2;
							}else if (castingSpell->base_dmg == 3) {
								g.hp = players[PlayerId].level / 3;
							}
							//if we get a zero (for low levels we just make it 1)
							g.hp = g.hp == 0 ? 1 : g.hp;						}
					}
					
					
					g.hpmax = castingSpell->duration;
					g.mp = castingSpell->cost;
					gPacketManager.AddPacket(g);
					nHost->SendBroadcast(g);
				}
				
				castBar->Hide();
				castingSpell->recast_elapased = 0;
				ResetCurrentCasting();
				
			} else {
				castBar->SetPercentage(castingSpell->elapsed / castingSpell->cast_time);
			}	
		}
	}
	
	for (int i =0; i < 6; ++i) {
	
		if (player_spells[i]->waitingOnRecast()) {
			player_spells[i]->recast_elapased += dt;
			 
			for (std::map<int,int>::iterator itr = spellMapping.begin(); itr != spellMapping.end(); itr++) {
				if (i == itr->second) {
					float value = ceilf((player_spells[i]->recast - player_spells[i]->recast_elapased)*10)/10;
					recastTimers[itr->first]->SetText(FloatToString(value));
					recastTimers[itr->first]->Show();
					spellSlots[itr->first]->Hide();

				}
			}
						
		} else {
			for (std::map<int,int>::iterator itr = spellMapping.begin(); itr != spellMapping.end(); itr++) {
				if (i == itr->second) {
					spellSlots[itr->first]->Show();
					recastTimers[itr->first]->Hide();
					
				}
			}
			
		}
		
		
	}

}

void Game::RenderRecast() {

}
bool Game::HandleTargetEnemySpell(int sId, Enemy *tEnemy, int pIndex, int amount, int tIndex) {
	
	bool killed = false;
	bool isDamage = true;
	
	if (spells[sId].db_id == 5 || spells[sId].db_id == 6 || spells[sId].db_id == 23 || spells[sId].db_id == 10) {
		isDamage = false;
	}

	if (isDamage) {
	//------------ IF SPELL IS DAMAGE-----------------------------		
		
		
		if (tEnemy->hp > 0) { // Make sure monster is alive, might have been killed by someone else
			//If you are the player lets log this
			if (pIndex == PlayerId) {
				combatLog->AddItem(spells[sId].name + " hits " + tEnemy->name + " for " + IntToString(amount), TEAL);
			}
			
			if (tEnemy->TakeDamage(amount, pIndex)) {
				
				killed = true;
				if (TargetEnemy == tEnemy) {
					TargetEnemy = NULL;
					players[pIndex].ToggleAttack();
				}
				
				//if close enough you get exp
				float PlayerDistSqrd = 50 * 50;
				M3DVector3f player_pos;
				players[PlayerId].object->GetOrigin(player_pos);
				if (tEnemy->isCloseForPlayerToAttack(player_pos, PlayerDistSqrd)) {
					players[PlayerId].GainExp(tEnemy->level, 0);
				}
			}
			
			
		}
	} else {
		//--------------- If Spell is Condition -------------------
		GamePacket g;
		g.player_id = tIndex;
		g.data1 = 1;
		g.data2 = spells[sId].duration;
		
		//stuns
		if (spells[sId].db_id == 5) {
			g.packet_id = 26;
		}
		else if (spells[sId].db_id == 6 || spells[sId].db_id == 23) {
			g.packet_id = 27;
		} else if (spells[sId].db_id == 10) {
			tEnemy->Taunt(pIndex, amount);
		}
		
			nHost->SendBroadcast(g);
			gPacketManager.AddPacket(g);
	}
	
	
	
	
	
	
	
	
	return killed;
	
}
void Game::HandleTargetAllySpell(int sId, Player *Ally, int amount) {
	Ally->Heal(amount, true);
}
void Game::HandleTargetGroupSpell(int sId, int pid, int amount) {
	
	Buff b(sId);
	b.Init(spells[sId].duration, spells[sId].tick, (spells[sId].forever && PlayerId == pid), spells[sId].passive, amount);
	switch (spells[sId].db_id) {

// --- BARD ---------------------------
		case 1: //Power Song
			b.SetMp(amount);
			break;
		case 2: //Run Song
			b.SetSpeed(1.5f);
			break;
		case 3: //Bladeturn Song
			players[PlayerId].bladeturn = true;
			break;
		case 7: //Passive Assassin Buff
			b.SetAgi(amount);
			b.SetInt(amount);
			break;
			
// --- Sorcerress ---------------------------
		case 21: //Fire Mastery
			b.SetInt(amount);
			break;
		case 24: //Ice Mastery
			b.SetInt(amount);
			break;
		case 26: //Rock armor
			b.SetDef(amount);
			break;
		case 27: //Rock armor
			b.SetWis(1);
			break;

// --- DRUID ---------------------------
		case 29: //Regrowth
			players[PlayerId].Heal(amount, true);
			break;
		case 30: //Nature
			b.SetInt(amount);
			b.SetAgi(amount);
			b.SetStr(amount);
			b.SetWis(1);
			b.SetVit(amount);
			break;
		case 34: //Pine
		{
			//players[PlayerId].sucuplant = true;
			//players[PlayerId].sucuplant_amount = amount;

			
			b.SetPulse(true);
			b.SetPulseAmount(amount);
			b.SetPulseRange(15);
			M3DVector3f p;
			players[pid].object->GetOrigin(p);
			b.SetPulsePos(p);
			
			break;		
		}
		case 35: //BriaTorns
		{
			//players[PlayerId].torns = true;
			//players[PlayerId].torns_amount = amount;
			b.SetPulse(true);
			b.SetPulseAmount(amount);
			b.SetPulseRange(15);
			M3DVector3f p;
			players[pid].object->GetOrigin(p);
			b.SetPulsePos(p);
		}	
		case 36: //Oak
		{	
			b.SetPulse(true);
			b.SetPulseAmount(amount);
			b.SetPulseRange(15);
			M3DVector3f p;
			players[pid].object->GetOrigin(p);
			b.SetPulsePos(p);
			
			//players[PlayerId].torns = true;
			//players[PlayerId].torns_amount = amount;
			
			break;		
		}
// --- Pally ---------------------------
		case 11: //Group Armor Buff
			b.SetDef(amount);
		case 13: //Group Armor Buff
			b.SetStr(amount);
			break;
		case 14: //self buff
			b.SetDef(amount);
			break;
		case 15: //heal chant
			b.SetHp(amount);
			break;		
		case 17: //haste
			b.SetHaste();
			break;
		case 18: //passive st
			b.SetStr(amount);
			break;
	
		default:
			break;
	}
	
	gBuffs[sId] = b;
}

void Game::UpdateBuffs(float dt) {
	std::map < int, Buff>::iterator iter;
	for (iter = gBuffs.begin(); iter != gBuffs.end(); ++iter) {
		iter->second.Update(dt);
	}
}

void Game::RenderBuffs() {
	
	int b = 0;
	std::map < int, Buff>::iterator iter;
	for (iter = gBuffs.begin(); iter != gBuffs.end(); ++iter) {
		if (iter->second.isAlive()) {
			//Special check for bladeturn
			if (spells[iter->second.spellIndex].db_id == 3) {
				if (players[PlayerId].bladeturn) {
					bufficons[b].SetTexture(spells[iter->second.spellIndex].icon);
					bufficons[b].Render();
				}
			} else {
				bufficons[b].SetTexture(spells[iter->second.spellIndex].icon);
				bufficons[b].Render();

			}
		}
		b++;
	}
}
void Game::CreateSpellEmitter(int spell_id, int tIndex, int aIndex, int spell_array_index) {
	
	switch (spell_id) {
			
//---BARD--------------------------------
		case 1:
		{
			//Check if this buff is already on
			std::map < int, Buff>::iterator iter = gBuffs.find(spell_array_index);
			if (iter != gBuffs.end()) {
				break;
			}
			//Power song
			M3DVector4f color;
			color[0] = 0.0f;
			color[1] = 0.0f;
			color[2] = 1.0f;
			color[3] = 1.0f;
			
			SpinCircleEmitter *emitter = new SpinCircleEmitter(1200,3,color, aIndex);
			gEmitters.push_back(emitter);
			
			PowerSongEmiter *pEmitter = new PowerSongEmiter(3,aIndex);
			gEmitters.push_back(pEmitter);
			
			break;
		}
		case 2:
		{
			std::map < int, Buff>::iterator iter = gBuffs.find(spell_array_index);
			if (iter != gBuffs.end()) {
				break;
			}
			//Run song
			M3DVector4f color;
			color[0] = 0.1f;
			color[1] = 0.7f;
			color[2] = 0.3f;
			color[3] = 1.0f;
			
			SpinCircleEmitter *emitter = new SpinCircleEmitter(1200,2,color, aIndex);
			gEmitters.push_back(emitter);
			
			SpeedSongEmiter *pEmitter = new SpeedSongEmiter(3,aIndex);
			gEmitters.push_back(pEmitter);
			
			break;
		}
		case 3:
		{
			std::map < int, Buff>::iterator iter = gBuffs.find(spell_array_index);
			if (iter != gBuffs.end()) {
				break;
			}
			//blade song
			M3DVector4f color;
			color[0] = 1.0f;
			color[1] = 1.0f;
			color[2] = 0.3f;
			color[3] = 1.0f;
			
			SpinCircleEmitter *emitter = new SpinCircleEmitter(1200,2,color, aIndex);
			gEmitters.push_back(emitter);
			
			break;
		}
			
		case 4:
		{
			//DD shout
		
			BardShoutEmiter *remitter = new BardShoutEmiter(0.5,tIndex, &mDDSpell);
			gEmitters.push_back(remitter);
			
			break;
		}
		case 5:
		{
			//Stun
			M3DVector4f color;
			color[0] = 1.0f;
			color[1] = 1.0f;
			color[2] = 1.0f;
			color[3] = 1.0f;
			
			//SpinCircleEmitter *emitter = new SpinCircleEmitter(1200,spells[spell_array_index].duration,color,tIndex, false);
			StunEmiter *emitter = new StunEmiter(spells[spell_array_index].duration,tIndex);
			gEmitters.push_back(emitter);
			
			BardShoutEmiter *remitter = new BardShoutEmiter(0.5,tIndex, &mBardStunSpell);
			gEmitters.push_back(remitter);

			
			break;
		}
		case 6:
		{
			//mez
			M3DVector4f color;
			color[0] = 0.95f;
			color[1] = 0.11f;
			color[2] = 0.80f;
			color[3] = 1.0f;
			
			//SpinCircleEmitter *emitter = new SpinCircleEmitter(1200,spells[spell_array_index].duration,color,tIndex, false);
			MezEmiter *emitter = new MezEmiter(spells[spell_array_index].duration,tIndex);
			gEmitters.push_back(emitter);
			
			BardShoutEmiter *remitter = new BardShoutEmiter(0.5,tIndex, &mBardMezSpell);
			gEmitters.push_back(remitter);

			
			break;
		}
		case 7:
		{
			//Assassin Buff
			
			BuffEmitter *semitter = new BuffEmitter(3.0f,aIndex, &mAssassinSpell);
			gEmitters.push_back(semitter);
			break;
		} 
		case 8:
		{
			//slash
			M3DVector4f color;
			color[0] = 0.58f;
			color[1] = 0.50f;
			color[2] = 0.56f;
			color[3] = 1.0f;
			
			SlashEmitter *semitter = new SlashEmitter(0.5f,color,tIndex);
			gEmitters.push_back(semitter);
			
			break;
			
		}
		case 9:
		{
			//Long slash Swipe
			M3DVector4f color;
			//color[0] = 0.58f;
			//color[1] = 0.50f;
			//color[2] = 0.56f;
			//color[3] = 1.0f;
			color[0] = 1.0f;
			color[1] = 0.0f;
			color[2] = 0.0f;
			color[3] = 1.0f;
			
			SlashRoundEmitter *semitter = new SlashRoundEmitter(1.0f,color,tIndex);
			gEmitters.push_back(semitter);
			
			break;
			
		}
			
		case 10:
		{
			//taunt
			TauntEmitter *semitter = new TauntEmitter(0.5f,tIndex);
			gEmitters.push_back(semitter);
			
			break;
			
		}
			
		case 11:
		{
			//group Buff
			GroupDefEmitter *semitter = new GroupDefEmitter(3.0f,aIndex);
			gEmitters.push_back(semitter);
			
			M3DVector4f color;
			color[0] = 0.57f;
			color[1] = 0.41f;
			color[2] = 0.27f;
			color[3] = 1.0f;
			
			//SpinCircleEmitter *emitter = new SpinCircleEmitter(1200,3,color, aIndex);
			//gEmitters.push_back(emitter);

			
			break;
			
		}
		case 13:
		{
			//Stun
			HolySwordEmitter *emitter = new HolySwordEmitter(spells[spell_array_index].duration, aIndex);
			gEmitters.push_back(emitter);
						
			break;
		}
		case 14:
		{
			//Holy Armor
			
			HolyArmorEmitter *emitter = new HolyArmorEmitter(spells[spell_array_index].duration,aIndex);
			gEmitters.push_back(emitter);
			
			break;
		}
		case 15:
		{
			//Heal chant
			
			HealChantEmitter *emitter = new HealChantEmitter(spells[spell_array_index].duration,aIndex);
			gEmitters.push_back(emitter);
			
			break;
		}
			
		case 16:
		{
			//Righteous blast
			HolyAttackEmitter *emitter = new HolyAttackEmitter(0.5f, tIndex);
			gEmitters.push_back(emitter);
			
			
			
			break;
			
		}	
		case 17:
		{
			//Wings haste Buff
			WingsEmitter *semitter = new WingsEmitter(spells[spell_array_index].duration,aIndex);
			gEmitters.push_back(semitter);
						
			break;
			
		}	
			
		case 18:
		{
			//Self Blessing Buff
			BuffEmitter *semitter = new BuffEmitter(3.0f,aIndex, &mBlessingSpell);
			gEmitters.push_back(semitter);
			
			break;
			
		}	
		case 19:
		{
			//Fire Bolt
			FireBoltEmitter *semitter = new FireBoltEmitter(aIndex, tIndex);
			gEmitters.push_back(semitter);
			
			break;
			
		}	
		case 20:
		{
			BardShoutEmiter *remitter = new BardShoutEmiter(0.5,tIndex, &mInfernoSpell);
			gEmitters.push_back(remitter);
			break;
		}
		case 21:
		{
			//Fire Mastery Buff
			BuffEmitter *semitter = new BuffEmitter(3.0f,aIndex, &mFireMasterySpell);
			gEmitters.push_back(semitter);
			
			break;
			
		}	
		case 22:
		{
			//Ice Bolt
			IceBoltEmitter *semitter = new IceBoltEmitter(aIndex, tIndex);
			gEmitters.push_back(semitter);
			
			break;
			
		}	
		case 23:
		{
			//Freeze
			GroundEmitter *semitter = new GroundEmitter(1.0f, tIndex, false, &mIceSpell, 1.3f);
			gEmitters.push_back(semitter);
			
			MezEmiter *emitter = new MezEmiter(spells[spell_array_index].duration,tIndex);
			gEmitters.push_back(emitter);

			
			break;
			
		}	
			
		case 24:
		{
			//Frost Mastery Buff
			BuffEmitter *semitter = new BuffEmitter(3.0f,aIndex, &mFrostMasterySpell);
			gEmitters.push_back(semitter);
			
			break;
			
		}	
		case 25:
		{
			//Life Steal
			LifeTapEmitter *semitter = new LifeTapEmitter(tIndex);
			gEmitters.push_back(semitter);
			
			break;
			
		}	
			
		case 26:
		{
			//Rock Armor
			
			BuffEmitter *emitter = new BuffEmitter(3,aIndex, &mRockSpell);
			gEmitters.push_back(emitter);
			
			break;
		}
		case 27:
		{
			//tranfuse
			
			GroundEmitter *emitter = new GroundEmitter(1.5,aIndex,true, &mTranfuseSpell);
			gEmitters.push_back(emitter);
			
			M3DVector4f color;
			color[0] = 0.66f;
			color[1] = 0.46f;
			color[2] = 0.24f;
			color[3] = 1.0f;
			
			VerticalEmitter *remitter = new VerticalEmitter(2200,1.5,color,color, aIndex);
			gEmitters.push_back(remitter);
			
			break;
		}
			
		case 28:
		case 29:
		{
			//heal
			
			GroundEmitter *emitter = new GroundEmitter(1.5,aIndex,true, &mHealCircleSpell );
			gEmitters.push_back(emitter);
			
			M3DVector4f color;
			color[0] = 0.0f;
			color[1] = 0.5f;
			color[2] = 0.0f;
			color[3] = 1.0f;
			
			VerticalEmitter *remitter = new VerticalEmitter(1200,1.5,color,color, aIndex);
			gEmitters.push_back(remitter);
			
			break;
		}
			
		case 30:
		{
			//Nature buff
			
			BuffEmitter *emitter = new BuffEmitter(3,aIndex, &mNatureSpell);
			gEmitters.push_back(emitter);
			
			break;
		}
		case 31:
		{
			//Healing Rain
			
			BuffEmitter *emitter = new BuffEmitter(1.5,aIndex, &mCloudSpell);
			gEmitters.push_back(emitter);
			
			M3DVector4f color;
			color[0] = 0.24f;
			color[1] = 0.33f;
			color[2] = 0.66f;
			color[3] = 1.0f;
			
			AntiVerticalEmitter *remitter = new AntiVerticalEmitter(1200,1.5,color,color, aIndex);
			gEmitters.push_back(remitter);

			
			
			break;
		}
		case 32:
		{
			//Hurricane 
			
			HurricaneEmitter *emitter = new HurricaneEmitter(aIndex, tIndex);
			gEmitters.push_back(emitter);
			
			break;
		}
		case 33:
		{
			//Lighting 
			
			LightingEmitter *emitter = new LightingEmitter(0.5,aIndex, tIndex);
			gEmitters.push_back(emitter);
			
			break;
		}
			
		case 34:
		{
			//Pine Tree
			
			M3DVector3f pos;
			players[aIndex].object->GetOrigin(pos);
			TreeEmitter *emitter = new TreeEmitter(spells[spell_array_index].duration, pos, mPineTree);
			gEmitters.push_back(emitter);
			break;
		}
		case 35:
		{
			//Briathorn Tree
			M3DVector3f pos;
			players[aIndex].object->GetOrigin(pos);
			TreeEmitter *emitter = new TreeEmitter(spells[spell_array_index].duration, pos, mPineTree);
			gEmitters.push_back(emitter);
			
			break;
		}
		case 36:
		{
			//Oak Tree
			M3DVector3f pos;
			players[aIndex].object->GetOrigin(pos);
			TreeEmitter *emitter = new TreeEmitter(spells[spell_array_index].duration, pos, mPineTree);
			gEmitters.push_back(emitter);
			break;
		}	
		default:
			break;
	}
	
	
	
	
	

}