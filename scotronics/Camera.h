/*
 *  Camera.h
 *  MapEditor
 *
 *  Created by Scott Scaccianoce on 6/19/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */




#ifndef _GLCAMERA_
#define _GLCAMERA_


#ifdef __APPLE__
#import <OpenGL/OpenGL.h>
#include "glfw.h"
#else
#include "GL/glew.h"
#include "GL/glfw.h"
#endif

#include "math3d.h"

// The GLCamera (OrthonormalFrame) class. Possibly the most useful little piece of 3D graphics
// code for OpenGL immersive environments.
// Richard S. Wright Jr.
class GLCamera
{
protected:
	M3DVector3f vOrigin;	// Where am I?
	M3DVector3f vForward;	// Where am I going?
	M3DVector3f vUp;		// Which way is up?
	
	float m_near;
	float m_far;
	
public:
	
	float viewHeight;
	float viewWidth;
	
	// Default position and orientation. At the origin, looking
	// down the positive Z axis (right handed coordinate system).
	GLCamera(void) {
		m_near = 0.3f;
		//m_far = 150.0f;
		m_far = 200.0f;
		viewWidth = viewHeight = 0.0f;
		// At origin
		vOrigin[0] = 0.0f; vOrigin[1] = 0.0f; vOrigin[2] = 0.0f; 
		
		// Up is up (+Y)
		vUp[0] = 0.0f; vUp[1] = 1.0f; vUp[2] = 0.0f;
		
		// Forward is -Z (default OpenGL)
		vForward[0] = 0.0f; vForward[1] = 0.0f; vForward[2] = -1.0f;
	}
	
	
	/////////////////////////////////////////////////////////////
	// Set Location
	inline void SetOrigin(const M3DVector3f vPoint) {
		m3dCopyVector3(vOrigin, vPoint); }
	
	inline void SetOrigin(float x, float y, float z) { 
		vOrigin[0] = x; vOrigin[1] = y; vOrigin[2] = z; }
	
	inline void GetOrigin(M3DVector3f vPoint) {
		m3dCopyVector3(vPoint, vOrigin); }
	
	inline float GetOriginX(void) { return vOrigin[0]; }
	inline float GetOriginY(void) { return vOrigin[1]; } 
	inline float GetOriginZ(void) { return vOrigin[2]; }
	
	inline void SetOriginX(float value) {  vOrigin[0] = value; }
	inline void SetOriginY(float value) {  vOrigin[1] = value; } 
	inline void SetOriginZ(float value) {  vOrigin[2] = value; }
	
	/////////////////////////////////////////////////////////////
	// Set Forward Direction
	inline void SetForwardVector(const M3DVector3f vDirection) {
		m3dCopyVector3(vForward, vDirection); }
	
	inline void SetForwardVector(float x, float y, float z)
	{ vForward[0] = x; vForward[1] = y; vForward[2] = z; }
	
	inline void GetForwardVector(M3DVector3f vVector) { m3dCopyVector3(vVector, vForward); }
	
	/////////////////////////////////////////////////////////////
	// Set Up Direction
	inline void SetUpVector(const M3DVector3f vDirection) {
		m3dCopyVector3(vUp, vDirection); }
	
	inline void SetUpVector(float x, float y, float z)
	{ vUp[0] = x; vUp[1] = y; vUp[2] = z; }
	
	inline void GetUpVector(M3DVector3f vVector) { m3dCopyVector3(vVector, vUp); }
	
	
	/////////////////////////////////////////////////////////////
	// Get Axes
	inline void GetZAxis(M3DVector3f vVector) { GetForwardVector(vVector); }
	inline void GetYAxis(M3DVector3f vVector) { GetUpVector(vVector); }
	inline void GetXAxis(M3DVector3f vVector) { m3dCrossProduct3(vVector, vUp, vForward); }
	
	
	/////////////////////////////////////////////////////////////
	// Translate along orthonormal axis... world or local
	inline void TranslateWorld(float x, float y, float z)
	{ vOrigin[0] += x; vOrigin[1] += y; vOrigin[2] += z; }
	
	inline void TranslateLocal(float x, float y, float z)
	{ MoveForward(z); MoveUp(y); MoveRight(x);	}
	
	
	/////////////////////////////////////////////////////////////
	// Move Forward (along Z axis)
	inline void MoveForward(float fDelta)
	{
		// Move along direction of front direction
		vOrigin[0] += vForward[0] * fDelta;
		vOrigin[1] += vForward[1] * fDelta;
		vOrigin[2] += vForward[2] * fDelta;
	}
	
	// Move along Y axis
	inline void MoveUp(float fDelta)
	{
		// Move along direction of up direction
		vOrigin[0] += vUp[0] * fDelta;
		vOrigin[1] += vUp[1] * fDelta;
		vOrigin[2] += vUp[2] * fDelta;
	}
	
	// Move along X axis
	inline void MoveRight(float fDelta)
	{
		// Move along direction of right vector
		M3DVector3f vCross;
		m3dCrossProduct3(vCross, vUp, vForward);
		
		vOrigin[0] += vCross[0] * fDelta;
		vOrigin[1] += vCross[1] * fDelta;
		vOrigin[2] += vCross[2] * fDelta;
	}
	
	
	///////////////////////////////////////////////////////////////////////
	// Just assemble the matrix
	void GetMatrix(M3DMatrix44f matrix, bool bRotationOnly = false)
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
		
		// Translation (already done)
		if(bRotationOnly == true)
		{
			matrix[12] = 0.0f;
			matrix[13] = 0.0f;
			matrix[14] = 0.0f;
		}
		else
			m3dSetMatrixColumn44(matrix, vOrigin, 3);
		
		matrix[15] = 1.0f;
	}
	
	
	
	////////////////////////////////////////////////////////////////////////
	// Assemble the camera matrix
	void GetCameraMatrix(M3DMatrix44f m, bool bRotationOnly = false)
	{
		M3DVector3f x, z;
		
		// Make rotation matrix
		// Z vector is reversed
		z[0] = -vForward[0];
		z[1] = -vForward[1];
		z[2] = -vForward[2];
		
		// X vector = Y cross Z 
		m3dCrossProduct3(x, vUp, z);
		
		// Matrix has no translation information and is
		// transposed.... (rows instead of columns)
#define M(row,col)  m[col*4+row]
		M(0, 0) = x[0];
		M(0, 1) = x[1];
		M(0, 2) = x[2];
		M(0, 3) = 0.0;
		M(1, 0) = vUp[0];
		M(1, 1) = vUp[1];
		M(1, 2) = vUp[2];
		M(1, 3) = 0.0;
		M(2, 0) = z[0];
		M(2, 1) = z[1];
		M(2, 2) = z[2];
		M(2, 3) = 0.0;
		M(3, 0) = 0.0;
		M(3, 1) = 0.0;
		M(3, 2) = 0.0;
		M(3, 3) = 1.0;
#undef M
		
		
		if(bRotationOnly)
			return;
		
		// Apply translation too
		M3DMatrix44f trans, M;
		m3dTranslationMatrix44(trans, -vOrigin[0], -vOrigin[1], -vOrigin[2]);  
		
		m3dMatrixMultiply44(M, m, trans);
        
		// Copy result back into m
		memcpy(m, M, sizeof(float)*16);
	}
	
	
	// Rotate around local Y
	void RotateLocalY(float fAngle)
	{
		M3DMatrix44f rotMat;
		
		// Just Rotate around the up vector
		// Create a rotation matrix around my Up (Y) vector
		m3dRotationMatrix44(rotMat, fAngle,
							vUp[0], vUp[1], vUp[2]);
		
		M3DVector3f newVect;
		
		// Rotate forward pointing vector (inlined 3x3 transform)
		newVect[0] = rotMat[0] * vForward[0] + rotMat[4] * vForward[1] + rotMat[8] *  vForward[2];	
		newVect[1] = rotMat[1] * vForward[0] + rotMat[5] * vForward[1] + rotMat[9] *  vForward[2];	
		newVect[2] = rotMat[2] * vForward[0] + rotMat[6] * vForward[1] + rotMat[10] * vForward[2];	
		m3dCopyVector3(vForward, newVect);
	}
	
	
	// Rotate around local Z
	void RotateLocalZ(float fAngle)
	{
		M3DMatrix44f rotMat;
		
		// Only the up vector needs to be rotated
		m3dRotationMatrix44(rotMat, fAngle,
							vForward[0], vForward[1], vForward[2]);
		
		M3DVector3f newVect;
		newVect[0] = rotMat[0] * vUp[0] + rotMat[4] * vUp[1] + rotMat[8] *  vUp[2];	
		newVect[1] = rotMat[1] * vUp[0] + rotMat[5] * vUp[1] + rotMat[9] *  vUp[2];	
		newVect[2] = rotMat[2] * vUp[0] + rotMat[6] * vUp[1] + rotMat[10] * vUp[2];	
		m3dCopyVector3(vUp, newVect);
	}
	
	void RotateLocalX(float fAngle)
	{
		M3DMatrix33f rotMat;
		M3DVector3f  localX;
		M3DVector3f  rotVec;
		
		// Get the local X axis
		m3dCrossProduct3(localX, vUp, vForward);
		
		// Make a Rotation Matrix
		m3dRotationMatrix33(rotMat, fAngle, localX[0], localX[1], localX[2]);
		
		// Rotate Y, and Z
		m3dRotateVector(rotVec, vUp, rotMat);
		m3dCopyVector3(vUp, rotVec);
		
		m3dRotateVector(rotVec, vForward, rotMat);
		m3dCopyVector3(vForward, rotVec);
	}
	
	
	// Reset axes to make sure they are orthonormal. This should be called on occasion
	// if the matrix is long-lived and frequently transformed.
	void Normalize(void)
	{
		M3DVector3f vCross;
		
		// Calculate cross product of up and forward vectors
		m3dCrossProduct3(vCross, vUp, vForward);
		
		// Use result to recalculate forward vector
		m3dCrossProduct3(vForward, vCross, vUp);	
		
		// Also check for unit length...
		m3dNormalizeVector3(vUp);
		m3dNormalizeVector3(vForward);
	}
	
	
	// Rotate in world coordinates...
	void RotateWorld(float fAngle, float x, float y, float z)
	{
		M3DMatrix44f rotMat;
		
		// Create the Rotation matrix
		m3dRotationMatrix44(rotMat, fAngle, x, y, z);
		
		M3DVector3f newVect;
		
		// Transform the up axis (inlined 3x3 rotation)
		newVect[0] = rotMat[0] * vUp[0] + rotMat[4] * vUp[1] + rotMat[8] *  vUp[2];	
		newVect[1] = rotMat[1] * vUp[0] + rotMat[5] * vUp[1] + rotMat[9] *  vUp[2];	
		newVect[2] = rotMat[2] * vUp[0] + rotMat[6] * vUp[1] + rotMat[10] * vUp[2];	
		m3dCopyVector3(vUp, newVect);
		
		// Transform the forward axis
		newVect[0] = rotMat[0] * vForward[0] + rotMat[4] * vForward[1] + rotMat[8] *  vForward[2];	
		newVect[1] = rotMat[1] * vForward[0] + rotMat[5] * vForward[1] + rotMat[9] *  vForward[2];	
		newVect[2] = rotMat[2] * vForward[0] + rotMat[6] * vForward[1] + rotMat[10] * vForward[2];	
		m3dCopyVector3(vForward, newVect);
	}
	
	
	// Rotate around a local axis
	void RotateLocal(float fAngle, float x, float y, float z) 
	{
		M3DVector3f vWorldVect;
		M3DVector3f vLocalVect;
		m3dLoadVector3(vLocalVect, x, y, z);
		
		LocalToWorld(vLocalVect, vWorldVect, true);
		RotateWorld(fAngle, vWorldVect[0], vWorldVect[1], vWorldVect[2]);
	}
    
	
	// Convert Coordinate Systems
	// This is pretty much, do the transformation represented by the rotation
	// and position on the point
	// Is it better to stick to the convention that the destination always comes
	// first, or use the conventions that "sounds" like the function...
	void LocalToWorld(const M3DVector3f vLocal, M3DVector3f vWorld, bool bRotOnly = false)
	{
		// Create the rotation matrix based on the vectors
		M3DMatrix44f rotMat;
		
		GetMatrix(rotMat, true);
		
		// Do the rotation (inline it, and remove 4th column...)
		vWorld[0] = rotMat[0] * vLocal[0] + rotMat[4] * vLocal[1] + rotMat[8] *  vLocal[2];	
		vWorld[1] = rotMat[1] * vLocal[0] + rotMat[5] * vLocal[1] + rotMat[9] *  vLocal[2];	
		vWorld[2] = rotMat[2] * vLocal[0] + rotMat[6] * vLocal[1] + rotMat[10] * vLocal[2];	
		
		// Translate the point
		if(!bRotOnly) {
			vWorld[0] += vOrigin[0];
			vWorld[1] += vOrigin[1];
			vWorld[2] += vOrigin[2];
		}
	}
	
	// Change world coordinates into "local" coordinates
	void WorldToLocal(const M3DVector3f vWorld, M3DVector3f vLocal)
	{
		////////////////////////////////////////////////
		// Translate the origin
		M3DVector3f vNewWorld;
		vNewWorld[0] = vWorld[0] - vOrigin[0];
		vNewWorld[1] = vWorld[1] - vOrigin[1];
		vNewWorld[2] = vWorld[2] - vOrigin[2];
		
		// Create the rotation matrix based on the vectors
		M3DMatrix44f rotMat;
		M3DMatrix44f invMat;
		GetMatrix(rotMat, true);
		
		// Do the rotation based on inverted matrix
		m3dInvertMatrix44(invMat, rotMat);
		
		vLocal[0] = invMat[0] * vNewWorld[0] + invMat[4] * vNewWorld[1] + invMat[8] *  vNewWorld[2];	
		vLocal[1] = invMat[1] * vNewWorld[0] + invMat[5] * vNewWorld[1] + invMat[9] *  vNewWorld[2];	
		vLocal[2] = invMat[2] * vNewWorld[0] + invMat[6] * vNewWorld[1] + invMat[10] * vNewWorld[2];	
	}
	
	/////////////////////////////////////////////////////////////////////////////
	// Transform a point by frame matrix
	void TransformPoint(M3DVector3f vPointSrc, M3DVector3f vPointDst)
	{
		M3DMatrix44f m;
		GetMatrix(m, false);    // Rotate and translate
		vPointDst[0] = m[0] * vPointSrc[0] + m[4] * vPointSrc[1] + m[8] *  vPointSrc[2] + m[12];// * v[3];	 
		vPointDst[1] = m[1] * vPointSrc[0] + m[5] * vPointSrc[1] + m[9] *  vPointSrc[2] + m[13];// * v[3];	
		vPointDst[2] = m[2] * vPointSrc[0] + m[6] * vPointSrc[1] + m[10] * vPointSrc[2] + m[14];// * v[3];	
	}
	
	////////////////////////////////////////////////////////////////////////////
	// Rotate a vector by frame matrix
	void RotateVector(M3DVector3f vVectorSrc, M3DVector3f vVectorDst)
	{
		M3DMatrix44f m;
		GetMatrix(m, true);    // Rotate only
		
		vVectorDst[0] = m[0] * vVectorSrc[0] + m[4] * vVectorSrc[1] + m[8] *  vVectorSrc[2];	 
		vVectorDst[1] = m[1] * vVectorSrc[0] + m[5] * vVectorSrc[1] + m[9] *  vVectorSrc[2];	
		vVectorDst[2] = m[2] * vVectorSrc[0] + m[6] * vVectorSrc[1] + m[10] * vVectorSrc[2];	
	}
	
	void Render() {
		glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed  
		glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)  
		gluPerspective(60, (GLfloat)viewWidth / (GLfloat)viewHeight,m_near, m_far); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and m_far planes  
		glMatrixMode(GL_MODELVIEW); // Sw
		glLoadIdentity();
		M3DMatrix44f m;
		GetCameraMatrix(m);
		glMultMatrixf(m);
		
		
	}
	void SetClippingDistance(float n, float f) {
		m_near = n;
		m_far = f;
	}
	float GetNearClippingDistance() { return m_near;}
	float GetFarClippingDistance() { return m_far;}
	
	float GetRotateY() {
		M3DVector3f North, Forward;
		GetForwardVector(Forward);
		North[0] = 0.0f;
		North[1] = 0.0f;
		North[2] = -1.0f;
		Forward[1] = 0.0f;
		m3dNormalizeVector3(Forward);
		float angleDir = m3dRadToDeg(m3dGetAngleBetweenVectors3(Forward,North));
		
		if (Forward[0] > 0.0f) {
			angleDir = 360 - angleDir;
		}
		
		return angleDir;
		
	}
	
};

extern GLCamera gCamera;

#endif