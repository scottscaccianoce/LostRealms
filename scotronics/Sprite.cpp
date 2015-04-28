#include "Sprite.h"

Sprite::Sprite() {
	texLoaded = false;
	shader = new Shader();
	shader->Load("Sprite.vs", "Sprite.fs");
	
	
}
Sprite::~Sprite() {
	if (texLoaded) {
		tex = NULL;
	}
	delete shader;
}

void Sprite::SetTexture(sTexture *TexPtr) {
	tex = TexPtr;
}
void Sprite::RenderShader(float texStartU,float texStartV,float width,float height, bool wrap )
{
	shader->Begin();
	shader->SetUniform1i("spriteTex",0);
	shader->SetUniform3f("colorKey", 1.0f,0.0f,1.0f);
	glActiveTexture(GL_TEXTURE0);
	if (wrap) {
		tex->bindTextureWrap();
	} else {
		tex->bindTexture();
	}

	
	
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	
	
	
	M3DVector3f verts[4];
	M3DVector2f	texUVs[4];
	float x = pos[0];
	float y = pos[1];
	
	m3dLoadVector3(verts[0],x + width, y, 0.0f);
	m3dLoadVector3(verts[1],x , y, 0.0f);
	m3dLoadVector3(verts[2],x , y + height, 0.0f);
	m3dLoadVector3(verts[3],x + width, y + height, 0.0f);
	
	float leftx, lefty,rightx,righty;
	leftx =  texStartU / width;
	lefty =  (side - texStartV) / side ;
	
	rightx = (texStartU + width) / side;
	righty = (side - texStartV - height) / side ;
	
	
	
	/*m3dLoadVector2(texUVs[3],rightx, lefty);
	m3dLoadVector2(texUVs[2],leftx,lefty);
	m3dLoadVector2(texUVs[1],leftx,righty );
	m3dLoadVector2(texUVs[0],rightx,righty );
	*/
	m3dLoadVector2(texUVs[0],rightx, lefty);
	m3dLoadVector2(texUVs[1],leftx,lefty);
	m3dLoadVector2(texUVs[2],leftx,righty );
	m3dLoadVector2(texUVs[3],rightx,righty );
	
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(M3DVector3f),&verts[0][0]);
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,sizeof(M3DVector2f),&texUVs[0][0]);
	
	glDrawArrays(GL_QUADS, 0, 4);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	
	
	
	shader->End();
	
	
}
void Sprite::RenderShader() {
	shader->Begin();
	shader->SetUniform1i("spriteTex",0);
	shader->SetUniform3f("colorKey", 1.0f,0.0f,1.0f);
	glActiveTexture(GL_TEXTURE0);
	tex->bindTexture();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	

	
	M3DVector3f verts[4];
	M3DVector2f	texUVs[4];
	float x = pos[0];
	float y = pos[1];
	
	m3dLoadVector3(verts[0],x + side, y, 0.0f);
	m3dLoadVector3(verts[1],x , y, 0.0f);
	m3dLoadVector3(verts[2],x , y + side, 0.0f);
	m3dLoadVector3(verts[3],x + side, y + side, 0.0f);
	
	m3dLoadVector2(texUVs[0],1.0f, 1.0f);
	m3dLoadVector2(texUVs[1],0.0f,1.0f);
	m3dLoadVector2(texUVs[2],0.0f,0.0f);
	m3dLoadVector2(texUVs[3],1.0f,0.0f);
	
	
	
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(M3DVector3f),&verts[0][0]);
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,sizeof(M3DVector2f),&texUVs[0][0]);
	
	glDrawArrays(GL_QUADS, 0, 4);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	
	
	
	shader->End();
}
void Sprite::Render() {
	
	//glPushMatrix();
	;
	tex->bindTexture();
	
	glBegin(GL_QUADS);
	float x = pos[0];
	float y = pos[1];
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x  + side , y ,0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x , y ,0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x , y +side ,0.0f);
	glTexCoord2f(1.0f, 0.0f);glVertex3f(x + side , y + side ,0.0f);
	
	glEnd();
	
	 /*glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
     glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
     glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
	*/
	 // glPopMatrix();
	
}

void Sprite::Move(float x, float y) {
	pos[0] =x;
	pos[1] = y;
}

void Sprite::SetSide(float width) {
	side = width;
	
}
