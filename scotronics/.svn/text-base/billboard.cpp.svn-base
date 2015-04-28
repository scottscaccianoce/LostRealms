#include "billboard.h"


Billboard::Billboard(float w, float h, BillboardType bt, sTexture *texture) {
	width = w;
	height = h;
	bType = bt;
	tex = texture;
	scale = 2.0f;
	useAlphaChannel = 0.0f;
	float half_width = width /2.0f;
	float half_height = height /2.0f;
	
	
	switch (bt) {
		case nCamera:
			m3dLoadVector3(verts[0].pos, -half_width, -half_height, 0);
			m3dLoadVector3(verts[1].pos, half_width, -half_height, 0);
			m3dLoadVector3(verts[2].pos, half_width, half_height, -0);
			m3dLoadVector3(verts[3].pos, -half_width, half_height, -0);
			
			m3dLoadVector2(verts[0].tex, 0, 0);
			m3dLoadVector2(verts[1].tex, 1, 0);
			m3dLoadVector2(verts[2].tex, 1, 1);
			m3dLoadVector2(verts[3].tex, 0, 1);
			
			break;
		case nHorizontal:
			m3dLoadVector3(verts[0].pos, -half_width, 0, half_height);
			m3dLoadVector3(verts[1].pos, half_width, 0, half_height);
			m3dLoadVector3(verts[2].pos, half_width, 0, -half_height);
			m3dLoadVector3(verts[3].pos, -half_width, 0, -half_height);
			
			m3dLoadVector2(verts[0].tex, 0, 0);
			m3dLoadVector2(verts[1].tex, 1, 0);
			m3dLoadVector2(verts[2].tex, 1, 1);
			m3dLoadVector2(verts[3].tex, 0, 1);
			
			
			break;
		case nVertical:
			m3dLoadVector3(verts[0].pos, -half_width, -half_height, 0);
			m3dLoadVector3(verts[1].pos, half_width, -half_height, 0);
			m3dLoadVector3(verts[2].pos, half_width, half_height, -0);
			m3dLoadVector3(verts[3].pos, -half_width, half_height, -0);
			
			m3dLoadVector2(verts[0].tex, 0, 0);
			m3dLoadVector2(verts[1].tex, 1, 0);
			m3dLoadVector2(verts[2].tex, 1, 1);
			m3dLoadVector2(verts[3].tex, 0, 1);

			
			break;
		
	}
	
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 3;
	indices[4] = 1;
	indices[5] = 2;
	
	
	
}
Billboard::~Billboard() {}

void Billboard::Render(Shader &shader,  float alpha , M3DVector3f cameraPos) {

	glPushMatrix();
	glDisable(GL_CULL_FACE);
	M3DMatrix44f mWorld, mScale, mRot;
	
	if (bType == nHorizontal || bType == nVertical) {
		
		m3dScaleMatrix44(mScale, scale, scale, scale);
		m3dTranslationMatrix44(mWorld, pos[0],pos[1],pos[2]);
		
		glMultMatrixf(mWorld);
		glMultMatrixf(mScale);
		
	} else if (bType == nCamera) {
		
		
		M3DVector3f objToCamProj, lookAt,upAux;
		M3DVector3f tempCamera, tempPos;
		m3dCopyVector3(tempCamera, cameraPos);
		m3dCopyVector3(tempPos,pos);
		tempPos[1] = 0;
		tempCamera[1] = 0;
		
		m3dSubtractVectors3(objToCamProj, tempCamera, tempPos);
		objToCamProj[1] = 0;
		m3dLoadVector3(lookAt, 0, 0, 1);
		m3dNormalizeVector3(objToCamProj);
		m3dNormalizeVector3(lookAt);
		
		m3dCrossProduct3(upAux, lookAt, objToCamProj);
		float angleCosine = m3dDotProduct3(lookAt, objToCamProj);
		
		if ((angleCosine < 0.9999) && (angleCosine > -0.9999)) {
			m3dRotationMatrix44(mRot,acosf(angleCosine),0,1,0);	
		} else {
			m3dRotationMatrix44(mRot,acosf(angleCosine),0,-1,0);
		}

		
		
		m3dScaleMatrix44(mScale, scale, scale, scale);
		m3dTranslationMatrix44(mWorld, pos[0],pos[1],pos[2]);
		
		
		glMultMatrixf(mWorld);
		
		glMultMatrixf(mScale);
		glMultMatrixf(mRot);
	 
	 
		
	}

	shader.Begin();

	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(BillVert),&verts[0].pos[0]);
	
	shader.SetUniform1i("billTex",0);
	shader.SetUniform1f("alphaFactor", alpha);
	shader.SetUniform1f("useAlpha", useAlphaChannel);
	glActiveTexture(GL_TEXTURE0);
	tex->bindTexture();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,sizeof(BillVert),&verts[0].tex[0]);
	
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT ,&indices[0]);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	
	shader.End();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glEnable(GL_CULL_FACE);
	glPopMatrix();
	
}

void Billboard::RenderWithRotY(Shader &shader, float alpha, float value) {

	glPushMatrix();
	glDisable(GL_CULL_FACE);
	M3DMatrix44f mWorld, mScale, mRot;
	

	
	m3dScaleMatrix44(mScale, scale, scale, scale);
	m3dTranslationMatrix44(mWorld, pos[0],pos[1],pos[2]);
	m3dRotationMatrix44(mRot, m3dDegToRad(value),0,1,0);
	
	glMultMatrixf(mWorld);
	glMultMatrixf(mScale);
	glMultMatrixf(mRot);

	shader.Begin();
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(BillVert),&verts[0].pos[0]);
	
	shader.SetUniform1i("billTex",0);
	shader.SetUniform1f("alphaFactor", alpha);
	shader.SetUniform1f("useAlpha", useAlphaChannel);
	glActiveTexture(GL_TEXTURE0);
	tex->bindTexture();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,sizeof(BillVert),&verts[0].tex[0]);
	
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT ,&indices[0]);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	
	shader.End();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glEnable(GL_CULL_FACE);
	glPopMatrix();

}