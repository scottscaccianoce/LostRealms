/*
 *  UILabel.cpp
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/7/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "UILabel.h"

void UILabel::Init() {
	overflow = true;
	char_height = 13;
	char_width = 8;
	text = "default";
}

UILabel::UILabel() {
	BaseInit();
	Init();
}

UILabel::UILabel(UIWidget *ptr , const std::string &t) {
	
	BaseInit();
	AddParent(ptr);
	Init();
	text = t;
}
				 
UILabel::UILabel(UIWidget *ptr) {
	
	BaseInit();
	AddParent(ptr);
	Init();
}

UILabel::~UILabel() {}

void UILabel::Draw(float xo, float yo ) {
	if (visible) {
				
		UIPos pos  = GetParentPos();
		pos.x += xo;
		pos.y += yo;
		
		if(display_border) {
			RenderBorder();
		}
		
		if (overflow) {
			font.DrawTextb(text.c_str(), pos.x, pos.y + char_height, font_color[kNone].r, font_color[kNone].g, font_color[kNone].b, 0);
		} else {
			string fixed_text = text;
			int len = text.length();
			int fixed_len = (int)rect.w / char_width;
			if (len > fixed_len) {
				fixed_text = text.substr(0, fixed_len);
			}
			font.DrawTextb(fixed_text.c_str(), pos.x, pos.y + char_height, font_color[kNone].r, font_color[kNone].g, font_color[kNone].b, 0);
		}
				
	}
}


void UILabel::Render() {
	Draw();
}

void UILabel::Render(float xoffset, float yoffset){
	Draw(xoffset, yoffset);
}
bool UILabel::MouseHandler(float x, float y) {
	return false;
}

void UILabel::SetText(const std::string &s) {
	text = s;
}