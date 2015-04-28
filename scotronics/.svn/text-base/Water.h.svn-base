/*
 *  Water.h
 *  MapEditor
 *
 *  Created by Scott Scaccianoce on 6/23/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifndef __WATER__
#define __WATER__
#include "math3d.h"
#include "util.h"
#include "shader.h"
#include "texture.h"
#include "app.h"

#include <vector>

class Water {
	
public:
	Water(int numVertRows, int numVertCols, float dx, float dz, const char* wave1_filename, const char* wave2_filename);
	~Water();
	void Render();
	void AdvRender();
	void Update(float dt);
	
	void StartReflection();
	void EndReflection();
	
	void StartRefaction();
	void EndRefaction();
	
private:
	
	//sVert verts [4];
	//int indices[6];
	
	std::vector<sVert> verts;
	std::vector<unsigned int>indices;
	
	Shader *waterShader;
	Shader *advWaterShader;
	
	sTexture map0;
	sTexture map1;
	
	M3DVector2f map0_offset;
	M3DVector2f map1_offset;
	
	M3DVector2f map0_velocity;
	M3DVector2f map1_velocity;
	
	unsigned int mNumVertices;
	unsigned int mNumTriangles;
	
	int numCellRows;
	int numCellCols;
	float width, depth;
	int numberVertRows;
	int numberVertCols;
	int numIndices;
	
	int texSize;
	
	GLuint reflection;
	GLuint refaction;
	GLuint tdepth;
	
	
};

#endif