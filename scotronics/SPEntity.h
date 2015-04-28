/*
 *  SPEntity.h
 *  SPMexporter
 *
 *  Created by Scott Scaccianoce on 7/3/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */


#ifndef __SPENTITY__
#define __SPENTITY__


#include "math3d.h";
#include "Camera.h";
#include "spMesh.h"

class SPEntity : public GLCamera {
	
public:
	SPEntity(SPMesh *mesh_ptr);
	void Scale(float x, float y, float z);
	~SPEntity();
	
	void GetMatrix(M3DMatrix44f matrix);
	void Render();
	void RenderBB();
	bool RayIntersect(M3DVector3f pos, M3DVector3f dir, float &dist);
	
	//Animate functions
	void Update(float dt);
	void HasAnimation(bool value) { hasAnimation = value;}
	void ShouldRepeat(bool value) { repeat = value;}
	void GetAnimationInfo();
	
	void StartAnimation(bool reset = true);
	void StopAnimation() { animate = false;};
	bool Animating();
	
	int GetDbId();
	void RefreshMesh();
	
	bool TestCollision(SPEntity &object);
	void GetBBMin(M3DVector4f &value);
	void GetBBMax(M3DVector4f &value);
	void GetBBCenter(M3DVector4f &value);

	void GetBBLocalMin(M3DVector4f &value);
	void GetBBLocalMax(M3DVector4f &value);
	void GetBBLocalCenter(M3DVector4f &value);
	
	float GetRadius();
	float GetHalfX();
	float GetHalfZ();
	
	
private:
	float elapsed;
	bool hasAnimation;
	bool animate;
	bool repeat;
	
	//animation specific
	vector<float> keyframes; //Keyframe step values
	float animationSpeed;
	int frame;
	int totalFrames;

	
	float scale[3];
	SPMesh *mesh;
	
	
};



#endif