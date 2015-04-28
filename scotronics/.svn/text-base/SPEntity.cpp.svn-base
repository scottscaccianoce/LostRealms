/*
 *  SPEntity.cpp
 *  SPMexporter
 *
 *  Created by Scott Scaccianoce on 7/3/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "SPEntity.h"

/*
 *  Entity.cpp
 *  MapEditor
 *
 *  Created by Scott Scaccianoce on 6/19/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "SPEntity.h"

SPEntity::SPEntity(SPMesh *mesh_ptr) {
	mesh = mesh_ptr;
	/*scale[0] = 0.075f;
	 scale[1] = 0.075f;
	 scale[2] = 0.075f;
	 */
	scale[0] = mesh->scale[0];
	scale[1] = mesh->scale[1];
	scale[2] = mesh->scale[2];
	
	hasAnimation = false;
	animate = false;
	repeat = false;
	
}
void SPEntity::Scale(float x, float y, float z) {
	scale[0] = x;
	scale[1] = y;
	scale[2] = z;
	
	
}
void SPEntity::RefreshMesh() {
	scale[0] = mesh->scale[0];
	scale[1] = mesh->scale[1];
	scale[2] = mesh->scale[2];
	
}

SPEntity::~SPEntity() {
	
}
float SPEntity::GetRadius() {
	return mesh->GetRadius();
}
void SPEntity::GetMatrix(M3DMatrix44f matrix)
{
	
	// Calculate the right side (x) vector, drop it right into the matrix
	M3DVector3f vXAxis;
	
	
	m3dCrossProduct3(vXAxis, vUp, vForward);
	
	// Set matrix column does not fill in the fourth value...
	m3dSetMatrixColumn44(matrix, vXAxis, 0);
	matrix[3] = 0.0f;
	
	// Y Column
	m3dSetMatrixColumn44(matrix, vUp, 1);
	matrix[7] = 0.0f;       
	
	// Z Column
	m3dSetMatrixColumn44(matrix, vForward, 2);
	matrix[11] = 0.0f;
	
	
		
	//matrix[0] *= scale[0]; //x scale
	//matrix[5] *= scale[1]; //y scale
	//matrix[10] *= scale[2]; //z scale
	
	M3DMatrix44f scaleMat;
	m3dLoadIdentity44(scaleMat);
	scaleMat[0] *= scale[0];
	scaleMat[5] *= scale[1];
	scaleMat[10] *= scale[2];
	
	m3dMatrixMultiply44(matrix,matrix,scaleMat);
	
	m3dSetMatrixColumn44(matrix, vOrigin, 3);

	matrix[15] = 1.0f;
	
	
}

void SPEntity::Render() {
	glPushMatrix();
	
	M3DMatrix44f mWorld;
	GetMatrix(mWorld);
	glMultMatrixf(mWorld);
	
	if (animate) {
		float steps = keyframes[frame];
		float tween = (elapsed / animationSpeed ) / ( steps * animationSpeed)  *  animationSpeed;
		bool increaseFrame = false;
		
		if (tween > 1.0f) {
			tween = 1.0f;
			increaseFrame = true;
			elapsed = elapsed -  (steps * animationSpeed);
			
		}
		
		mesh->RenderAnimation(frame, tween); 
		
		if (increaseFrame) {
			frame++;
			
			//Check if we reached end of animation
			if (frame +1 > totalFrames) {
				animate = repeat; //turns animate off if not repeat
				frame = 0;
			}
		}
	
	} else {
		mesh->Render();
	}
	glPopMatrix();
}

void SPEntity::RenderBB() {
	glPushMatrix();
	
	M3DMatrix44f mWorld;
	GetMatrix(mWorld);
	glMultMatrixf(mWorld);
	
	mesh->RenderBB();
	
	glPopMatrix();
	
	
	//Uncomment for debug to view axis lines
	/*
	glPushMatrix();
	M3DVector3f point;
	GetOrigin(point);
	m3dTranslationMatrix44(mWorld, point[0], point[1], point[2]);
	glMultMatrixf(mWorld);
	glBegin(GL_LINES);
	glColor4f(1,0,0,0);
	glLineWidth(6.0);
	M3DVector3f v,p;
	M3DVector4f c;
	GetBBLocalCenter(c);
	GetOrigin(p);
	GetForwardVector(v);
	m3dNormalizeVector3(v);
	float w = GetHalfZ();
	m3dScaleVector3(v,w);
	glVertex3f(0,1,0);
	glVertex3f(v[0], 1, v[2]);
	glEnd();
	glBegin(GL_LINES);
	
	
	glColor4f(0,0,1,0);
	GetBBLocalCenter(c);
	GetXAxis(v);
	m3dNormalizeVector3(v);
	w = GetHalfX();
	m3dScaleVector3(v,w);
	glVertex3f(0,1,0);
	glVertex3f(v[0], 1, v[2]);
	glEnd();
	glPopMatrix();
	 */
	
}
bool SPEntity::RayIntersect(M3DVector3f pos, M3DVector3f dir, float &dist) {
	
	M3DVector4f WorldPos;
	GetOrigin(WorldPos);
	
	
	dist = m3dRaySphereTest(pos,dir,WorldPos,mesh->radius * scale[0]);
	return dist > 0 ? true : false;
	
}

void SPEntity::Update(float dt) {
	if (animate) {
		elapsed += dt;
	}
}

void SPEntity::StartAnimation(bool reset) {
	if (hasAnimation) {
		animate = true;
		if (reset) {
			elapsed = 0.0f;
			frame = 0;
			
		}
	}
}
void SPEntity::GetAnimationInfo() {
	animationSpeed = mesh->speed;
	keyframes.clear(); 
	mesh->SetKeyframeTotals(keyframes);
	totalFrames = keyframes.size();
	repeat = mesh->repeat;
	hasAnimation = mesh->hasAnimation;
	
	scale[0] = mesh->scale[0];
	scale[1] = mesh->scale[1];
	scale[2] = mesh->scale[2];
	
	
}

int SPEntity::GetDbId() {
	return mesh->db_id;
}

void SPEntity::GetBBMin(M3DVector4f &value) {
	M3DMatrix44f mWorld;
	M3DVector4f mLocalMin;
	GetMatrix(mWorld);
	mesh->GetBBMin(mLocalMin);
	m3dTransformVector4(value,mLocalMin,mWorld);
}

void SPEntity::GetBBMax(M3DVector4f &value) {
	M3DMatrix44f mWorld;
	M3DVector4f  mLocalMax;
	GetMatrix(mWorld);
	mesh->GetBBMax(mLocalMax);
	m3dTransformVector4(value,mLocalMax,mWorld);
}

void SPEntity::GetBBCenter(M3DVector4f &value) {
	M3DMatrix44f mWorld;
	GetMatrix(mWorld);
	m3dTransformVector4(value,mesh->center,mWorld);
}


void SPEntity::GetBBLocalMin(M3DVector4f &value) {
	mesh->GetBBMin(value);
	m3dScaleVector3(value, mesh->scale[0]);
	
}

void SPEntity::GetBBLocalMax(M3DVector4f &value) {
	mesh->GetBBMax(value);
	m3dScaleVector3(value, mesh->scale[0]);
	
}

void SPEntity::GetBBLocalCenter(M3DVector4f &value) {
	m3dCopyVector3(value, mesh->center);
	m3dScaleVector3(value, mesh->scale[0]);
	
}

bool SPEntity::TestCollision( SPEntity &object) {
	//self is rect a, other is rec B
	M3DVector4f aTemp, bTemp;
	M3DVector3f aCenter, aXAxis, aZAxis, T;
	M3DVector3f bCenter, bXAxis, bZAxis;
	
	
	//Load Centers
	GetBBCenter(aTemp);
	object.GetBBCenter(bTemp);
	
	m3dLoadVector3(aCenter, aTemp[0], 0, aTemp[2]);
	m3dLoadVector3(bCenter, bTemp[0], 0, bTemp[2]);

	GetOrigin(aCenter);
	object.GetOrigin(bCenter);

	aCenter[1] = 0;
	bCenter[1] = 0;
	
	//Load unit axis vectors
	GetForwardVector(aZAxis);
	GetXAxis(aXAxis);
	
	object.GetForwardVector(bZAxis);
	object.GetXAxis(bXAxis);

	m3dNormalizeVector3(aXAxis);
	m3dNormalizeVector3(bXAxis);
	m3dNormalizeVector3(aZAxis);
	m3dNormalizeVector3(bZAxis);
	
	
	//Load Half Vectors
	float Wa = GetHalfX();
	float Ha = GetHalfZ();
	float Wb = object.GetHalfX();
	float Hb = object.GetHalfZ();
	

	
	/*Wa *= 0.9f;
	Ha *= 0.9f;
	Wb *= 0.9f;
	Hb *= 0.9f;
	*/
	
	
	//Run test for intersection
	m3dSubtractVectors3(T, bCenter, aCenter);
	
	
	/*
	 http://www.jkh.me/files/tutorials/Separating%20Axis%20Theorem%20for%20Oriented%20Bounding%20Boxes.pdf
	 
	 PA = coordinate position of the center of rectangle A
	 Ax = unit vector representing the local x-axis of A
	 Ay = unit vector representing the local y-axis of A
	 WA = half width of A (corresponds with the local x-axis of A)
	 HA = half height of A (corresponds with the local y-axis of A)
	 PB = coordinate position of the center of rectangle Bx = unit vector representing the local x-axis of B
	 By = unit vector representing the local y-axis of B
	 WB = half width of B (corresponds with the local x-axis of B)
	 HB = half height of B (corresponds with the local y-axis of B)
	*/

	// L = Ax  ------------------------------------------------------------------
	//	 | T • Ax | > WA + | ( WB*Bx ) • Ax | + |( HB*By ) • Ax |
	//        LT                step1              step2
	//                        <-----step3--->    <-------step4--->
	
	float LT = 0;
	LT = m3dDotProduct3(T, aXAxis);
	M3DVector3f step1;
	m3dCopyVector3(step1, bXAxis);
	m3dScaleVector3(step1, Wb);
	
	M3DVector3f step2;
	m3dCopyVector3(step2, bZAxis);
	m3dScaleVector3(step2, Hb);
	if (fabs(LT) > Wa + fabs(m3dDotProduct3(step1, aXAxis)) + fabs(m3dDotProduct3(step2, aXAxis))) {
		return false;
	}
	
	
	// L = Ay ------------------------------------------------------------------
	//| T • Ay | > HA + | ( WB*Bx ) • Ay | + |( HB*By ) • Ay |
	//                     step1               step2
	LT = m3dDotProduct3(T, aZAxis);
	//step1 is the same from last step
	//step2 is the same from lasdt step
	if (fabs(LT) > Ha + fabs(m3dDotProduct3(step1, aZAxis)) + fabs(m3dDotProduct3(step2, aZAxis))) {
		return false;
	}
	
	// L = Bx ------------------------------------------------------------------
	// | T • Bx | > | ( WA* Ax ) • Bx | + | ( HA*Ay ) • Bx | + WB
	//                   step1                step2

	LT = m3dDotProduct3(T, bXAxis);
	m3dCopyVector3(step1, aXAxis);
	m3dScaleVector3(step1, Wa);
	
	m3dCopyVector3(step2, aZAxis);
	m3dScaleVector3(step2, Ha);

	if (fabs(LT) > Wb + fabs(m3dDotProduct3(step1, bXAxis)) + fabs(m3dDotProduct3(step2, bXAxis))) {
		return false;
	}
	
	
	// L = By ------------------------------------------------------------------
	// | T • By | > | ( WA* Ax ) • By | + | ( HA*Ay ) • By | + HB
	LT = m3dDotProduct3(T, bZAxis);
	//step1 is the same from last step
	//step2 is the same from lasdt step
	if (fabs(LT) > Hb + fabs(m3dDotProduct3(step1, bZAxis)) + fabs(m3dDotProduct3(step2, bZAxis))) {
		return false;
	}
	
	
	return true;
	
}
	
float SPEntity::GetHalfX() {
	return mesh->halfX;
}
float SPEntity::GetHalfZ() {
	return mesh->halfZ;
}
	
/*
 //THIS IS FOR AABB collision detection.  (no rotation allowed)
bool SPEntity::TestCollision( SPEntity &object) {
	M3DVector4f self_min, self_max, other_min, other_max;
	
	GetBBMin(self_min);
	GetBBMax(self_max);
	object.GetBBMin(other_min);
	object.GetBBMax(other_max);
	
	if (self_max[0] > other_min[0] ||
		self_max[2] > other_min[2] ||
		self_min[0] < other_max[0] ||
		self_min[2] < other_max[2]  )
	{
		return false;
	}
	return true;
	
}
*/
bool SPEntity::Animating() {
	return animate && !(frame +1 > totalFrames);
}