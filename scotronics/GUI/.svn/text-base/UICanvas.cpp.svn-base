/*
 *  UICanvas.cpp
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/7/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "UICanvas.h"

UICanvas::UICanvas(UIRect rect){
	SetBounds(rect);
	mouse_active = false;
}

UICanvas::~UICanvas() {

}

void UICanvas::Render() {
	unsigned int len = children.size();
	UIWidget *active_window = NULL;
	
	for (unsigned int i = 0; i < len; ++i) {
		if (children[i]->IsMouseActive()) {
			active_window = children[i];
		} else {
			children[i]->Render();
		}
	}
	
	if (active_window != NULL) {
		active_window->Render();
	}

}

bool UICanvas:: MouseHandler(float x, float y) {
	
	unsigned int len = children.size();
	
	//Have to do this twice because we have to find the active window
	for (unsigned int i = 0 ; i < len; ++i) {
		if (children[i]->IsMouseActive()) {
			bool result = children[i]->MouseHandler(x,y);
			if (result) {
				return true; //child did it so return false;
			}
		}
	}
	
	//So no active window we get one
	if (len > 0) {
		for ( int i = (len -1) ; i >= 0; --i) {
			bool result = children[i]->MouseHandler(x,y);
			if (result) {
				return true; //child did it so return false;
			}
		}
	}
	 
	
	/*unsigned int len = children.size();
	for (unsigned int i = 0 ; i < len; ++i) {
		bool result = children[i]->MouseHandler(x,y);
		if (result) {
			return true; //child did it so return false;
		}
	}
	 */
	
	return false;
}

/* void UICanvas::BringToFront(UIWidget *ptr) {
	
	rendering_order.clear();
	unsigned int len = children.size();
	
	for( unsigned int i =0; i < len; ++i) {
		if (children[i] != ptr) {
			rendering_order.push_back(children[i]);
		}
	}
	rendering_order.push_back(ptr);
	
		
}*/