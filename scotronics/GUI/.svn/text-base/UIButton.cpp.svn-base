/*
 *  UIButton.cpp
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/8/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "UIButton.h"


UIButton::UIButton(UIWidget *ptr ) {
	AddParent(ptr);
	BaseInit();
	Init();
	
}
UIButton::UIButton(UIWidget *ptr , const std::string &t){
	AddParent(ptr);
	BaseInit();
	Init();	
	text = t;
	
}

UIButton::~UIButton(){}

void UIButton::Render() {
	if (visible) {
	
		RenderBg();
		
		UIPos pos = GetParentPos();
		float x =  pos.x;
		float y =   pos.y;
		
		glLineWidth(2.0f);
		
		glColor3ub(36,36,36);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x + rect.w, y);
		glVertex2f(x, y);
		glVertex2f(x, y + rect.h);
		glVertex2f(x + rect.w, y + rect.h );
		
		glEnd();
		
		
		
		RenderBorder();
		
		
		//Render String centered
		string fixed_text = text;
		int len = text.length();

		int fixed_len = (int)rect.w / char_width;
		if (len > fixed_len) {
			fixed_text = text.substr(0, fixed_len);
		}
		int text_width = fixed_text.length() * char_width;
		int centerx = (rect.w  - (text_width )) / 2;
		int centery = (rect.h - (char_height) ) /2;
		font.DrawTextb(fixed_text.c_str(), centerx + pos.x, pos.y + (char_height - 2) + centery, font_color[kNone].r, font_color[kNone].g, font_color[kNone].b, 0);
	}
}


bool UIButton::MouseHandler(float x, float y ) {
	
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

void UIButton::SetText(const std::string &t) {
	text = t;
}

void UIButton::Init() {

	char_height = 13;
	char_width = 8;
	text = "default";
	SetBgColor(kNone, UIColor(159,159,159,255));

}