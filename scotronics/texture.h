/*
 
 typedef struct {

	int Width, Height; // Image dimensions
	
	int Format; // OpenGL pixel format
	
	int BytesPerPixel; // Number of bytes per pixel
	
	unsigned char *Data; // Pointer to pixel data
 
 } GLFWimage;
 
 
 
 
*/

#ifndef __TEXTURE__
#define __TEXTURE__

#ifdef __APPLE__
#import    <OpenGL/OpenGL.h> 
#include "GL/glfw.h"
#endif


#include <iostream>
#ifdef __APPLE__
#include "GL/glfw.h"
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#else
#include "gl/glew.h"
#include "GL/glfw.h"
#endif

#include <fstream>
using namespace std;

class sTexture {
	
	
public:
	sTexture();
	~sTexture();
	void loadTexture(const std::string & filename);
	
	
	void LoadTexData(const char* filename);
	void CopyDataToImage();
	
	void bindTexture();
	void bindTextureWrap();
	unsigned char* GetDataPointer();
	
	GLFWimage image;
	
	
	//Model texture get/set functions
	void WriteTextureInfo(fstream *fh);
	void ReadTextureInfo(fstream *fh);
protected:
	
	
private:
	bool read_image;
	
	GLuint texture;
	
	
};



#endif