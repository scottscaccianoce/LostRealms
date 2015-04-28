/*
 *  skyplane.cpp
 *  MapEditor
 *
 *  Created by Scott Scaccianoce on 6/19/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "skyplane.h"


SkyPlane::SkyPlane(M3DVector3f Position, int Size, int Scale, float Tile, float AtmosphereRadius) {
	
	skyshader = new Shader();
	cloudshader = new Shader();
	
	skyshader->Load("sky.vs", "sky.fs");
	
	cloudshader->Load("clouds.vs", "clouds.fs");
	
	clouds.loadTexture("clouds.tga");
	
	/*morning[0] = 0.519f;
	morning[1] =  0.679f;
	morning[2] = 0.890f;
	morning[3] = 1.0f;
	
	day[0] =0.89f;
	day[1] = 0.59f;
	day[2] = 0.29f;
	day[3] = 1.0f;
	
	night[0] = 0.04f;
	night[1] = 0.05f;
	night[2] = 0.25f;
	night[3] = 1.0f;
	*/
	
	m3dCopyVector3(position, Position);
	size = Size;
	scale = Scale;
	atmosphereRadius = AtmosphereRadius;
	tile = Tile;
	verts.resize(size * size);
	indexCount = (size -1) * (size *2 + 2) -2;
	indices.resize(indexCount);
	
	int iCX = size / 2;
	int iCY = iCX;
	float fDistSq = 0.0f;
	float fRadiusSq = ((float)size/2)*((float)size/2);
	float fHeight = 0.0f;
	int index = 0;
	
	for (unsigned int i = 0; i < size; ++i) {
		for(unsigned int k = 0; k < size; ++k) {
			verts[index].pos.x = position[0] + (-(iCX) * scale) + k * scale;
			verts[index].pos.z = position[2] + ((iCX)*scale) + (-(int)i * scale);
			
			verts[index].pos.y = position[1];
			fDistSq = (float)(( iCX -k) * (iCX - k) + (iCY -i) * (iCY - i));
			fHeight = fRadiusSq - fDistSq;
			fHeight *= atmosphereRadius;
			verts[index].pos.y += fHeight;
			verts[index].tex3.z = (fRadiusSq - (fDistSq -10)) / fRadiusSq;
			
			//Pull edges of the sky plane down so we don't see beind
			if (k == 0 || k == size - 1 || i == 0 || i == size -1) {
				verts[index].pos.y -= 10000.0f;
				verts[index].tex3.z = 0.0f;
			}
			
			if (k == 1 || k == (size -2) || i == 1 || i == (size -2) ){
				verts[index].tex3.z = 0.0f;
			}
								  
			//tex cords
			verts[index].tex3.x = tile * ((float)k/size);
			verts[index].tex3.y = tile * (1.0f - (float) i / size);
					
			++index;
			
		}
	}

	index = 0;
	int cache = 0;
	int tmpCache = 0;
	//Calculate indices
	for(unsigned int i =0; i < size -1; ++i) {
		tmpCache = i * size;
		for(unsigned int k = 0; k < size; ++k) {
			cache = k + tmpCache;
			if (k == 0 && i != 0) {
				indices[index++] = cache;
			}
			indices[index++] = cache;
			indices[index++] = cache + size;
			if (k == size -1 && i != size -2) {
				indices[index++] = cache + size;
			}
		}
	}
	
	ComputeDimensions();
}

SkyPlane::~SkyPlane() {
	
	delete skyshader;
	delete cloudshader;
}
void SkyPlane::Render() {

	
	
	
	glPushMatrix();
	
	
	
	skyshader->Begin();		
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(sVert),&verts[0].pos.x);
	
	
	skyshader->SetUniform1f("skyMin", min[1] + gCamera.GetOriginY());
	skyshader->SetUniform1f("skyMax", max[1] + gCamera.GetOriginY());

	
	skyshader->SetUniform4f("skycolor", gDayNight.currentColor[0], gDayNight.currentColor[1], gDayNight.currentColor[2], 1.0f);
	
	glDrawElements(GL_TRIANGLE_STRIP,indexCount,GL_UNSIGNED_INT ,&indices[0]);
	skyshader->End();
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	cloudshader->Begin();
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(sVert),&verts[0].pos.x);
	
	cloudshader->SetUniform1i("cloudTex",0);
	
	if(gDayNight.curTime >= 20 || (gDayNight.curTime >= 0 && gDayNight.curTime <= 8) ) {
		cloudshader->SetUniform1f("cloudDensity",0.45f);
	} else {
		cloudshader->SetUniform1f("cloudDensity",0.45f);
	}
	
	
	glActiveTexture(GL_TEXTURE0);
	clouds.bindTexture();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(3,GL_FLOAT,sizeof(sVert),&verts[0].tex3.x);
	
	glDrawElements(GL_TRIANGLE_STRIP,indexCount,GL_UNSIGNED_INT ,&indices[0]);
	cloudshader->End();
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glDisable (GL_BLEND);
	
	
	glPopMatrix();
	
	
	
	glActiveTexture(GL_TEXTURE0);
}

void SkyPlane::ComputeDimensions()
{
	max[0] = -100000.0f; 
	max[1] = -100000.0f;
	max[2] = -100000.0f;
	
	min[0] = 100000.0f; 
	min[1] = 100000.0f;
	min[2] = 100000.0f;
	
    for(unsigned int i=0; i< size * size ; i++)
	{
        if(max[0] < verts[i].pos.x) max[0] = verts[i].pos.x;
        if(max[1] < verts[i].pos.y) max[1] = verts[i].pos.y;
        if(max[2] < verts[i].pos.z) max[2] = verts[i].pos.z;
		
        if(min[0] > verts[i].pos.x) min[0] = verts[i].pos.x;
        if(min[1] > verts[i].pos.y) min[1] = verts[i].pos.y;
        if(min[2] > verts[i].pos.z) min[2] = verts[i].pos.z;
    }

	
	
    
}