//
//  main.m
//  glfw
//
//  Created by Scott Scaccianoce on 9/12/11.
//  Copyright 2011 Scotronics. All rights reserved.
//
#ifndef __APPLE__
#include "gl/glew.h"
#endif

#include "game.h"

int main(int argc, char *argv[]) {
	Game *myGame;
	
#ifdef __APPLE__
	myGame = new Game(1440,852,false);
#else
	myGame = new Game(1440,852,false);
#endif
	
	glfwEnable(GLFW_MOUSE_CURSOR);
	
	myGame->Main();
	delete myGame;
	
}
