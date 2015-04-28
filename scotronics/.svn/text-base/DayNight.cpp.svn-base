/*
 *  DayNight.cpp
 *  MapEditor
 *
 *  Created by Scott Scaccianoce on 6/29/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "DayNight.h"

DayNight::DayNight() {
	
	day[0] = 0.519f;
	day[1] =  0.679f;
	day[2] = 0.890f;
		
	set[0] =0.89f;
	set[1] = 0.59f;
	set[2] = 0.29f;
		
	night[0] = 0.04f;
	night[1] = 0.05f;
	night[2] = 0.25f;
	
	timeValues[0] = 0;
	timeValues[1] = 5;
	timeValues[2] = 7;
	timeValues[3] = 9;
	timeValues[4] = 17;
	timeValues[5] = 19;
	timeValues[6] = 21;
	timeValues[7] = 24;
	
	colorValues[0] = &night;
	colorValues[1] = &night;
	colorValues[2] = &set;
	colorValues[3] = &day;
	colorValues[4] = &day;
	colorValues[5] = &set;
	colorValues[6] = &night;
	colorValues[7] = &night;
	
	
}

DayNight::~DayNight() {
	
}

void DayNight::Update() {
	
}

float DayNight::interpolate(float a, float b, float coefficient)
{
	return a + coefficient * (b - a);
}


void DayNight::SetTimeOfDayAsPercentage(float percentage) {
	
	
	float timeValue = 23.9f * percentage;
	
	curTime = timeValue;
	if (curTime >= 24.0f) {
		curTime = 0;
	}
	UpdateTimeAttributes();
}

void DayNight::UpdateTimeAttributes() {

	M3DVector3f *color1 = NULL;
	M3DVector3f *color2 = NULL;
	float maxTime;
	float minTime;
	
	for (int i = 0; i < 8; ++i) {
		if (curTime < timeValues[i]) {
			
			color1 = colorValues[i-1];
			color2 = colorValues[i];
			minTime = timeValues[i-1];
			maxTime = timeValues[i];
			
			break;
		}
	}
	
	if (color1 == NULL || color2 == NULL) {
		return;
	}
	
	//Getting coeffiencent
	float coeffiecient = (curTime - minTime) / (maxTime - minTime);
	
	
	for (int i =0; i < 3; ++i) {
		float c1 = (*color1)[i];
		float c2 = (*color2)[i];
		
		currentColor[i] = interpolate(c1, c2, coeffiecient);
	}
	
	glFogfv(GL_FOG_COLOR, currentColor);

}

DayNight gDayNight;