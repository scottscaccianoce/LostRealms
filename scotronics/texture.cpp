#include "texture.h"

sTexture::sTexture(){
	read_image = false;


}

sTexture::~sTexture(){
	if(texture){
		glDeleteTextures( 1, &texture );
	}
	
	if(read_image) {
		glfwFreeImage( &image );
	}
}


void sTexture::loadTexture(const std::string & filename){

	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	
	
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	glfwLoadTexture2D(filename.c_str(),GLFW_BUILD_MIPMAPS_BIT);
	
}

void sTexture::bindTexture(){
	glBindTexture(GL_TEXTURE_2D,texture);


}

void sTexture::bindTextureWrap(){
	
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	
	
}

void sTexture::LoadTexData(const char* filename) {
	glGenTextures(1,&texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	read_image = true;
	int res = glfwReadImage( filename, &image , GLFW_BUILD_MIPMAPS_BIT );
	if (res==GL_FALSE) {
	exit;
	}
}

void sTexture::CopyDataToImage() {

	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	/*
	for(int i =0; i <= (1024 * 4 *1024)/2; i+=4) {
		image.Data[i] =255;
		image.Data[i+1] =0;
		image.Data[i+2] =0;
		image.Data[i+3] =0;
	}
	
	for(int i =(1024 * 1024 * 4) / 2; i < 1024 * 4 *1024; i+=4) {
		image.Data[i] =0;
		image.Data[i+1] =255;
		image.Data[i+2] =0;
		image.Data[i+3] =0;
	}
	*/
	
	glfwLoadTextureImage2D( &image, GLFW_BUILD_MIPMAPS_BIT);
	
}

unsigned char * sTexture::GetDataPointer() {
	return image.Data;
}


void sTexture::WriteTextureInfo(fstream *fh) {
	
	GLint width,height,internalFormat,texFormat;
	glBindTexture(GL_TEXTURE_2D, texture);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPONENTS, &internalFormat); // get internal format type of GL texture
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width); // get width of GL texture
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height); // get height of GL texture
	// GL_TEXTURE_COMPONENTS and GL_INTERNAL_FORMAT are the same.
	// just work with RGB8 and RGBA8
	GLint numBytes = 0;
	switch(internalFormat) // determine what type GL texture has...
	{
		case GL_RGB:
			numBytes = width * height * 3;
			texFormat = GL_RGB;
			break;
		case GL_RGBA:
			numBytes = width * height * 4;
			texFormat = GL_RGBA;
			break;
		case GL_RGB8:
			numBytes = width * height * 3;
			texFormat = GL_RGB;
			break;
		case GL_RGBA8:
			numBytes = width * height * 4;
			texFormat = GL_RGBA;
			break;
		default: // unsupported type (or you can put some code to support more formats if you need)
			break;
	}
	
	if(numBytes)
	{
		//unsigned char *pixels = new unsigned char[numBytes];//(unsigned char*)malloc(numBytes); // allocate image data into RAM
		unsigned char *pixels = (unsigned char*)malloc(numBytes);
		GLenum error = glGetError();
		glGetTexImage(GL_TEXTURE_2D, 0, texFormat, GL_UNSIGNED_BYTE, pixels);
		//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB , GL_UNSIGNED_BYTE, pixels);
		error = glGetError();
		if (error != GL_NO_ERROR) {
			cout << "Failed exporting image stuff" << endl;
			exit;
		}
		//write info
		(*fh).write((char *)&internalFormat, sizeof(GLint));
		(*fh).write((char *)&width, sizeof(GLint));
		(*fh).write((char *)&height, sizeof(GLint));
		(*fh).write((char *)&numBytes, sizeof(GLint));
		(*fh).write((char *)pixels, sizeof(unsigned char) * numBytes);
		
		
		
		free(pixels);
		//delete [] pixels; // when you don't need 'pixels' anymore clean a memory page to avoid memory leak.
		//pixels = NULL;
	}
	
}

void sTexture::ReadTextureInfo(fstream *fh) {
	
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS,GL_TRUE );
	read_image = true;
	
	GLint width,height,internalFormat;
	GLint numBytes = 0;
	
	(*fh).read((char *)&internalFormat, sizeof(GLint));
	(*fh).read((char *)&width, sizeof(GLint));
	(*fh).read((char *)&height, sizeof(GLint));
	(*fh).read((char *)&numBytes, sizeof(GLint));
	unsigned char *pixels = (unsigned char*)malloc(numBytes);
	
	(*fh).read((char *)pixels, sizeof(unsigned char) * numBytes);
	
	GLint textureFormat;
	if (internalFormat == GL_RGB8) {
		textureFormat = GL_RGB;
	}
	
	else if (internalFormat == GL_RGBA8) {
		textureFormat = GL_RGBA;
	}
	else {
		textureFormat = internalFormat;
	}


	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, pixels);
	
	
	
	delete []pixels;
}
