/*
 *  UIStepSlider.h
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/11/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */


#ifndef __UISTEPSLIDER__
#define __UISTEPSLIDER__
#include "UIWidget.h"
#include <vector>


class UIStepSlider : public UIWidget {
	
public:
	UIStepSlider(UIWidget *ptr, int min, int max);
	~UIStepSlider();
	void Render();
	bool MouseHandler(float, float);
	
	void SetValue(int value);
	void SetBounds(const UIRect &bounds);
	int GetValue();
	void ClearMouseActive();
	
private:
	void SetBoxPos();
	int GetNotchIndex();
	void CalculateNotches();
	int GetSafeValue(int);
	int max,min,value;
	UIRect box;
	int oldx;
	
	std::vector<float>notches;
	std::vector<float>values;
	
	int step;
	
	
};


#endif