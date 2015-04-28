/*
 *  quadterrain.cpp
 *  MapEditor
 *
 *  Created by Scott Scaccianoce on 6/21/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */


#include "quadterrain.h"
#include "Frustum.h"

 int QuadTerrain::counter = 0;
 int QuadTerrain::rowVertCount = 0;
int QuadTerrain::total = 0;

void QuadTerrain::init() {
	isLeaf = false;
	isRoot = false;
	for(int i =0; i < 4; ++i) {
		QuadNodes[i] = NULL;
	}
	QuadTerrain::total++;
	lower_detail_range_squared = 3000;
}
QuadTerrain::QuadTerrain(int numVertRows, int numVertCols, float dx, float dz, const sVector3 &center, int desired_count) : Terrain(numVertRows,numVertCols,dx,dz,center){
	init();
	isRoot = true;
	desiredCount = desired_count;
	subGridWidth = numVertRows;
	rowVertCount = numVertRows;
	index_offset = 0;
	
	if ((subGridWidth - 1) <= desired_count) {
		isLeaf = true;
	}
	
	if(!isLeaf) {
		ComputeQuads();
	}
	
	//No need to compute indices
	
}

QuadTerrain::QuadTerrain(const std::vector<VertexTerrain> &vertices, int subgridwidth, int desired_count, unsigned int offset, float cwidth ) {
	init();
	cellWidth = cwidth;
	cellHeight =cwidth;
	subGridWidth = subgridwidth;
	desiredCount = desired_count;
	QuadVerts = vertices;
	index_offset  = offset;
	if ((subGridWidth - 1) <= desired_count) {
		isLeaf = true;
	}
	
	if(!isLeaf) {
		ComputeQuads();

	} else {
		ComputeIndices();	
	}
	ComputeBoundry();
	QuadVerts.clear();
	
}
QuadTerrain::~QuadTerrain() {

	ClearNodes();
}
void QuadTerrain::ComputeQuads() {
	std::vector<VertexTerrain> NW;
	std::vector<VertexTerrain> NE;
	std::vector<VertexTerrain> SW;
	std::vector<VertexTerrain> SE;
	
	if(!isLeaf) {
		if (isRoot) {
			ClearNodes();
			SplitVerts(verts, NW, NE, SW, SE);
			
		} else {
			SplitVerts(QuadVerts, NW, NE, SW, SE);
		}
	
		int newWidth = (subGridWidth + 1) /2;
		QuadNodes[0] = new QuadTerrain(NW, newWidth, desiredCount, index_offset, cellWidth);
		QuadNodes[1] = new QuadTerrain(NE, newWidth, desiredCount, index_offset + newWidth -1, cellWidth);
		QuadNodes[2] = new QuadTerrain(SW, newWidth, desiredCount, index_offset +  (newWidth -1) * rowVertCount, cellWidth);
		QuadNodes[3] = new QuadTerrain(SE, newWidth, desiredCount, index_offset +  (newWidth -1) * rowVertCount + newWidth -1 , cellWidth);
	} 
	
}

void QuadTerrain::ClearNodes() {
	for(int i =0; i < 4; ++i) {
		if ( QuadNodes[i] != NULL) {
			QuadNodes[i]->ClearNodes();
			delete QuadNodes[i];
			QuadNodes[i] = NULL;
		}
	}
	
	QuadVerts.clear();
	QuadIndices.clear();
	LowerQuadIndices.clear();
	
	
}

void QuadTerrain::SplitVerts(const std::vector<VertexTerrain> &source, std::vector<VertexTerrain> &NW, std::vector<VertexTerrain> &NE , std::vector<VertexTerrain> &SW , std::vector<VertexTerrain> &SE) {

	
	int split = (subGridWidth+1) / 2;
	for(int i =0; i < subGridWidth; ++i) {
		for(int k =0; k < subGridWidth; ++k) {
			
			if (i < split && k < split) {
				NW.push_back(source[(i * subGridWidth) + k]);
			}
			if (i < split && k >= split -1) {
				NE.push_back(source[(i * subGridWidth) + k]);
			}
			if (i >= split -1 && k < split) {
				SW.push_back(source[(i * subGridWidth) + k]);
			}
			if (i >= split -1 && k >= split -1) {
				SE.push_back(source[(i * subGridWidth) + k]);
			}
			
		}
		
	}
}


void QuadTerrain::ComputeIndices() {
	unsigned int numTris = (subGridWidth -1 ) * (subGridWidth -1 ) * 2;
	//indices
	numIndices = numTris * 3;
	QuadIndices.resize(numIndices);
	
	unsigned int k =0;
	unsigned int Cells = (subGridWidth -1 );
	for(unsigned int i = 0; i < (unsigned int)Cells; ++i)
	{
		for(unsigned int j =0; j < (unsigned int)Cells; ++j) {
			
			
		/*	QuadIndices[k]		= (i+1) * subGridWidth + j;
			QuadIndices[k + 1]	= i		* subGridWidth + j;
			QuadIndices[k + 2]	= (i+1) * subGridWidth + j + 1;
			
			QuadIndices[k + 3]	= (i+1) * subGridWidth + j + 1;
			QuadIndices[k + 4]	= i		* subGridWidth + j;
			QuadIndices[k + 5]	= i		* subGridWidth + j +1; 
		 */
			QuadIndices[k]		= (i+1) * rowVertCount + j + index_offset;
			QuadIndices[k + 1]	= i		* rowVertCount + j + index_offset;
			QuadIndices[k + 2]	= (i+1) * rowVertCount + j + 1 + index_offset;
			
			QuadIndices[k + 3]	= (i+1) * rowVertCount + j + 1 + index_offset;
			QuadIndices[k + 4]	= i		* rowVertCount + j + index_offset;
			QuadIndices[k + 5]	= i		* rowVertCount + j +1 + index_offset; 
			
			k+= 6;
			
		}
		
	}
	
	//Lower Quality
	int levelSkip = 4; // first level
	unsigned int LowernumTris = ((subGridWidth -1 ) * (subGridWidth -1 )) / levelSkip * 2;
	//indices
	numIndices = LowernumTris * 3;
	LowerQuadIndices.resize(numIndices);
	
	k =0;
	Cells = (subGridWidth -1 ) ;
	for(unsigned int i = 0; i < (unsigned int)Cells; i += 2)
	{
		for(unsigned int j =0; j < (unsigned int)Cells; j += 2) {
			
			
			
			LowerQuadIndices[k]		= (i+2) * rowVertCount + j + index_offset;
			LowerQuadIndices[k + 1]	= i		* rowVertCount + j + index_offset;
			LowerQuadIndices[k + 2]	= (i+2) * rowVertCount + j + 2 + index_offset;
			
			LowerQuadIndices[k + 3]	= (i+2) * rowVertCount + j + 2 + index_offset;
			LowerQuadIndices[k + 4]	= i		* rowVertCount + j + index_offset;
			LowerQuadIndices[k + 5]	= i		* rowVertCount + j +2 + index_offset; 
			
			k+= 6;
			
		}
		
	}
	
}

void QuadTerrain::Render() {
	//GLubyte *err;
	//err = new GLubyte[1024];
	//GLenum error;// = glGetError();
	// GLubyte *err = gluErrorString(error);
	
	
	QuadTerrain::counter = 0;
	
	glPushMatrix();
	
	glTranslatef(center.x,center.y, center.z);
	if(bRenderWireFrame){
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		//glDisable(GL_LIGHTING);
		
		//glDisable(GL_TEXTURE_2D);
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,sizeof(VertexTerrain),&verts[0].pos.x);
		
		gridShader->Begin();
		
		if (!renderCircle) {
			gridShader->SetUniform1i("draw_circle", 0);
		} else {
			sVector3 temp;
			if (MouseIntersect(temp)) {
				gridShader->SetUniform1i("draw_circle", 1 );
				gridShader->SetUniform1f("radius", raiseRadius);
				gridShader->SetUniform3f("circle_pos",temp.x, temp.y, temp.z);
			} else {
				gridShader->SetUniform1i("draw_circle", 0);
			}
		}
		std::vector<unsigned int> indexs;
		GetIndices(indexs);
		unsigned int num = indexs.size();
		glDrawElements(GL_TRIANGLES,num,GL_UNSIGNED_INT ,&indexs[0]);
		
	
		
		gridShader->End();		
		
		glDisableClientState(GL_VERTEX_ARRAY);
		
		
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	} else {
		
		M3DVector4f vLightPos = {100.0f, 50.0f, 0.0f, 1.0f};
		M3DVector4f vLightEyePos;
		M3DMatrix44f mCamera;
		gCamera.GetCameraMatrix(mCamera);
		m3dTransformVector3(vLightEyePos, vLightPos, mCamera);
		
		
		texShader->Begin();
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT,sizeof(VertexTerrain),&verts[0].tex1.x);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(VertexTerrain), &verts[0].normal.x);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,sizeof(VertexTerrain),&verts[0].pos.x);
		
		texShader->SetUniform1i("grassTex",0);
		texShader->SetUniform1i("dirtTex",1);
		texShader->SetUniform1i("rockTex",2);
		texShader->SetUniform1i("snowTex",3);
		
		texShader->SetUniform1i("AlphaMapTex",4);
		
		//texShader->SetUniform3f("lightdir", vLightEyePos[0], vLightEyePos[1], vLightEyePos[2]);
		texShader->SetUniform3f("lightPos", vLightEyePos[0], vLightEyePos[1], vLightEyePos[2]);
		
		
		glActiveTexture(GL_TEXTURE0);
		grass.bindTexture();
		glActiveTexture(GL_TEXTURE1);
		dirt.bindTexture();
		glActiveTexture(GL_TEXTURE2);
		rock.bindTexture();
		glActiveTexture(GL_TEXTURE3);
		snow.bindTexture();
		glActiveTexture(GL_TEXTURE4);
		alphaMap.bindTexture();
		
		
		
		if (!renderCircle) {
			texShader->SetUniform1i("draw_circle", 0);
		} else {
			sVector3 temp;
			if (MouseIntersect(temp)) {
				texShader->SetUniform1i("draw_circle", 1 );
				texShader->SetUniform1f("radius", raiseRadius);
				texShader->SetUniform3f("circle_pos",temp.x, temp.y, temp.z);
			} else {
				texShader->SetUniform1i("draw_circle", 0);
			}
		}
		
		//glColor3f(0.0f,1.0f,0.0f);
		
		
		//DrawQuad(false);
				
		std::vector<unsigned int> indexs;
		GetIndices(indexs);
		unsigned int num = indexs.size();
		glDrawElements(GL_TRIANGLES,num,GL_UNSIGNED_INT ,&indexs[0]);
		
		
		
		texShader->End();		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		
		
		glActiveTexture(GL_TEXTURE0);
		
		
	}
	glPopMatrix();
		
	
}

void QuadTerrain::DrawQuad(bool wireframe ) {
	
	if (gFrustum.TestSphere(center_x,center_y,center_z, radius) || isRoot) {
		if (!isLeaf) {
			for( int i =0 ; i < 4; ++i) {
				QuadNodes[i]->DrawQuad(wireframe);
			}
		} else {
			QuadTerrain::counter++;
			if (wireframe) {
				glVertexPointer(3,GL_FLOAT,sizeof(VertexTerrain),&QuadVerts[0].pos.x);
				glDrawElements(GL_TRIANGLES,numIndices,GL_UNSIGNED_INT ,&QuadIndices[0]);
			} else {
				glTexCoordPointer(2,GL_FLOAT,sizeof(VertexTerrain),&QuadVerts[0].tex1.x);
				glNormalPointer(GL_FLOAT, sizeof(VertexTerrain), &QuadVerts[0].normal.x);
				glVertexPointer(3,GL_FLOAT,sizeof(VertexTerrain),&QuadVerts[0].pos.x);
				glDrawElements(GL_TRIANGLES,numIndices,GL_UNSIGNED_INT ,&QuadIndices[0]);
				
				
			}
		}
	} 	
	
}

void QuadTerrain::ComputeBoundry() {
	//calculate AABB
	min_x = 100000.0f;
	min_y = 100000.0f;
	min_z = 100000.0f;
	
	max_x = -100000.0f;
	max_y = -100000.0f;
	max_z = -100000.0f;
	
	unsigned int len = QuadVerts.size();
	for (int i = 0; i < len; ++i) {
		if (QuadVerts[i].pos.x < min_x) {
			min_x = QuadVerts[i].pos.x;
		}
		if (QuadVerts[i].pos.x > max_x) {
			max_x = QuadVerts[i].pos.x;
		}
		
		if (QuadVerts[i].pos.y < min_y) {
			min_y = QuadVerts[i].pos.y;
		}
		if (QuadVerts[i].pos.y > max_y) {
			max_y = QuadVerts[i].pos.y;
		}
		
		if (QuadVerts[i].pos.z < min_z) {
			min_z = QuadVerts[i].pos.z;
		}
		if (QuadVerts[i].pos.z > max_z) {
			max_z = QuadVerts[i].pos.z;
		}
		
	}
	
	center_x = (min_x + max_x ) /2;
	center_y = (min_y + max_y ) /2;
	center_z = (min_z + max_z ) /2;
	
	radius = sqrtf((max_x - min_x) * (max_x - min_x) + (max_z - min_z) * (max_z - min_z)) / 2.0f;
	orthoRadius = (subGridWidth -1) * cellWidth;
		
}



bool QuadTerrain::MouseIntersect(sVector3 &intersect_point) {
	sVector3 mpos1, mdir, mpos2;
	GetMouseRay(mpos1, mpos2, mdir);
	
	M3DVector3f pos, dir,spos;
	pos[0] = mpos1.x;
	pos[1] = mpos1.y, 
	pos[2] = mpos1.z;
	
	dir[0] = mdir.x;
	dir[1] = mdir.y, 
	dir[2] = mdir.z;
	
	

	
	std::vector<TerrainTriangle> triangles;
	std::vector<sVector3> hits;
	
	CheckQuadIntersect(&verts,triangles, hits,pos,mpos1,mpos2,dir,spos);
	
	if(hits.size() > 0) {
		
		if(hits.size() == 1){
			//get closest
			intersect_point = hits[0];
			
		} else {
			float min = 99999999;
			float index = 0;
			
			for(int i = 0; i < hits.size(); ++i){
				//Find distance from pt1 to hit and use the lowest distance;
				float x2 = hits[i].x;
				float y2 = hits[i].y;
				float z2 = hits[i].z;
				
				float x1 = mpos1.x;
				float y1 = mpos1.y;
				float z1 = mpos1.z;
				
				float dist = sqrt((x2 - x1) * (x2-x1) + (y2 - y1) * (y2-y1) + (z2 - z1) * (z2-z1));
				
				if(dist < min){
					index = i;
					min = dist;
				}
			}
			
			intersect_point = hits[index];
			
			
			
		}
		return true;
	}
	return false;
	

}


void QuadTerrain::CheckQuadIntersect(std::vector<VertexTerrain>*rootVerts, std::vector<TerrainTriangle> &triangles, std::vector<sVector3> &hits, M3DVector3f pos, sVector3 &pos1 ,sVector3 &pos2, M3DVector3f dir, M3DVector3f spos) {
	if (gFrustum.TestSphere(center_x,center_y,center_z, radius) || isRoot) {
				
		spos[0] = center_x;
		spos[1] = center_y, 
		spos[2] = center_z;
		
		if (!isRoot) {
			
			if (!RaySphereTest(pos, dir, spos, radius)) {
				return;
			}
		}
		if (!isLeaf) {
			for( int i =0 ; i < 4; ++i) {
				QuadNodes[i]->CheckQuadIntersect(rootVerts,triangles,hits,pos,pos1,pos2,dir,spos);
			}
		} else {
			//visible so check vertices
			
			unsigned int len = QuadIndices.size();
			sVector3 HitPos;
			for (int i = 0; i < len; i += 3) {
				unsigned int id1 = QuadIndices[i];
				unsigned int id2 = QuadIndices[i +1];
				unsigned int id3 = QuadIndices[i +2];
				
				if(CheckLineTri((*rootVerts)[id1].pos,(*rootVerts)[id2].pos,(*rootVerts)[id3].pos, pos1, pos2, HitPos)){
					TerrainTriangle t;
					t.pos1 = (*rootVerts)[id1].pos;
					t.pos2 = (*rootVerts)[id2].pos;
					t.pos3 = (*rootVerts)[id3].pos;
					triangles.push_back(t);
					hits.push_back(HitPos);
				}
			}
						
			
			
		}
	} 	
	
	
}

bool QuadTerrain::SaveMap(const string &filename,  std::vector<SPEntity *>&objects, std::vector<SPEntity*>&enemies) {
	string file = filename + ".quad";
	fstream fileHandler(file.c_str(), ios::binary | ios::out);
	
	int vertsize = verts.size();
	fileHandler.write((char*)&vertsize, sizeof(int));
	for (int i = 0; i < vertsize; ++i) {
		fileHandler.write((char*)&verts[i], sizeof(VertexTerrain));
	}
	
	//blendmap
	int blendDataSize = alphaMapSize * 4 * alphaMapSize;
	unsigned char * data = alphaMap.GetDataPointer();
	for(int i =0; i < blendDataSize; i++) {
		fileHandler.write((char*)&data[i] ,sizeof(unsigned char));
	}
	
	int objectssize = objects.size();
	fileHandler.write((char*)&objectssize, sizeof(int));
	for (int i =0; i < objectssize; ++i) {
		int dbid = objects[i]->GetDbId();
		M3DVector3f origin, obj_dir;
		objects[i]->GetOrigin(origin);
		objects[i]->GetForwardVector(obj_dir);
		fileHandler.write((char*)&dbid ,sizeof(int));
		fileHandler.write((char*)&origin ,sizeof(M3DVector3f));
		fileHandler.write((char*)&obj_dir ,sizeof(M3DVector3f));
		
	}
	
	//enemies
	int enemyssize = enemies.size();
	fileHandler.write((char*)&enemyssize, sizeof(int));
	for (int i =0; i < enemyssize; ++i) {
		int dbid = enemies[i]->GetDbId();
		M3DVector3f origin, enemy_dir;
		enemies[i]->GetOrigin(origin);
		enemies[i]->GetForwardVector(enemy_dir);
		fileHandler.write((char*)&dbid ,sizeof(int));
		fileHandler.write((char*)&origin ,sizeof(M3DVector3f));
		fileHandler.write((char*)&enemy_dir ,sizeof(M3DVector3f));
	}
	
	return true;
	
	
}

bool  QuadTerrain::LoadMap(const string &filename, std::vector<SPEntity *>&objects, std::vector<SPMesh *>&meshes,std::vector<SPEntity *>&enemy_objects, std::vector<SPMesh *>&enemy_meshes) {
	std::string file = filename + ".quad";
	
	fstream fileHandler(file.c_str(), ios::binary | ios::in);
	int vertsize;
	fileHandler.read((char*)&vertsize, sizeof(int));
	
	verts.clear();
	verts.resize(vertsize);
	for (int i = 0; i < vertsize; ++i) {
		fileHandler.read((char*)&verts[i], sizeof(VertexTerrain));
	}
	
	//blendmap
	int blendDataSize = alphaMapSize * 4 * alphaMapSize;
	unsigned char * data = alphaMap.GetDataPointer();
	for(int i =0; i < blendDataSize; i++) {
		fileHandler.read((char*)&data[i] ,sizeof(unsigned char));
	}
	alphaMap.CopyDataToImage();
	
	
	//objects
	for(int i =0; i < objects.size(); ++i) {
		delete objects[i];
		objects[i] = NULL;
	}
	
	int objectssize;
	fileHandler.read((char*)&objectssize, sizeof(int));
	objects.clear();
	objects.resize(objectssize);
	
	for (int i =0; i < objectssize; ++i) {
		int dbid;
		M3DVector3f origin,dir;
		fileHandler.read((char*)&dbid ,sizeof(int));
		fileHandler.read((char*)&origin ,sizeof(M3DVector3f));
		fileHandler.read((char*)&dir ,sizeof(M3DVector3f));
		
		
		int mesh_index = dbid - 1;
		SPEntity *entity = new SPEntity(meshes[mesh_index]);
		entity->SetOrigin(origin[0],origin[1],origin[2]);
		entity->SetForwardVector(dir);
		
		objects[i] = entity;
		
		
	}
	
	//ENemies
	//objects
	for(int i =0; i < enemy_objects.size(); ++i) {
		delete enemy_objects[i];
		enemy_objects[i] = NULL;
	}
	
	objectssize;
	fileHandler.read((char*)&objectssize, sizeof(int));
	enemy_objects.clear();
	enemy_objects.resize(objectssize);
	
	for (int i =0; i < objectssize; ++i) {
		int dbid;
		M3DVector3f origin,enemy_dir;
		fileHandler.read((char*)&dbid ,sizeof(int));
		fileHandler.read((char*)&origin ,sizeof(M3DVector3f));
		fileHandler.read((char*)&enemy_dir ,sizeof(M3DVector3f));
		
		
		int mesh_index = dbid - 1;
		SPEntity *entity = new SPEntity(enemy_meshes[mesh_index]);
		entity->SetOrigin(origin[0],origin[1],origin[2]);
		entity->SetForwardVector(enemy_dir);
		
		enemy_objects[i] = entity;
		
		
	}
	
	
	return true;
}

bool QuadTerrain::LoadMapWithEnemyData(const string &filename, std::vector<SPEntity *>&objects, std::vector<SPMesh *>&meshes,std::vector<Enemy *>&enemy_objects, std::vector<SPMesh *>&enemy_meshes, std::vector<SPMesh *>&enemy_attack_meshes, Database *database) {
	std::string file = filename + ".quad";
	
	fstream fileHandler(file.c_str(), ios::binary | ios::in);
	int vertsize;
	fileHandler.read((char*)&vertsize, sizeof(int));
	
	verts.clear();
	verts.resize(vertsize);
	for (int i = 0; i < vertsize; ++i) {
		fileHandler.read((char*)&verts[i], sizeof(VertexTerrain));
	}
	
	//blendmap
	int blendDataSize = alphaMapSize * 4 * alphaMapSize;
	unsigned char * data = alphaMap.GetDataPointer();
	for(int i =0; i < blendDataSize; i++) {
		fileHandler.read((char*)&data[i] ,sizeof(unsigned char));
	}
	alphaMap.CopyDataToImage();
	
	
	//objects
	for(int i =0; i < objects.size(); ++i) {
		delete objects[i];
		objects[i] = NULL;
	}
	
	int objectssize;
	fileHandler.read((char*)&objectssize, sizeof(int));
	objects.clear();
	objects.resize(objectssize);
	
	for (int i =0; i < objectssize; ++i) {
		int dbid;
		M3DVector3f origin, dir;
		fileHandler.read((char*)&dbid ,sizeof(int));
		fileHandler.read((char*)&origin ,sizeof(M3DVector3f));
		fileHandler.read((char*)&dir ,sizeof(M3DVector3f));
		
		
		int mesh_index = dbid - 1;
		SPEntity *entity = new SPEntity(meshes[mesh_index]);
		entity->SetOrigin(origin[0],origin[1],origin[2]);
		entity->SetForwardVector(dir);
		//entity->SetOrigin(0,0,0);
		objects[i] = entity;
		
		
	}
	
	//ENemies
	//objects
	for(int i =0; i < enemy_objects.size(); ++i) {
		delete enemy_objects[i];
		enemy_objects[i] = NULL;
	}
	
	objectssize;
	fileHandler.read((char*)&objectssize, sizeof(int));
	enemy_objects.clear();
	enemy_objects.resize(objectssize);
	
	for (int i =0; i < objectssize; ++i) {
		int dbid;
		M3DVector3f origin, enemy_dir;
		fileHandler.read((char*)&dbid ,sizeof(int));
		fileHandler.read((char*)&origin ,sizeof(M3DVector3f));
		fileHandler.read((char*)&enemy_dir ,sizeof(M3DVector3f));
		
		
		int mesh_index = dbid - 1;
		Enemy *entity = new Enemy(enemy_meshes[mesh_index]);
		entity->GetAnimationInfo();
		
		SPEntity *attackEntity = new SPEntity(enemy_attack_meshes[mesh_index]);
		attackEntity->GetAnimationInfo();
		attackEntity->ShouldRepeat(false);
		
		entity->attackingObject = attackEntity;
		entity->SetOrigin(origin[0],origin[1],origin[2]);
		entity->SetForwardVector(enemy_dir);
		
		m3dCopyVector3(entity->starting_pos, origin);
		if (database->Query("SELECT hp,attack, level, hostile, baf_radius,respawn, name, exp from Enemies where mesh_id = " + IntToString(dbid))) {
			entity->hp_max = database->GetInteger(0, 0);
			entity->hp = database->GetInteger(0, 0);
			entity->attack = database->GetInteger(0, 1);
			
			entity->level = database->GetInteger(0, 2);
			entity->hostile = (database->GetInteger(0, 3) == 1) ? true: false;
			
			entity->baf_radius = database->GetInteger(0, 4);
			entity->respawn = database->GetInteger(0, 5);
			entity->name = database->GetValue(0, 6);
			entity->enemy_id = dbid;
			entity->exp = database->GetInteger(0,7);
			
			
			
		}
		//entity->SetOrigin(0,0,0);
		enemy_objects[i] = entity;
		
		
	}
	
	
	return true;
	
}



bool QuadTerrain::GetHeightAtPos(float x, float y, float z, float &height) {
	
	sVector3 pos_top, pos_bottom;
	pos_top.x = x;
	pos_top.y = y + 50;
	pos_top.z = z;
	
	pos_bottom.x = x;
	pos_bottom.y = y -50;
	pos_bottom.z = z;
	
	
	
	int xid, zid;
	/*
	int myx = (numberVertCols/2) * cellWidth + x;
	int myz = (numberVertCols/2) * cellWidth + z;
	
	 if (x >= max_pos_x) {
	 xid = numberVertCols - 2;
	 }
	 else {
	 xid = floor(((numberVertCols/2) * cellWidth + x) / cellWidth) ;
	 
	 }
	 
	 if (z >= max_pos_z) {
	 zid = numberVertRows - 2;
	 } else {
	 zid = floor(((numberVertCols/2) * cellWidth + z) / cellHeight);
	 }
	 
	 //since we have an offset to the map we need to add offset to theids
	 //int offset = numberVertCols/2;
	 //xid += offset;
	 //zid -= offset;
	 
	 VertexTerrain *v0,*v1,*v2,*v3;
	 v0 = &verts[zid * numberVertCols + xid];
	 v1 = &verts[zid * numberVertCols + xid + 1];
	 v2 = &verts[((zid + 1) * numberVertCols) + xid];
	 v3 = &verts[((zid + 1) * numberVertCols) + xid + 1];
	 
*/
	
	
	
	if (x >= max_pos_x) {
		xid = numberVertCols - 2;
	}
	else {
		xid = floor(x/(float)cellWidth) ;

	}
	
	if (z >= max_pos_z) {
		zid = numberVertRows - 2;
	} else {
		zid = floor(-z/(float)cellHeight);
	}
	
	//since we have an offset to the map we need to add offset to theids
	int offset = numberVertCols/2;
	xid += offset;
	zid += offset;
	
	VertexTerrain *v0,*v1,*v2,*v3;
	v0 = &verts[zid * numberVertCols + xid];
	v1 = &verts[zid * numberVertCols + xid + 1];
	v2 = &verts[((zid + 1) * numberVertCols) + xid];
	v3 = &verts[((zid + 1) * numberVertCols) + xid + 1];
	
	
	
	
	sVector3 HitPos;

	
	if(CheckLineTri( v0->pos,  v2->pos,  v3->pos, pos_top, pos_bottom, HitPos)) {
		height = HitPos.y;
		return true;
	}
	
	if(CheckLineTri( v0->pos,  v3->pos,  v1->pos, pos_top, pos_bottom, HitPos)) {
		height = HitPos.y;
		return true;
	}
	
	return false;
}

void QuadTerrain::SetLowerDetailRange(float dist) 
{
	if (!isLeaf) {
		for( int i =0 ; i < 4; ++i) {
			QuadNodes[i]->SetLowerDetailRange(dist);
		}
	}
	lower_detail_range_squared = dist * dist;
}



void QuadTerrain::GetIndices(std::vector<unsigned int> &inds) {
	if (gFrustum.TestSphere(center_x,center_y,center_z, radius) || isRoot) {
		
		
		//Uncomment to debug if mouse intersects with quad
		/*sVector3 mpos1, mdir, mpos2;
		 GetMouseRay(mpos1, mpos2, mdir);
		 
		 M3DVector3f pos,dir,spos;
		 pos[0] = mpos1.x;
		 pos[1] = mpos1.y, 
		 pos[2] = mpos1.z;
		 
		 dir[0] = mdir.x;
		 dir[1] = mdir.y, 
		 dir[2] = mdir.z;
		 
		 spos[0] = center_x;
		 spos[1] = center_y, 
		 spos[2] = center_z;
		 
		 if (!isRoot) {
		 if (!RaySphereTest(pos, dir, spos, radius)) {
		 return;
		 }
		 }
		 */
		
		if (!isLeaf) {
			for( int i =0 ; i < 4; ++i) {
				QuadNodes[i]->GetIndices(inds);
			}
		} else {
			QuadTerrain::counter++;
			M3DVector3f spos, camera_pos;
			spos[0] = center_x;
			spos[1] = 0, 
			spos[2] = center_z;
			gCamera.GetOrigin(camera_pos);

			float dist = m3dGetDistanceSquared3(spos, camera_pos);
			if (dist >= lower_detail_range_squared) {
				unsigned int len = LowerQuadIndices.size(); 
				for (int i =0; i< len; ++i) {
					inds.push_back(LowerQuadIndices[i]);
				}
			} else {
				bool north,east, west, south;
				north = east = south = west = false;
				
				if (IsNorthIsLowerDetail()) {
					north = true;
				}
				if (IsEastIsLowerDetail()) {
					east = true;
				}
				if (IsWestIsLowerDetail()) {
					west = true;
				}
				if (IsSouthIsLowerDetail()) {
					south = true;
				}
				
				if (!north && !south && !east && !west) {
					
					
					unsigned int len = QuadIndices.size(); 
					for (int i =0; i< len; ++i) {
						inds.push_back(QuadIndices[i]);
					}
				} else {
					GetIndicesWithFixingBorder(inds, north,west,east,south);
				}
				
			}
		}
	} 	
}





bool QuadTerrain::IsNorthIsLowerDetail() {
	M3DVector3f spos, camera_pos;
	spos[0] = center_x;
	spos[1] = 0, 
	spos[2] = center_z;
	
	//Make adjustment by radius^2
	spos[2] -= (orthoRadius  );
	gCamera.GetOrigin(camera_pos);
	float dist = m3dGetDistanceSquared3(spos, camera_pos);
	camera_pos[1] = 0;
	return dist >= lower_detail_range_squared;
}

bool QuadTerrain::IsEastIsLowerDetail() {
	M3DVector3f spos, camera_pos;
	spos[0] = center_x;
	spos[1] = 0, 
	spos[2] = center_z;
	
	//Make adjustment by radius^2
	spos[0] += (orthoRadius);
	gCamera.GetOrigin(camera_pos);
	float dist = m3dGetDistanceSquared3(spos, camera_pos);
	return dist >= lower_detail_range_squared;
}

bool QuadTerrain::IsWestIsLowerDetail() {
	M3DVector3f spos, camera_pos;
	spos[0] = center_x;
	spos[1] = 0, 
	spos[2] = center_z;
	
	//Make adjustment by radius^2
	spos[0] -= (orthoRadius );
	gCamera.GetOrigin(camera_pos);

	float dist = m3dGetDistanceSquared3(spos, camera_pos);
	return dist >= lower_detail_range_squared;
}

bool QuadTerrain::IsSouthIsLowerDetail() {
	M3DVector3f spos, camera_pos;
	spos[0] = center_x;
	spos[1] = 0, 
	spos[2] = center_z;

	//Make adjustment by radius^2
	spos[2] += (orthoRadius);
	gCamera.GetOrigin(camera_pos);

	float dist = m3dGetDistanceSquared3(spos, camera_pos);
	
	return dist >= lower_detail_range_squared;
}

void QuadTerrain::GetIndicesWithFixingBorder(std::vector<unsigned int> &inds, bool north , bool west, bool east, bool south) {
	
	
	unsigned int len = QuadIndices.size(); 
	
	int northStartVertex = QuadIndices[len - 3] - subGridWidth + 1;
	int northEndVertex = northStartVertex + subGridWidth - 1;
	
	int southStartVertex = QuadIndices[1];
	int southEndVertex = southStartVertex + subGridWidth - 1;
	
	int westStartVertex = QuadIndices[1];
	
	int eastoffset = (6 * (subGridWidth - 1)) ;
	int eastStartVertex = QuadIndices[eastoffset-1 ] ;
	
	
	
	for (int i =0; i< len; ++i) {
		
		//Correct top border
		
		
		if (north  ) {
			if ((int)QuadIndices[i] > northStartVertex && (int)QuadIndices[i] < northEndVertex && (QuadIndices[i] % 2 == 1) ) {
				inds.push_back(QuadIndices[i] - 1);
				continue;
			}
		} 
		
		
		if(west  ) {
			if( ((int) QuadIndices[i] - westStartVertex ) % rowVertCount == 0  && QuadIndices[i] % 2 == 1) {		
				inds.push_back(QuadIndices[i] + rowVertCount  );
				continue;
			}
			
		}
		
		if(east   ) {
			
			if( (  (int)QuadIndices[i] - eastStartVertex   ) % rowVertCount == 0  && QuadIndices[i] % 2 == 1 ) {
				inds.push_back(QuadIndices[i] - rowVertCount );
				continue;
			}
			
		}
		if (south      ) {
			if ((int)QuadIndices[i] > southStartVertex && (int)QuadIndices[i] < southEndVertex && (QuadIndices[i] % 2 == 1) ) {
				inds.push_back(QuadIndices[i] + 1);
				continue;
			}
		} 
		
		inds.push_back(QuadIndices[i]);	
		
	}
	
	
	
}


/*
void QuadTerrain::CircleRaise(const sVector3 &point, float dt) {
	Terrain::CircleRaise(point,dt);
	//ComputeQuads();
}

void QuadTerrain::CircleLower(const sVector3 &point, float dt) {
	Terrain::CircleLower(point,dt);
	ComputeQuads();
}

void QuadTerrain::CircleSmooth(const sVector3 &point, float dt) {
	Terrain::CircleSmooth(point,dt);
	ComputeQuads();
}

void QuadTerrain::CircleFlat(const sVector3 &point, float dt) {
	Terrain::CircleFlat(point,dt);
	ComputeQuads();
}
*/
