/*
 *  UIButton.cpp
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/8/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "UIImage.h"


UIImage::UIImage(UIWidget *ptr, sTexture *tex ) {
	texture = tex;
	AddParent(ptr);
	BaseInit();
	Init();
	textureSet = true;
	
}


UIImage::UIImage(UIWidget *ptr ) {
	AddParent(ptr);
	BaseInit();
	Init();
	textureSet = false;
}




UIImage::~UIImage(){}

void UIImage::Render() {
	if (texture == NULL) {
		return;
	}
	if (visible) {
	
		RenderBg();
		
		UIPos pos = GetParentPos();
		float x =  pos.x;
		float y =   pos.y;
		
		if (textureSet) {
			
		
		texture->bindTexture();
			glEnable(GL_TEXTURE_2D);
			glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x  + rect.w , y ,0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x , y ,0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x , y +rect.h ,0.0f);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(x + rect.w , y + rect.h ,0.0f);
			
			glEnd();	
			glDisable(GL_TEXTURE_2D);
		}
		RenderBorder();
		
		
		
	}
}


bool UIImage::MouseHandler(float x, float y ) {
	
	UIPos pos = GetParentPos();
	UIRect temp = UIRect( pos.x, pos.y, rect.w, rect.h);
	if ( temp.Intersects(x,y) ||  mouse_active ){
		
		//if first click, register it as a click
		if (!mouse_active) {
			mouse_active = true;
			gMouse = this;
			return true;
		}
		
	}
	
	
	return false;
	
}


void UIImage::Init() {

	SetBgColor(kNone, UIColor(0,0,0,255));
	ShowBorder(false);
	
	

}

void UIImage::SetTexture(sTexture *tex) {
	texture = tex;
}