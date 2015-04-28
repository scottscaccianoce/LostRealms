/*
 *  UITextbox.cpp
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/12/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "UITextbox.h"

UITextbox::UITextbox(UIWidget *ptr) {
	AddParent(ptr);
	BaseInit();
	height = 19;
	char_height = 13;
	char_width = 8;
	padding = 4.0f;
	SetBounds(UIRect(10,10,100,0));
	SetBgColor(kNone, UIColor(180,180,180,255));

}

UITextbox::~UITextbox() {
}

bool UITextbox::MouseHandler(float x,float y) {
	UIPos pos = GetParentPos();
	UIRect temp = UIRect( pos.x, pos.y, rect.w, rect.h);
	if ( temp.Intersects(x,y) ||  mouse_active ){
		
		//if first click, register it as a click
		if (!mouse_active) {
			mouse_active = true;
			gCarot = this;
		}
		return true;
	}
	
	
	return false;
}

void UITextbox::Render() {
	
	if (!visible) {
		return;
	}
	
	UIPos pos = GetParentPos();
	float x =  pos.x;
	float y =   pos.y;
	
	RenderBg();
	
	if(gCarot == this) {
		glLineWidth(2.0f);
		//glColor3ub(36,36,36);
		glColor3ub(border_color[state].r,border_color[state].g,border_color[state].b);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x + rect.w, y);
		glVertex2f(x, y);
		glVertex2f(x, y + rect.h);
		glVertex2f(x + rect.w, y + rect.h );
		glEnd();
	}
	RenderBorder();
	
	
	//Render Text
	string fixed_text = text;
	int len = text.length();
	int fixed_len = (int)(rect.w - padding*2) / char_width;
	if (len > fixed_len) {
		int start = len - fixed_len;
		fixed_text = text.substr(start, fixed_len);
	}
	font.DrawTextb(fixed_text.c_str(), pos.x + padding, pos.y + char_height, font_color[kNone].r, font_color[kNone].g, font_color[kNone].b, 0);
	
}
void UITextbox::SetBounds(const UIRect &bounds) {
	rect = bounds;
	rect.h = height;
}

void UITextbox::SetText(const std::string &s) {
	text = s;
}
void UITextbox::AppendText(const std::string &s) {
	text += s;
}
void UITextbox::Backspace() {
	int len = text.length() -1;
	text = text.substr(0,len);
}

float UITextbox::GetTextAsFloat() {
	return atof(text.c_str());
}

string UITextbox::GetText() {
	return text;
}

void UITextbox::Clear() {
	text = "";
}
