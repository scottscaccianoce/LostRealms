/*
 *  UIButton.h
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/8/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */


#ifndef __UIBUTTON__
#define __UIBUTTON__

#include "UIWidget.h"
#include <string>
#include "font.h"


using namespace std;

class UIButton : public UIWidget {
	
public:
	
	UIButton(UIWidget *ptr );
	UIButton(UIWidget *ptr , const std::string &);
	~UIButton();
	void Render();
	bool MouseHandler(float x, float y );
	void SetText(const std::string &);
	
	//Callback function
	//UIButton(UIWidget *ptr , const std::string &, void (*f)());
	//void (*func)(void);
private:
	void Init();
	string text;
	Font font;
	int char_height;
	float char_width;
};




#endif