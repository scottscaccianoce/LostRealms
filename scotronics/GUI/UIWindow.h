/*
 *  UIWindow.h
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/7/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */
#ifndef __UIWINDOW__
#define __UIWINDOW__
#include "UIWidget.h"
#include <string>
#include "font.h"

class UIWindow : public UIWidget {
	
public:
	UIWindow(UIWidget *pParent);
	~UIWindow();
	
	// Abstract Functions  --------
	void Render();
	bool MouseHandler(float x, float y);
	//------------------------------
	void ClearMouseActive();
	//UIPos GetParentPos();
	UIPos GetLocalOffset();
	void SetTitle(const std::string &s);
	void DisplayTitle(bool value) { display_title = value;}
protected:
	
	
private:
	bool display_title;
	//UIColor caption_color;
	float caption_height;
	std::string caption;
	Font font;
	
	UIPos mouse_offset;
};



#endif