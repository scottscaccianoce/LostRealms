/*
 *  game.cpp
 *  triangle
 *
 *  Created by Scott Scaccianoce on 3/4/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#define MAX_RADIUS 150;

#include "game.h"
int PlayerId; 
NetworkHost *nHost;
std::vector<Player> gPlayers;
std::vector<SPMesh*> meshes;
std::vector<SPMesh*> enemy_meshes;
std::vector<SPMesh*> enemy_attack_meshes;
std::vector<sTexture*> item_textures;
vector<Enemy*> enemy_objects;
vector<SPEntity*> objects;
Item equipment[9];
Spell *castingSpell;
UIListbox *combatLog;
Enemy *TargetEnemy;
int TargetIndex;
Player *TargetAlly;
int AllyIndex;

int currentCastingPlayerIndex;
int currentCastingEnemyIndex;


Player players[4];
bool connected[4];
std::vector<Emitter*> gEmitters;
std::vector<int> GroupedAllies;

std::map <int, int> gLevels;

UIColor RED = UIColor(255,0,0);
UIColor TEAL = UIColor(0,200,200);
UIColor YELLOW = UIColor(210,225,48);
UIColor GREEN = UIColor(0,225,20);


Game::Game(int w, int h, bool fullscreen) : App(w,h,fullscreen) {
	
//---- Load Textures used in UI
	tSettings.loadTexture("settings.tga");
	tEquip.loadTexture("equip.tga");
	tSpells.loadTexture("spells.tga");
	tInventory.loadTexture("bag.tga");
	tMap.loadTexture("map.tga");
	tQuests.loadTexture("quests.tga");
	tGroup.loadTexture("group.tga");
	tStats.loadTexture("stats.tga");
	tTrinket.loadTexture("trinket.tga");
	
	tCompass.loadTexture("totalcompass.tga");
	tMainWeapon[0].loadTexture("sword.tga");
	tMainWeapon[1].loadTexture("sword.tga");
	tMainWeapon[2].loadTexture("sword.tga");
	tMainWeapon[3].loadTexture("sword.tga");
	tOffWeapon[0].loadTexture("beer.tga");
	tOffWeapon[1].loadTexture("beer.tga");
	tOffWeapon[2].loadTexture("beer.tga");
	tOffWeapon[3].loadTexture("beer.tga");
	tClasses[0].loadTexture("paladin.tga");
	tClasses[1].loadTexture("wizard.tga");
	tClasses[2].loadTexture("bard.tga");
	tClasses[3].loadTexture("druid.tga");	
	tEnemyIcon.loadTexture("enemy.tga");

	
//---- Initalize Databases
	dbPlayers = new Database("players.sqlite3");
	dbGame = new Database("game.sqlite3");

//---- Load Meshes form Database
	SPMesh *model =NULL;
	if (dbGame->Query("SELECT mesh_id, filename from Meshes ORDER BY mesh_id")) {
		for (int i = 0; i < dbGame->total_rows; ++i) {
			model = new SPMesh();
			model->db_id = atoi(dbGame->results[i][0].c_str());
			model->ImportMesh(dbGame->results[i][1], "");
			meshes.push_back(model);
		}
	}
	
//---- Load Item Textures
	sTexture *itemTexture =NULL;
	if (dbGame->Query("SELECT filename from ItemsIcon ORDER BY item_icon_id")) {
		for (int i = 0; i < dbGame->total_rows; ++i) {
			itemTexture = new sTexture();
			itemTexture->loadTexture(dbGame->GetValue(i, 0));
			item_textures.push_back(itemTexture);
		}
	}

	
//---- Load Enemy Meshes from Database
	if (dbGame->Query("SELECT mesh_id, filename,attack_filename from Enemies ORDER BY mesh_id")) {
		for (int i = 0; i < dbGame->total_rows; ++i) {
			model = new SPMesh();
			model->db_id = atoi(dbGame->results[i][0].c_str());
			model->ImportMesh(dbGame->results[i][1], "");
			enemy_meshes.push_back(model);
			
			//Load the attack mesh as well
			if (dbGame->results[i][2] == "") {
				//if no attack animation is givin
				enemy_attack_meshes.push_back(model);
			} else {
				model = new SPMesh();
				model->db_id = atoi(dbGame->results[i][0].c_str());
				model->ImportMesh(dbGame->results[i][2], "");
				enemy_attack_meshes.push_back(model);
				
			}
		}
	}

	
//---- Load Spells (textures and data
	if (dbGame->Query("SELECT spell_id, class_id, name, school, filename, cast_time,recast_time,range,cost,attack_value,spellType,duration, tick, forever, passive from Spells ORDER BY class_id, school_id, spell_id")) {
		if (dbGame->total_rows == 36) {
			for (int i = 0; i < dbGame->total_rows; ++i) {
				spells[i].name = dbGame->results[i][2];
				spells[i].class_id = atoi(dbGame->results[i][2].c_str());
				spells[i].db_id = atoi(dbGame->results[i][0].c_str());
				spells[i].LoadTexture(dbGame->results[i][4]);
				spells[i].school = dbGame->results[i][3];
				spells[i].cast_time = dbGame->GetFloat(i, 5);
				spells[i].recast = dbGame->GetFloat(i, 6);
				spells[i].recast_elapased = spells[i].recast;
				spells[i].range = dbGame->GetFloat(i, 7);
				spells[i].cost = dbGame->GetFloat(i, 8);
				spells[i].base_dmg = dbGame->GetFloat(i, 9);
				spells[i].spellType = dbGame->GetFloat(i, 10);	
				spells[i].duration = dbGame->GetInteger(i, 11);	
				spells[i].tick = dbGame->GetInteger(i, 12);	
				spells[i].forever = (dbGame->GetInteger(i, 13) == 1);	
				spells[i].passive = (dbGame->GetInteger(i, 14) == 1);	
				
				
			}
		}
	}
// ---- INIT Sprites for Buffs
	int spacing = 4;
	int side = 32;
	// 4 x 4 buff window
	
	int startx, starty;
	startx = gWidth /2 + 150;
	starty = 0;
	for (int i =0; i < 4; ++i) {
		for (int j =0; j < 4; ++j) {
			int x, y;
			x = startx + j * spacing + j * side;
			y = starty + i * spacing + i * side;
			int b = (i * 4) + j;
			bufficons[b].SetSide(side);	
			bufficons[b].Move(x, y);
		}
	}
	
//---- Load Class Ids to Class Name hash,
	if (dbGame->Query("SELECT class_id, name from Classes")) {
		classIdsToName[0] = "Any";
		for (int i = 0; i < dbGame->total_rows; ++i) {
			classIdsToName[dbGame->GetInteger(0, 0)] = dbGame->GetValue(i, 1);
		}
	
	}
	
//---- Initalize Keyboard Input Callback
	glfwSetCharCallback( &onKey );
	
//---- Setup Viewport and Rendering options
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); // Set our viewport to the size of our window 
	glShadeModel( GL_SMOOTH );                // Enable smooth shading
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );   // Black background
	glClearDepth( 1.0f );                     // Depth buffer setup
	glEnable( GL_DEPTH_TEST );                // Enable depth testing
	glDepthFunc( GL_LEQUAL );                 // Type of depth test to do
	// Really nice perspective calculations
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

//---- FOG
	float fogColor[4] = {0.519f, 0.679f, 0.890f, 1.0f};
	glFogfv(GL_FOG_COLOR, fogColor);            // Set Fog Color
	glFogf(GL_FOG_DENSITY, 1.0f);              // How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);          // Fog Hint Value
	/*glFogf(GL_FOG_START, 10.0f);             // Fog Start Depth
	glFogf(GL_FOG_END, 150.0f);				// Fog End Depth
	*/
	glFogf(GL_FOG_START, 30.0f);             // Fog Start Depth
	glFogf(GL_FOG_END, 200);				// Fog End Depth
//---- CAMERA
	gCamera.viewHeight =gHeight;
	gCamera.viewWidth = gWidth;
	gCamera.SetOrigin(15.0f, 20.0f, -10.0f);
	gCamera.RotateLocalX(0.4f);
	gCamera.SetClippingDistance(0.3,800);

//---- FRUSTRUM
	gFrustum.SetPerspective(60,(GLfloat)gWidth / (GLfloat)gHeight, 0.3f, gCamera.GetFarClippingDistance() );
	objectFrustum.SetPerspective(60,(GLfloat)gWidth / (GLfloat)gHeight, 0.3f, 205 );
	
//---- LIGHTING
	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]= { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat LightPosition[]= { 0.0f, 1000.0f, 2000.0f, 1.0f };   
	glLightfv(GL_LIGHT0,GL_AMBIENT,LightAmbient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,LightDiffuse);
	glLightfv(GL_LIGHT0,GL_POSITION,LightPosition);
	glEnable(GL_LIGHT0);
	
//---- TERRAIN
	sVector3 p = sVector3(0.0f,2.0f,0.0f);
	land  = new QuadTerrain(257,257,3.0f,3.0f,p, 32);
	land->LoadMapWithEnemyData("map", objects, meshes,enemy_objects,enemy_meshes, enemy_attack_meshes, dbGame);
	land->SetLowerDetailRange(200);
	land->SetRenderCircle(false);

//---- SKY
	M3DVector3f skypos = {0.0f, 0.0f, 0.0f};
	//sky = new SkyPlane(skypos, 32, 40, 1.4, 0.75 );
	sky = new SkyPlane(skypos, 32, 60, 1.4, 1.0f );
	gDayNight.SetTimeOfDayAsPercentage(0.5);
	
//---- WATER
	//water = new Water(64,64,900,10,"waterNormalMap1.tga", "waterNormalMap2.tga");
	water = new Water(64,64,1400,40,"waterNormalMap1.tga", "waterNormalMap2.tga");
	
//---- Initialize Networking Code
	nHost = NULL;
	if (enet_initialize () != 0) {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
    }
	
//---- Intialize Network Players Array
	for (int i =0; i < 4; ++i) {
		players[i].connected = false;
		players[i].SetTerrain(land);
		players[i].SetDatabases(dbPlayers, dbGame);
		
	}

//---- Intialize Levels for experience
	float exp_needed = 10.0f;
	for (int i =1; i <= 50; ++i) {
		gLevels[i] = (int)exp_needed;
		exp_needed *= 1.13f;
	}
	
//---- Initalize Equipment array to empty
	GetEquipFromDatabase();
	
//---- Set Default Values to game variables
	elapsed_player_update = 0;
	moving_forward = false;
	moving_back = false;
	turning_right = false;
	turning_left = false;
	mouseDownLock = false;
	renderRefraction = true;
	renderReflection = true;
	safe_delete = true;
	RotatingCamera = false;
	RotateCameraX = 0;
	RotateCameraY = 0;
	cameraDist = 5.0f;
	oldMouseWheelValue=0;
	selectedEquipIndex = 1;
	TargetEnemy = NULL;
	PlayerToggledAttack = false;
	castingSpell = NULL;
	castSelected = 0;
	seed();
	
	timeElapsed=0.0f;
	InitEmitterShaders();
	InitParticleModels();
	
//---- Set First Screen
	current_screen = CharacterScreen;
	next_screen = CharacterScreen;
	InitCharacterScreen();
	
}

Game::~Game() {
	if (nHost != NULL) {
		delete nHost;
	}

	if (current_screen == CharacterScreen) {
		DeleteCharacterScreen();
	} else if (current_screen == NewCharacterScreen) {
		DeleteNewCharacterScreen();
	} else if (current_screen == PlayScreen) {
		DeletePlayScreen();
	}

	delete dbPlayers;
	delete dbGame;
	delete land;
	delete water;
	int meshes_len = meshes.size();
	for(int i =0; i < meshes_len; ++i) {
		delete meshes[i];
		meshes[i] = NULL;
	}
	
	for(int i =0; i < objects.size(); ++i) {
		delete objects[i];
		objects[i] = NULL;
	}
	
	meshes_len = enemy_meshes.size();
	for(int i =0; i < meshes_len; ++i) {
		delete enemy_meshes[i];
		enemy_meshes[i] = NULL;
	}
	
	for(int i =0; i < enemy_objects.size(); ++i) {
		delete enemy_objects[i];
		enemy_objects[i] = NULL;
	}
	
	for(int i =0; i < item_textures.size(); ++i) {
		delete item_textures[i];
		item_textures[i] = NULL;
	}
	
	for(int i =0; i < inventory.size(); ++i) {
		delete inventory[i];
		inventory[i] = NULL;
	}
	delete sky;
	
	DeleteParticleModels();
	
	enet_deinitialize();
	
}

void Game::Init() {
	
	
}

void Game::Draw() {
	glViewport(0,0, viewWidth, viewHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	gCamera.Render();
		
	if (current_screen == PlayScreen) {
		DrawPlayScreen();
	} else if (current_screen == CharacterScreen) {
		DrawCharacterScreen();
	} else if (current_screen == NewCharacterScreen) {
		DrawNewCharacterScreen();
	} 
}

void Game::Draw2D() {
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed  
	glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)  
	gluOrtho2D(0, gWidth, gHeight, 0);
	glMatrixMode(GL_MODELVIEW); // Sw
	glLoadIdentity();
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	
	if (current_screen == PlayScreen) {
		Draw2DPlayScreen();
	} else if (current_screen == CharacterScreen) {
		Draw2DCharacterScreen();
	} else if (current_screen == NewCharacterScreen) {
		Draw2DNewCharacterScreen();
	}
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable (GL_DEPTH_TEST);
}

void Game::Update() {
	numFrames++;
	
	timeElapsed += dt;
	
	if(timeElapsed >= 1.0f){
		mFPS = numFrames;
		timeElapsed = 0.0f;
		numFrames = 0.0f;
	}
	
	
	//Handle Deletes
	if (glfwGetKey( GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
		if (gCarot != NULL  && safe_delete) {
			gCarot->Backspace();
			safe_delete = false;
		}
	} else {
		safe_delete = true;
	}
	
	
	//Delete Current screen
	if (current_screen != next_screen) {
		if (current_screen == CharacterScreen) {
			DeleteCharacterScreen();
		} else if (current_screen == NewCharacterScreen) {
			DeleteNewCharacterScreen();
		} else if (current_screen == PlayScreen) {
			DeletePlayScreen();
		}
	}
	
	//init new screen
	if (current_screen != next_screen) {
		if (next_screen == CharacterScreen) {
			InitCharacterScreen();
		} else if (next_screen == NewCharacterScreen) {
			InitNewCharacterScreen();
		} else if (next_screen == PlayScreen) {
			InitPlayScreen();
		}
		
		current_screen = next_screen;
	}
	
	//UPDATE SCREEN
	if (current_screen == PlayScreen) {
		UpdatePlayScreen(dt);
	} else if (current_screen == CharacterScreen) {
		UpdateCharacterScreen();
	} else if (current_screen == NewCharacterScreen) {
		UpdateNewCharacterScreen();
	}
}


