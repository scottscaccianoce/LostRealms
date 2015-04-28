/*
 *  UIButton.h
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/8/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */


#ifndef __UIIMAGE__
#define __UIIMAGE__

#include "UIWidget.h"
#include "texture.h"

using namespace std;

class UIImage : public UIWidget {
	
public:
	
	UIImage(UIWidget *ptr, sTexture *tex );
	UIImage(UIWidget *ptr);
	
	void SetTexture(sTexture *tex);
	~UIImage();
	void Render();
	bool MouseHandler(float x, float y );
	
	
private:
	void Init();
	sTexture *texture;
	bool textureSet;
};




#endif