/*
 *  game.h
 *  triangle
 *
 *  Created by Scott Scaccianoce on 3/4/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifdef __APPLE__


#include "GL/glfw.h"
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#import <GLUT/GLUT.h>
#else

#include "gl/glew.h"
#include "GL/GLFW.h"

//#include <GL/glu.h>

#endif


#include "app.h"
#include "font.h"
#include "UIWidget.h";
#include "UICanvas.h";
#include "UIWindow.h";
#include "UILabel.h";
#include "UIButton.h";
#include "UICheckbox.h";
#include "UIListbox.h";
#include "UISlider.h";
#include "UIStepSlider.h";
#include "UITextbox.h";

#include "texture.h"
#include "Camera.h"
#include "terrain.h"

#include "skyplane.h"
#include "quadterrain.h"
#include "Frustum.h"
#include "util.h"
#include "spMesh.h"
#include "SPEntity.h"



#include <iostream>
#include <fstream>

#ifdef __APPLE__
#include <dirent.h>
#else
//#define GLEW_STATIC
#endif

#include <stdio.h>
#include <string>

enum editor_mode{kSCULPT=0,kPAINT,kOBJECT,kQUEST};

class Game : public App {

public:
	Game(int w, int h, bool fullscreen);
	~Game();
	void Init();
	void Draw();
	void Draw2D();
	void Update();
	void HandleInput(float dt);
			
	void ExportMesh();
private:
	bool mouseDownLock;
	editor_mode mode;
	float oldTime;
	Terrain *land;
	
	// UI
	void CreateUI();
	
	UICanvas *canvas;
	
	//terrain form
	UIWindow *mainForm;
	UIWindow *dialog;
	
	UIListbox *lstFiles;
	UIListbox *lstFrames;
	
	UIButton *btnLoadFile;
	UIButton *btnPlay;
	UIButton *btnStop;
	UIButton *btnExport;
	
	UITextbox *txtSpeed;
	UITextbox *txtSteps;
	UITextbox *txtFile;
	UITextbox *txtScale;
	
	UICheckbox *chkRepeat;
	UIButton	*btnOk;
	
	
	
	UIWindow *importForm;
	UITextbox *txtImport;
	UIButton *btnImport;
	SPMesh LoadedMesh;
	SPEntity *LoadedEntity;
	bool imported;
	
	Font gFont;
	float mFPS;
	int old_mouse_x;
	int old_mouse_y;
	
	vector<string> files;
	
	SPMesh mesh;
	SPEntity *object;

};


