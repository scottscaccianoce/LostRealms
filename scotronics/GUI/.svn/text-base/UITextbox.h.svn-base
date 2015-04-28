/*
 *  UITextbox.h
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/12/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifndef __UITEXTBOX__
#define __UITEXTBOX__

#include "UIWidget.h"
#include <string>
#include "font.h"

class UITextbox : public UIWidget {
	
public:
	UITextbox(UIWidget *ptr);
	~UITextbox();
	bool MouseHandler(float,float);
	void Render();
	void SetBounds(const UIRect &bounds);
	
	void SetText(const std::string &s);
	void AppendText(const std::string &s);
	void Backspace();
	void Clear();
	float GetTextAsFloat();
	string GetText();
	
private:
	string text;
	int char_width;
	int char_height;
	float height;
	Font font;
	float padding;
	
};

#endif