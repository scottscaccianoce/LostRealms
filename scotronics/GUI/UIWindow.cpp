/*
 *  UIWindow.cpp
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/7/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "UIWindow.h"

UIWindow::UIWindow(UIWidget *pParent) {
	BaseInit();
	AddParent(pParent);
	caption = "Title";
	rect = UIRect(10,10,100,100);
	//caption_color = UIColor(159,159,159,255);
	display_title = true;
	dragable = true;
	caption_height = 22;
	mouse_active = false;
	display_border = true;
	
}

UIWindow::~UIWindow() {
	
}

UIPos UIWindow::GetLocalOffset() {
	UIPos ret = UIPos(0.0f, 0.0f);
	if (display_title) {
		ret.y = caption_height;
	}
	return ret;
}

void UIWindow::Render() {
	if (visible) {
		
		RenderBg();
		
		UIPos offset = GetParentPos();
		
		/*if (mouse_active) {
			offset.x -= mouse_offset.x;
			offset.y -= mouse_offset.y;
			
		}*/
		
		float x = offset.x;
		float y = offset.y;
		
		float fonty = y - 9 ;
		
		if (display_title) {
			
		
			//draw caption background
			glBegin(GL_QUADS);
			glColor3ub(caption_color[state].r,caption_color[state].g,caption_color[state].b);
			glVertex3f(x  + rect.w , y -caption_height,0.0f);
			glVertex3f(x +1, y - caption_height,0.0f);
			glVertex3f(x + 1, y ,0.0f);
			glVertex3f(x + rect.w , y  ,0.0f);
			glEnd();

		
			//Draw caption border
			glLineWidth(1.0f);
			
			glBegin(GL_LINES);
			glColor3ub(border_color[state].r,border_color[state].g,border_color[state].b);
			glVertex3f(x , y , 0.0f);
			glVertex3f(x + rect.w, y , 0.0f);
			glEnd();

			font.DrawTextb(caption.c_str(), x + 6, fonty, font_color[kNone].r, font_color[kNone].g, font_color[kNone].b, 0);
		
		}
		if(display_border) {
			RenderBorder();
		}
		
		RenderChildren();
		
	}
}


bool UIWindow:: MouseHandler(float x, float y) {
	if (visible == true) {
		unsigned int len = children.size();
		for (unsigned int i = 0 ; i < len; ++i) {
			bool result = children[i]->MouseHandler(x,y);
			if (result) {
				return true; //child did it so return false;
			}
		}
		
		
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

void UIWindow::ClearMouseActive() {
	if(mouse_active) {
		parent->BringToFront(this);
	}
	UIWidget::ClearMouseActive();
	
}

void UIWindow::SetTitle(const std::string &s) {
	caption = s;
}