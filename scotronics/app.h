
#ifndef __APP__
#define __APP__

#ifndef __APPLE__
#include "GL/glew.h"
#endif

#include "GL/glfw.h"


class App {
	
	
public:
	App(int width, int height,bool fullscreen);
	virtual ~App();
	
	void Main();
	
protected:
	
	virtual void Draw() = 0;
	virtual void Draw2D() = 0;
	virtual void Update() = 0;
	
	float numFrames;
	float timeElapsed ;
	
	
protected: 
	bool running;
	
	int gWidth;
	int gHeight;
	float dt;
	
private:
	

	
};
extern int viewWidth;
extern int viewHeight;




#endif