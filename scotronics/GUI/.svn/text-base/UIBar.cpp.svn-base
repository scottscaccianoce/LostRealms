/*
 *  UISlider.cpp
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/11/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "UIBar.h"

UIBar::UIBar(UIWidget *ptr ) {
	AddParent(ptr);
	BaseInit();

	SetBgColor(kNone, UIColor(39,39,39));
	percentage = 1.0f;
	SetBarColor(UIColor(0,0,255));
	ShowBorder(true);
}

UIBar::~UIBar() {
	
}
void UIBar::SetBarColor(UIColor c) {
	barColor = c;
}

void UIBar::SetPercentage(float p) {
	percentage = p;
}

void UIBar::Render() {
		
	if (!visible) {
		return;
	}
	
	UIPos pos = GetParentPos();
	
	
	RenderBg();
	int width = rect.w * percentage;
	//render bar
	glColor3ub (barColor.r,barColor.g,barColor.b);
	glBegin(GL_QUADS);
	glVertex2f(pos.x  + width	,  pos.y);
	glVertex2f(pos.x			,  pos.y );
	glVertex2f(pos.x			,  pos.y + rect.h);
	glVertex2f(pos.x + width	,  pos.y + rect.h);
	glEnd();
	
	RenderBorder();
	
		
}

bool UIBar::MouseHandler(float x, float y) {
	if (visible == true) {
		
		if ( rect.Intersects(x,y) ||  mouse_active ){
			if (dragable) {
				//if first click, bring to front
				if (!mouse_active) {
					mouse_offset = UIPos(x - rect.x, y - rect.y);
					
				}
				mouse_active = true;
				rect.x = x - mouse_offset.x;
				rect.y = y - mouse_offset.y;
			}
			return true; //we claimed the event so return true;
		}
		
	}
	
	return false;
	
}
