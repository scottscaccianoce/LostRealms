/*
 *  UISlider.h
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/11/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

// Will return gMouse onChange event


#ifndef __UISLIDER__
#define __UISLIDER__
#include "UIWidget.h"

class UISlider : public UIWidget {
	
public:
	UISlider(UIWidget *ptr);
	~UISlider();
	void Render();
	bool MouseHandler(float, float);
	
	void SetValue(float value);
	float GetValue();
	void SetBounds(const UIRect &bounds);
	void ClearMouseActive();
private:
	float GetSafeValue(float);
	UIRect box;
	int oldx;
};


#endif