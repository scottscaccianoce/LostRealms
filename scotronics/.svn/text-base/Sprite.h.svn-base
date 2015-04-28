
#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "texture.h"
#include "math3d.h"
#include "shader.h"

class Sprite : public sTexture {
	
	
public:
	Sprite();
	~Sprite();
	void SetTexture(sTexture *TexPtr);
	void Render();
	void RenderShader();
	void RenderShader(float texStartU,float texStartV,float width,float length, bool wrap = false);
	void SetSide(float width);
	
	void Move(float x, float y);
	
	
protected:
	
	
	
protected: 
	
	
private:
	bool texLoaded;
	sTexture *tex;
	M3DVector2f pos;
	float side;
	Shader	*shader;
	
};




#endif