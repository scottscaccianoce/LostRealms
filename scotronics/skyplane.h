/*
 *  skyplane.h
 *  MapEditor
 *
 *  Created by Scott Scaccianoce on 6/19/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifndef __skyplane__
#define __skyplane__

#include "math3d.h"
#include <vector>
#include "util.h"
#include "shader.h"
#include "DayNight.h"
#include "texture.h"
#include "Camera.h"

class SkyPlane {
public:
	
	SkyPlane(M3DVector3f pos, int Size, int Scale, float tile, float AtmosphereRadius);
	~SkyPlane();
	void Render();
	
private:
	void ComputeDimensions();
	float max[3];
	float min[3];
	M3DVector3f position;
	int size;
	int scale;
	float atmosphereRadius;
	float tile;
	unsigned int indexCount;
	std::vector<unsigned int> indices;	
	std::vector<sVert>verts;
	
	float morning[4];
	float day[4];
	float night[4];
	
	Shader *skyshader;
	Shader *cloudshader;
	
	sTexture clouds;
	
	
};




#endif