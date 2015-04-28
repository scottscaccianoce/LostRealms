
#ifndef __BILLBOARD_H__
#define __BILLBOARD_H__

#include "texture.h"
#include "math3d.h"
#include "shader.h"

enum BillboardType {
	nCamera=0, nHorizontal, nVertical
};

struct BillVert {
	M3DVector3f pos;
	M3DVector2f tex;
};

class Billboard {
	
	
public:
	
	Billboard(float width, float height, BillboardType bt, sTexture *texture);
	~Billboard();
	
	void Render(Shader &shader, float alpha = 1.0, M3DVector3f cameraPos = NULL);
	void RenderWithRotY(Shader &shader, float alpha, float value);
	
	void SetScale(float value) { scale = value; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }	
	void GetPos(M3DVector3f &p) { m3dCopyVector3(p, pos);}
	void SetPos(M3DVector3f &p) { m3dCopyVector3(pos, p);}
	void SetUseAlphaChannel(bool value) {useAlphaChannel = value ? 1.0f : 0.0f;}
	
private:
	
	sTexture *tex;
	M3DVector3f pos;
	float scale;
	float width;
	float height;
	BillboardType bType;
	
	BillVert verts[6];
	int indices[6];
	float useAlphaChannel;
};




#endif