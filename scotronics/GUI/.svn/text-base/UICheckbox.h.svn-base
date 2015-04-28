/*
 *  UICheckbox.h
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/9/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifndef __UICHECKBOX__
#define __UICHECKBOX__
#include "UIWidget.h"
#include "UILabel.h"
#include <string>


class UICheckbox : public UIWidget {
	
public:
	UICheckbox(UIWidget *ptr,const std::string &s);
	~UICheckbox();
	void Render();
	bool MouseHandler(float x, float y );
	bool GetChecked() const{ return checked;}
	void Toggle();
	void SetValue(bool value) {checked = value;}
	void SetFontColor(const UIState &, const UIColor &);
	void SetText(const std::string &s);
private:
	
	UILabel *label;
	bool checked;
	int border_length;
	int char_height;
	
	
};


#endif