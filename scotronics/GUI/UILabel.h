/*
 *  UILabel.h
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/7/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifndef __UILABEL__
#define __UILABEL__

#include "UIWidget.h"
#include <string>
#include "font.h"

using namespace std;

class UILabel : public UIWidget {
	
public:
	UILabel();
	UILabel(UIWidget *ptr );
	UILabel(UIWidget *ptr , const std::string &);
	
	~UILabel();
	
	void Render();
	void Render(float , float );
	void Draw(float xo = 0.0f, float yo =0.0f);
	bool MouseHandler(float x, float y );
	void SetText(const std::string &s);
	void SetOverflow(bool value){ overflow = value;}
	std::string GetText(){return text;}
	
protected:
	
	string text;
	bool overflow;
	Font font;
	int char_height;
	float char_width;
	
private:
	void Init();
};




#endif