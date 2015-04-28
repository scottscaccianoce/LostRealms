/*
 *  emitters.h
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 1/5/13.
 *  Copyright 2013 Scotronics. All rights reserved.
 *
 */

#ifndef __EMITTERS_H__
#define __EMITTERS_H__

#include "particle_system.h"
#include "SPEntity.h"
#include "shader.h"
#include "spMesh.h"
#include "texture.h"
#include "billboard.h"


struct Line {
	M3DVector3f start;
	M3DVector3f stop;
};

class ExplosionEmitter : public Emitter {
public:
	ExplosionEmitter(int count, float max_t , M3DVector4f color_vec, M3DVector4f color_vec2, int eIndex );
	~ExplosionEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	M3DVector4f color, color2;
	int enemy_index;
	
};


class VerticalEmitter : public Emitter {
public:
	VerticalEmitter(int count, float max_t , M3DVector4f color_vec, M3DVector4f color_vec2 , int ind);
	~VerticalEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	M3DVector4f color, color2;
	int mIndex;
	
};


class SpinCircleEmitter : public Emitter {
public:
	SpinCircleEmitter(int count, float max_t , M3DVector4f color_vec, int index, bool isPlayer = true);
	~SpinCircleEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	M3DVector4f color;
	int player_index;
	bool isPlayer;
};



class SlashEmitter : public Emitter {
public:
	SlashEmitter( float max_t , M3DVector4f color_vec, int index);
	~SlashEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	M3DVector4f colors[4];
	int mIndex;
	M3DVector3f pMin,pMax;
	std::vector<Line> lines;
	float rot;
};

class PowerSongEmiter : public Emitter {
public:
	PowerSongEmiter( float max_t, int index);
	~PowerSongEmiter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int player_index;
	SPEntity *entity;
	float angle;
};


class SpeedSongEmiter : public Emitter {
public:
	SpeedSongEmiter( float max_t, int index);
	~SpeedSongEmiter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int player_index;
	SPEntity *entity;
	float angle;
};



class HorizontalRingEmiter : public Emitter {
public:
	HorizontalRingEmiter( float max_t, int index, sTexture *texture);
	~HorizontalRingEmiter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	bool bPlayer;
	int mIndex;
	sTexture *tex;
	Billboard *quad;
	SPEntity *entity;
	M3DVector3f colorKey;
};


class BardShoutEmiter : public Emitter {
public:
	BardShoutEmiter( float max_t, int index, sTexture *texture);
	~BardShoutEmiter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int mIndex;
	sTexture *tex;
	Billboard *quad;
	
};

class StunEmiter : public Emitter {
public:
	StunEmiter( float max_t, int index);
	~StunEmiter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int mIndex;
	sTexture *tex;
	Billboard *quad;

	
};

class MezEmiter : public Emitter {
public:
	MezEmiter( float max_t, int index);
	~MezEmiter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int mIndex;
	sTexture *tex;
	Billboard *quad;
	int enemy_hp;
	
};



class SlashRoundEmitter : public Emitter {
public:
	SlashRoundEmitter( float max_t , M3DVector4f color_vec, int index, float heightoffset = 0.5f);
	~SlashRoundEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int mIndex;
	M3DVector3f pMin,pMax;
	std::vector<Line> lines;
	float rot;
	float height;
	M3DVector4f color;
};




class TauntEmitter : public Emitter {
public:
	TauntEmitter( float max_t, int index);
	~TauntEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int mIndex;
	sTexture *tex;
	Billboard *quad;
	
	
};


class GroupDefEmitter : public Emitter {
public:
	GroupDefEmitter( float max_t, int index);
	~GroupDefEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int mIndex;
	sTexture *tex;
	Billboard *quad;
	
	
};

class WingsEmitter : public Emitter {
public:
	WingsEmitter( float max_t, int index);
	~WingsEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int mIndex;
	sTexture *tex;
	Billboard *quad;
	
	
};


class BuffEmitter : public Emitter {
public:
	BuffEmitter( float max_t, int index, sTexture *texture);
	~BuffEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int mIndex;
	sTexture *tex;
	Billboard *quad;
	
	
};



class HolyAttackEmitter : public Emitter {
public:
	HolyAttackEmitter( float max_t, int index);
	~HolyAttackEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int mIndex;
	M3DVector3f pMin,pMax;
	std::vector<Line> lines;
	float rot;
};

class HolyArmorEmitter : public Emitter {
public:
	HolyArmorEmitter( float max_t, int index);
	~HolyArmorEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int mIndex;
	sTexture *tex;
	Billboard *quad;
	
	
};

class HealChantEmitter : public Emitter {
public:
	HealChantEmitter( float max_t, int index);
	~HealChantEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int mIndex;
	sTexture *tex;
	Billboard *quad;
	
	
};

class IceBoltEmitter : public Emitter {
public:
	IceBoltEmitter( int aindex, int tindex);
	~IceBoltEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int aIndex, tIndex;
	SPEntity *entity;
	float speed;
	
};

class FireBoltEmitter : public Emitter {
public:
	FireBoltEmitter( int aindex, int tindex);
	~FireBoltEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int aIndex, tIndex;
	SPEntity *entity;
	float speed;
	
};


class RockArmorEmitter : public Emitter {
public:
	RockArmorEmitter( float max_t, int index);
	~RockArmorEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int mIndex;
	sTexture *tex;
	Billboard *quad;
	
	
};




class GroundEmitter : public Emitter {
public:
	GroundEmitter( float max_t, int index, bool isPlayer, sTexture *texture, float initScale = 1.0f);
	~GroundEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	bool bPlayer;
	int mIndex;
	Billboard *quad;
	SPEntity *entity;
};

class LifeTapEmitter : public Emitter {
public:
	LifeTapEmitter( int index);
	~LifeTapEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int tIndex;
	float speed;
	SPEntity *entity;
};

class LightingEmitter : public Emitter {
public:
	LightingEmitter( float max_t , int aindex, int tindex);
	~LightingEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int aIndex, tIndex;
	M3DVector3f pMin,pMax;
	M3DVector4f color;
	std::vector<Line> lines;
	
};

class AntiVerticalEmitter : public Emitter {
public:
	AntiVerticalEmitter(int count, float max_t , M3DVector4f color_vec, M3DVector4f color_vec2 , int ind);
	~AntiVerticalEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	M3DVector4f color, color2;
	int mIndex;
	
};

class HurricaneEmitter : public Emitter {
public:
	HurricaneEmitter( int aindex, int tindex);
	~HurricaneEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	
	int aIndex,tIndex;
	Billboard *quads[6];
	int num;
	
};

class HolySwordEmitter : public Emitter {
public:
	HolySwordEmitter( float max_t, int index);
	~HolySwordEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	int mIndex;
	sTexture *tex;
	Billboard *quad;
	
	
};

class TreeEmitter : public Emitter {
public:
	TreeEmitter( float max_t, M3DVector3f pos, SPMesh *mesh );
	~TreeEmitter();
	
	virtual void Update(float dt);
	virtual void Render();
	
	SPEntity *tree;
};


//Emitters
extern Shader explosionShader;
extern Shader verticalShader;
extern Shader billboardShader;

extern SPMesh *mPowerSong;
extern SPMesh *mSpeedSong;
extern SPMesh *mIceBolt;
extern SPMesh *mFireBolt;
extern SPMesh *mLifeTap;
extern SPMesh *mPineTree;
extern SPMesh *mBriathornTree;
extern SPMesh *mOakTree;



extern sTexture mRing;
extern sTexture mRedRing;
extern sTexture mDDSpell;
extern sTexture mBardStunSpell;
extern sTexture mBardMezSpell;

extern sTexture mStunSpell;
extern sTexture mMezSpell;
extern sTexture mTauntSpell;
extern sTexture mGroupDefSpell;
extern sTexture mWingsSpell;
extern sTexture mBlessingSpell;
extern sTexture mHolyArmorSpell;
extern sTexture mHealChantSpell;
extern sTexture mBlastSpell;
extern sTexture mHolySwordSpell;

extern sTexture mFrostMasterySpell;
extern sTexture mFireMasterySpell;
extern sTexture mRockSpell;
extern sTexture mAssassinSpell;
extern sTexture mInfernoSpell;
extern sTexture mIceSpell;
extern sTexture mTranfuseSpell;
extern sTexture mBrownRing;

extern sTexture mHealCircleSpell;
extern sTexture mNatureSpell;
extern sTexture mTornsSpell;
extern sTexture mFlowerSpell;
extern sTexture mCloudSpell;
extern sTexture mHurricaneSpell;



void InitEmitterShaders();
void InitParticleModels();
void DeleteParticleModels();

#endif
