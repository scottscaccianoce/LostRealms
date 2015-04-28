/*
 *  UISlider.cpp
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/11/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "UISlider.h"

UISlider::UISlider(UIWidget *ptr ) {
	AddParent(ptr);
	BaseInit();

	SetBgColor(kNone, UIColor(39,39,39));
	
	box.x = 0;
	box.y = -5;
	box.w = 9;
	box.h = 14;
}

UISlider::~UISlider() {
	
}

void UISlider::Render() {
	if (!visible) {
		return;
	}
	
	UIPos pos = GetParentPos();
	
	//render bar
	glColor3ub (bg_color[kNone].r,bg_color[kNone].g,bg_color[kNone].b);
	glBegin(GL_QUADS);
	glVertex2f(pos.x  + rect.w	,  pos.y);
	glVertex2f(pos.x			,  pos.y );
	glVertex2f(pos.x			,  pos.y + rect.h);
	glVertex2f(pos.x + rect.w	,  pos.y + rect.h);
	glEnd();
	
		//Render Box
	glColor3ub (159,159,159);
	glBegin(GL_QUADS);
	glVertex2f(pos.x + box.w + box.x	,  pos.y + box.y);
	glVertex2f(pos.x + box.x			,  pos.y + box.y);
	glVertex2f(pos.x + box.x			,  pos.y + box.h + box.y);
	glVertex2f(pos.x + box.x + box.w	,  pos.y + box.h + box.y);
	glEnd();
	
	glColor3ub (border_color[kNone].r,border_color[kNone].g,border_color[kNone].b);
	glBegin(GL_LINE_LOOP);
	glVertex2f(pos.x + box.w + box.x	,  pos.y + box.y);
	glVertex2f(pos.x + box.x			,  pos.y + box.y);
	glVertex2f(pos.x + box.x			,  pos.y + box.h + box.y);
	glVertex2f(pos.x + box.x + box.w	,  pos.y + box.h + box.y);
	glEnd();
	
}

bool UISlider::MouseHandler(float x, float y) {
	unsigned int len = children.size();
	for (unsigned int i = 0 ; i < len; ++i) {
		bool result = children[i]->MouseHandler(x,y);
		if (result) {
			return true; //child did it so return false;
		}
	}
	UIPos pos = GetParentPos();
	UIRect temp = UIRect(box.x + pos.x, box.y + pos.y);
		if ( temp.Intersects(x,y) ||  mouse_active ){
			
			
			mouse_active = true;
			gMouse = this;
			box.x = x - pos.x - box.w/2;
			
			
			if ( box.x < -box.w/2 ) {
				box.x = 0 - box.w/2;
			}
			if ( box.x > rect.w  - box.w/2 ) {
				box.x = rect.w - box.w/2;
			}
			
			
			return true; //we claimed the event so return true;
		}
	
	
	return false;
	
}

void UISlider::SetValue(float val) {
	box.x = val * rect.w;
}

float UISlider::GetValue() {
	return(box.x + box.w/2) / rect.w;
	
}

void UISlider::SetBounds(const UIRect &bounds) {
	rect.x = bounds.x;
	rect.y = bounds.y;
	rect.w = bounds.w;
	rect.h = 4;
	
	box.x = 0;
}

void UISlider::ClearMouseActive() {
	
	
	if(mouse_active) {
		gMouse = this;
	}
	UIWidget::ClearMouseActive();
	
}