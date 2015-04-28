
#ifndef __TERRAIN__
#define __TERRAIN__

#ifndef __APPLE__
#include "GL/glew.h"
#endif

#include "GL/glfw.h"
#include <string>
#include "util.h"
#include <vector>
#include "shader.h"
#include "texture.h"
#include "math3d.h"


struct TerrainTriangle{
	sVector3 pos1;
	sVector3 pos2;
	sVector3 pos3;
	
};

struct triangle {
	unsigned int vert1Id;
	unsigned int vert2Id;
	unsigned int vert3Id;
	float u;
	float v;
	float dist;
	
};

struct VertexTerrain {
	sVector3 pos;
	sVector3 normal;
	sVector2 tex0;
	sVector2 tex1;
	VertexTerrain(){
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
		
		
	}
	
	
};

class Terrain {
	
public:
	Terrain(int numVertRows, int numVertCols, float dx, float dz, const sVector3 &center);
	Terrain(); //used in quadtree
	~Terrain();
	
	
	void Render();
	void RenderWireFrame();
	void buildBuffers();
	void computeNormals();
	
	void RaiseVertex(int vertexIndex, float amount);
	void CircleRaise(const sVector3 &point, float dt);
	void CircleLower(const sVector3 &point, float dt);
	void CircleSmooth(const sVector3 &point, float dt);
	void CircleFlat(const sVector3 &p, float dt);
	
	//Texture splating
	void DrawSplat( sVector3 &p, int channel, int brushlength );
	void DrawFilter(sVector3 &p, int brushlength = 1);
	void SplatAdj(int index, int row , int col, int brushlength, unsigned char *data, int channel_offset);
	
	bool getIntersect( sVector3 &pt1,  sVector3 &pt2, sVector3 &HitPos);
	bool MouseIntersect(sVector3 &intersect_point);
	
	void SetRadius(float value);
	void RenderWireframe(bool value);
	void SetRenderCircle(bool value) {renderCircle = value;}
	bool bRenderWireFrame;
	
	
protected:
	
	void ComputeNormals();
	
	void SetAlphaMapColor(unsigned char* data, int index, int channel);

	bool inBounds(int i, int j);
	float sampleHeight3x3(int i, int j);
	int sampleTex3x3(int i, int j, int texCoord);
	bool inBoundsGeo(int i, int j);
	
	std::vector<unsigned int> indices;
	
	//sVector3 verticies;
	sVector3 center;
	
	std::vector<VertexTerrain>verts;
	
	
	unsigned int mNumVertices;
	unsigned int mNumTriangles;
	
	int numCellRows;
	int numCellCols;
	float width, depth;
	float cellWidth, cellHeight;
	float max_pos_x, max_pos_z;
	float min_pos_x, min_pos_z;
	
	int numberVertRows;
	int numberVertCols;
	int numIndices;
	bool bRebuildBuffers;
	
	bool renderCircle;
	
	bool intersect;
	sVector3 iverts[3];
	
	float raiseAmount;
	float raiseRadius;
	
	Shader *gridShader;
	Shader *texShader;
	
	sTexture grass;
	sTexture dirt;
	sTexture rock;
	sTexture snow;
	
	sTexture alphaMap;
	unsigned char **alpha_data;
	unsigned int channels;
	unsigned int alpha_size;
	int alphaMapSize;
	
};




#endif