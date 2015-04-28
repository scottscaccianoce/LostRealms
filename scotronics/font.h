
#ifndef __FONT__
#define __FONT__
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#import <string>
#else
#include "gl/glew.h"
#include "glut.h"
#include <string>
#endif


class Font {
	
	
public:
	void DrawText( const std::string &str, float x, float y, float r, float g, float b ,int pt = 0);
	void RenderText( const std::string &str, float x, float y, float r, float g, float b ,int pt = 0);
	
	void DrawTextb( const std::string &str, float x, float y, GLubyte r, GLubyte g, GLubyte b ,int pt = 0);
	Font();
	
private:
	void *fonts[4];
	
	
};




#endif