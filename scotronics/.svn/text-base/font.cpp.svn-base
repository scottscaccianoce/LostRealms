#include "font.h"


Font::Font()
{
	fonts[0] =GLUT_BITMAP_8_BY_13;
	fonts[1] =GLUT_BITMAP_9_BY_15;
	fonts[2] =GLUT_BITMAP_TIMES_ROMAN_10;
	fonts[3] =GLUT_BITMAP_TIMES_ROMAN_24;
	

}

void Font::DrawText(const std::string &str,float x, float y, float r, float g, float b, int pt){
	
	
	int len = str.length();
	glColor3f(r,g,b);
	glRasterPos2f(x,y);
	
	for(int i =0; i < len; ++i){
		
		glutBitmapCharacter(fonts[pt], str.c_str()[i]);	
	}
	

}

void Font::RenderText(const std::string &str,float x, float y, float r, float g, float b, int pt){
	
	
	int len = str.length();
	glColor3f(r,g,b);
	glRasterPos2f(x,y);
	
	for(int i =0; i < len; ++i){
		
		glutBitmapCharacter(fonts[pt], str.c_str()[i]);	
	}
	

}


void Font::DrawTextb( const std::string &str, float x, float y, GLubyte r, GLubyte g, GLubyte b ,int pt) {

	int len = str.length();
	glColor3ub (r,g,b);
	glRasterPos2f(x,y);
	
	for(int i =0; i < len; ++i){
		
		glutBitmapCharacter(fonts[pt], str.c_str()[i]);	
	}
}