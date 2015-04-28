
#ifndef __util__
#define __util__
#include "math3d.h"




#ifdef __APPLE__
#import <OpenGL/OpenGL.h>
#import <OpenGL/glu.h>
#import <math.h>
#else
//#include <GL/gl.h>
#include <math.h>
#include <time.h>
#endif

#include "Camera.h"
#include "GL/glfw.h"


#define PI 3.1415265359
#define PIdiv180 3.1415265359/180.0
#include <sstream>
#include <string>

struct sVector3
{
	GLfloat x,y,z;
	sVector3(GLfloat X, GLfloat Y, GLfloat Z) {
		x = X;
		y = Y;
		z = Z;
		
	}
	sVector3(){
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		
	}
};

struct sVector2 {
	GLfloat x,y;
	sVector2(GLfloat X, GLfloat Y) {
		x = X;
		y = Y;
		
	}
	sVector2(){
		x = 0.0f;
		y = 0.0f;
	}
	
};

//sVector3 F3dVector ( GLfloat x, GLfloat y, GLfloat z );
sVector3 AddsVectors3 ( sVector3 * u, sVector3 * v);

sVector3 SubsVectors3 ( sVector3 * v, sVector3 * u);
void AddVector3ToVector ( sVector3 * Dst, sVector3 * V2);

float Dot( sVector3 *u, sVector3 * v);
sVector3 Cross( sVector3 *u, sVector3 * v);


void Normalize(sVector3 &v);
bool CheckLineTri( sVector3 TP1, sVector3 TP2, sVector3 TP3, sVector3 PT1, sVector3 PT2, sVector3 & HitPos);






//-------------------------------


struct sVert {
	sVector3 pos;
	sVector3 normal;
	sVector2 tex0;
	sVector2 tex1;
	sVector3 tex3;

	sVert(){
		pos.x = 0.0f;
		pos.y = 0.0f;
		pos.z = 0.0f;
		
		normal.x = 0.0f;
		normal.y = 0.0f;
		normal.z = 0.0f;
		
		tex0.x = 0.0f;
		tex0.y = 0.0f;
		
		tex1.x = 0.0f;
		tex1.y = 0.0f;
		
		tex3.x = 0.0f;
		tex3.y = 0.0f;
		tex3.z = 0.0f;
		
	}

};

bool RaySphereTest(const M3DVector3f point, const M3DVector3f ray, const M3DVector3f sphereCenter, float sphereRadius);
void GetMouseRay(sVector3 &pos1, sVector3  &pos2, sVector3 &Dir);
std::string IntToString(int number);
std::string FloatToString(float number);


// Generate a random number between 0 and 1
// return a uniform number in [0,1].
float unifRand();
// Generate a random number in a real interval.
// param a one end point of the interval
// param b the other end of the interval
// return a inform rand numberin [a,b].
float unifRand(float a, float b);
// Reset the random number generator with the system clock.
void seed();

#endif