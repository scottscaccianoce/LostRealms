/*
 *  Buff.h
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 12/5/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifndef BUFF_H
#define BUFF_H
#include "Spell.h"


class Buff {
public:
	Buff();
	Buff(int sIndex);
	void Init(int buff_duration, int t_duration, bool lasting, bool pass, int a );
	~Buff();
	void Update(float dt);
	
	void SetInt(int value) { mInt = value;}
	void SetStr(int value) { mStr = value;}
	void SetAgi(int value) { mAgi = value;}
	void SetWis(int value) { mWis = value;}
	void SetDef(int value) { mDef = value;}
	void SetVit(int value) { mVit = value;}
	void SetHp(int value) { mHp = value;}
	void SetMp(int value) { mMp = value;}
	void SetForever(bool value) { forever = value;}

	void SetSpeed(int value);
	void SetPulse(bool value) {mPulse = value;}
	void SetPulseAmount(int value) {mPulseAmount = value;}
	void SetPulseRange(float value) {mPulseRange = value;}
	void SetPulsePos(M3DVector3f value) {m3dCopyVector3(mPulsePos, value);}
	void SetPulseOwner(int value) {mPulseOwner = value;}
	void SetHaste();
	void DoPulse();
	
	
	
	int GetInt() { return alive ? mInt : 0;}
	int GetStr() { return alive ? mStr : 0;}
	int GetAgi() { return alive ? mAgi : 0;}
	int GetWis() { return alive ? mWis : 0;}
	int GetDef() { return alive ? mDef : 0;}
	int GetVit() { return alive ? mVit : 0;}
	bool isAlive() {return alive; }
	
	int spellIndex;
private:
	//Used to determin total time of buff.  
	float duration; 
	float elapsed;
	
	//Used for minor ticks like hp and mp regen
	// These may or may not be used
	float tick_duration;
	float tick_elapsed;
	
	//stats
	int mInt;
	int mStr;
	int mAgi;
	int mWis;
	int mDef;
	int mVit;
	
	//Special buffs
	int mSpeed;
	bool mPulse;
	int mPulseAmount;
	float mPulseRange;
	M3DVector3f mPulsePos;
	int mPulseOwner;
	
	int mHp;
	int mMp;
	

	
	bool alive;
	bool forever;
	bool passive;
	bool haste;
	
	int amount;  //used for recasting buffs
};


extern std::map<int, Buff> gBuffs;



#endif