/*
 *  UICheckbox.cpp
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/9/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "UICheckbox.h"

UICheckbox::UICheckbox(UIWidget *ptr,const std::string &s) {
	AddParent(ptr);
	label = new UILabel(this,s);
	BaseInit();	
	
	checked = true;
	//label.SetText("Checky");
	//label.AddParent(this);
	border_length = 11;
	char_height = 13;
	SetBorderColor(kNone, UIColor(0.0f,0.0f,0.0f));

}

UICheckbox::~UICheckbox() {}

void UICheckbox::SetText(const std::string &s) {
	label->SetText(s);
}

bool UICheckbox::MouseHandler(float x, float y) {
	UIPos pos = GetParentPos();
	UIRect temp = UIRect( pos.x, pos.y, border_length, border_length);
	if ( temp.Intersects(x,y) ||  mouse_active ){
		
		//if first click, register it as a click
		if (!mouse_active) {
			mouse_active = true;
			gMouse = this;
			Toggle();
			
		}
		return true;
	}
	
	
	return false;
	
	
}

void UICheckbox::Render() {
	if (visible) {
		
		UIPos pos = GetParentPos();
		
		int border_offset = (char_height -border_length) /2;
		pos.y += + border_offset + 2;
		pos.x += 1;
		
		
		glBegin(GL_LINE_LOOP);
		glColor3ub(border_color[kNone].r,border_color[kNone].g,border_color[kNone].b);
		glVertex2f(pos.x  + border_length + 1 , pos.y -1);
		glVertex2f(pos.x , pos.y -1);
		glVertex2f(pos.x , pos.y + border_length);
		glVertex2f(pos.x + border_length + 1 , pos.y + border_length);
		glEnd();
		
		int minimizer = 2;
		if (checked) {
			glBegin(GL_QUADS);
			glColor3ub(border_color[kNone].r,border_color[kNone].g,border_color[kNone].b);
			glVertex2f(pos.x  + border_length - minimizer , pos.y + minimizer );
			glVertex2f(pos.x + minimizer +1 , pos.y + minimizer);
			glVertex2f(pos.x + minimizer +1 , pos.y + border_length - minimizer -1);
			glVertex2f(pos.x + border_length - minimizer, pos.y + border_length -minimizer -1);
			glEnd();
		}
		
		label->Render(border_length + 6,0);
		
		
		
		
	}
}
void UICheckbox::SetFontColor(const UIState &s, const UIColor &c) {
	label->SetFontColor(s,c);
}
void UICheckbox::Toggle() {
	checked = !checked;

}