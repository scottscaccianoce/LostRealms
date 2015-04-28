/*
 *  UISlider.h
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/11/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

// Will return gMouse onChange event


#ifndef __UIBAR__
#define __UIBAR__
#include "UIWidget.h"

class UIBar : public UIWidget {
	
public:
	UIBar(UIWidget *ptr);
	~UIBar();
	void Render();
	bool MouseHandler(float, float);
	
	void SetBarColor(UIColor c);
	void SetPercentage(float p);
private:
	UIColor barColor;
	float percentage;
	UIPos mouse_offset;
};


#endif