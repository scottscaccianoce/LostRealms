/*
 *  shader.cpp
 *  game
 *
 *  Created by Scott Scaccianoce on 5/29/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "shader.h"

Shader::Shader() {}

Shader::~Shader() {
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(program);
	
}
	
unsigned long Shader::getFileLength(ifstream& file)
{
    if(!file.good()) return 0;
    
   // unsigned long pos=file.tellg();
    file.seekg(0,ios::end);
    unsigned long len = file.tellg();
    file.seekg(ios::beg);
    
    return len;
}
char* Shader::TextFileRead(const char *path)
{
	FILE *fd;
	long len,
	r;
	char *str;
	
	if (!(fd = fopen(path, "r")))
	{
		fprintf(stderr, "Can't open file '%s' for reading\n", path);
		return NULL;
	}
	
	fseek(fd, 0, SEEK_END);
	len = ftell(fd);
	
	printf("File '%s' is %ld long\n", path, len);
	
	fseek(fd, 0, SEEK_SET);
	
	if (!(str = (char*)malloc(len * sizeof(char))))
	{
		fprintf(stderr, "Can't malloc space for '%s'\n", path);
		return NULL;
	}
	
	r = fread(str, sizeof(char), len, fd);
	
	str[r - 1] = '\0'; /* Shader sources have to term with null */
	
	fclose(fd);
	
	return str;	
	
}



int Shader::LoadSource(char* filename, GLchar** ShaderSource)
{
	
	string line;
	ifstream infile;
	infile.open("simple.vs");
	getline(infile,line);
	getline(infile,line);
	getline(infile,line);
	
	
	
	
	
	ifstream file;
	file.open(filename, ios::in); // opens as ASCII!
	if(!file) return -1;
    
	unsigned long len = getFileLength(file);
    
	if (len==0) return -2;   // Error: Empty File 
    
	*ShaderSource = (GLchar*) new char [len + 1];
	if (*ShaderSource == 0) return -3;   // can't reserve memory
	
    // len isn't always strlen cause some characters are stripped in ascii read...
    // it is important to 0-terminate the real length later, len is just max possible value... 
	*ShaderSource[len] = 0; 
	
	unsigned int i=0;
	while (file.good())
	{
		*ShaderSource[i] = file.get();       // get character from file.
		if (!file.eof())
			i++;
	}
    
	*ShaderSource[i] = 0;  // 0-terminate it at the correct position
    
	file.close();
	
	return 0; // No Error
}


bool Shader::unloadshader(GLchar** ShaderSource)
{
	if (*ShaderSource != 0)
		delete[] *ShaderSource;
	*ShaderSource = 0;
	return true;
}

bool Shader::Load(const char *vertex_filename, const char *fragment_filename) {
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	
	// LOAD SOURCE
	char*  vsSource;
	vsSource = TextFileRead(vertex_filename);
	glShaderSource(vertexShader,1,(const GLchar **)&vsSource, NULL);
	
	
	char* fsSource;
	fsSource = TextFileRead(fragment_filename);
	glShaderSource(fragmentShader,1,(const GLchar **)&fsSource, NULL);
	
	
	//COMPILE
	GLint vertcompiled = 0;
	GLint fragcompiled = 0;
	
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertcompiled);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragcompiled);
	free(vsSource);
	free(fsSource);
	
	//if (!vertcompiled || !fragcompiled) {
		
	int maxLength=1;
	int infologLength = 0;
	if (!vertcompiled ) {
		
		glGetShaderiv(vertexShader,GL_INFO_LOG_LENGTH,&maxLength);
		//char infoLog[maxLength];
		char *infoLog = new char[maxLength];
		glGetShaderInfoLog(vertexShader, maxLength, &infologLength, infoLog);
		delete infoLog;
		return false;
		
	} else if (!fragcompiled) {
		
		glGetShaderiv(fragmentShader,GL_INFO_LOG_LENGTH,&maxLength);
		char *infoLog = new char[maxLength];
		glGetShaderInfoLog(fragmentShader, maxLength, &infologLength, infoLog);
		delete infoLog;
		return false;
	}
	

	
	//Link
	program = glCreateProgram();
	
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	
	GLint linked;
	glGetProgramiv(program,GL_LINK_STATUS, &linked);
	if (!GL_TRUE) {
		return false;
	}
	
	
	return true;
}

void Shader::Begin() {
	glUseProgram(program);
}
void Shader::End() {
	glUseProgram(0);
}

GLint Shader::GetUniformLocation( const char *varname) {
	GLint loc;
	loc = glGetUniformLocation(program, varname);
	return loc;
	
	
}

bool Shader::SetUniform1f( const char *varname, float a, GLint index) {
	
	GLuint loc;
	if (varname) {
		loc = GetUniformLocation(varname);
	} else {
		loc = index;
	} 
	
	if (loc == -1) {
		return false;
	}
	
	glUniform1f(loc, a);
	return true;
}

bool Shader::SetUniform1i( const char *varname, int a, GLint index) {
	
	GLuint loc;
	if (varname) {
		loc = GetUniformLocation(varname);
	} else {
		loc = index;
	} 
	
	if (loc == -1) {
		return false;
	}
	
	glUniform1i(loc, a);
	return true;
}

	
bool Shader::SetUniform2f(const char *varname, float a, float b, GLint index ) {
	GLuint loc;
	if (varname) {
		loc = GetUniformLocation(varname);
	} else {
		loc = index;
	} 
	
	if (loc == -1) {
		return false;
	}
	
	glUniform2f(loc, a, b);
	return true;
}
bool Shader::SetUniform3f( const char *varname, float a, float b, float c, GLint index) {
	
	GLuint loc;
	if (varname) {
		loc = GetUniformLocation(varname);
	} else {
		loc = index;
	} 
	
	if (loc == -1) {
		return false;
	}
	
	glUniform3f(loc, a, b , c);
	return true;
}

bool Shader::SetUniform4f( const char *varname, float a, float b, float c, float d, GLint index) {
	
	GLuint loc;
	if (varname) {
		loc = GetUniformLocation(varname);
	} else {
		loc = index;
	} 
	
	if (loc == -1) {
		return false;
	}
	
	glUniform4f(loc, a, b , c, d);
	return true;
}

bool Shader::SetUniform3fv(const char *varname, GLsizei count, GLfloat *value, GLint index) {
	GLuint loc;
	if (varname) {
		loc = GetUniformLocation(varname);
	} else {
		loc = index;
	} 
	
	if (loc == -1) {
		return false;
	}
	
	glUniform3fv(loc, count, value);
	return true;
}
bool Shader::SetUniformMatrix4fv(GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index) {
	GLint loc;
    if (varname)
		loc = GetUniformLocation(varname);
    else
		loc = index;
	
    if (loc==-1) 
		return false;  // can't find variable / invalid index
    
    glUniformMatrix4fv(loc, count, transpose, value);
	
    return true;
	
}
