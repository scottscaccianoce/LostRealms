/*
 *  quadterrain.h
 *  MapEditor
 *
 *  Created by Scott Scaccianoce on 6/21/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifndef __QUADTERRAIN__ 
#define __QUADTERRAIN__

#include "terrain.h"
#include <vector>
#include "util.h"
#include "Camera.h"
#include "SPEntity.h"
#include "Enemy.h"
#include "Database.h"

class QuadTerrain : public Terrain {

public:
	
	QuadTerrain(int numVertRows, int numVertCols, float dx, float dz, const sVector3 &center, int desired_count); //Root instatation
	QuadTerrain(const std::vector<VertexTerrain> &vertices, int subgrid_cell_count, int desired_count, unsigned int offset, float cwidth );
	~QuadTerrain();
	void ComputeQuads();
	void ComputeIndices();
	void ComputeBoundry();
	void init();
	void ClearNodes();
	void SplitVerts(const std::vector<VertexTerrain> &source, std::vector<VertexTerrain> &NW, std::vector<VertexTerrain> &NE , std::vector<VertexTerrain> &SW , std::vector<VertexTerrain> &SE);
	void Render();
	
	static int counter;
	static int total;
	
	bool MouseIntersect(sVector3 &intersect_point);
	void CheckQuadIntersect(std::vector<VertexTerrain>*rootVerts,std::vector<TerrainTriangle> &triangles,std::vector<sVector3> &hits, M3DVector3f pos, sVector3 &pos1 ,sVector3 &pos2, M3DVector3f dir, M3DVector3f spos);
	
	/*void CircleRaise(const sVector3 &point, float dt);
	void CircleLower(const sVector3 &point, float dt);
	void CircleSmooth(const sVector3 &point, float dt);
	void CircleFlat(const sVector3 &p, float dt);
	*/
	
	bool SaveMap(const string &filename, std::vector<SPEntity*>&objects, std::vector<SPEntity*>&enemies);
	bool LoadMap(const string &filename, std::vector<SPEntity *>&objects, std::vector<SPMesh *>&meshes,std::vector<SPEntity *>&enemy_objects, std::vector<SPMesh *>&enemy_meshes);
	bool LoadMapWithEnemyData(const string &filename, std::vector<SPEntity *>&objects, std::vector<SPMesh *>&meshes,std::vector<Enemy *>&enemy_objects, std::vector<SPMesh *>&enemy_meshes,std::vector<SPMesh *>&enemy_attack_meshes, Database *database);
	
	
	bool GetHeightAtPos(float x, float y, float z, float &height);
	
	void SetLowerDetailRange(float dist);
	bool IsNorthIsLowerDetail();
	bool IsEastIsLowerDetail();
	bool IsWestIsLowerDetail();
	bool IsSouthIsLowerDetail();
	
	void GetIndicesWithFixingBorder(std::vector<unsigned int> &inds, bool n, bool w, bool e, bool s);

	
	
	
private:
	
	void DrawQuad(bool wireframe = false);
	void GetIndices(std::vector<unsigned int> &inds);
	bool isLeaf;
	bool isRoot;
	int subGridWidth;
	int desiredCount;
	static int rowVertCount;
	QuadTerrain *QuadNodes[4];
	std::vector<VertexTerrain> QuadVerts;
	std::vector<unsigned int> QuadIndices;
	std::vector<unsigned int> LowerQuadIndices;
	
	
	
	//Shere testing for Frustrum Calling
	float min_x;
	float min_y;
	float min_z;
	
	float max_x;
	float max_y;
	float max_z;
	
	float center_x;
	float center_y;
	float center_z;
	
	float radius;
	float orthoRadius;
	unsigned int index_offset;
	
	float lower_detail_range_squared;
	
	
};






#endif