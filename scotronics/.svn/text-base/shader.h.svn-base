/*
 *  shader.h
 *  game
 *
 *  Created by Scott Scaccianoce on 5/29/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifndef SHADER
#define SHADER

#ifdef __APPLE__
#import    <OpenGL/OpenGL.h> 
#else
#include "gl/glew.h"
#endif

#include <fstream>
#include <iostream>
#include <map>
#include <string>
using namespace std;

class Shader {


public:
	bool Load(const char* vertex_filename, const char* fragment_filename);
	Shader();
	~Shader();
	void Begin();
	void End();
	
	GLint GetUniformLocation(const char *varname);
	bool SetUniform1i( const char *varname, int a, GLint index = -1);
	
	bool SetUniform1f(const char *varname, float a, GLint index = -1);
	bool SetUniform3f(const char *varname, float a, float b, float c, GLint index = -1);
	bool SetUniform2f(const char *varname, float a, float b, GLint index = -1);
	
	bool SetUniform4f(const char *varname, float a, float b, float c, float d, GLint index = -1);
	
	
	bool SetUniform3fv(const char *varname, GLsizei count, GLfloat *value, GLint index = -1);
	
	
	bool SetUniformMatrix4fv(GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index = -1); //!< Specify values of uniform 4x4 matrix. \param varname The name of the uniform variable.
	
	GLuint GetProgram() { return program;}
private:

			 
	unsigned long getFileLength(ifstream& );
	int LoadSource(char* , GLchar**);
	bool unloadshader(GLchar** );
	
	GLuint vertexShader, fragmentShader;
	GLuint program;
	GLubyte*   ShaderSource;
	std::map<std::string, GLint> uniform_locations;
	
	char* TextFileRead(const char *filename);

};

#endif