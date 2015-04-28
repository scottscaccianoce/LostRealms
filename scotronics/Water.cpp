/*
 *  Water.cpp
 *  MapEditor
 *
 *  Created by Scott Scaccianoce on 6/23/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "Water.h"
//#include "Camera.h"

Water::Water(int numVertRows, int numVertCols, float dx, float dz, const char* filename1, const char* filename2 ) {
	
	texSize = 512;
	glGenTextures(1, &reflection);	
	glBindTexture(GL_TEXTURE_2D,reflection);	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texSize, texSize, 0,
				 GL_RGB, GL_UNSIGNED_BYTE, NULL);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glGenTextures(1, &refaction);	
	glBindTexture(GL_TEXTURE_2D,refaction);	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texSize, texSize, 0,
				 GL_RGB, GL_UNSIGNED_BYTE, NULL);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	
	glGenTextures(1, &tdepth);	
	glBindTexture(GL_TEXTURE_2D,tdepth);	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texSize, texSize, 0,
				 GL_RGB, GL_UNSIGNED_BYTE, NULL);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	map0.loadTexture(filename1);
	map1.loadTexture(filename2);
	
	int numVertices = numVertRows * numVertCols;
	mNumVertices = numVertices;
	numCellRows = numVertRows -1;
	numCellCols = numVertCols -1;
	numberVertRows = numVertRows;
	numberVertCols = numVertCols;
	//float scale = numVertCols;
	float scale = 5.0f;

	int numTris = numCellRows * numCellCols * 2;
	
	width = (float)numCellCols * dx;
	depth = (float)numCellRows * dz;
	
	/*verts.resize(numVertices);

	
	float xOffset = -width * 0.5f;
	float zOffset = depth * 0.5f;
	
	unsigned int k = 0;
	float udx = 0;
	
	for(int i = 0; i < numVertRows; ++i){
		float vdx = 0;
		for(int j = 0; j < numVertCols; ++j){
			//negate the depth coordinate to put in quadrant
			//then offset to center
			verts[k].pos.x = (float)(j* dx +xOffset);
			verts[k].pos.z = (float)(-i * dz + zOffset);
			verts[k].pos.y = 0.0f;
			verts[k].normal.x = 0.0f;
			verts[k].normal.y = 1.0f;
			verts[k].normal.z = 0.0f;
			verts[k].tex0.x = (float)(j / (float)(numVertCols -1)) * scale;
			verts[k].tex0.y = 1 - (float)(i / (float)(numVertRows -1)) * scale * 0.90f;
			
			vdx += 1.0f;
			++k;
			
		}
		udx += 1.0f;
	}
	
	//indices
	numIndices = numTris * 3;
	indices.resize(numIndices);
	
	k =0;
	for(unsigned int i = 0; i < (unsigned int)numCellRows; ++i)
	{
		for(unsigned int j =0; j < (unsigned int)numCellCols; ++j) {
			
			indices[k]		= (i+1) * numVertCols + j;
			indices[k + 1]	= i		* numVertCols + j;
			indices[k + 2]	= (i+1) * numVertCols + j + 1;
			
			indices[k + 3]	= (i+1) * numVertCols + j + 1;
			indices[k + 4]	= i		* numVertCols + j;
			indices[k + 5]	= i		* numVertCols + j +1; 
			
			k+= 6;
			
		}
		
		
	}
*/
	
	float scalex = 60.0f;
	float scaley = 35.0f;
	verts.resize(4);
	float offset = dx/2.0f;
	verts[0].pos.x = -offset;
	verts[0].pos.y = 0.0f;
	verts[0].pos.z = -offset;
	verts[0].normal.x = 0.0f;
	verts[0].normal.y = 1.0f;
	verts[0].normal.z = 0.0f;
	verts[0].tex0.x = 0.0f;
	verts[0].tex0.y = 1.0f * scaley;
	
	
	
	verts[1].pos.x = offset;
	verts[1].pos.y = 0.0f;
	verts[1].pos.z = -offset;
	verts[1].normal.x = 0.0f;
	verts[1].normal.y = 1.0f;
	verts[1].normal.z = 0.0f;
	verts[1].tex0.x = 1.0f * scalex;
	verts[1].tex0.y = 1.0f * scaley;
	
	
	verts[2].pos.x = -offset;
	verts[2].pos.y = 0.0f;
	verts[2].pos.z = offset;
	verts[2].normal.x = 0.0f;
	verts[2].normal.y = 1.0f;
	verts[2].normal.z = 0.0f;
	verts[2].tex0.x = 0.0f;
	verts[2].tex0.y = 0.0f;
	
	verts[3].pos.x = offset;
	verts[3].pos.y = 0.0f;
	verts[3].pos.z = offset;
	verts[3].normal.x = 0.0f;
	verts[3].normal.y = 1.0f;
	verts[3].normal.z = 0.0f;
	verts[3].tex0.x = 1.0f * scalex;
	verts[3].tex0.y = 0.0f;
	
	indices.resize(6);
	indices[0] = 2;
	indices[1] = 3;
	indices[2] = 0;
	
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 1;
	 
	numIndices = 6;
	
	waterShader = new Shader();
	waterShader->Load("water.vs", "water.fs");
	
	advWaterShader = new Shader();
	advWaterShader->Load("advWater.vs", "advWater.fs");
	
	map0_offset[0] = 0.03f;
	map0_offset[1] = 0.05f;
	
	map1_offset[0] = -0.9f;
	map1_offset[1] = 0.4f;
	
	map0_velocity[0] = 0.03f / scalex;
	map0_velocity[1] = 0.05f  / scaley;
	
	map1_velocity[0] = -0.3f  / scalex;
	map1_velocity[1] = 0.2f   / scaley;
	
	

}

Water::~Water() {
	delete waterShader;
	delete advWaterShader;
	glDeleteTextures( 1, &refaction );
	glDeleteTextures( 1, &reflection );
	glDeleteTextures( 1, &tdepth );
}
void Water::Render() {
	glPushMatrix();
	//glColor3f(0.0f, 0.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	waterShader->Begin();
	
	M3DVector4f vLightPos = {250.0f, 500.0f, 0.0f, 1.0f};
	M3DVector4f vLightEyePos;
	M3DMatrix44f mCamera;
	gCamera.GetCameraMatrix(mCamera);
	m3dTransformVector3(vLightEyePos, vLightPos, mCamera);
	
	M3DVector3f lDir = {0.0f, -2.0f, 1.0f};
	m3dNormalizeVector3(lDir);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(sVert),&verts[0].pos.x);
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,sizeof(sVert),&verts[0].tex0.x);
	
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(sVert), &verts[0].normal.x);
	
	float proj[16];
	glGetFloatv(GL_PROJECTION_MATRIX, proj);
	//waterShader->SetUniformMatrix4fv("proj", <#GLsizei count#>, <#GLboolean transpose#>, <#GLfloat *value#>, <#GLint index#>)
	
	waterShader->SetUniform1i("map0",0);
	waterShader->SetUniform1i("map1",1);
	waterShader->SetUniform3f("lightdir",lDir[0], lDir[1], lDir[2]);
	waterShader->SetUniform4f("eyePosW", 0.0f, 100.0f, 0.0f, 1.0f);
	waterShader->SetUniform2f("map0_offset", map0_offset[0], map0_offset[1]);
	waterShader->SetUniform2f("map1_offset", map1_offset[0], map1_offset[1]);
	
	waterShader->SetUniform3f("diffuseMtrl",0.16f, 0.33f, 0.9f, 0.70);
	waterShader->SetUniform3f("ambientMtrl",0.16f, 0.33f, 0.6f, 0.70);
	waterShader->SetUniform3f("specularMtrl", 1.0f, 1.0f, 1.0f, 1.0f);
	waterShader->SetUniform1f("specPower", 64.0f);
	
	glActiveTexture(GL_TEXTURE0);
	map0.bindTexture();
	glActiveTexture(GL_TEXTURE1);
	map1.bindTexture();
	
	glDrawElements(GL_TRIANGLES,numIndices,GL_UNSIGNED_INT ,&indices[0]);
	
//	glDrawElements(GL_TRIANGLE_STRIP,6,GL_UNSIGNED_INT ,&indices[0]);
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	waterShader->End();
	
	glDisable(GL_BLEND);
	glPopMatrix();
	glActiveTexture(GL_TEXTURE0);
}
void Water::Update(float dt) {
	
	// Update texture coordinate offsets.  These offsets are added to the
	// texture coordinates in the vertex shader to animate them.
	M3DVector2f velocity0, velocity1;
	m3dCopyVector2(velocity0, map0_velocity);
	m3dCopyVector2(velocity1, map1_velocity);
	
	
	
	m3dScaleVector2(velocity0, dt);
	m3dScaleVector2(velocity1, dt);
	
	m3dAddVectors2(map0_offset, map0_offset,velocity0);
	m3dAddVectors2(map1_offset, map1_offset, velocity1);				   
								  
	
	// Textures repeat every 1.0 unit, so reset back down to zero
	// so the coordinates do not grow too large.
	if(map0_offset[0] >= 1.0f || map0_offset[0] <= -1.0f)
		map0_offset[0] = 0.0f;
	
	if(map1_offset[0] >= 1.0f || map1_offset[0] <= -1.0f)
		map1_offset[0] = 0.0f;
	
	if(map0_offset[1] >= 1.0f || map0_offset[1] <= -1.0f)
		map0_offset[1] = 0.0f;
	
	if(map1_offset[1] >= 1.0f || map1_offset[1] <= -1.0f)
		map1_offset[1] = 0.0f;
}









void Water::StartReflection() {
gCamera.Render();
	glPushMatrix();
	
	glViewport(0,0, texSize, texSize);
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glScalef(1.0f, -1.0f, 1.0f);
	//double plane[4] = {0.0, 1.0, 0.0, 0.5}; //water at y=0
	double plane[4] = {0.0, 5.0, 0.0, 1.0}; //water at y=0
   	glEnable(GL_CLIP_PLANE0);
   	glClipPlane(GL_CLIP_PLANE0, plane);
	
	

}
void Water::EndReflection() {
	//glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	//render reflection to texture
   	glBindTexture(GL_TEXTURE_2D, reflection);
    //glCopyTexSubImage2D copies the frame buffer 
    //to the bound texture
   	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,texSize, texSize);
	
	
	
}



void Water::StartRefaction() {
	gCamera.Render();
	glPushMatrix();
	
	glViewport(0,0, texSize, texSize);
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
   	//gCamera.Render();
	double plane[4] = {0.0, 0.0, 0.0, 0.0}; //water at y=0
   	glEnable(GL_CLIP_PLANE0);
   	glClipPlane(GL_CLIP_PLANE0, plane);
	
	
	
}
void Water::EndRefaction() {
	
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	//render reflection to texture
   	glBindTexture(GL_TEXTURE_2D, refaction);
    //glCopyTexSubImage2D copies the frame buffer 
    //to the bound texture
   	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,texSize, texSize);
	
	//render depth to texture
   	glBindTexture(GL_TEXTURE_2D, tdepth);
   	glCopyTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT, 0,0, texSize,texSize, 0);
	
	
	
}

void Water::AdvRender() {
	glPushMatrix();
	//glColor3f(0.0f, 0.0f, 1.0f);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	advWaterShader->Begin();
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(sVert),&verts[0].pos.x);
	
	
	
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(sVert), &verts[0].normal.x);
	

	
	advWaterShader->SetUniform1i("water_normalmap",0);
	advWaterShader->SetUniform1i("water_dudvmap",1);
	advWaterShader->SetUniform1i("water_reflection",2);
	advWaterShader->SetUniform1i("water_refraction",3);
	advWaterShader->SetUniform1i("water_depthmap",4);
	
	advWaterShader->SetUniform4f("waterColor", 0.15f, 0.33f, 0.9f, 0.0f);
	advWaterShader->SetUniform4f("waterDepth", 0.0f, 0.0f, 0.0f, 0.0f);
	
	M3DVector3f pos;
	gCamera.GetOrigin(pos);
	
	waterShader->SetUniform3f("viewpos",pos[0], pos[1], pos[2]);
	waterShader->SetUniform4f("lightpos", 0.0f, 50.0f, 0.0f, 1.0f);
	advWaterShader->SetUniform2f("map0_offset", map0_offset[0], map0_offset[1]);
	advWaterShader->SetUniform2f("map1_offset", map1_offset[0], map1_offset[1]);
	
	
	glActiveTexture(GL_TEXTURE0);
	map1.bindTexture();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,sizeof(sVert),&verts[0].tex0.x);
	
	
	glActiveTexture(GL_TEXTURE1);
	map1.bindTexture();
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,reflection);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,refaction);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D,tdepth);

	
	
	glDrawElements(GL_TRIANGLES,numIndices,GL_UNSIGNED_INT ,&indices[0]);
	
	//	glDrawElements(GL_TRIANGLE_STRIP,6,GL_UNSIGNED_INT ,&indices[0]);
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	advWaterShader->End();
	
	glDisable(GL_BLEND);
	glPopMatrix();
	glActiveTexture(GL_TEXTURE0);
}