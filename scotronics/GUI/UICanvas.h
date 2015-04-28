/*
 *  UICanvas.h
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/7/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */
#ifndef __UICANVAS__
#define __UICANVAS__

#include "UIWidget.h"
#include <vector>
using namespace std;

class UICanvas : public UIWidget {

public:
	UICanvas( UIRect);
	~UICanvas();
	
	// Abstract Functions  --------
	void Render();
	bool MouseHandler(float x, float y);
	//-----------------------
	
protected:
	
	
private:
	vector <UIWidget*> rendering_order;
};

#endif