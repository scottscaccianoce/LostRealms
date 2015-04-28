/*
 *  DayNight.h
 *  MapEditor
 *
 *  Created by Scott Scaccianoce on 6/29/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifndef __DAYNIGHT__
#define __DAYNIGHT__

#include "math3d.h"
#ifdef __APPLE__
#import <OpenGL/OpenGL.h>
#else

#include "GL/glew.h"
#endif
class DayNight  {
public:
	DayNight();
	~DayNight();
	void Update();
	void SetTimeOfDayAsPercentage(float percentage);
	
	M3DVector3f currentColor;
	float curTime;
	
private:
	
	
	float interpolate(float a, float b, float coefficient);
	void UpdateTimeAttributes();

	
	float timeValues[8];
	M3DVector3f *colorValues[8];
	
	M3DVector3f day;
	M3DVector3f set;
	M3DVector3f night;
	
	
};

extern DayNight gDayNight; 

#endif