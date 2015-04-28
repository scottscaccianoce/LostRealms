/*
 *  game.h
 *  triangle
 *
 *  Created by Scott Scaccianoce on 3/4/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#ifdef __APPLE__

//#include "GLTools.h"
//#include "GLShaderManager.h"

#include "GL/glfw.h"
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#import <GLUT/GLUT.h>


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
#include "spMesh.h"
#include "SPEntity.h"
#include "skyplane.h"
#include "quadterrain.h"
#include "Frustum.h"
#include "Water.h"
#include "util.h"
#include "Database.h"
#include "Sprite.h"

enum editor_mode{kSCULPT=0,kPAINT,kOBJECT,kQUEST,kENEMY};

class Game : public App {

public:
	Game(int w, int h, bool fullscreen);
	~Game();
	void Init();
	void Draw();
	void Draw2D();
	void Update();
	void HandleInput(float dt);
	
	void RandomRotateEntity(SPEntity *obj);
	void DeleteSelectedEntity();
private:
	bool mouseDownLock;
	editor_mode mode;
	float oldTime;
	
	// UI
	void CreateUI();
	UIWidget *canvas;
	UIButton *menuSculpt;
	UIButton *menuPaint;
	UIButton *menuObject;
	UIButton *menuQuest;
	UIButton *menuMap;
	UIButton *menuEnemies;
	
	
	// Map form
	UIWindow *mapForm;
	UISlider *timeValue;
	UITextbox *txtSaveFilename;
	UIButton *btnSave;
	UITextbox *txtLoadFilename;
	UIButton *btnLoad;
	
	//terrain form
	UIWindow *terrainForm;
	UISlider *brushRadius;
	UIButton *raiseVerts;
	UIButton *lowerVerts;
	UIButton *FlatVerts;
	UIButton *smoothVerts;
	UILabel *vertMode;
	UICheckbox *renderWireframe;
	
	//Paint form
	UIWindow *paintForm;
	UIStepSlider	*texBrushsize;
	UIListbox *texturesList;
	UIButton *paintTex;
	UIButton *filterTex;
	UILabel  *texMode;
	
	//Object Form
	UIWindow *objectForm;
	UIListbox *meshList;
	UISlider *rotationSlider;
	UICheckbox *randomRotation;
    UITextbox *heightValue;
	
	
	//ENEMY Form
	UIWindow *enemyForm;
	UIListbox *enemyList;
	
	UITextbox *enemyName;
	UITextbox *enemyFilename;
	UITextbox *enemyAttackFilename;
	
	UITextbox *enemyHp;
	UITextbox *enemyLevel;
	UITextbox *enemyAttack;
	UICheckbox *enemyHostile;
	UITextbox *enemyBAFRadius;
	UITextbox *enemyRespawn;
	UITextbox *enemyExp;
	
	
	UIButton *enemySaveStats;
	void LoadEnemyStats();
	void SaveEnemyStats();
	
	
	
	//Terrain *land;
	QuadTerrain *land;
	
	Font gFont;
	float mFPS;
	int old_mouse_x;
	int old_mouse_y;
	
	vector<SPMesh*>meshes;
	vector<SPMesh*>enemyMeshes;
	
	vector<SPEntity*> objects;
	vector<SPEntity*> enemyObjects;
	
	SkyPlane *sky;
	Water *water;
	SPEntity *selectedEntity;
	
	Database *dbGame;
	
	//Compass stuff
	sTexture tCompass;
	Sprite	sCompass;
	float compassRot;
	
	
	Frustum objectFrustum;

};


