/*
 *  model.cpp
 *  game
 *
 *  Created by Scott Scaccianoce on 6/5/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "SPMesh.h"
#include <fstream>
#include <sstream>


SPMesh::SPMesh(){ 
	hasTexture = false;
	hasAnimation = false;
	
	shader = new Shader();
	shader->Load("model.vs", "model.fs");
	
	animationShader = new Shader();
	animationShader->Load("modelA.vs", "modelA.fs");

	scale[0] = 1.0f;
	scale[1] = 1.0f;
	scale[2] = 1.0f;
	
	speed = 0.2f;
	db_id = 0;
}

SPMesh::~SPMesh() {
	delete shader;
	delete animationShader;
}
void SPMesh::AddObjModel(const char* filename, const char *path, bool first ) {
	LoadObjFile(filename,path, first);
	
}

void SPMesh::LoadObjFile(const char* filename, const char* path , bool first ) {
	hasAnimation = !first;
	
	//elements.clear();
	//elementsTex.clear();
	//elementsNorm.clear();
	vertices.clear();
	normals.clear();
	SPFrame frame;
	
	vector<SPMeshVert> *previousAnimationVerts;
	if (!first) {
		previousAnimationVerts = &frames[frames.size() -1].second;
		(*previousAnimationVerts).clear();
	}
	unsigned int indexCounter = 0;
	
	string fullfilename = string(path) + string(filename);
	ifstream in(fullfilename.c_str(), ios::in);
	if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }
	
	string line;
	while (getline(in, line)) {
		if (line.substr(0,6) == "mtllib" && first) {
			istringstream s(line.substr(6));
			string mtlfilename;
			s >> mtlfilename;
			LoadMtlFile(mtlfilename.c_str(), path);
		}
		
		if (line.substr(0,2) == "v ") {
			istringstream s(line.substr(2));
			spVec3 v; s >> v.x; s >> v.y; s >> v.z;
			vertices.push_back(v);
		} else if (line.substr(0,2) == "vt") {
			istringstream s(line.substr(2));
			spVec2 v; s >> v.x; s >> v.y;
			Texcoords.push_back(v);
		} else if (line.substr(0,2) == "vn") {
			istringstream s(line.substr(2));
			spVec3 v; s >> v.x; s >> v.y; s >> v.z;
			normals.push_back(v);
		}  else if (line.substr(0,2) == "f ") {
			GLuint vert[3];
			GLuint vn[3];
			GLuint vt[3];
			
			istringstream s(line.substr(2));
			string buffer;
			int i = 0;
			while (s >> buffer) {
				vert[i] = 0;
				vn[i] = 0;
				vt[i] = 0;
				
				if ( strstr(buffer.c_str(), "//" ) ) { //  v//n
					sscanf(buffer.c_str(), "%d//%d", &vert[i], &vn[i]);
					
				} else if (sscanf(buffer.c_str(), "%d/%d/%d", &vert[i], &vt[i], &vn[i]) == 3) { //  v/vt/vn
					
				} else if (sscanf(buffer.c_str(), "%d/%d", &vert[i], &vt[i]) == 2) { //  v/vt
					
				} else {
					sscanf(buffer.c_str(), "%d", &vert[i]);
					
				}
				++i;
				
			}
			
			vert[0]--; vert[1]--; vert[2]--;
			vt[0]--; vt[1]--; vt[2]--;
			vn[0]--; vn[1]--; vn[2]--;
			
			
			for (int k = 0; k < 3; ++k) {
				SPMeshVert spvert;
				spvert.pos.x = vertices[vert[k]].x;
				spvert.pos.y = vertices[vert[k]].y;
				spvert.pos.z = vertices[vert[k]].z;
				spvert.normal.x = normals[vn[k]].x;
				spvert.normal.y = normals[vn[k]].y;
				spvert.normal.z = normals[vn[k]].z;
				spVec2 t;
				
				if (first) {
					t.x = Texcoords[vt[k]].x;
					t.y = Texcoords[vt[k]].y;
					texUVs.push_back(t);
				}
				frame.first.push_back(spvert);
				frame.second.push_back(spvert);
				if (!first) {
					(*previousAnimationVerts).push_back(spvert);
				}
				frame.steps = 5.0f;
				indices.push_back(indexCounter++);
				
				
			}
			
			
			//elements.push_back(vert[0]); elements.push_back(vert[1]); elements.push_back(vert[2]);
			//elementsTex.push_back(vt[0]); elementsTex.push_back(vt[1]); elementsTex.push_back(vt[2]);
			//elementsNorm.push_back(vn[0]); elementsNorm.push_back(vn[1]); elementsNorm.push_back(vn[2]);
			
		}
		else if (line[0] == '#') { /* ignoring this line */ }
		else { /* ignoring this line */ }
	}
	
	//Build SPMeshVerts
	indexCount = indices.size();
	//indices.resize(indexCount);
	//texUVs.resize(vertices.size());
	//SPFrame frame;
	//frame.first.resize(vertices.size());
	//frame.second.resize(vertices.size());
	
	/*
	for(int i = 0; i < elements.size(); ++i) {
		frame.first[elements[i]].pos.x = vertices[ elements[i] ].x;
		frame.first[elements[i]].pos.y = vertices[ elements[i] ].y;
		frame.first[elements[i]].pos.z = vertices[ elements[i] ].z;
		frame.first[elements[i]].normal.x = normals[ elementsNorm[i] ].x;
		frame.first[elements[i]].normal.y = normals[ elementsNorm[i] ].y;
		frame.first[elements[i]].normal.z = normals[ elementsNorm[i] ].z;
		
		frame.second[elements[i]].pos.x = vertices[ elements[i] ].x;
		frame.second[elements[i]].pos.y = vertices[ elements[i] ].y;
		frame.second[elements[i]].pos.z = vertices[ elements[i] ].z;
		frame.second[elements[i]].normal.x = normals[ elementsNorm[i] ].x;
		frame.second[elements[i]].normal.y = normals[ elementsNorm[i] ].y;
		frame.second[elements[i]].normal.z = normals[ elementsNorm[i] ].z;
		
		texUVs[ elements[i] ].x = Texcoords[ elementsTex[i] ].x;
		texUVs[ elements[i] ].y = Texcoords[ elementsTex[i] ].y;
		
		indices[i] = elements[i];
	}
		*/
		
	frames.push_back(frame);
	if (first) {
		BuildBB();
	}
}
void SPMesh::LoadMtlFile(const char* filename, const char* path) {
	string fullfilename = string(path) + string(filename);
	ifstream in(fullfilename.c_str(), ios::in);
	if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }
	
	string line;
	while (getline(in, line)) {
		int tab_offset = 0;
		if (line.c_str()[0] == '\t') {
			tab_offset = 1;
		}
		if ((line.substr(tab_offset,6) == "map_Ka" || line.substr(tab_offset,6) == "map_Kd" ) && !hasTexture) {
			istringstream s(line.substr(7));
			string texfilename;
			s >> texfilename;
			string fullTexName = path +texfilename;
			texture.loadTexture(fullTexName);
			hasTexture = true;
		}
	}
	
}

void SPMesh::Render() {
	
	
	
	
	M3DVector4f vLightPos = {250.0f, 500.0f, 0.0f, 1.0f};
	M3DVector4f vLightEyePos;
	M3DVector4f cameraPos;
	M3DMatrix44f mCamera;
	gCamera.GetCameraMatrix(mCamera);
	gCamera.GetOrigin(cameraPos);
	m3dTransformVector3(vLightEyePos, vLightPos, mCamera);
	

	shader->Begin();
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(SPMeshVert),&frames[0].first[0].pos.x);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(SPMeshVert), &frames[0].first[0].normal.x);
	
	shader->SetUniform1i("modelTex",0);
	shader->SetUniform3f("lightdir", vLightEyePos[0], vLightEyePos[1], vLightEyePos[2]);
	shader->SetUniform3f("CameraPos", cameraPos[0], cameraPos[1], cameraPos[2]);

	glActiveTexture(GL_TEXTURE0);
	texture.bindTexture();
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,sizeof(spVec2),&texUVs[0].x);
	glDrawElements(GL_TRIANGLES,indexCount,GL_UNSIGNED_INT ,&indices[0]);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	
	
	shader->End();

	
	
	
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	/*
	 glBegin(GL_TRIANGLES);
	 
	 for(int i = 0; i < elements.size(); ++i) {
	 glNormal3f(normals[elementsNorm[i]].x,normals[elementsNorm[i]].y,normals[elementsNorm[i]].z);
	 glTexCoord2f(Texcoords[elementsTex[i]].x,Texcoords[elementsTex[i]].y);
	 glVertex3f(vertices[elements[i]].x,vertices[elements[i]].y,vertices[elements[i]].z);
	 
	 }
	 
	 glEnd();
	 */
	
}

// Return true if finished animation.  Return carry over elapsed
void SPMesh::RenderAnimation(int frame, float tween) {
	
	M3DVector4f vLightPos = {250.0f, 500.0f, 0.0f, 1.0f};
	M3DVector4f vLightEyePos;
	M3DVector4f cameraPos;
	M3DMatrix44f mCamera;
	gCamera.GetCameraMatrix(mCamera);
	gCamera.GetOrigin(cameraPos);
	m3dTransformVector3(vLightEyePos, vLightPos, mCamera);
	

	animationShader->Begin();
	
	GLint loc = glGetAttribLocation(animationShader->GetProgram(), "NextVertex");
	glVertexAttribPointer(loc,3, GL_FLOAT,GL_FALSE,sizeof(SPMeshVert), &frames[frame].second[0].pos.x);
	glEnableVertexAttribArray(loc);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(SPMeshVert),&frames[frame].first[0].pos.x);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(SPMeshVert), &frames[frame].first[0].normal.x);
	
	animationShader->SetUniform1i("modelTex",0);
	animationShader->SetUniform3f("lightdir", vLightEyePos[0], vLightEyePos[1], vLightEyePos[2]);
	animationShader->SetUniform3f("CameraPos", cameraPos[0], cameraPos[1], cameraPos[2]);
	animationShader->SetUniform1f("tween", tween);
	glActiveTexture(GL_TEXTURE0);
	texture.bindTexture();
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,sizeof(spVec2),&texUVs[0].x);
	glDrawElements(GL_TRIANGLES,indexCount,GL_UNSIGNED_INT ,&indices[0]);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableVertexAttribArray(loc);
	
	
	animationShader->End();
	
	
	
}

void SPMesh::BuildBB() {
	
	
	
	min[0] = 10000000;
	min[1] = 10000000;
	min[2] = 10000000;
	
	max[0] = -10000000;
	max[1] = -10000000;
	max[2] = -10000000;
	
	int len = vertices.size();
	for(int i =0; i < len; ++i) {
		if ( vertices[i].x < min[0]) min[0] = vertices[i].x;
		if ( vertices[i].y < min[1]) min[1] = vertices[i].y;
		if ( vertices[i].z < min[2]) min[2] = vertices[i].z;
		
		if ( vertices[i].x > max[0]) max[0] = vertices[i].x;
		if ( vertices[i].y > max[1]) max[1] = vertices[i].y;
		if ( vertices[i].z > max[2]) max[2] = vertices[i].z;
	}
	
	//now make box
	BBPoints[0][0] = min[0];	BBPoints[0][1] = min[1];	BBPoints[0][2] = max[2];
	BBPoints[1][0] = max[0];	BBPoints[1][1] = min[1];	BBPoints[1][2] = max[2];
	BBPoints[3][0] = min[0];	BBPoints[3][1] = max[1];	BBPoints[3][2] = max[2];
	BBPoints[2][0] = max[0];	BBPoints[2][1] = max[1];	BBPoints[2][2] = max[2];
	
	BBPoints[4][0] = min[0];	BBPoints[4][1] = min[1];	BBPoints[4][2] = min[2];
	BBPoints[5][0] = max[0];	BBPoints[5][1] = min[1];	BBPoints[5][2] = min[2];
	BBPoints[7][0] = min[0];	BBPoints[7][1] = max[1];	BBPoints[7][2] = min[2];
	BBPoints[6][0] = max[0];	BBPoints[6][1] = max[1];	BBPoints[6][2] = min[2];
	
	center[0] = ( max[0] + min[0] ) / 2.0f;
	center[1] = ( max[1] + min[1] ) / 2.0f;
	center[2] = ( max[2] + min[2] ) / 2.0f;
	center[3] = 1.0f;
	
	radius = sqrtf((max[0] - min[0]) * (max[0] - min[0]) + (max[1] - min[1]) * (max[1] - min[1]) + (max[2] - min[2]) * (max[2] - min[2])) / 2.0f;
	LoadHalfVectors();
}

void SPMesh::RenderBB() {
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	
	glLineWidth(1.5f);
	glColor3f(1.0f, 1.0f, 1.0f);
	
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(BBPoints[0][0], BBPoints[0][1], BBPoints[0][2]);
	glVertex3f(BBPoints[1][0], BBPoints[1][1], BBPoints[1][2]);
	glVertex3f(BBPoints[2][0], BBPoints[2][1], BBPoints[2][2]);
	glVertex3f(BBPoints[3][0], BBPoints[3][1], BBPoints[3][2]);
	
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(BBPoints[4][0], BBPoints[4][1], BBPoints[4][2]);
	glVertex3f(BBPoints[5][0], BBPoints[5][1], BBPoints[5][2]);
	glVertex3f(BBPoints[6][0], BBPoints[6][1], BBPoints[6][2]);
	glVertex3f(BBPoints[7][0], BBPoints[7][1], BBPoints[7][2]);
	glEnd();
	
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(BBPoints[0][0], BBPoints[0][1], BBPoints[0][2]);
	glVertex3f(BBPoints[4][0], BBPoints[4][1], BBPoints[4][2]);
	
	glVertex3f(BBPoints[1][0], BBPoints[1][1], BBPoints[1][2]);
	glVertex3f(BBPoints[5][0], BBPoints[5][1], BBPoints[5][2]);
	
	glVertex3f(BBPoints[2][0], BBPoints[2][1], BBPoints[2][2]);
	glVertex3f(BBPoints[6][0], BBPoints[6][1], BBPoints[6][2]);
	
	glVertex3f(BBPoints[3][0], BBPoints[3][1], BBPoints[3][2]);
	glVertex3f(BBPoints[7][0], BBPoints[7][1], BBPoints[7][2]);
	glEnd();
	
	
}

void SPMesh::SetKeyframeTotals(vector<float> &keyframes) {
	int len = frames.size();
	for (int i =0; i < len; ++i) {
		keyframes.push_back(frames[i].steps);
	}

}

void SPMesh::ExportMesh(const string &filename) {
	string file = filename + ".spm";
	fstream fileHandler(file.c_str(), ios::binary | ios::out);
	
	//save class information
	
	//center
	fileHandler.write((char*)&center, sizeof(M3DVector4f));
	
	//scale
	fileHandler.write((char*)&scale, sizeof(M3DVector3f));
	
	//center
	fileHandler.write((char*)&radius, sizeof(float));
	
	//Has Animation
	fileHandler.write((char*)&hasAnimation, sizeof(bool));
	
	//speed
	fileHandler.write((char*)&speed, sizeof(float));
	
	//repeat
	fileHandler.write((char*)&repeat, sizeof(bool));
	
	//has texture
	fileHandler.write((char*)&hasTexture, sizeof(bool));
	
	//indexcount
	fileHandler.write((char*)&indexCount, sizeof(unsigned int));
	
	//BBpoints
	fileHandler.write((char*)&BBPoints, sizeof(M3DVector3f) * 8);
	
	//TexUVs array
	int size = texUVs.size();
	fileHandler.write((char *)&size, sizeof(int));
	for(int i =0; i < size; ++i) {
		fileHandler.write((char*)&texUVs[i], sizeof(spVec2));
	}
	
	//indicies array
	size = indices.size();
	fileHandler.write((char *)&size, sizeof(int));
	for(int i =0; i < size; ++i) {
		fileHandler.write((char*)&indices[i], sizeof(unsigned int));
	}
	
	//frames
	int framesize = frames.size();
	fileHandler.write((char *)&framesize, sizeof(int));
	for (int i =0; i < framesize; ++i) {
		//write out steps
		fileHandler.write((char *)&frames[i].steps, sizeof(float));
		
		//write out first
		int firstsize = frames[i].first.size();
		fileHandler.write((char *)&firstsize, sizeof(int));
		for (int j =0; j < firstsize; ++j) {
			fileHandler.write((char *)&frames[i].first[j], sizeof(SPMeshVert));
		}
		
		//write out second
		int secondsize = frames[i].second.size();
		fileHandler.write((char *)&secondsize, sizeof(int));
		for (int j =0; j < secondsize; ++j) {
			fileHandler.write((char *)&frames[i].second[j], sizeof(SPMeshVert));
		}
	}
	
	//Texture
	//Print texture dimension
	
	texture.WriteTextureInfo(&fileHandler);
	
	fileHandler.close();
}

void SPMesh::ImportMesh(const string &filename, const char *path) {

	size_t pos = filename.find(".spm");
	if ( pos ==string::npos) {
		return;
	}
	name = filename.substr(0,pos);
	
	string fullpath = path + filename;
	fstream fileHandler(fullpath.c_str(), ios::binary | ios::in);
	if (!fileHandler) { cerr << "Cannot open " << filename << endl; exit(1); }
	
	//save class information
	
	//center
	fileHandler.read((char*)&center, sizeof(M3DVector4f));
	
	//scale
	fileHandler.read((char*)&scale, sizeof(M3DVector3f));
	
	
	//center
	fileHandler.read((char*)&radius, sizeof(float));
	
	//Has Animation
	fileHandler.read((char*)&hasAnimation, sizeof(bool));
	
	//speed
	fileHandler.read((char*)&speed, sizeof(float));
	
	//repeat
	fileHandler.read((char*)&repeat, sizeof(bool));
	
	//has texture
	fileHandler.read((char*)&hasTexture, sizeof(bool));
	
	//indexcount
	fileHandler.read((char*)&indexCount, sizeof(unsigned int));
	
	//BBpoints
	fileHandler.read((char*)&BBPoints, sizeof(M3DVector3f) * 8);
	LoadHalfVectors();
	
	//TexUVs array
	int size = texUVs.size();
	fileHandler.read((char *)&size, sizeof(int));
	texUVs.resize(size);
	for(int i =0; i < size; ++i) {
		fileHandler.read((char*)&texUVs[i], sizeof(spVec2));
	}
	
	//indicies array
	size = indices.size();
	fileHandler.read((char *)&size, sizeof(int));
	indices.resize(size);
	for(int i =0; i < size; ++i) {
		fileHandler.read((char*)&indices[i], sizeof(unsigned int));
	}
	
	//frames
	int framesize = frames.size();
	fileHandler.read((char *)&framesize, sizeof(int));
	frames.resize(framesize);
	for (int i =0; i < framesize; ++i) {
		//write out steps
		fileHandler.read((char *)&frames[i].steps, sizeof(float));
		
		//write out first
		int firstsize;
		fileHandler.read((char *)&firstsize, sizeof(int));
		frames[i].first.resize(firstsize);
		
		for (int j =0; j < firstsize; ++j) {
			fileHandler.read((char *)&frames[i].first[j], sizeof(SPMeshVert));
		}
		
		//write out second
		int secondsize;
		fileHandler.read((char *)&secondsize, sizeof(int));
		frames[i].second.resize(secondsize);
		
		for (int j =0; j < secondsize; ++j) {
			fileHandler.read((char *)&frames[i].second[j], sizeof(SPMeshVert));
		}
	}
	
	
	//Texture
	//Print texture dimension
	texture.ReadTextureInfo(&fileHandler);

	fileHandler.close();
	
}


void SPMesh::GetBBMin(M3DVector4f &outMin) {
	outMin[0] = BBPoints[4][0];
	outMin[1] = BBPoints[4][1];
	outMin[2] = BBPoints[4][2];
	outMin[3] = 1;
	
}

void SPMesh::GetBBMax(M3DVector4f &outMax) {
	outMax[0] = BBPoints[2][0];
	outMax[1] = BBPoints[2][1];
	outMax[2] = BBPoints[2][2];
	outMax[3] = 1;
}

float SPMesh::GetRadius() {
	return radius;
}

void SPMesh::LoadHalfVectors() {
	M3DVector4f min, max;
	GetBBMin(min);
	GetBBMax(max);
	//half to scale because we messed up this design
	
	m3dScaleVector3(min, scale[0]);
	m3dScaleVector3(max, scale[0]);
	
	halfX = max[0];
	halfZ = max[2];
	//halfX = (max[0] - min[0]) / 2.0f;
	//halfZ = (max[2] - min[2]) / 2.0f;
	
}
