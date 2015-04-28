/*
 *  spMesh.h
 *  SPMexporter
 *
 *  Created by Scott Scaccianoce on 7/2/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifndef __SPMESH__
#define __SPMESH__

#include "math3d.h"

#include <cstdio>
#include <map>
#include <string>
#include <vector>

#ifdef __APPLE__
#import <OpenGL/gl.h>
#else
#include "gl/glew.h"
#endif

#include "texture.h"
#include "shader.h"
#include "math3d.h"
#include "Camera.h"

struct spVec3 {
	float x;
	float y;
	float z;
	spVec3() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	spVec3(float a,float b,float c) : x(a),y(b),z(c)
	{}
};

struct spVec2 {
	float x;
	float y;
	spVec2() {
		x = 0.0f;
		y = 0.0f;
	}
	spVec2(float a,float b) : x(a),y(b)
	{}
};

struct SPMeshVert {
	spVec3 pos;
	spVec3 normal;
};

struct SPFrame {
	vector<SPMeshVert> first;
	vector<SPMeshVert> second;
	float steps;
};



using namespace std;

class SPMesh {
	
public:
	SPMesh();
	~SPMesh();
	
	void BuildBB();
	void Render();
	void RenderAnimation(int frame, float tween);
	void RenderBB();
	void AddObjModel(const char* filename, const char *path, bool first = true);

	void SetKeyframeTotals(vector<float> &keyframes);
	float GetStepCount(int frame_index) {return frames[frame_index].steps;}
	void SetStepCount(int frame_index, float value) { frames[frame_index].steps = value;}	
	
	void GetBBMin(M3DVector4f &outMin);
	void GetBBMax(M3DVector4f &outMax);
	
	float GetRadius();
	void ExportMesh(const string &filename);
	void ImportMesh(const string &filename, const char *path);
	
	M3DVector4f center;
	M3DVector3f scale;
	float radius;
	float halfX;
	float halfZ;
	bool hasAnimation;
	float speed;
	bool repeat;
	unsigned int db_id;
	std::string name;
	
	
	
private:
	
	void LoadObjFile(const char* filename, const char *path, bool first);
	void LoadMtlFile(const char* filename, const char *path);
	
	vector<spVec3> vertices;
	vector<spVec3> normals;
	vector<spVec2> Texcoords;
	
	//vector<GLushort> elements;
	//vector<GLushort> elementsTex;
	//vector<GLushort> elementsNorm;
	
	vector<SPFrame> frames;
	vector<spVec2> texUVs;
	vector<unsigned int> indices;

	sTexture texture;
	bool hasTexture;
	Shader	*shader;
	Shader  *animationShader;
	
	
	M3DVector3f BBPoints[8];
	M3DVector3f min;
	M3DVector3f max;
	
	unsigned int indexCount;
	
	void LoadHalfVectors();
		
	
	
};

#endif