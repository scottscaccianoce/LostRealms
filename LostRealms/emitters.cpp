/*
 *  emitters.cpp
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 1/5/13.
 *  Copyright 2013 Scotronics. All rights reserved.
 *
 */

#include "emitters.h"
#include "util.h"
#include "game.h"

/*  
----------------------------------------------------
	EXPLOSION EMITTER
----------------------------------------------------
 //Need Explosion shader
 sphere.vs
 sphere.ps
*/

Shader explosionShader;
Shader verticalShader;
Shader billboardShader;

SPMesh *mPowerSong;
SPMesh *mSpeedSong;
SPMesh *mIceBolt;
SPMesh *mFireBolt;
SPMesh *mLifeTap;
SPMesh *mPineTree;
SPMesh *mBriathornTree;
SPMesh *mOakTree;

sTexture mRing;
sTexture mRedRing;
sTexture mDDSpell;
sTexture mBardStunSpell;
sTexture mBardMezSpell;
sTexture mStunSpell;
sTexture mMezSpell;
sTexture mTauntSpell;
sTexture mGroupDefSpell;
sTexture mWingsSpell;
sTexture mBlessingSpell;
sTexture mHolyArmorSpell;
sTexture mHealChantSpell;
sTexture mFrostMasterySpell;
sTexture mFireMasterySpell;
sTexture mRockSpell;
sTexture mAssassinSpell;
sTexture mInfernoSpell;
sTexture mIceSpell;
sTexture mTranfuseSpell;
sTexture mHealCircleSpell;
sTexture mNatureSpell;
sTexture mTornsSpell;
sTexture mFlowerSpell;
sTexture mCloudSpell;
sTexture mHurricaneSpell;
sTexture mBrownRing;
sTexture mBlastSpell;
sTexture mHolySwordSpell;

void InitEmitterShaders() {
	explosionShader.Load("sphere.vs", "sphere.fs");
	verticalShader.Load("vertical.vs", "sphere.fs");	
	billboardShader.Load("billboard.vs", "billboard.fs");
}

void InitParticleModels() {
	mPowerSong = new SPMesh();
	mPowerSong->ImportMesh("powersong.spm","");
	mSpeedSong = new SPMesh();
	mSpeedSong->ImportMesh("speedsong.spm","");
	mIceBolt = new SPMesh();
	mIceBolt->ImportMesh("icebolt.spm", "");
	mFireBolt = new SPMesh();
	mFireBolt->ImportMesh("firebolt.spm", "");
	mLifeTap = new SPMesh();
	mLifeTap->ImportMesh("lifetap.spm", "");
	
	mPineTree = new SPMesh();
	mPineTree->ImportMesh("tree.spm", "");
	mBriathornTree = new SPMesh();
	mBriathornTree->ImportMesh("tree.spm", "");	
	mOakTree = new SPMesh();
	mOakTree->ImportMesh("tree.spm", "");
	
	mRing.loadTexture("ring_particle.tga");
	mRedRing.loadTexture("red_ring.tga");
	mBrownRing.loadTexture("brown_ring.tga");
	mDDSpell.loadTexture("ddspell.tga");	
	mBardStunSpell.loadTexture("bardstun.tga");	
	mBardMezSpell.loadTexture("bardmez.tga");	
	mStunSpell.loadTexture("stunspell.tga");	
	mMezSpell.loadTexture("mezspell.tga");	
	mTauntSpell.loadTexture("tauntspell.tga");	
	mGroupDefSpell.loadTexture("groupdefensespell.tga");	
	mWingsSpell.loadTexture("wingsspell.tga");
	mBlessingSpell.loadTexture("blessingspell.tga");
	mHolyArmorSpell.loadTexture("defensebuffSpell.tga");
	mHealChantSpell.loadTexture("healchantSpell.tga");
	mFrostMasterySpell.loadTexture("frost_masteryspell.tga");
	mFireMasterySpell.loadTexture("fire_masteryspell.tga");
	mRockSpell.loadTexture("rockspell.tga");
	mAssassinSpell.loadTexture("assassinspell.tga");
	mInfernoSpell.loadTexture("infernospell.tga");
	mIceSpell.loadTexture("icespell.tga");
	mTranfuseSpell.loadTexture("tranfusespell.tga");
	mHealCircleSpell.loadTexture("healcircle.tga");
	mNatureSpell.loadTexture("naturespell.tga");
	mFlowerSpell.loadTexture("flowerspell.tga");
	mTornsSpell.loadTexture("tornsspell.tga");
	mCloudSpell.loadTexture("cloud.tga");
	mHurricaneSpell.loadTexture("hurricanespell.tga");
	mBlastSpell.loadTexture("rblastspell.tga");
	mHolySwordSpell.loadTexture("holyswordspell.tga");
}

void DeleteParticleModels() {
	delete mPowerSong;
	delete mSpeedSong;
	delete mIceBolt;
	delete mFireBolt;
	delete mLifeTap;
	delete mPineTree;
	delete mBriathornTree;
	delete mOakTree;
}
ExplosionEmitter::ExplosionEmitter(int count, float max_t, M3DVector4f color_vec, M3DVector4f color_vec2, int eIndex) {
	number_of_particles = count;
	particles.resize(number_of_particles);
	max_time = max_t;
	elapsed_time = 0;
	particleShader = NULL;
	m3dCopyVector4(color,color_vec);
	m3dCopyVector4(color2,color_vec2);
	enemy_index = eIndex;
	
	for (int i =0; i < number_of_particles; ++i) {
		particles[i].starting_pos[0] = 0.0f;
		particles[i].starting_pos[1] = 0.0f;
		particles[i].starting_pos[2] = 0.0f;
		
		particles[i].dir[0] = unifRand(-1,1);
		particles[i].dir[1] = unifRand(-1,1);
		particles[i].dir[2] = unifRand(-1,1);
		m3dNormalizeVector3(particles[i].dir);
		
	}
	M3DVector3f p;
	enemy_objects[enemy_index]->GetOrigin(p);
	p[1] += 0.5;
	SetEmitterPos(p);
	particleShader = &explosionShader;
}

ExplosionEmitter::~ExplosionEmitter() {particles.clear();}


void ExplosionEmitter::Update(float dt) {
	elapsed_time += dt;
	M3DVector3f p;
	enemy_objects[enemy_index]->GetOrigin(p);
	p[1] += 0.5;
	SetEmitterPos(p);

}
void ExplosionEmitter::Render() {
	glPushMatrix();
	if (particleShader != NULL) {
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
		glEnable( GL_POINT_SMOOTH );
		glEnable(GL_BLEND);
	
		//Translate into world coordnates
		M3DMatrix44f mWorld;
		m3dTranslationMatrix44(mWorld,emitter_pos[0],emitter_pos[1],emitter_pos[2]);
		glMultMatrixf(mWorld);
		
		particleShader->Begin();
		particleShader->SetUniform4f("color", color[0], color[1], color[2], 1.0f - (elapsed_time / max_time));
		particleShader->SetUniform4f("color2", color2[0], color2[1], color2[2], 1);
		
		particleShader->SetUniform1f("elapsed", elapsed_time);
		particleShader->SetUniform1f("percent", elapsed_time/max_time);
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,sizeof(Particle),&particles[0].starting_pos[0]);
	
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer( GL_FLOAT, sizeof(Particle), &particles[0].dir[0]);
	
		glDrawArrays(GL_POINTS, 0, number_of_particles);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		
		particleShader->End();
		glDisable( GL_POINT_SMOOTH );
		glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
		glDisable(GL_BLEND);
	}
	glPopMatrix();
}


/*--------------------------------------------------*/

 /*  
 ----------------------------------------------------
 Verticle EMITTER
 ----------------------------------------------------
  
  shaders ---
  vertcle.vs
  sphere.ps
 */

VerticalEmitter::VerticalEmitter(int count, float max_t, M3DVector4f color_vec, M3DVector4f color_vec2, int ind) {
	number_of_particles = count;
	particles.resize(number_of_particles);
	max_time = max_t;
	elapsed_time = 0;
	particleShader = NULL;
	m3dCopyVector4(color,color_vec);
	m3dCopyVector4(color2,color_vec2);

	
	for (int i =0; i < number_of_particles; ++i) {
		particles[i].dir[0] = 0.0f;
		particles[i].dir[1] = unifRand(0.00f,0.5);
		particles[i].dir[2] = 0.0f;
		
		particles[i].starting_pos[0] = unifRand(-1,1);
		particles[i].starting_pos[1] = 0;
		particles[i].starting_pos[2] = unifRand(-1,1);
		m3dNormalizeVector3(particles[i].starting_pos);
		m3dScaleVector3(particles[i].starting_pos, 0.45);
	}
	particleShader = &verticalShader;
	mIndex = ind;
	M3DVector3f p;
	players[mIndex].object->GetOrigin(p);

	SetEmitterPos(p);

	
}

VerticalEmitter::~VerticalEmitter() {particles.clear();}

void VerticalEmitter::Update(float dt) {
	elapsed_time += dt;
	M3DVector3f p;
	players[mIndex].object->GetOrigin(p);

	SetEmitterPos(p);
	
}
void VerticalEmitter::Render() {
	glPushMatrix();
	if (particleShader != NULL) {
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
		glEnable( GL_POINT_SMOOTH );
		glEnable(GL_BLEND);
		
		//Translate into world coordnates
		M3DMatrix44f mWorld;
		m3dTranslationMatrix44(mWorld,emitter_pos[0],emitter_pos[1],emitter_pos[2]);
		glMultMatrixf(mWorld);
		
		particleShader->Begin();
		particleShader->SetUniform4f("color", color[0], color[1], color[2], 1.0f - (elapsed_time / max_time));
		particleShader->SetUniform4f("color2", color[0], color[1], color[2], 1);
		
		particleShader->SetUniform1f("elapsed", elapsed_time);
		particleShader->SetUniform1f("percent", elapsed_time/max_time);
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,sizeof(Particle),&particles[0].starting_pos[0]);
		
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer( GL_FLOAT, sizeof(Particle), &particles[0].dir[0]);
		
		glDrawArrays(GL_POINTS, 0, number_of_particles);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		
		particleShader->End();
		glDisable( GL_POINT_SMOOTH );
		glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
		glDisable(GL_BLEND);
	}
	glPopMatrix();
}





/*--------------------------------------------------*/

/*  
 ----------------------------------------------------
 SpiningCircle EMITTER
 ----------------------------------------------------
 
 shaders ---
 sphere.vs
 sphere.ps
 */

SpinCircleEmitter::SpinCircleEmitter(int count, float max_t, M3DVector4f color_vec, int index, bool isP ) {
	number_of_particles = count;
	particles.resize(number_of_particles);
	max_time = max_t;
	elapsed_time = 0;
	particleShader = NULL;
	m3dCopyVector4(color,color_vec);
	isPlayer = isP;

	player_index = index;
	
	for (int i =0; i < number_of_particles; ++i) {
		particles[i].starting_pos[0] = 0.0f;
		particles[i].starting_pos[1] = 0.0f;
		particles[i].starting_pos[2] = 0.0f;
		
		particles[i].dir[0] = unifRand(-1,1);
		particles[i].dir[1] = unifRand(-1,1);
		particles[i].dir[2] = unifRand(-1,1);
		m3dNormalizeVector3(particles[i].dir);
		
	}
	M3DVector3f p;
	if (isPlayer) {
		players[player_index].object->GetOrigin(p);
	} else {
		enemy_objects[player_index]->GetOrigin(p);
	}
	
	p[1] += 0.5;
	SetEmitterPos(p);
	particleShader = &explosionShader;
}

SpinCircleEmitter::~SpinCircleEmitter() {particles.clear();}

void SpinCircleEmitter::Update(float dt) {
	elapsed_time += dt;
	M3DVector3f p;
	if (isPlayer) {
		players[player_index].object->GetOrigin(p);
	} else {
		enemy_objects[player_index]->GetOrigin(p);
		if (enemy_objects[player_index]->hp <=0) {
			elapsed_time += max_time;
		}
	}
	p[1] += 0.5;
	SetEmitterPos(p);
	

}
void SpinCircleEmitter::Render() {
	glPushMatrix();
	if (particleShader != NULL) {
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
		glEnable( GL_POINT_SMOOTH );
		glEnable(GL_BLEND);
		
		//Translate into world coordnates
		M3DMatrix44f mWorld, mRot;
		m3dTranslationMatrix44(mWorld,emitter_pos[0],emitter_pos[1],emitter_pos[2]);
		m3dRotationMatrix44(mRot, elapsed_time, 2, 4, 0);
		
		glMultMatrixf(mWorld);
		glMultMatrixf(mRot);
		
		particleShader->Begin();
		particleShader->SetUniform4f("color", color[0], color[1], color[2], 1.0f - (elapsed_time / max_time));
		particleShader->SetUniform4f("color2", color[0], color[1], color[2], 0);
		
		particleShader->SetUniform1f("elapsed", 0.90f);
		particleShader->SetUniform1f("percent", elapsed_time/max_time);
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,sizeof(Particle),&particles[0].starting_pos[0]);
		
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer( GL_FLOAT, sizeof(Particle), &particles[0].dir[0]);
		
		glDrawArrays(GL_POINTS, 0, number_of_particles);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		
		particleShader->End();
		glDisable( GL_POINT_SMOOTH );
		glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
		glDisable(GL_BLEND);
	}
	glPopMatrix();
}



/*--------------------------------------------------*/

/*  
 ----------------------------------------------------
 Slash EMITTER
 ----------------------------------------------------
 
 shaders ---
 verticle.vs
 verticle.ps
 */

SlashEmitter::SlashEmitter (float max_t, M3DVector4f color_vec, int index ) {
	number_of_particles = 1;
	lines.resize(number_of_particles);
	
	max_time = max_t;
	elapsed_time = 0;
	particleShader = NULL;

	
	mIndex = index;
	M3DVector4f min, max;
	enemy_objects[index]->GetBBLocalMax(max);
	enemy_objects[index]->GetBBLocalMin(min);
	pMin[0] = min[0];
	pMin[1] = min[1];
	pMin[2] = min[2];
	
	pMax[0] = max[0];
	pMax[1] = max[1];
	pMax[2] = max[2];
	
	m3dScaleVector3(pMax, 2.0);
	m3dScaleVector3(pMin, 2.0);
	
	m3dCopyVector3(lines[0].start,pMax);
	m3dCopyVector3(lines[0].stop,pMin);
	
	
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	SetEmitterPos(p);
}

SlashEmitter::~SlashEmitter() {particles.clear();}

void SlashEmitter::Update(float dt) {
	elapsed_time += dt;
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	SetEmitterPos(p);
	rot = enemy_objects[mIndex]->GetRotateY();
	
}
void SlashEmitter::Render() {
	glPushMatrix();
	
		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		//glDisable(GL_DEPTH_TEST);
		
		float alphaFactor =  (elapsed_time / max_time) / 2;
		glColor4f(1,0,0,1 - 0.5 - alphaFactor);
		
		glLineWidth(15.0);
		//Translate into world coordnates
		M3DMatrix44f mWorld, mRot;
		m3dTranslationMatrix44(mWorld,emitter_pos[0],emitter_pos[1],emitter_pos[2]);
		m3dRotationMatrix44(mRot, m3dDegToRad(rot),0,1,0);
		glMultMatrixf(mWorld);
		glMultMatrixf(mRot);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,sizeof(M3DVector3f),&lines[0]);
		glDrawArrays(GL_LINES, 0, 2);
		glDisableClientState(GL_VERTEX_ARRAY);

		//Draw Line Again
		glColor4f(1,0,0,1 - 0.25 - alphaFactor);
		
		glLineWidth(5.0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,sizeof(M3DVector3f),&lines[0]);
		glDrawArrays(GL_LINES, 0, 2);
		glDisableClientState(GL_VERTEX_ARRAY);
		
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
		//glEnable(GL_DEPTH_TEST);
	
	glPopMatrix();
}



/*--------------------------------------------------*/

/*  
 ----------------------------------------------------
 PowerSong EMITTER
 ----------------------------------------------------
 */

PowerSongEmiter::PowerSongEmiter( float max_t, int index ) {
	
	max_time = max_t;
	elapsed_time = 0;
	particleShader = NULL;
	player_index = index;
	entity = new SPEntity(mPowerSong);

	M3DVector3f p;
	players[player_index].object->GetOrigin(p);
	//p[1] += 0.5;
	angle = 0;
	
}

PowerSongEmiter::~PowerSongEmiter() {
	delete entity;
}

void PowerSongEmiter::Update(float dt) {
	elapsed_time += dt;
	M3DVector3f p;
	
	players[player_index].object->GetOrigin(p);
	//p[1] += 0.5;
	entity->SetOrigin(p);
	angle = 4 * dt;
	entity->RotateLocal(-angle, 0, 1, 0);
	
}
void PowerSongEmiter::Render() {
	entity->Render();
}



/*--------------------------------------------------*/

/*  
 ----------------------------------------------------
 SpeedSong EMITTER
 ----------------------------------------------------
 */

SpeedSongEmiter::SpeedSongEmiter( float max_t, int index ) {
	
	max_time = max_t;
	elapsed_time = 0;
	particleShader = NULL;
	player_index = index;
	entity = new SPEntity(mSpeedSong);
	
	M3DVector3f p;
	players[player_index].object->GetOrigin(p);
	//p[1] += 0.5;
	angle = 0;
	
}

SpeedSongEmiter::~SpeedSongEmiter() {
	delete entity;
}

void SpeedSongEmiter::Update(float dt) {
	elapsed_time += dt;
	M3DVector3f p;
	
	players[player_index].object->GetOrigin(p);
	//p[1] += 0.5;
	entity->SetOrigin(p);
	angle = 4 * dt;
	entity->RotateLocal(-angle, 0, 1, 0);
	
}
void SpeedSongEmiter::Render() {
	entity->Render();
}




/*--------------------------------------------------*/

/*  
 ----------------------------------------------------
 Ring EMITTER
 ----------------------------------------------------
 */

HorizontalRingEmiter::HorizontalRingEmiter( float max_t, int index, sTexture *texture) {
	
	colorKey[0] = 0;
	colorKey[1] = 0;
	colorKey[2] = 0;
	
	quad = new Billboard(1,1,nHorizontal, texture);
	mIndex = index;
	max_time = max_t;
	elapsed_time = 0;

	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	p[1] += 0.5f;
	quad->SetPos(p);

}
HorizontalRingEmiter::~HorizontalRingEmiter() {
	delete quad;
}
	
void HorizontalRingEmiter::Update(float dt) {
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	p[1] += 0.5f;
	quad->SetPos(p);
	
	elapsed_time += dt;
	quad->SetScale(1.0 + elapsed_time * 6);
}

void HorizontalRingEmiter::Render() {
	
	glEnable(GL_BLEND);

	quad->Render(billboardShader, 1.2 - (elapsed_time / max_time));
	glDisable(GL_BLEND);

}




/*--------------------------------------------------*/

/*  
 ----------------------------------------------------
 Bard Shout EMITTER
 ----------------------------------------------------
 */

BardShoutEmiter::BardShoutEmiter( float max_t, int index, sTexture *texture) {
	
	
	
	quad = new Billboard(1,1,nCamera,texture);
	mIndex = index;
	max_time = max_t;
	elapsed_time = 0;
	
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	p[1] += 0.8f;
	quad->SetPos(p);
	
}
BardShoutEmiter::~BardShoutEmiter() {
	delete quad;
}

void BardShoutEmiter::Update(float dt) {
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	p[1] += 0.8f;
	quad->SetPos(p);
	
	elapsed_time += dt;
	quad->SetScale(1.0 + elapsed_time * 4);
}

void BardShoutEmiter::Render() {
	
	glEnable(GL_BLEND);
	M3DVector3f cameraPos;
	gCamera.GetOrigin(cameraPos);
	quad->Render(billboardShader, 1.0f - (elapsed_time / max_time), cameraPos);
	glDisable(GL_BLEND);
	
}

/*  
 ----------------------------------------------------
 Stun EMITTER
 ----------------------------------------------------
 */

StunEmiter::StunEmiter( float max_t, int index) {
	
	
	
	quad = new Billboard(0.5,0.5,nVertical,&mStunSpell);
	mIndex = index;
	max_time = max_t;
	elapsed_time = 0;
	
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	p[1] += 1.5f;
	quad->SetPos(p);
		
}
StunEmiter::~StunEmiter() {
	delete quad;
}

void StunEmiter::Update(float dt) {
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	p[1] += 1.5f;
	quad->SetPos(p);
	
	elapsed_time += dt;
	if (enemy_objects[mIndex]->hp <= 0 ) {
		elapsed_time = max_time;
	}
		
}

void StunEmiter::Render() {
	
	glEnable(GL_BLEND);
	M3DVector3f cameraPos;
	gCamera.GetOrigin(cameraPos);
	quad->RenderWithRotY(billboardShader, 1.0, elapsed_time * 3);
	glDisable(GL_BLEND);
	
}



/*  
 ----------------------------------------------------
 Mez EMITTER
 ----------------------------------------------------
 */

MezEmiter::MezEmiter( float max_t, int index) {
	
	
	
	quad = new Billboard(0.5,0.5,nVertical,&mMezSpell);
	mIndex = index;
	max_time = max_t;
	elapsed_time = 0;
	
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	p[1] += 1.5f;
	quad->SetPos(p);
	enemy_hp = enemy_objects[index]->hp;
	
}
MezEmiter::~MezEmiter() {
	delete quad;
}

void MezEmiter::Update(float dt) {
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	p[1] += 1.5f;
	quad->SetPos(p);
	
	elapsed_time += dt;
	if(enemy_hp != enemy_objects[mIndex]->hp) {
		elapsed_time = max_time;	
	}
}

void MezEmiter::Render() {
	
	glEnable(GL_BLEND);
	M3DVector3f cameraPos;
	gCamera.GetOrigin(cameraPos);
	quad->RenderWithRotY(billboardShader, 1.0, elapsed_time);
	glDisable(GL_BLEND);
	
}


/*  
 ----------------------------------------------------
 SlashRound EMITTER
 ----------------------------------------------------
 */

SlashRoundEmitter::SlashRoundEmitter (float max_t, M3DVector4f color_vec, int index, float heightoffset ) {
	number_of_particles = 50;
	lines.resize(number_of_particles);
	m3dCopyVector4(color, color_vec);
	
	max_time = max_t;
	elapsed_time = 0;
	mIndex = index;
	height = heightoffset;
	
	for (int i =0; i < number_of_particles; ++i) {
		lines[i].start[0] = 0;
		lines[i].start[1] = 0;
		lines[i].start[2] = 0;
		
		lines[i].stop[0] = unifRand(-1,1);
		lines[i].stop[1] = unifRand(-1,1);
		lines[i].stop[2] = unifRand(-1,1);
		
		m3dNormalizeVector3(lines[i].stop);
		m3dScaleVector3(lines[i].stop, 2.0f);
		
		
		
	}

	
	
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	p[1] += 0.5;
	SetEmitterPos(p);
}

SlashRoundEmitter::~SlashRoundEmitter() {particles.clear();}

void SlashRoundEmitter::Update(float dt) {
	elapsed_time += dt;
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	p[1] += height;
	SetEmitterPos(p);
	rot = enemy_objects[mIndex]->GetRotateY();
	
}
void SlashRoundEmitter::Render() {
	glPushMatrix();
	
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	
	
	float alphaFactor =  (elapsed_time / max_time) / 2;
	glColor4f(color[0],color[1],color[2],1 - 0.5 - alphaFactor);
	
	glLineWidth(5.0);
	//Translate into world coordnates
	M3DMatrix44f mWorld, mRot;
	m3dTranslationMatrix44(mWorld,emitter_pos[0],emitter_pos[1],emitter_pos[2]);
	m3dRotationMatrix44(mRot, m3dDegToRad(rot),0,1,0);
	glMultMatrixf(mWorld);
	glMultMatrixf(mRot);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(M3DVector3f),&lines[0]);
	glDrawArrays(GL_LINES, 0, number_of_particles * 2);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	//Draw Line Again
	glColor4f(color[0],color[1],color[2],1 - 0.25 - alphaFactor);
	
	glLineWidth(2.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(M3DVector3f),&lines[0]);
	glDrawArrays(GL_LINES, 0,  number_of_particles * 2);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	
	glPopMatrix();
}

/*  
 ----------------------------------------------------
 Taunt EMITTER
 ----------------------------------------------------
 */

TauntEmitter::TauntEmitter( float max_t, int index) {
	
	
	
	quad = new Billboard(1.5,1.5,nCamera,&mTauntSpell);
	mIndex = index;
	max_time = max_t;
	elapsed_time = 0;
	
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	p[1] += 0.5f;
	quad->SetPos(p);
	
}
TauntEmitter::~TauntEmitter() {
	delete quad;
}

void TauntEmitter::Update(float dt) {
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	p[1] += 0.5f;
	quad->SetPos(p);
	
	elapsed_time += dt;
	
	quad->SetScale(1.0 + elapsed_time * 4);
}

void TauntEmitter::Render() {
	
	glEnable(GL_BLEND);
	M3DVector3f cameraPos;
	gCamera.GetOrigin(cameraPos);
	quad->Render(billboardShader, 0.25,cameraPos);
	glDisable(GL_BLEND);
	
}


/*  
 ----------------------------------------------------
 GroupDef EMITTER
 ----------------------------------------------------
 */

GroupDefEmitter::GroupDefEmitter( float max_t, int index) {
	
	
	
	quad = new Billboard(0.5,0.5,nCamera,&mGroupDefSpell);
	mIndex = index;
	max_time = max_t;
	elapsed_time = 0;
	
	M3DVector3f p;
	players[mIndex].object->GetOrigin(p);
	p[1] += 1.5f;
	quad->SetPos(p);
	
}
GroupDefEmitter::~GroupDefEmitter() {
	delete quad;
}

void GroupDefEmitter::Update(float dt) {
	M3DVector3f p;
	players[mIndex].object->GetOrigin(p);
	p[1] += 1.5f;
	quad->SetPos(p);
	

	
	elapsed_time += dt;

}

void GroupDefEmitter::Render() {
	
	glEnable(GL_BLEND);
	M3DVector3f cameraPos;
	gCamera.GetOrigin(cameraPos);
	quad->Render(billboardShader, 1.0 - (elapsed_time / max_time),cameraPos);
	glDisable(GL_BLEND);
	
}


/*  
 ----------------------------------------------------
 Wings EMITTER
 ----------------------------------------------------
 */

WingsEmitter::WingsEmitter( float max_t, int index) {
	
	
	
	quad = new Billboard(0.5,0.5,nVertical,&mWingsSpell);
	mIndex = index;
	max_time = max_t;
	elapsed_time = 0;
	
	M3DVector3f p;
	players[mIndex].object->GetOrigin(p);
	p[1] += 1.0f;
	quad->SetPos(p);
	
	
	
}
WingsEmitter::~WingsEmitter() {
	delete quad;
}

void WingsEmitter::Update(float dt) {
	M3DVector4f center;
	M3DVector3f final,forward, right;
	
	players[mIndex].object->GetBBCenter(center);
	players[mIndex].object->GetForwardVector(forward);
	players[mIndex].object->GetXAxis(right);
	m3dScaleVector3(forward, -0.28);
	m3dScaleVector3(right, -0.06);
	
	final[0] = center[0];
	final[1] = center[1] + 0.5f;
	final[2] = center[2];
	m3dAddVectors3(final, final, forward);	
	m3dAddVectors3(final, final, right);	
	
	quad->SetPos(final);
	
	elapsed_time += dt;
	
}

void WingsEmitter::Render() {
	
	glEnable(GL_BLEND);
	quad->RenderWithRotY(billboardShader, 0.45 ,	players[mIndex].object->GetRotateY());
	glDisable(GL_BLEND);
	
}

/*  
 ----------------------------------------------------
 BUFF EMITTER
 ----------------------------------------------------
 */

BuffEmitter::BuffEmitter( float max_t, int index, sTexture *texture) {
	
	
	
	quad = new Billboard(0.3,0.3,nCamera,texture);
	mIndex = index;
	max_time = max_t;
	elapsed_time = 0;
	
	
}
BuffEmitter::~BuffEmitter() {
	delete quad;
}

void BuffEmitter::Update(float dt) {
	M3DVector3f p;
	players[mIndex].object->GetOrigin(p);
	p[1] += 1.5f;
	quad->SetPos(p);
	elapsed_time += dt;
}

void BuffEmitter::Render() {
	
	
	glEnable(GL_BLEND);
	M3DVector3f cameraPos;
	gCamera.GetOrigin(cameraPos);
	quad->Render(billboardShader, 1.0 - (elapsed_time / max_time),cameraPos);
	glDisable(GL_BLEND);
	
	
}


/*  
 ----------------------------------------------------
 holy EMITTER
 ----------------------------------------------------
 */

HolyAttackEmitter::HolyAttackEmitter (float max_t,  int index ) {
	number_of_particles = 50;
	lines.resize(number_of_particles);
	
	max_time = max_t;
	elapsed_time = 0;
	mIndex = index;
	
	for (int i =0; i < number_of_particles; ++i) {
		lines[i].start[0] = 0;
		lines[i].start[1] = 0;
		lines[i].start[2] = 0;
		
		lines[i].stop[0] = unifRand(-1,1);
		lines[i].stop[1] = unifRand(-1,1);
		lines[i].stop[2] = unifRand(-1,1);
		
		m3dNormalizeVector3(lines[i].stop);
		m3dScaleVector3(lines[i].stop, 2.0f);
		
		
		
	}
	
	
	
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	p[1] += 0.5;
	SetEmitterPos(p);
}

HolyAttackEmitter::~HolyAttackEmitter() {particles.clear();}

void HolyAttackEmitter::Update(float dt) {
	elapsed_time += dt;
	M3DVector3f p;
	enemy_objects[mIndex]->GetOrigin(p);
	p[1] += 0.5;
	SetEmitterPos(p);
	rot = enemy_objects[mIndex]->GetRotateY();
	
}
void HolyAttackEmitter::Render() {
	glPushMatrix();
	
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	
	
	float alphaFactor =  (elapsed_time / max_time) / 2;
	glColor4f(0.2,0.64,0.39,1 - 0.5 - alphaFactor);
	
	glLineWidth(5.0);
	//Translate into world coordnates
	M3DMatrix44f mWorld, mRot;
	m3dTranslationMatrix44(mWorld,emitter_pos[0],emitter_pos[1],emitter_pos[2]);
	m3dRotationMatrix44(mRot, m3dDegToRad(rot),0,1,0);
	glMultMatrixf(mWorld);
	glMultMatrixf(mRot);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(M3DVector3f),&lines[0]);
	glDrawArrays(GL_LINES, 0, number_of_particles * 2);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	//Draw Line Again
	glColor4f(0.2,0.64,0.39,1 - 0.25 - alphaFactor);
	
	glLineWidth(2.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(M3DVector3f),&lines[0]);
	glDrawArrays(GL_LINES, 0,  number_of_particles * 2);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	
	glPopMatrix();
}



/*  
 ----------------------------------------------------
 holysword EMITTER
 ----------------------------------------------------
 */

HolySwordEmitter::HolySwordEmitter( float max_t, int index) {
	
	
	
	quad = new Billboard(0.3,0.3,nVertical,&mHolySwordSpell);
	mIndex = index;
	max_time = max_t;
	elapsed_time = 0;
	
	M3DVector3f p;
	players[mIndex].object->GetOrigin(p);
	p[1] += 1.0f;
	quad->SetPos(p);
	
	
	
}
HolySwordEmitter::~HolySwordEmitter() {
	delete quad;
}

void HolySwordEmitter::Update(float dt) {
	M3DVector4f center;
	M3DVector3f final,forward, right;
	
	players[mIndex].object->GetBBCenter(center);
	players[mIndex].object->GetForwardVector(forward);
	players[mIndex].object->GetXAxis(right);
	
	m3dScaleVector3(forward, 0.15);
	m3dScaleVector3(right, 0.30);
	
	final[0] = center[0];
	final[1] = center[1] + 0.2f;
	final[2] = center[2];
	m3dAddVectors3(final, final, forward);	
	m3dAddVectors3(final, final, right);
	quad->SetPos(final);
	
	elapsed_time += dt;
	
}

void HolySwordEmitter::Render() {
	
	glEnable(GL_BLEND);
	quad->RenderWithRotY(billboardShader, 0.55 ,	players[mIndex].object->GetRotateY());
	glDisable(GL_BLEND);
	
}




/*  
 ----------------------------------------------------
 HolyArmor EMITTER
 ----------------------------------------------------
 */

HolyArmorEmitter::HolyArmorEmitter( float max_t, int index) {
	
	
	
	quad = new Billboard(0.3,0.3,nVertical,&mHolyArmorSpell);
	mIndex = index;
	max_time = max_t;
	elapsed_time = 0;
	
	M3DVector3f p;
	players[mIndex].object->GetOrigin(p);
	p[1] += 1.0f;
	quad->SetPos(p);
	
	
	
}
HolyArmorEmitter::~HolyArmorEmitter() {
	delete quad;
}

void HolyArmorEmitter::Update(float dt) {
	M3DVector4f center;
	M3DVector3f final,forward, right;
	
	players[mIndex].object->GetBBCenter(center);
	players[mIndex].object->GetForwardVector(forward);
	players[mIndex].object->GetXAxis(right);
	
	m3dScaleVector3(forward, 0.15);
	m3dScaleVector3(right, -0.30);
	
	final[0] = center[0];
	final[1] = center[1] + 0.2;
	final[2] = center[2];
	m3dAddVectors3(final, final, forward);
	m3dAddVectors3(final, final, right);
	
	quad->SetPos(final);
	
	elapsed_time += dt;
	
}

void HolyArmorEmitter::Render() {
	
	glEnable(GL_BLEND);
	quad->RenderWithRotY(billboardShader, 0.55 ,	players[mIndex].object->GetRotateY());
	glDisable(GL_BLEND);
	
}





/*  
 ----------------------------------------------------
 HealChant EMITTER
 ----------------------------------------------------
 */

HealChantEmitter::HealChantEmitter( float max_t, int index) {
	
	
	
	quad = new Billboard(0.3,0.3,nVertical,&mHealChantSpell);
	mIndex = index;
	max_time = max_t;
	elapsed_time = 0;
	
	M3DVector3f p;
	players[mIndex].object->GetOrigin(p);
	p[1] += 1.0f;
	quad->SetPos(p);
	
	
	
}
HealChantEmitter::~HealChantEmitter() {
	delete quad;
}

void HealChantEmitter::Update(float dt) {
	M3DVector4f center;
	M3DVector3f final,forward;
	
	players[mIndex].object->GetBBCenter(center);
	players[mIndex].object->GetForwardVector(forward);
	m3dScaleVector3(forward, 0.15);
	final[0] = center[0];
	final[1] = center[1] + 0.8f;
	final[2] = center[2];
	m3dAddVectors3(final, final, forward);
	quad->SetPos(final);
	
	elapsed_time += dt;
	
}

void HealChantEmitter::Render() {
	
	glEnable(GL_BLEND);
	quad->RenderWithRotY(billboardShader, 0.55 ,	players[mIndex].object->GetRotateY());
	glDisable(GL_BLEND);
	
}




/*  
 ----------------------------------------------------
 IceBolt EMITTER
 ----------------------------------------------------
 */

IceBoltEmitter::IceBoltEmitter(  int aindex, int tindex) {
	
	aIndex = aindex;
	tIndex = tindex;
	
	max_time = 6;
	elapsed_time = 0;
	speed = 18.0f;
	
	entity = new SPEntity(mIceBolt);
	M3DVector3f aPos, tPos, dir;
	players[aIndex].object->GetOrigin(aPos);
	enemy_objects[tIndex]->GetOrigin(tPos);
	entity->SetOrigin(aPos);
	
	m3dSubtractVectors3(dir, tPos, aPos);
	m3dNormalizeVector3(dir);
	entity->SetForwardVector(dir);
	
	
}
IceBoltEmitter::~IceBoltEmitter() {
	delete entity;
}

void IceBoltEmitter::Update(float dt) {
	
	//just a check to make sure we don't lose this forever
	elapsed_time += dt;
	
	// check distance
	if (entity->TestCollision(*enemy_objects[tIndex])) {
		elapsed_time = max_time;
		HorizontalRingEmiter *semitter = new HorizontalRingEmiter(0.5f,tIndex, &mRing);
		gEmitters.push_back(semitter);

		
	}
	
	
	
	M3DVector3f aPos, tPos, dir;
	entity->GetOrigin(aPos);
	enemy_objects[tIndex]->GetOrigin(tPos);
	
	m3dSubtractVectors3(dir, tPos, aPos);
	m3dNormalizeVector3(dir);
	entity->SetForwardVector(dir);
	
	entity->MoveForward(dt * speed );
	
	
	
	
	
}

void IceBoltEmitter::Render() {
	
	entity->Render();
	
}




/*  
 ----------------------------------------------------
 FireBolt EMITTER
 ----------------------------------------------------
 */

FireBoltEmitter::FireBoltEmitter(  int aindex, int tindex) {
	
	aIndex = aindex;
	tIndex = tindex;
	
	max_time = 6;
	elapsed_time = 0;
	speed = 18.0f;
	
	entity = new SPEntity(mFireBolt);
	M3DVector3f aPos, tPos, dir;
	players[aIndex].object->GetOrigin(aPos);
	enemy_objects[tIndex]->GetOrigin(tPos);
	entity->SetOrigin(aPos);
	
	m3dSubtractVectors3(dir, tPos, aPos);
	m3dNormalizeVector3(dir);
	entity->SetForwardVector(dir);
	
	
}
FireBoltEmitter::~FireBoltEmitter() {
	delete entity;
}

void FireBoltEmitter::Update(float dt) {
	
	//just a check to make sure we don't lose this forever
	elapsed_time += dt;
	
	// check distance
	if (entity->TestCollision(*enemy_objects[tIndex])) {
		elapsed_time = max_time;
		HorizontalRingEmiter *semitter = new HorizontalRingEmiter(0.5f,tIndex, &mRedRing);
		gEmitters.push_back(semitter);
		
		
	}
	
	
	
	M3DVector3f aPos, tPos, dir;
	entity->GetOrigin(aPos);
	enemy_objects[tIndex]->GetOrigin(tPos);
	
	m3dSubtractVectors3(dir, tPos, aPos);
	m3dNormalizeVector3(dir);
	entity->SetForwardVector(dir);
	
	entity->MoveForward(dt * speed );
	
}

void FireBoltEmitter::Render() {
	
	entity->Render();
	
}



/*  
 ----------------------------------------------------
 RockArmor EMITTER
 ----------------------------------------------------
 */

RockArmorEmitter::RockArmorEmitter( float max_t, int index) {
	
	
	
	quad = new Billboard(0.3,0.3,nVertical,&mRockSpell);
	mIndex = index;
	max_time = max_t;
	elapsed_time = 0;
	
	M3DVector3f p;
	players[mIndex].object->GetOrigin(p);
	p[1] += 1.0f;
	quad->SetPos(p);
	
	
	
}
RockArmorEmitter::~RockArmorEmitter() {
	delete quad;
}

void RockArmorEmitter::Update(float dt) {
	M3DVector4f center;
	M3DVector3f final,forward;
	
	players[mIndex].object->GetBBCenter(center);
	players[mIndex].object->GetForwardVector(forward);
	m3dScaleVector3(forward, 0.15);
	final[0] = center[0];
	final[1] = center[1] + 0.2;
	final[2] = center[2];
	m3dAddVectors3(final, final, forward);	
	quad->SetPos(final);
	
	elapsed_time += dt;
	
}

void RockArmorEmitter::Render() {
	
	glEnable(GL_BLEND);
	quad->RenderWithRotY(billboardShader, 0.55 ,	players[mIndex].object->GetRotateY());
	glDisable(GL_BLEND);
	
}



/*--------------------------------------------------*/

/*  
 ----------------------------------------------------
 Ground EMITTER
 ----------------------------------------------------
 */

GroundEmitter::GroundEmitter( float max_t, int index, bool isPlayer, sTexture *texture,  float initScale) {
	
	quad = new Billboard(1.0f,1.0f,nHorizontal, texture);
	mIndex = index;
	max_time = max_t;
	elapsed_time = 0;
	bPlayer = isPlayer;
	quad->SetScale(initScale);
	
		
}
GroundEmitter::~GroundEmitter() {
	delete quad;
}

void GroundEmitter::Update(float dt) {
	M3DVector3f p;
	if (bPlayer) {
		players[mIndex].object->GetOrigin(p);
		p[1] += 0.075f;
		quad->SetPos(p);
		
	} else {
		enemy_objects[mIndex]->GetOrigin(p);
		p[1] += 0.075f;
		quad->SetPos(p);
		
	}	
	elapsed_time += dt;

}

void GroundEmitter::Render() {
	
	glEnable(GL_BLEND);
	quad->Render(billboardShader, 1.0 - (elapsed_time / max_time));
	glDisable(GL_BLEND);
	
}




/*  
 ----------------------------------------------------
 LifeTap EMITTER
 ----------------------------------------------------
 */

LifeTapEmitter::LifeTapEmitter( int tindex) {
	
	tIndex = tindex;
	
	max_time = 6;
	elapsed_time = 0;
	speed = 3.0f;
	
	entity = new SPEntity(mLifeTap);
	M3DVector3f  tPos;
	
	enemy_objects[tIndex]->GetOrigin(tPos);
	tPos[1] += 2.5f;
	entity->SetOrigin(tPos);
	//entity->SetForwardVector(0,-1,0);
	
	
}
LifeTapEmitter::~LifeTapEmitter() {
	delete entity;
}

void LifeTapEmitter::Update(float dt) {
	
	//just a check to make sure we don't lose this forever
	elapsed_time += dt;
	
	entity->MoveUp(- speed * dt);
	// check distance
	if (entity->GetOriginY() < enemy_objects[tIndex]->GetOriginY() + 1.0f ) {
		elapsed_time = max_time;
		HorizontalRingEmiter *semitter = new HorizontalRingEmiter(0.5f,tIndex, &mBrownRing);
		gEmitters.push_back(semitter);
	}
	
	
	
	M3DVector3f tPos, ePos;
	entity->GetOrigin(ePos);
	enemy_objects[tIndex]->GetOrigin(tPos);
	tPos[1] = ePos[1];
	entity->SetOrigin(tPos);	
	
	
}

void LifeTapEmitter::Render() {
	
	entity->Render();
	
}


/*  
 ----------------------------------------------------
 Lighting EMITTER
 ----------------------------------------------------
 */
LightingEmitter::LightingEmitter (float max_t, int aindex, int tindex ) {
	number_of_particles = 6;
	lines.resize(number_of_particles);
	//m3dLoadVector4(color , 0.24f, 0.33f, 0.66f, 1.0f);
	m3dLoadVector4(color , 0.94f, 0.93f, 0.06f, 1.0f);
	
	max_time = max_t;
	elapsed_time = 0;
	aIndex = aindex;
	tIndex = tindex;
	
	M3DVector3f ray, ray2, ray3, ray4, ray5, aPos, tPos;
	enemy_objects[tIndex]->GetOrigin(tPos);
	players[aIndex].object->GetOrigin(aPos);
	tPos[1] += 0.35f;
	aPos[1] += 0.35f;
	
	m3dSubtractVectors3(ray, tPos, aPos);
	m3dCopyVector3(ray2, ray);
	m3dCopyVector3(ray3, ray);
	m3dCopyVector3(ray4, ray);
	m3dCopyVector3(ray5, ray);
	
	m3dScaleVector3(ray, 0.25);
	m3dScaleVector3(ray2, 0.40);
	m3dScaleVector3(ray3, 0.55);
	m3dScaleVector3(ray4, 0.60);
	m3dScaleVector3(ray5, 0.90);
	
	
	float amt = 0.5f;
	ray[1] += unifRand(-amt, amt);
	ray2[1] += unifRand(-amt, amt);
	ray3[1] += unifRand(-amt, amt);
	ray4[1] += unifRand(-amt, amt);
	ray5[1] += unifRand(-amt, amt);
	
	ray[0] += unifRand(-amt, amt);
	ray2[0] += unifRand(-amt, amt);
	ray3[0] += unifRand(-amt, amt);
	ray4[1] += unifRand(-amt, amt);
	ray5[1] += unifRand(-amt, amt);
	
	ray[2] += unifRand(-amt, amt);
	ray2[2] += unifRand(-amt, amt);
	ray3[2] += unifRand(-amt, amt);
	ray4[1] += unifRand(-amt, amt);
	ray5[1] += unifRand(-amt, amt);
	
	m3dAddVectors3(ray, aPos, ray);
	m3dAddVectors3(ray2, aPos, ray2);
	m3dAddVectors3(ray3, aPos, ray3);
	m3dAddVectors3(ray4, aPos, ray4);
	m3dAddVectors3(ray5, aPos, ray5);
	
	
	
	m3dCopyVector3(lines[0].start, aPos);
	m3dCopyVector3(lines[0].stop, ray);
	
	m3dCopyVector3(lines[1].start, ray);
	m3dCopyVector3(lines[1].stop, ray2);
	
	m3dCopyVector3(lines[2].start, ray2);
	m3dCopyVector3(lines[2].stop, ray3);
	
	m3dCopyVector3(lines[3].start, ray3);
	m3dCopyVector3(lines[3].stop, ray4);
	
	m3dCopyVector3(lines[4].start, ray4);
	m3dCopyVector3(lines[4].stop, ray5);
	
	m3dCopyVector3(lines[5].start, ray5);
	m3dCopyVector3(lines[5].stop, tPos);
	
	
}

LightingEmitter::~LightingEmitter() {lines.clear();}

void LightingEmitter::Update(float dt) {
	elapsed_time += dt;
	
	
}
void LightingEmitter::Render() {
	glPushMatrix();
	
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	
	
	float alphaFactor =  (elapsed_time / max_time) / 2;
	glColor4f(color[0],color[1],color[2],1 - 0.5 - alphaFactor);
	
	glLineWidth(6.0);
	//Translate into world coordnates
	//M3DMatrix44f mWorld, mRot;
	//m3dTranslationMatrix44(mWorld,emitter_pos[0],emitter_pos[1],emitter_pos[2]);
	//m3dRotationMatrix44(mRot, m3dDegToRad(rot),0,1,0);
	
	
	//glMultMatrixf(mWorld);
	//glMultMatrixf(mRot);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(M3DVector3f),&lines[0]);
	glDrawArrays(GL_LINES, 0, number_of_particles * 2);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	//Draw Line Again
	glColor4f(color[0],color[1],color[2],1 - alphaFactor);
	
	glLineWidth(2.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(M3DVector3f),&lines[0]);
	glDrawArrays(GL_LINES, 0,  number_of_particles * 2);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	
	glPopMatrix();
}


/*  
 ----------------------------------------------------
 AntiVerticle EMITTER
 ----------------------------------------------------
 */

AntiVerticalEmitter::AntiVerticalEmitter(int count, float max_t, M3DVector4f color_vec, M3DVector4f color_vec2, int ind) {
	number_of_particles = count;
	particles.resize(number_of_particles);
	max_time = max_t;
	elapsed_time = 0;
	particleShader = NULL;
	m3dCopyVector4(color,color_vec);
	m3dCopyVector4(color2,color_vec2);
	
	
	for (int i =0; i < number_of_particles; ++i) {
		particles[i].dir[0] = 0.0f;
		particles[i].dir[1] = unifRand(0.00f,0.5) * -1;
		particles[i].dir[2] = 0.0f;
		
		particles[i].starting_pos[0] = unifRand(-1,1);
		particles[i].starting_pos[1] = 0;
		particles[i].starting_pos[2] = unifRand(-1,1);
		m3dNormalizeVector3(particles[i].starting_pos);
		m3dScaleVector3(particles[i].starting_pos, 0.45);
	}
	particleShader = &verticalShader;
	mIndex = ind;

	
	
}

AntiVerticalEmitter::~AntiVerticalEmitter() {particles.clear();}

void AntiVerticalEmitter::Update(float dt) {
	elapsed_time += dt;
	M3DVector3f p;
	players[mIndex].object->GetOrigin(p);
	p[1] += 1.5f;
	SetEmitterPos(p);
	
}
void AntiVerticalEmitter::Render() {
	glPushMatrix();
	if (particleShader != NULL) {
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
		glEnable( GL_POINT_SMOOTH );
		glEnable(GL_BLEND);
		
		//Translate into world coordnates
		M3DMatrix44f mWorld;
		m3dTranslationMatrix44(mWorld,emitter_pos[0],emitter_pos[1],emitter_pos[2]);
		glMultMatrixf(mWorld);
		
		particleShader->Begin();
		particleShader->SetUniform4f("color", color[0], color[1], color[2], 1.0f - (elapsed_time / max_time));
		particleShader->SetUniform4f("color2", color[0], color[1], color[2], 1);
		
		particleShader->SetUniform1f("elapsed", elapsed_time);
		particleShader->SetUniform1f("percent", elapsed_time/max_time);
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,sizeof(Particle),&particles[0].starting_pos[0]);
		
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer( GL_FLOAT, sizeof(Particle), &particles[0].dir[0]);
		
		glDrawArrays(GL_POINTS, 0, number_of_particles);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		
		particleShader->End();
		glDisable( GL_POINT_SMOOTH );
		glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
		glDisable(GL_BLEND);
	}
	glPopMatrix();
}




/*--------------------------------------------------*/

/*  
 ----------------------------------------------------
Hurricane EMITTER
 ----------------------------------------------------
 */

HurricaneEmitter::HurricaneEmitter( int aindex, int tindex) {
	num = 6;
	for (int i =0; i < num; ++i) {
		quads[i] = new Billboard(1.0f,1.0f,nHorizontal, &mHurricaneSpell);
	}
	
	aIndex = aindex;
	tIndex = tindex;
	max_time = 6;
	elapsed_time = 0;
	
	M3DVector3f aPos;
	M3DVector3f pos;
	players[aIndex].object->GetOrigin(aPos);
	for (int i =0; i < num; ++i) {
		
		m3dCopyVector3(pos, aPos);
		pos[1] += i * 0.2f + 0.2;
		quads[i]->SetPos(pos);
		quads[i]->SetScale(0.25 + (0.30 * i));
	}

}
HurricaneEmitter::~HurricaneEmitter() {
	for (int i =0; i < num; ++i) {
		delete quads[i];
		quads[i] = NULL;
	}
}

void HurricaneEmitter::Update(float dt) {
	elapsed_time += dt;
	
	M3DVector3f aPos, tPos, dir;
	
	for (int i =0; i < num; ++i) {
		quads[i]->GetPos(aPos);
		enemy_objects[tIndex]->GetOrigin(tPos);
		tPos[1] = aPos[1];
		m3dSubtractVectors3(dir, tPos, aPos);
		m3dNormalizeVector3(dir);
		m3dScaleVector3(dir, 10 * dt);
		m3dAddVectors3(aPos,aPos,dir);
		quads[i]->SetPos(aPos);
		
		
	}
	
	
	
}

void HurricaneEmitter::Render() {
	
	glEnable(GL_BLEND);
	for (int i =0; i < num; ++i) {
		quads[i]->Render(billboardShader, 0.8);
	}
	
	glDisable(GL_BLEND);
	
}


/* ------------------------------- 
    Tree Emitter
 ---------------------------------*/

TreeEmitter::TreeEmitter( float max_t , M3DVector3f pos, SPMesh *mesh ) {
	max_time = max_t;
	elapsed_time = 0;
	
	tree = new SPEntity(mesh);
	tree->SetOrigin(pos);
}
TreeEmitter::~TreeEmitter() {
	delete tree;
}

void TreeEmitter::Update(float dt) {
	elapsed_time += dt;
}

void TreeEmitter::Render() {
	
	tree->Render();
	
}


