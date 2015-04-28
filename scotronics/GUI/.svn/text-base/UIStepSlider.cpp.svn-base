/*
 *  UIStepSlider.cpp
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/11/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "UIStepSlider.h"
#include <math.h>

UIStepSlider::UIStepSlider(UIWidget *ptr, int minimum , int maximum ) {
	
	min = minimum;
	max = maximum;
	value = min;
	AddParent(ptr);
	BaseInit();
	SetBgColor(kNone, UIColor(39,39,39));
	
	box.x = 0;
	box.y = -18;
	box.w = 11;
	box.h = 11;
	oldx = box.x;
	
	SetBounds(UIRect(10,10,100,0));
	
	
}

UIStepSlider::~UIStepSlider() {
	
}

void UIStepSlider::Render() {
	
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
	
	//Render Notches
	glBegin(GL_LINES);
	for (int i =0; i < notches.size(); ++i) {
		glVertex2f(notches[i] +  pos.x, pos.y + 8);
		glVertex2f(notches[i] +  pos.x, pos.y - 4);
		
	}
	
	
	glEnd();
	
	//Render Box
	float boxx = box.x - box.w/2;
	float boxy = box.y;
	glColor3ub (159,159,159);
	glBegin(GL_QUADS);
	glVertex2f(pos.x + box.w + boxx	,  pos.y + boxy);
	glVertex2f(pos.x + boxx			,  pos.y + boxy);
	glVertex2f(pos.x + boxx			,  pos.y + box.h + boxy);
	glVertex2f(pos.x + boxx + box.w	,  pos.y + box.h + boxy);
	glEnd();
	
	glColor3ub (border_color[kNone].r,border_color[kNone].g,border_color[kNone].b);
	glBegin(GL_LINE_LOOP);
	glVertex2f(pos.x + box.w + boxx	,  pos.y + boxy);
	glVertex2f(pos.x + boxx			,  pos.y + boxy);
	glVertex2f(pos.x + boxx			,  pos.y + box.h + boxy);
	glVertex2f(pos.x + boxx + box.w	,  pos.y + box.h + boxy);
	glEnd();
	
	//triangle point
	glColor3ub (159,159,159);
	glBegin(GL_TRIANGLES);
	glVertex2f(pos.x + boxx			,  pos.y + box.h + boxy);
	glVertex2f(pos.x + boxx + box.w	,  pos.y + box.h + boxy);
	glVertex2f(pos.x + boxx + box.w/2	,  pos.y );
	glEnd();
	
	
	glColor3ub (border_color[kNone].r,border_color[kNone].g,border_color[kNone].b);
	glBegin(GL_LINE_LOOP);
	glVertex2f(pos.x + boxx			,  pos.y + box.h + boxy);
	glVertex2f(pos.x + boxx + box.w	,  pos.y + box.h + boxy);
	glVertex2f(pos.x + boxx + box.w/2	,  pos.y );
	glEnd();
	
}

bool UIStepSlider::MouseHandler(float x, float y) {
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
		box.x = x - pos.x;
		
		
		if ( box.x < box.w/2 ) {
			box.x = 0;
		}
		if ( box.x > rect.w  ) {
			box.x = rect.w ;
		}
		
		float temp = box.x / (float)step;
		temp += 0.5;
		value = (int)temp;
		value = values[(int)temp];
		SetBoxPos();
		
		return true; //we claimed the event so return true;
	}
	
	
	return false;
	
}

void UIStepSlider::SetValue(int val) {
	value = GetSafeValue(val);
}

int UIStepSlider::GetValue() {
	return value;
}

int UIStepSlider::GetSafeValue(int val) {
	
	if (val < min) {
		return min;
	}
	if (val > max) {
		return max;
	}
	return val;
}

void UIStepSlider::SetBounds(const UIRect &bounds) {
	rect.x = bounds.x;
	rect.y = bounds.y;
	rect.w = bounds.w;
	rect.h = 4;
	
	box.x = 0;
	
	CalculateNotches();
	
	
}

void UIStepSlider::CalculateNotches() {
	bool dynamic_width = false;
	int width = (int)rect.w;
	step = (int)width /(max - min); 
	if (width % step != 0) {
		dynamic_width = true;
	}
	
	int total = max - min + 1;
	notches.resize(total);
	values.resize(total);
	
	for (int i = 0; i < total; ++i) {
		notches[i] = (float)i * (float)step;
		values[i] = min + i;
	}
	
	if (dynamic_width) {
		rect.w = notches[total -1];
	}
	
	//set box x
	SetBoxPos();
}

int UIStepSlider::GetNotchIndex() {
	int size = values.size();
	for(int i = 0; i < size; ++i) {
		if (values[i] == value)
			return i;
	}
	
	return 0;
}

void UIStepSlider::ClearMouseActive() {
	if(mouse_active) {
		//UIPos pos = GetParentPos();
		//float distance = box.x - pos.x;
		float temp = box.x / (float)step;
		temp += 0.5;
		value = (int)temp;
		value = values[(int)temp];
		SetBoxPos();
	}
	UIWidget::ClearMouseActive();
	
}

void UIStepSlider::SetBoxPos() {
	box.x = notches[GetNotchIndex()];
}


