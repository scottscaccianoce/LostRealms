#include "app.h"

#ifdef __APPLE__

#include "GL/glfw.h"
#include <unistd.h>
#include <stdint.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <iostream>
#else
#include <windows.h>
//#include <unistd.h>
//#include <stdint.h>
//#include <mach/mach.h>
//#include <mach/mach_time.h>

typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#endif

int viewWidth = 0;
int viewHeight = 0;

#ifdef __APPLE__
double subtractTimes( uint64_t endTime, uint64_t startTime )
{
    uint64_t difference = endTime - startTime;
    static double conversion = 0.0;
    
    if( conversion == 0.0 )
    {
        mach_timebase_info_data_t info;
        kern_return_t err = mach_timebase_info( &info );
        
		//Convert the timebase into seconds
        if( err == 0  )
			conversion = 1e-9 * (double) info.numer / (double) info.denom;
    }
    
    return conversion * (double) difference;
}



#endif

App::App(int width, int height, bool fullscreen){

	

	if(!glfwInit()){
	}

	if(fullscreen){
		if(!glfwOpenWindow(width,height,0,0,0,0,8,0,GLFW_FULLSCREEN)){
			glfwTerminate();
		}
	}
	else {
		
		if(!glfwOpenWindow(width,height,0,0,0,0,8,0,GLFW_WINDOW)){
			glfwTerminate();
		}
	}
	#ifndef __APPLE__
	GLenum err = glewInit();
	#else
	
	#endif
	
	running = true;
	gWidth = width;
	gHeight = height;
	viewHeight = height;
	viewWidth =width;

}

App::~App(){
	glfwTerminate();
}

void App::Main(){
	
#ifdef __APPLE__
	//-  MAC-----------------------------------------------
	uint64_t prevTimeStamp;
	uint64_t currTimeStamp;
	
	mach_timebase_info_data_t info;
	mach_timebase_info(&info);

	prevTimeStamp = mach_absolute_time();
	
	//float FPSLimit = 70.0f;
	while(running){
		currTimeStamp = mach_absolute_time();
		
		dt = subtractTimes(currTimeStamp, prevTimeStamp);
		//if (dt < (1 / FPSLimit)) {
		//	sleep((1/(FPSLimit - 10)) - dt);
	//		continue;
//		}
		
		prevTimeStamp = currTimeStamp;
		
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam( GLFW_OPENED);
		Update();
		Draw();
		Draw2D();
		glfwSwapBuffers();
	}
	glfwTerminate();

#else
	//-  WINDOWS-----------------------------------------------
/*
	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f/ (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	while(running){
		__int64 currTimeStamp = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
		float dt = (currTimeStamp - prevTimeStamp) * cntsPerSec;
		
	
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam( GLFW_OPENED);
		Update();
		Draw();
		Draw2D();
		glfwSwapBuffers();

		prevTimeStamp = currTimeStamp;
	}
	glfwTerminate();
*/

__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f/ (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	while(running){
		__int64 currTimeStamp = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
		dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;
		

		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam( GLFW_OPENED);
		Update();
		Draw();
		Draw2D();
		glfwSwapBuffers();

		prevTimeStamp = currTimeStamp;
	}



/*
LARGE_INTEGER perfFreq;
LARGE_INTEGER currentTime;
LARGE_INTEGER prevTime;
 
// Query the performance frequency
QueryPerformanceFrequency(&perfFreq);
 
// Compute frequency ratio
float ratio = 1.0f / perfFreq.QuadPart;
 
// Start the timing at previous time for the first frame of the game
QueryPerformanceCounter(&prevTime);
while (running)
{
   QueryPerformanceCounter(&currentTime);
    dt = (currentTime.QuadPart - prevTime.QuadPart) * ratio;
 
  running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam( GLFW_OPENED);
		Update();
		Draw();
		Draw2D();
		glfwSwapBuffers();
    
   prevTime = currentTime;

}
*/
   glfwTerminate();
#endif
}
