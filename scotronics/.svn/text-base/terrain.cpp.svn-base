#include "terrain.h"
#include <vector>
#include <math.h>
#include "Camera.h"

#define RED		0
#define GREEN	1
#define BLUE	2
#define ALPHA	3

Terrain::Terrain() {
	gridShader =NULL;
	texShader =NULL;
}
Terrain::Terrain(int numVertRows, int numVertCols, float dx, float dz, const sVector3 &center)
{
	
	cellWidth = dx;
	cellHeight = dz;
	
	
	intersect = false;
	raiseAmount = 30.0f;
	raiseRadius = 25.0f;
	
	int numVertices = numVertRows * numVertCols;
	mNumVertices = numVertices;
	numCellRows = numVertRows -1;
	numCellCols = numVertCols -1;
	numberVertRows = numVertRows;
	numberVertCols = numVertCols;
	
	int numTris = numCellRows * numCellCols * 2;
	
	width = (float)numCellCols * dx;
	depth = (float)numCellRows * dz;
	
	verts.resize(numVertices);
	
	//Might need to change these
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
			verts[k].pos.y = center.y;
			verts[k].tex0.x = vdx / 2;
			verts[k].tex0.y = udx /2;
			verts[k].tex1.x = (float)(j / (float)(numVertCols -1));
			verts[k].tex1.y = 1 - (float)(i / (float)(numVertRows -1));
			
			vdx += 1.0f;
			++k;
				
		}
		udx += 1.0f;
	}
	
	numIndices = numTris * 3;
	indices.resize(numIndices);
	
	
	//Shaders
	gridShader = new Shader();
	gridShader->Load("terrain_grid.vs", "terrain_grid.fs");
	
	texShader = new Shader();
	texShader->Load("terrain_tex.vs", "terrain_tex.fs");
	
	
	k =0;
	for(unsigned int i = 0; i < (unsigned int)numCellRows; ++i)
	{
		for(unsigned int j =0; j < (unsigned int)numCellCols; ++j) {
			
			/*
			 indices[k]     = i		* numVertCols + j;
			 indices[k + 1] = i		* numVertCols + j +1;
			 indices[k + 2] = (i+1)	* numVertCols + j;
			 
			 indices[k + 3] = (i+1)	* numVertCols + j;
			 indices[k + 4] = i		* numVertCols + j +1;
			 indices[k + 5] = (i+1)	* numVertCols + j + 1;
			 */
			indices[k]		= (i+1) * numVertCols + j;
			indices[k + 1]	= i		* numVertCols + j;
			indices[k + 2]	= (i+1) * numVertCols + j + 1;
			
			indices[k + 3]	= (i+1) * numVertCols + j + 1;
			indices[k + 4]	= i		* numVertCols + j;
			indices[k + 5]	= i		* numVertCols + j +1; 
			
			k+= 6;
			
		}
		
		
	}
	bRebuildBuffers = true;
	bRenderWireFrame = false;
	grass.loadTexture("grass.tga");
	dirt.loadTexture("dirt.tga");
	rock.loadTexture("rock.tga");
	snow.loadTexture("sand.tga");
	
	alphaMap.LoadTexData("blendmap.tga");
	
	
	//Alphamap must be vertsrow * 4 x vertscol *4
	// 64 x 64 must have 256 x 256 alphamap image
	alpha_size = numVertices * 16;
	channels = 4;
	//alpha_data = new unsigned char * [channels];
	//for (int i =0; i < channels; ++i) {
	//	alpha_data[i] = new unsigned char [alpha_size];
	//}
	
	alphaMapSize = (numVertRows - 1) * 4;
	
	unsigned char * data = alphaMap.GetDataPointer();
	for(int i =0; i < (alphaMapSize * 4 *alphaMapSize); i+=4) {
		data[i] =255;
		data[i+1] =0;
		data[i+2] =0;
		data[i+3] =0;
	}
	
	alphaMap.CopyDataToImage();
	ComputeNormals();
	
	//We divide by two because we center terrain at (0,0)
	max_pos_x = (numberVertCols/2) * cellWidth;
	max_pos_z = (numberVertRows/2) * cellWidth;
	min_pos_x = -max_pos_x;
	min_pos_z = -max_pos_z;
	
		
}


Terrain::~Terrain(){
	if (gridShader != NULL) {
		delete gridShader;
	}
	
	if(texShader != NULL) {
		delete texShader;
	}
}

void Terrain::RenderWireframe(bool value) {
	bRenderWireFrame = value;
}

void Terrain::Render()
{
	glPushMatrix();
	
	//glTranslatef(center.x,center.y, center.z);
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
		
		//glColor3f(0.0f,1.0f,0.0f);
		
		glDrawElements(GL_TRIANGLES,numIndices,GL_UNSIGNED_INT ,&indices[0]);
		
		gridShader->End();		
		
		glDisableClientState(GL_VERTEX_ARRAY);
		
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	
	
		/*if(intersect){
			
			glBegin(GL_TRIANGLES);
			glVertex3f(iverts[0].x,iverts[0].y,iverts[0].z);
			glVertex3f(iverts[1].x,iverts[1].y,iverts[1].z);
			glVertex3f(iverts[2].x,iverts[2].y,iverts[2].z);
			
			glEnd();
			
		}
	*/
		
		//glEnable(GL_TEXTURE_2D);
		//glEnable(GL_LIGHTING);
	
	
	} else {
			// TEXTURE SHADER
		M3DVector4f vLightPos = {250.0f, 500.0f, 0.0f, 1.0f};
		M3DVector4f vLightEyePos;
		M3DMatrix44f mCamera;
		gCamera.GetCameraMatrix(mCamera);
		m3dTransformVector3(vLightEyePos, vLightPos, mCamera);
		
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT,sizeof(VertexTerrain),&verts[0].tex1.x);
		
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(VertexTerrain), &verts[0].normal.x);
		
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,sizeof(VertexTerrain),&verts[0].pos.x);
		
		texShader->Begin();
		M3DVector4f cameraPos;
		gCamera.GetOrigin(cameraPos);
		texShader->SetUniform3f("CameraPos", cameraPos[0],cameraPos[1],cameraPos[2]);
		texShader->SetUniform1i("grassTex",0);
		texShader->SetUniform1i("dirtTex",1);
		texShader->SetUniform1i("rockTex",2);
		texShader->SetUniform1i("snowTex",3);
		
		texShader->SetUniform1i("AlphaMapTex",4);
		
		texShader->SetUniform3f("lightdir", vLightEyePos[0], vLightEyePos[1], vLightEyePos[2]);

		
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
		
		
		glDrawElements(GL_TRIANGLES,numIndices,GL_UNSIGNED_INT ,&indices[0]);
		
		
		
		texShader->End();		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		
		
		glActiveTexture(GL_TEXTURE0);
		
	}
	glPopMatrix();
}
void Terrain::RaiseVertex(int vertexIndex, float amount)
{
	verts[vertexIndex].pos.y += amount;

}

bool Terrain::getIntersect( sVector3 &pt1,  sVector3 &pt2, sVector3 &HitPos)
{
	bool result = false;
	std::vector<sVector3> hits;
	std::vector<TerrainTriangle> triangles;
	
	
	for (int i = 0; i  < numCellRows; ++i)
	{
		for(int j = 0; j < numCellCols; ++j){
		
			//first triangle
		
			
			unsigned int id1 = (i+1) * numberVertCols + j;
			unsigned int id2 = i		* numberVertCols + j;
			unsigned int id3 = (i+1) * numberVertCols + j + 1;

			
			
			if(CheckLineTri(verts[id1].pos,verts[id2].pos,verts[id3].pos, pt1, pt2, HitPos)){
				result = true;
				
				TerrainTriangle t;
				t.pos1 = verts[id1].pos;
				t.pos2 = verts[id2].pos;
				t.pos3 = verts[id3].pos;
				triangles.push_back(t);
				
				
				hits.push_back(HitPos);
				
			}
			
			//Second triangle
			id1 = (i+1) * numberVertCols + j + 1;
			id2 = i		* numberVertCols + j;
			id3 =  i		* numberVertCols + j +1; 
			
			
			if(CheckLineTri(verts[id1].pos,verts[id2].pos,verts[id3].pos, pt1, pt2, HitPos)){
				result = true;
				TerrainTriangle t;
				t.pos1 = verts[id1].pos;
				t.pos2 = verts[id2].pos;
				t.pos3 = verts[id3].pos;
				triangles.push_back(t);
				hits.push_back(HitPos);
				
				break;
			}
			
		}
	}
		
	intersect = result;
	if(intersect){
	
		if(hits.size() == 1){
			//get closest
			iverts[0] = triangles[0].pos1;
			iverts[1] = triangles[0].pos2;
			iverts[2] = triangles[0].pos3;
			HitPos = hits[0];
			
		} else {
			float min = 99999999;
			float index = 0;
			
			for(int i = 0; i < hits.size(); ++i){
				//Find distance from pt1 to hit and use the lowest distance;
				float x2 = hits[i].x;
				float y2 = hits[i].y;
				float z2 = hits[i].z;
				
				float x1 = pt1.x;
				float y1 = pt1.y;
				float z1 = pt1.z;
				
				float dist = sqrt((x2 - x1) * (x2-x1) + (y2 - y1) * (y2-y1) + (z2 - z1) * (z2-z1));
			
				if(dist < min){
					index = i;
					min = dist;
				}
			}
			
			iverts[0] = triangles[index].pos1;
			iverts[1] = triangles[index].pos2;
			iverts[2] = triangles[index].pos3;
			HitPos = hits[index];
			
			
			
		}
		
		//get closest
		//iverts[0] = verts[id1].pos;
		//iverts[1] = verts[id2].pos;
		//iverts[2] = verts[id3].pos;
	}
	
	
	return result;
	
}

void Terrain::CircleRaise(const sVector3 &point, float dt)
{
	for(unsigned int i = 0; i < mNumVertices; ++i){
		float dist = sqrt(
						  (point.x - verts[i].pos.x) * (point.x - verts[i].pos.x) +
						  (point.y - verts[i].pos.y) * (point.y - verts[i].pos.y) +
						  (point.z - verts[i].pos.z) * (point.z - verts[i].pos.z) 
						);
	
		if(dist <= raiseRadius){
			float amount = raiseAmount * (1-(dist/raiseRadius));
			RaiseVertex(i, amount * dt);
		}
	}
	ComputeNormals();
}

void Terrain::CircleLower(const sVector3 &point, float dt)
{
	for(unsigned int i = 0; i < mNumVertices; ++i){
		float dist = sqrt(
						  (point.x - verts[i].pos.x) * (point.x - verts[i].pos.x) +
						  (point.y - verts[i].pos.y) * (point.y - verts[i].pos.y) +
						  (point.z - verts[i].pos.z) * (point.z - verts[i].pos.z) 
						  );
		
		if(dist <= raiseRadius){
			float amount = raiseAmount * (1-(dist/raiseRadius));
			RaiseVertex(i, -amount * dt);
		}
	}
	ComputeNormals();
}

void Terrain::CircleSmooth(const sVector3 &p, float dt)
{
	
	
	
	for(unsigned int i = 0; i < mNumVertices; ++i)
	{
		float dist = sqrt( ((p.x - verts[i].pos .x)* (p.x - verts[i].pos .x)) + ((p.z - verts[i].pos .z)* (p.z - verts[i].pos .z)));
		if(dist <= raiseRadius)
		{
			//apply box filter.
			int temp = i / numberVertCols;
			float result = sampleHeight3x3((int)i /numberVertCols, i - (temp * numberVertCols));
			float answer = verts[i].pos.y - result;
			answer /= 2;
			verts[i].pos .y -= answer * (dt * raiseAmount);
		}
		
	}
	ComputeNormals();
	
}

void Terrain::CircleFlat(const sVector3 &p, float dt)
{	
	for(unsigned int i = 0; i < mNumVertices; ++i)
	{
		float dist = sqrt( ((p.x - verts[i].pos .x)* (p.x - verts[i].pos .x)) + ((p.z - verts[i].pos .z)* (p.z - verts[i].pos .z)));
		if(dist <= raiseRadius)
		{
			verts[i].pos .y = p.y;
		}
		
	}
	ComputeNormals();
	
}

float Terrain::sampleHeight3x3(int i, int j)
{
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------
	
	float avg = 0.0f;
	float num = 0.0f;
	
	//avg += 2* verts[j + (i * numberVertCols)].pos.y;
	for(int m = i-1; m <= i+1; ++m)
	{
		for(int n = j-1; n <= j+1; ++n)
		{
			if( inBoundsGeo(m,n) )
			{
				avg += verts[n + (m * numberVertCols)].pos.y;
				num += 1.0f;
			}
		}
	}
	
	return avg / num ;
}

bool Terrain::inBoundsGeo(int i, int j) {
    unsigned int rows =(numberVertRows) ;
	unsigned int cols =(numberVertRows) ;
	return i >= 0 && 	i < rows && 	j >= 0 && 	j < cols;
}

bool Terrain::MouseIntersect(sVector3 &intersect_point) {
	sVector3 pos1, pos2, dir;
	GetMouseRay(pos1,pos2,dir);
	return getIntersect(pos1,pos2,intersect_point);

}

void Terrain::SetRadius(float value) {
	raiseRadius = value;
}


//Texture Splating

void Terrain::DrawSplat(sVector3 &p, int channel , int brushlength) {

	
	float xOffset = -width * 0.5f; 
	float zOffset =  depth * 0.5f;
	
	p.x -= xOffset;
	p.z = zOffset + p.z;
	
	
	
	float j = p.x * (1/width);
    float i = p.z  * (1/width);
	
	if (i < 0) i = 0.0f;
	if (j > 1) j = 1.0f;
		//percentage of big image
	j = j * alphaMapSize;
	i *= alphaMapSize;
	
	int row = (int) i + 0.5f;
	int col = (int) j + 0.5f; // 0 - alphaMapSize
	
	
	
	int index = ((int)j* 4 + (int)i * (alphaMapSize * 4));
	unsigned char *data = alphaMap.GetDataPointer();
	SetAlphaMapColor(data,index,channel);
	
	if (brushlength > 1 ) {
		SplatAdj(index, row, col, brushlength, data, channel);
		
	}
	data = NULL;
	alphaMap.CopyDataToImage();
}

void Terrain::DrawFilter(sVector3 &p, int brushlength) {
	brushlength--;
	
	float xOffset = -width * 0.5f; 
	float zOffset =  depth * 0.5f;
	
	p.x -= xOffset;
	p.z = zOffset + p.z;
	
	
	
	float j = p.x * (1/width);
    float i = p.z  * (1/width);
	
	//percentage of big image
	j = j * alphaMapSize;
	i *= alphaMapSize;
	
	int row = (int) i + 0.5f;
	int col = (int) j + 0.5f; // 0 - alphaMapSize
	
	
	
	//int index = ((int)j* 4 + (int)i * (alphaMapSize * 4));
	unsigned char *data = alphaMap.GetDataPointer();
	
	//if (brushlength > 1) {
		for (int i = row -brushlength;  i <= row + brushlength; ++i) {
			if (i < 0 || i > alphaMapSize) {
				continue;
			}
			for (int j = col - brushlength; j <= col + brushlength ; ++j) {
				if (j < 0 || j > alphaMapSize) {
					continue;
				}
				
				for(int t =0; t < 4; ++t) {
					int avg = sampleTex3x3(i, j, t);
					int index = ((int)j* 4 + (int)i * (alphaMapSize * 4));
					data[index + t] = avg;

				}
			}
		}
		//for(int i = 0; i < 4; ++i) {
		//	int avg = sampleTex3x3( row,  col,  i);
		//	data[index + i] = avg;
		//}	
	//} else {
	
	//	for(int i = 0; i < 4; ++i) {
//			int avg = sampleTex3x3( row,  col,  i);
//			data[index + i] = avg;
//			
//		}
//	}
	
	data = NULL;
	alphaMap.CopyDataToImage();
	
}
void Terrain::SplatAdj(int index, int row, int col, int brushlength, unsigned char *data, int channel) {
	/*
              [	  ] 
		[   ] [ x ] [   ] 
	          [   ]
	 
	 */
	
	//Exit condition
	if (brushlength <= 1) {
		
		return;
	}
	
	//Top
	if (row - 1 >= 0) {
		int newindex = ( ((row - 1) * alphaMapSize )* 4) + col * 4;
		SetAlphaMapColor(data, newindex, channel);
		if (brushlength > 1)
			SplatAdj(newindex, row - 1, col, brushlength - 1, data, channel);
		
		
	}
	//Bottom
	if (row + 1 < alphaMapSize) {
		int newindex = ( ((row + 1) * alphaMapSize) * 4) + col* 4;
		SetAlphaMapColor(data, newindex, channel);
		if (brushlength > 1)
			SplatAdj(newindex, row + 1, col, brushlength - 1, data, channel);
		
	}
	//Left
	if (col - 1 >= 0) {
		int newindex = (( (row ) * alphaMapSize)  * 4) + (col -1) * 4;
		SetAlphaMapColor(data, newindex, channel);
		if (brushlength > 1)
			SplatAdj(newindex, row, col -1, brushlength - 1, data, channel);
		
	}
	//right
	if (col + 1 < alphaMapSize) {
		int newindex = (( (row ) * alphaMapSize ) *  4) + (col +1) * 4;
		SetAlphaMapColor(data, newindex, channel);
		if (brushlength > 1)
			SplatAdj(newindex, row, col +1, brushlength - 1, data, channel);
		
	}
	
	//topleft 
	if (col -1 >= 0 && row -1 >= 0) {
		int newindex = (( (row -1) * alphaMapSize)  * 4) + (col -1) * 4;
		SetAlphaMapColor(data, newindex, channel);
		if (brushlength > 1)
			SplatAdj(newindex, row -1, col -1, brushlength - 1, data, channel);
		
	}
	
	//topright 
	if (col +1 < alphaMapSize && row -1 >= 0) {
		int newindex = (( (row -1) * alphaMapSize)  * 4) + (col +1) * 4;
		SetAlphaMapColor(data, newindex, channel);
		if (brushlength > 1)
			SplatAdj(newindex, row -1, col +1, brushlength - 1, data, channel);
		
	}
	
	//bottomleft 
	if (col -1 >= 0 && row +1 < alphaMapSize) {
		int newindex = (( (row +1) * alphaMapSize)  * 4) + (col -1) * 4;
		SetAlphaMapColor(data, newindex, channel);
		if (brushlength > 1)
			SplatAdj(newindex, row +1, col -1, brushlength - 1, data, channel);
		
	}
	
	//bottomright 
	if (col +1 <= alphaMapSize && row +1 < alphaMapSize) {
		int newindex = (( (row +1) * alphaMapSize)  * 4) + (col +1) * 4;
		SetAlphaMapColor(data, newindex, channel);
		if (brushlength > 1)
			SplatAdj(newindex, row +1, col +1, brushlength - 1, data, channel);
		
	}
	
}



void Terrain::SetAlphaMapColor(unsigned char* data, int index, int channel) {
	for (int i = 0; i < 4; ++i) {
		if(i == channel) {
			data[index + i] = 255;
		} else {
			data[index + i ] = 0;
		}
	}
}

int Terrain::sampleTex3x3(int i, int j, int texCoord)
{
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------
	unsigned char *alphaMapData = alphaMap.GetDataPointer();
	float avg = 0.0f; 
	//unsigned int texVertCols = numberVertCols * 4;
	unsigned int texVertCols = alphaMapSize;
	int channels = 4;
	float num = 0.0f;
	for(int m = i-1; m <= i+1; ++m)
	{
		for(int n = j-1; n <= j+1; ++n)
		{
			
			if( inBounds(m,n) )
			{
				unsigned int vertex = ((n + (m * texVertCols)) * channels);
				
				avg += alphaMapData[ vertex + texCoord];
				num += 1.0f;
			}
		}
	}
	
	return avg / num ;
}

bool Terrain::inBounds(int i, int j)
{
    unsigned int rows =(numberVertRows) * 4;
	unsigned int cols =(numberVertRows) * 4;
	return i >= 0 && 	i < rows && 	j >= 0 && 	j < cols;
}

void Terrain::ComputeNormals() {
	unsigned int faceSize;
	faceSize = numCellCols * numCellCols * 2;
	sVector3* faceNormals;
	faceNormals = new sVector3[faceSize];
	
	//clear normals
	for(int i = 0; i < mNumVertices; ++i)
	{
		verts[i].normal = sVector3(0,0,0);
	}
	
	bRebuildBuffers = true;
	unsigned int index = 0;
	for(int i = 0; i < numCellRows; ++i)
	{
		
		for(int j = 0; j < numCellCols; ++j)
		{
			int aId = j + (i * numberVertCols);
			int bId = (j +1 ) + (i * numberVertCols);		
			int cId = j + ( (i + 1) * numberVertCols);
			int dId = (j + 1) + ((i+ 1) * numberVertCols);
			
			sVector3 vecA = verts[aId].pos ;
			sVector3 vecB = verts[bId].pos ;
			sVector3 vecC = verts[cId].pos ;
			sVector3 vecD = verts[dId].pos ;
			
			//  A-----B
			//  |   / |
			//  |  /  |
			//  | /   |
			//  C-----D
			
			
			
			sVector3 u,v,n;
			//n = sVector3(0,0,0);
			//get ABC normal
			u = SubsVectors3(&vecB, &vecA);
			v = SubsVectors3(&vecC, &vecA);
			
			//u = vecB - vecA;
			//v = vecC - vecA;
			//D3DXVec3Cross(&n,&u,&v);
			n = Cross(&u, &v);
			Normalize(n);
			
			//D3DXVec3Normalize(&n,&n);
			faceNormals[index] = n;
			++index;
			
			
			//get BDC normal
			u = SubsVectors3(&vecC, &vecD);
			v = SubsVectors3(&vecB, &vecD);
			n = Cross(&u, &v);
			Normalize(n);
			//u = vecC - vecD;
			//v = vecB - vecD;
			//D3DXVec3Cross(&n,&u,&v);
			//D3DXVec3Normalize(&n,&n);
			faceNormals[index] = n;
			++index;
			
			
			
		}
	}
	
	//now get all the face normals
	
	for(int i = 0; i < numberVertCols; ++i)
	{
		for(int j = 0; j < numberVertCols; ++j)
		{
			//vertex [j + i * numCellCols
			int triangleCounter = 0;
			sVector3 total = sVector3(0,0,0);
			
			//get the six adjacent triangles
			
			//get top left
			if(j > 0 && i > 0)
			{
				//quad 1
				unsigned int quad = (((i - 1) * numCellCols ) + (j - 1)) * 2;
				AddVector3ToVector(&total, &faceNormals[quad + 1]);

				++triangleCounter;
			}
			
			//get quad 2 (2 in top right
			if(j < numberVertCols - 1 && i > 0)
			{
				unsigned int quad = (((i - 1) * numCellCols ) + (j)) * 2;
				AddVector3ToVector(&total, &faceNormals[quad]);
				AddVector3ToVector(&total, &faceNormals[quad + 1]);
				++triangleCounter;
				++triangleCounter;
				
			}
			
			//get quad 3 lower left
			if( j > 0 && i < numberVertRows - 1)
			{
				unsigned int quad = (((i) * numCellCols ) + (j - 1)) * 2;
				AddVector3ToVector(&total, &faceNormals[quad]);
				AddVector3ToVector(&total, &faceNormals[quad + 1]);
				++triangleCounter;
				++triangleCounter;
			}
			
			//get quad 4 lower right
			if ( j < numberVertCols - 1 && i < numberVertRows - 1)
			{
				unsigned int quad = ((i * numCellCols ) + j) * 2;
				AddVector3ToVector(&total, &faceNormals[quad]);
				++triangleCounter;
			}
			
			//average the face normals
			total.x /= triangleCounter;
			total.y /= triangleCounter;
			total.z /= triangleCounter;
			
			verts[(i * numberVertCols) + j].normal = total;
			
			
		}
	}
	
	
	
	
	
	//normalize them all
	for(int i = 0; i < mNumVertices; ++i)
	{
		Normalize(verts[i].normal);

	}
	delete [] faceNormals;
	
}

