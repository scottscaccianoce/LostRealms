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

Game::Game(int w, int h, bool fullscreen) : App(w,h,fullscreen) {
	SPMesh *model =NULL;
	dbGame = new Database("game.sqlite3");
	
	if (dbGame->Query("SELECT mesh_id, filename from Meshes ORDER BY mesh_id")) {
		for (int i = 0; i < dbGame->total_rows; ++i) {
			model = new SPMesh();
			model->db_id = atoi(dbGame->results[i][0].c_str());
			model->ImportMesh(dbGame->results[i][1], "");
			meshes.push_back(model);
		}
	}
	
	if (dbGame->Query("SELECT mesh_id, filename from Enemies ORDER BY mesh_id")) {
		for (int i = 0; i < dbGame->total_rows; ++i) {
			model = new SPMesh();
			model->db_id = atoi(dbGame->results[i][0].c_str());
			model->ImportMesh(dbGame->results[i][1], "");
			enemyMeshes.push_back(model);
		}
	}
	
	
	glfwSetCharCallback( &onKey );
	CreateUI();
	
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); // Set our viewport to the size of our window 
	glShadeModel( GL_SMOOTH );                // Enable smooth shading
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );   // Black background
	glClearDepth( 1.0f );                     // Depth buffer setup
	glEnable( GL_DEPTH_TEST );                // Enable depth testing
	glDepthFunc( GL_LEQUAL );                 // Type of depth test to do
	// Really nice perspective calculations
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	float fogColor[4] = {0.519f, 0.679f, 0.890f, 1.0f};
	glFogfv(GL_FOG_COLOR, fogColor);            // Set Fog Color
	glFogf(GL_FOG_DENSITY, 1.0f);              // How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);          // Fog Hint Value
	glFogf(GL_FOG_START, 30);             // Fog Start Depth
	glFogf(GL_FOG_END, 200.0f);  
	
	
	sVector3 p = sVector3(0.0f,2.0f,0.0f);
	land  = new QuadTerrain(257,257,3.0f,3.0f,p, 32);
	land->SetLowerDetailRange(200);
	
	land->RenderWireframe(true);
	float value = 75.0f * brushRadius->GetValue();
	land->SetRadius(value);
	
	gCamera.viewHeight =gHeight;
	gCamera.viewWidth = gWidth;
	gCamera.SetOrigin(0.0f, 20.0f, 0.0f);
	gCamera.RotateWorld(-1.0f, 1,0,0);
	gCamera.SetClippingDistance(0.3,800);
	
	//gFrustum.SetPerspective(60,(GLfloat)gWidth / (GLfloat)gHeight, gCamera.GetNearClippingDistance(), gCamera.GetFarClippingDistance() );
	gFrustum.SetPerspective(60,(GLfloat)gWidth / (GLfloat)gHeight, 0.3f, gCamera.GetFarClippingDistance() );
	objectFrustum.SetPerspective(60,(GLfloat)gWidth / (GLfloat)gHeight, 0.3f, 205 );
	
	mode = kSCULPT;
	land->SetRenderCircle(true);
	
	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]= { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat LightPosition[]= { 0.0f, 10.0f, 20.0f, 1.0f };   
	
	glLightfv(GL_LIGHT0,GL_AMBIENT,LightAmbient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,LightDiffuse);
	glLightfv(GL_LIGHT0,GL_POSITION,LightPosition);
	glEnable(GL_LIGHT0);
	mouseDownLock = false;
	
	
	
	M3DVector3f skypos = {0.0f, 0.0f, 0.0f};
	//sky = new SkyPlane(skypos, 32, 20, 2, 0.5 );
	//sky = new SkyPlane(skypos, 32, 40, 1.4, 0.75 );
	sky = new SkyPlane(skypos, 32, 60, 1.4, 1.0f );
	
	
	//water = new Water(64,64,10,10,"waterNormalMap1.tga", "waterNormalMap2.tga");
	//water = new Water(64,64,900,10,"waterNormalMap1.tga", "waterNormalMap2.tga");
	//water = new Water(64,64,900,20,"waterNormalMap1.tga", "waterNormalMap2.tga");
	water = new Water(64,64,1400,40,"waterNormalMap1.tga", "waterNormalMap2.tga");
	
	gDayNight.SetTimeOfDayAsPercentage(timeValue->GetValue());
	selectedEntity = NULL;
	
	//Compass stuff
	
	tCompass.loadTexture("totalcompass.tga");
	sCompass.SetTexture(&tCompass);
	sCompass.Move((gWidth - 256)/2, 0);
	sCompass.SetSide(256);
	
	//Seed random Number Generator
	seed();
}

Game::~Game() {
	delete canvas;
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
	
	meshes_len = enemyMeshes.size();
	for(int i =0; i < meshes_len; ++i) {
		delete enemyMeshes[i];
		enemyMeshes[i] = NULL;
	}
	
	for(int i =0; i < enemyObjects.size(); ++i) {
		delete enemyObjects[i];
		enemyObjects[i] = NULL;
	}
	
	delete sky;
	delete dbGame;
}

void Game::Init() {
	
	
}

void Game::Draw() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	if (!land->bRenderWireFrame) {
		water->StartReflection();
		sky->Render();
		land->Render();
		int len = objects.size();
		for(int i =0; i < len; ++i) {
			M3DVector3f point;
			objects[i]->GetOrigin(point);
			if (objectFrustum.TestSphere(point, objects[i]->GetRadius())) {
				objects[i]->Render();
			}
		}
		
		water->EndReflection();
		
		water->StartRefaction();
	
		land->Render();
		water->EndRefaction();
		
	}
	
	glViewport(0,0, viewWidth, viewHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	gCamera.Render();
	
	gFrustum.Transform(gCamera);
	objectFrustum.Transform(gCamera);
	
	if (land->bRenderWireFrame) {
		land->Render();
	} else {
		sky->Render();
		land->Render();
		
		int len = objects.size();
		for(int i =0; i < len; ++i) {
			M3DVector3f point;
			objects[i]->GetOrigin(point);
			if (objectFrustum.TestSphere(point, objects[i]->GetRadius())) {
				objects[i]->Render();
			}
		}
		
		len = enemyObjects.size();
		for(int i =0; i < len; ++i) {
			M3DVector3f point;
			enemyObjects[i]->GetOrigin(point);
			if (objectFrustum.TestSphere(point, enemyObjects[i]->GetRadius())) {
				enemyObjects[i]->Render();
			}
		}
		
		if (selectedEntity != NULL) {
			selectedEntity->RenderBB();
		}
		
		water->AdvRender();
		
	}
	
		
	glDisable(GL_CULL_FACE);
	

	
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
	
	char buffer[256];
	sprintf(buffer, "Frames Per Second: %.2f",mFPS);
	gFont.DrawText(buffer, 10, 10, 1, 0.5, 0);
	
	char quadsbuffer[256];
	sprintf(quadsbuffer, "Quads Rendered %d out of %d", QuadTerrain::counter, QuadTerrain::total);
	gFont.DrawText(quadsbuffer, 10, 25, 1, 0.5, 0);
	
	char cbuffer[256];
	M3DVector3f corigin;
	gCamera.GetOrigin(corigin);
	sprintf(cbuffer, "x = %.2f    z= %.2f", corigin[0],corigin[2]);
	gFont.DrawText(cbuffer, 10, 40, 1, 0.5, 0);
	
	canvas->Render();
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable (GL_DEPTH_TEST);
	
	
	glEnable(GL_TEXTURE_2D);
	sCompass.RenderShader(compassRot,0,256,64, true);
	sCompass.RenderShader(0,64,256,64);
	glDisable(GL_TEXTURE_2D);
	
}

void Game::Update() {
	numFrames++;
	
	timeElapsed += dt;
	
	if(timeElapsed >= 1.0f){
		mFPS = numFrames;
		timeElapsed = 0.0f;
		numFrames = 0.0f;
	}
	HandleInput(dt);
	water->Update(dt);
	
	//Map Menu
	if (timeValue->GetValue() != oldTime) {
		oldTime = timeValue->GetValue();
		gDayNight.SetTimeOfDayAsPercentage(timeValue->GetValue());	
		
	}
	
	//COMPAS STUFF
	M3DVector3f North,Forward;
	North[0] = 0.0f;
	North[1] = 0.0f;
	North[2] = -1.0f;
	gCamera.GetForwardVector(Forward);
	Forward[1] = 0.0f;
	m3dNormalizeVector3(Forward);
	float angleDir = m3dRadToDeg(m3dGetAngleBetweenVectors3(Forward,North));
	
	if (Forward[0] < 0.0f) {
		angleDir = 360 - angleDir;
	}
	
	float texOffset = angleDir/90 * 64;
	float startingOffset = 158; //This gets us to the "N" for the north vector in the image
	compassRot = startingOffset + texOffset;
	
	//Speical update case for selected object
	if (selectedEntity != NULL) {
		selectedEntity->SetOriginY(atof(heightValue->GetText().c_str()));
		if (glfwGetKey( GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
			DeleteSelectedEntity();
		}
	}

}
void Game::HandleInput(float dt) {
	
	if(canvas->Update()) {
		if (gMouse != NULL) {
			//Handle Menu
			if(gMouse == menuSculpt) {
				paintForm->Hide();
				terrainForm->Show();
				objectForm->Hide();
				mapForm->Hide();
				enemyForm->Hide();
				
				mode = kSCULPT;
				land->SetRenderCircle(true);
			} else if (gMouse == menuPaint) {
				paintForm->Show();
				terrainForm->Hide();
				objectForm->Hide();
				mapForm->Hide();
				enemyForm->Hide();
				
				renderWireframe->SetValue(false);
				land->RenderWireframe(false);
				land->SetRenderCircle(false);
				mode = kPAINT;
			} else if (gMouse == menuObject) {
				objectForm->Show();
				terrainForm->Hide();
				paintForm->Hide();
				mapForm->Hide();
				enemyForm->Hide();
				
				land->RenderWireframe(false);
				land->SetRenderCircle(false);
				mode = kOBJECT;
			} else if (gMouse == menuQuest) {
				mode = kQUEST;
			} else if (gMouse == menuMap) {
				objectForm->Hide();
				terrainForm->Hide();
				paintForm->Hide();
				mapForm->Show();
				enemyForm->Hide();
				
				renderWireframe->SetValue(false);
				land->RenderWireframe(false);
				land->SetRenderCircle(false);
				
			} else if (gMouse == menuEnemies) {
				objectForm->Hide();
				terrainForm->Hide();
				paintForm->Hide();
				mapForm->Hide();
				enemyForm->Show();
				land->RenderWireframe(false);
				land->SetRenderCircle(false);
				LoadEnemyStats();

				mode = kENEMY;
			}
			
			
			
			//Handle any UI button callbacks
			if (gMouse == raiseVerts) {
				vertMode->SetText("Raise");
			}
			else if (gMouse == lowerVerts) {
				vertMode->SetText("Lower");
			}
			else  if (gMouse == smoothVerts) {
				vertMode->SetText("Smooth");
			}
			else  if (gMouse == FlatVerts) {
				vertMode->SetText("Flatten");
			}
			else if (gMouse == brushRadius) {
				float value = 75.0f * brushRadius->GetValue();
				land->SetRadius(value);
			}
			else if (gMouse == renderWireframe) {
				land->RenderWireframe(renderWireframe->GetChecked());
			}
			else if	(gMouse == btnSave) {
				std::string filename = txtSaveFilename->GetText();
				if (filename != "") {
					land->SaveMap(filename, objects,enemyObjects);
					gCarot = NULL;
				}
				
			}
			else if	(gMouse == btnLoad) {
				std::string filename = txtLoadFilename->GetText();
				if (filename != "") {
					land->LoadMap(filename, objects, meshes, enemyObjects,enemyMeshes);
					gCarot = NULL;
				}
				
			}
			else if	(gMouse == enemySaveStats) {
				SaveEnemyStats();
			} 
			else if (gMouse == enemyList) {
				LoadEnemyStats();	
			}else if (gMouse == rotationSlider && selectedEntity != NULL) {
				selectedEntity->SetForwardVector(0, 0, -1);
				selectedEntity->RotateLocalY(m3dDegToRad(360 * rotationSlider->GetValue()));	
			} 
			
			if (gMouse == filterTex) {
				texMode->SetText("Smooth");				
			} else if (gMouse == paintTex) {
				texMode->SetText("Paint");
			}
			
			
			
		gMouse = NULL;	
		}
		
		
	//------------------------------------	
	} else {
		
		//handle all mouse related clicks
		if (gCarot == NULL) {
			float moveSpeed = 60;
			if(glfwGetKey('W')){
				gCamera.MoveForward(moveSpeed *dt);
			}
			
			if(glfwGetKey('S')){
				gCamera.MoveForward(-moveSpeed *dt);
			}
			if(glfwGetKey('D')){
				gCamera.MoveRight(-moveSpeed * dt);
			}
				if(glfwGetKey('A')){
				gCamera.MoveRight(moveSpeed * dt);
			}
		}
		
		if(glfwGetKey(GLFW_KEY_RIGHT)){
			gCamera.RotateWorld(-1.5f * dt, 0,1,0);
		}
		if(glfwGetKey(GLFW_KEY_LEFT)){
			gCamera.RotateWorld(1.5f * dt, 0,1,0);
			//gCamera.RotateLocalY(0.4f *dt);
		}
		if(glfwGetKey(GLFW_KEY_UP)){
			//gCamera.RotateLocalX(200.0f*dt);
			gCamera.RotateLocalX(-1.5f *dt);
		}
		if(glfwGetKey(GLFW_KEY_DOWN)){
			gCamera.RotateLocalX(1.5f*dt);
		}
		
		if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
			if (!mouseDownLock) {
				if (selectedEntity == NULL ) {
					mouseDownLock = true;
					sVector3 mpos1, mdir, mpos2;
					GetMouseRay(mpos1, mpos2, mdir);
					M3DVector3f pos,dir,spos;
					pos[0] = mpos1.x;
					pos[1] = mpos1.y, 
					pos[2] = mpos1.z;
					dir[0] = mdir.x;
					dir[1] = mdir.y, 
					dir[2] = mdir.z;
					
					if (mode == kOBJECT) {
						int len = objects.size();
						for (int i =0; i < len; ++i) {
							float dist;
							if (objects[i]->RayIntersect(pos,dir,dist)) {
								selectedEntity = objects[i];
								heightValue->SetText(FloatToString(selectedEntity->GetOriginY()));
								float angle = selectedEntity->GetRotateY();
								rotationSlider->SetValue(angle / 360.0f);
								heightValue->Show();
								rotationSlider->Show();
								break;
							}
						}
					} else if ( mode == kENEMY) {
						int len = enemyObjects.size();
						for (int i =0; i < len; ++i) {
							float dist;
							if (enemyObjects[i]->RayIntersect(pos,dir,dist)) {
								selectedEntity = enemyObjects[i];
								break;
							}
						}	
							
					}
					
					
				} else {
					selectedEntity = NULL;
					heightValue->Hide();
					rotationSlider->Hide();
					
				}
			}
		} else if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
			if (mode == kSCULPT) {
				sVector3 newpos;
				std::string mode = vertMode->GetText();
				if (land->MouseIntersect(newpos)) {
					if (mode == "Raise") {
						land->CircleRaise(newpos,dt);
					} else if (mode == "Lower") {
						land->CircleLower(newpos,dt);
					} else if (mode == "Smooth") {
						land->CircleSmooth(newpos,dt);
					} else if (mode == "Flatten") {
						land->CircleFlat(newpos,dt);
					}
					
				}
			} else if(mode == kPAINT) {
				sVector3 newpos;
				std::string mode = texMode->GetText();
				if (land->MouseIntersect(newpos)) {
					if (mode == "Paint") {
						land->DrawSplat(newpos, texturesList->GetSelectedIndex(), texBrushsize->GetValue());
					} else if (mode =="Smooth") {
						land->DrawFilter(newpos, texBrushsize->GetValue());
					}
				}
				
			} else if (mode == kOBJECT) {
				if (!mouseDownLock && selectedEntity == NULL) {
					sVector3 newpos;
					if (land->MouseIntersect(newpos)) {
						mouseDownLock = true;
						SPEntity *entity = new SPEntity(meshes[meshList->GetSelectedIndex()]);
						entity->SetOrigin(newpos.x,newpos.y,newpos.z);
						if (randomRotation->GetChecked()) {
							RandomRotateEntity(entity);
						}
						objects.push_back(entity);
					}
				} else if ( selectedEntity != NULL) {
					sVector3 newpos;
					if (land->MouseIntersect(newpos)) {
						selectedEntity->SetOrigin(newpos.x,newpos.y,newpos.z);
						heightValue->SetText(FloatToString(newpos.y));
					}
					
				}
			} else if (mode == kENEMY) {
				if (!mouseDownLock && selectedEntity == NULL) {
					sVector3 newpos;
					if (land->MouseIntersect(newpos)) {
						mouseDownLock = true;
						SPEntity *entity = new SPEntity(enemyMeshes[enemyList->GetSelectedIndex()]);
						entity->SetOrigin(newpos.x,newpos.y,newpos.z);
						//entity->SetOrigin(0,0,0);
						enemyObjects.push_back(entity);
					}
				} else if ( selectedEntity != NULL) {
					sVector3 newpos;
					if (land->MouseIntersect(newpos)) {
						selectedEntity->SetOrigin(newpos.x,newpos.y,newpos.z);
					}
					
				}
			}
		} else if (mouseDownLock) {
			mouseDownLock = false;
		}
		
		
	}
	
	HandleSpecialKeyInput();
}

void Game::CreateUI() {
	//1280 - 800
	canvas = new UICanvas(UIRect(0,0,gWidth,gHeight));
	UIWidget *form;
	UIWidget *label;
	UIWidget *button;
	UIWidget *slider;
	UIWidget *stepslider;
	UIWidget *chk;
	UIWidget *text;
	
	int window_x_offset = 1140;
	int subwindow_height = 750;
	
	// MENU FORM -----------------------------------------------
	form = new UIWindow(canvas);
	form->SetBounds(UIRect(window_x_offset,2,298,94));
	reinterpret_cast<UIWindow*>(form)->DisplayTitle(false);
	reinterpret_cast<UIWindow*>(form)->SetDragable(false);

	
	button = new UIButton(form, "Sculpt");
	menuSculpt = reinterpret_cast<UIButton*>(button);
	menuSculpt->SetBounds(UIRect(4,4,70,20));
	
	button = new UIButton(form, "Paint");
	menuPaint = reinterpret_cast<UIButton*>(button);
	menuPaint->SetBounds(UIRect(4,30,70,20));
	
	button = new UIButton(form, "Object");
	menuObject = reinterpret_cast<UIButton*>(button);
	menuObject->SetBounds(UIRect(4,56,70,20));
	
	button = new UIButton(form, "Quest");
	menuQuest = reinterpret_cast<UIButton*>(button);
	menuQuest->SetBounds(UIRect(90,4,70,20));
	
	button = new UIButton(form, "Map");
	menuMap = reinterpret_cast<UIButton*>(button);
	menuMap->SetBounds(UIRect(90,30,70,20));
	
	menuEnemies = new UIButton(form, "Enemies");
	menuEnemies->SetBounds(UIRect(90,56,70,20));
	// MAP FORM -----------------------------------------------
	
	form = new UIWindow(canvas);
	mapForm = reinterpret_cast<UIWindow*>(form);
	mapForm->SetBounds(UIRect(window_x_offset,100,298,subwindow_height));
	mapForm->SetDragable(false);
	mapForm->SetTitle("Map Information");
	mapForm->Hide();
	
	label = new UILabel(form);
	label->SetPos(UIPos(10,20));
	label->SetText("Day/Night");
	
	slider = new UISlider(form);
	timeValue = reinterpret_cast<UISlider*>(slider);
	timeValue->SetBounds(UIRect(15,40,239));
	timeValue->SetValue(0.5f);
	
	text = new UITextbox(form);
	txtSaveFilename = reinterpret_cast<UITextbox*>(text);
	txtSaveFilename->SetBounds(UIRect(10,80,200));
	
	button = new UIButton(form, "Save");
	btnSave = reinterpret_cast<UIButton*>(button);
	btnSave->SetBounds(UIRect(220,80,50,20));
	
	
	text = new UITextbox(form);
	txtLoadFilename = reinterpret_cast<UITextbox*>(text);
	txtLoadFilename->SetBounds(UIRect(10,140,200));
	
	button = new UIButton(form, "Load");
	btnLoad = reinterpret_cast<UIButton*>(button);
	btnLoad->SetBounds(UIRect(220,140,50,20));

	
	
	// PAINT FORM -----------------------------------------------
	form = new UIWindow(canvas);
	paintForm = reinterpret_cast<UIWindow*>(form);
	paintForm->SetBounds(UIRect(window_x_offset,100,298,subwindow_height));
	paintForm->SetDragable(false);
	paintForm->SetTitle("Texture Painting");
	paintForm->Hide();
	
	label = new UILabel(form);
	label->SetPos(UIPos(10,20));
	label->SetText("Brush Size");
	
	stepslider = new UIStepSlider(form,1,7);
	texBrushsize = reinterpret_cast<UIStepSlider*>(stepslider);
	texBrushsize->SetBounds(UIRect(125,25,100));
	texBrushsize->SetValue(1);
	
	texturesList = new UIListbox(form);
	
	texturesList->SetBounds(UIRect(10,50,200,0));
	texturesList->SetViewCount(4);
	texturesList->AddItem("grass");
	texturesList->AddItem("dirt");
	texturesList->AddItem("rock");
	texturesList->AddItem("snow");
	
	button = new UIButton(form,"Paint");
	paintTex = reinterpret_cast<UIButton*> (button);
	paintTex->SetBounds(UIRect(10,150,100,20));
	
	button = new UIButton(form,"Smooth");
	filterTex = reinterpret_cast<UIButton*> (button);
	filterTex->SetBounds(UIRect(120,150,100,20));
	
	label = new UILabel(form);
	texMode = reinterpret_cast<UILabel*> (label);
	texMode->SetPos(UIPos(10,180));
	texMode->SetText("Paint");
	texMode->SetFontColor(kNone, UIColor(0,255,255));
	
	
	

	
	//TERRAIN FORM ---------------------------------------------
	
	form = new UIWindow(canvas);
	terrainForm = reinterpret_cast<UIWindow*>(form);
	terrainForm->SetBounds(UIRect(window_x_offset,100,298,subwindow_height));
	terrainForm->SetDragable(false);
	terrainForm->SetTitle("Terrain Sculpting");
	
	label = new UILabel(form);
	label->SetPos(UIPos(10,5));
	label->SetText("Brush Radius");
	
	slider = new UISlider(form);
	brushRadius = reinterpret_cast<UISlider*>(slider);
	brushRadius->SetBounds(UIRect(125,10,100));
	brushRadius->SetValue(0.3f);
	
	button = new UIButton(form, "Raise");
	raiseVerts = reinterpret_cast<UIButton*>(button);
	raiseVerts->SetBounds(UIRect(25,40,100,20));
	
	button = new UIButton(form, "Lower");
	lowerVerts = reinterpret_cast<UIButton*>(button);
	lowerVerts->SetBounds(UIRect(175,40,100,20));
	
	button = new UIButton(form, "Flatten");
	FlatVerts = reinterpret_cast<UIButton*>(button);
	FlatVerts->SetBounds(UIRect(25,70,100,20));
	
	button = new UIButton(form, "Smooth");
	smoothVerts = reinterpret_cast<UIButton*>(button);
	smoothVerts->SetBounds(UIRect(175,70,100,20));
	
	label = new UILabel(form);
	label->SetPos(UIPos(25, 110));
	label->SetText("Current Mode:");
	
	label = new UILabel(form);
	vertMode = reinterpret_cast<UILabel*> (label);
	vertMode->SetPos(UIPos(140, 110));
	vertMode->SetFontColor(kNone, UIColor(0,255,255));
	vertMode->SetText("Raise");
	
	chk = new UICheckbox(form, "Render Wireframe");
	renderWireframe = reinterpret_cast<UICheckbox*> (chk);
	renderWireframe->SetBounds(UIRect(10,145));
	
	
	//OBJECT FORM ---------------------------------------------
	form = new UIWindow(canvas);
	objectForm = reinterpret_cast<UIWindow*>(form);
	objectForm->SetBounds(UIRect(window_x_offset,100,298,subwindow_height));
	objectForm->SetDragable(false);
	objectForm->SetTitle("Place Objects");
	objectForm->Hide();
	
	meshList = new UIListbox(form);
	
	meshList->SetBounds(UIRect(10,50,200,0));
	meshList->SetViewCount(30);
	for (int i = 0; i < meshes.size(); ++i ) {
		meshList->AddItem(meshes[i]->name);
	}
	
	
	int selectionOffset = 540;
	randomRotation = new UICheckbox(form, "Random Rotation");
	randomRotation->SetBounds(UIRect(10,selectionOffset,200,0));
							  
	UILabel *tempObject = new UILabel(form, "Rotation");
	tempObject->SetPos(UIPos(10,selectionOffset + 30));	
	rotationSlider = new UISlider(form);
	rotationSlider->SetBounds(UIRect(100,selectionOffset + 40,150));
	rotationSlider->Hide();
	
	tempObject = new UILabel(form, "Height");
	tempObject->SetPos(UIPos(10,selectionOffset + 60));
	heightValue = new UITextbox(form);
	heightValue->SetBounds(UIRect(100,selectionOffset + 60, 100, 15));
	heightValue->Hide();
	
	//-----------------------------------------------
	
	//ENEMY FORM ---------------------------------------------
	enemyForm = new UIWindow(canvas);
	enemyForm->SetBounds(UIRect(window_x_offset,100,298,subwindow_height));
	enemyForm->SetDragable(false);
	enemyForm->SetTitle("Enemies");
	enemyForm->Hide();
	
	enemyList = new UIListbox(enemyForm);
	
	enemyList->SetBounds(UIRect(10,50,200,0));
	enemyList->SetViewCount(10);
	for (int i = 0; i < enemyMeshes.size(); ++i ) {
		enemyList->AddItem(enemyMeshes[i]->name);
	}
	UILabel *enemyTemp = new UILabel(enemyForm,"Name");
	enemyTemp->SetBounds(UIRect(10,220,100));
	enemyName = new UITextbox(enemyForm);
	enemyName->SetBounds(UIRect(80,220,150));
	
	enemyTemp = new UILabel(enemyForm,"Filename");
	enemyTemp->SetBounds(UIRect(10,240,100));
	enemyFilename = new UITextbox(enemyForm);
	enemyFilename->SetBounds(UIRect(80,240,200));
	
	enemyTemp = new UILabel(enemyForm,"Level");
	enemyTemp->SetBounds(UIRect(10,260,100));
	enemyLevel = new UITextbox(enemyForm);
	enemyLevel->SetBounds(UIRect(80,260,100));
	
	enemyTemp = new UILabel(enemyForm,"HP MAX");
	enemyTemp->SetBounds(UIRect(10,280,100));
	enemyHp = new UITextbox(enemyForm);
	enemyHp->SetBounds(UIRect(80,280,100));
	
	
	
	enemyTemp = new UILabel(enemyForm,"Attack");
	enemyTemp->SetBounds(UIRect(10,300,100));
	enemyAttack = new UITextbox(enemyForm);
	enemyAttack->SetBounds(UIRect(80,300,100));
	
	enemyTemp = new UILabel(enemyForm,"Respawn");
	enemyTemp->SetBounds(UIRect(10,320,100));
	enemyRespawn = new UITextbox(enemyForm);
	enemyRespawn->SetBounds(UIRect(80,320,100));
	
	
	enemyHostile = new UICheckbox(enemyForm, "Hostile");
	enemyHostile->SetPos(UIPos(80,340));
	

	enemyTemp = new UILabel(enemyForm,"Radius");
	enemyTemp->SetBounds(UIRect(10,360,100));
	enemyBAFRadius = new UITextbox(enemyForm);
	enemyBAFRadius->SetBounds(UIRect(80,360,100));
	
	enemyTemp = new UILabel(enemyForm,"Exp");
	enemyTemp->SetBounds(UIRect(10,380,100));
	enemyExp = new UITextbox(enemyForm);
	enemyExp->SetBounds(UIRect(80,380,100));
	
	enemyTemp = new UILabel(enemyForm,"Filename");
	enemyTemp->SetBounds(UIRect(10,400,100));
	enemyAttackFilename = new UITextbox(enemyForm);
	enemyAttackFilename->SetBounds(UIRect(80,400,200));
	
	
	enemySaveStats = new UIButton(enemyForm, "Save Stats");
	enemySaveStats->SetBounds(UIRect(10,420,170,20));
	
	
	
	//-----------------------------------------------
	
	
	

}

void Game::LoadEnemyStats() {
	if (enemyList->GetCount() > 0) {
		int selected = enemyList->GetSelectedIndex() + 1;
		dbGame->Query("SELECT mesh_id,filename,attack_filename,name,level,hp,attack,hostile,baf_radius,respawn,exp from Enemies where mesh_id = " + IntToString(selected));
		
		enemyFilename->SetText(dbGame->GetValue(0, 1));
		enemyAttackFilename->SetText(dbGame->GetValue(0,2));
		enemyName->SetText(dbGame->GetValue(0, 3));
		enemyLevel->SetText(dbGame->GetValue(0, 4));
		enemyHp->SetText(dbGame->GetValue(0, 5));
		enemyAttack->SetText(dbGame->GetValue(0, 6));
		
		std::string hos;
		hos = dbGame->GetValue(0, 7);
		
		enemyBAFRadius->SetText(dbGame->GetValue(0, 8));
		enemyRespawn->SetText(dbGame->GetValue(0, 9));
		
		if (hos == "1") {
			enemyHostile->SetValue(true);
		} else {
			enemyHostile->SetValue(false);
		}

	}
	
}
void Game::SaveEnemyStats() {
	if (enemyList->GetCount() > 0) {
		int db_id = enemyList->GetSelectedIndex() + 1;
		
		std::string dHp, dAttack, dRadius, dHostile, dName, dFilename, dRespawn,dLevel,dExp,dAttackFilename;
		
		dName = enemyName->GetText();
		dFilename = enemyFilename->GetText();
		dAttackFilename = enemyAttackFilename->GetText();
		dHp = enemyHp->GetText();
		dAttack = enemyAttack->GetText();
		dRadius = enemyBAFRadius->GetText();
		dRespawn = enemyRespawn->GetText();
		dHostile = (enemyHostile->GetChecked()) ? "1" : "0";
		dLevel = enemyLevel->GetText();
		dExp = enemyExp->GetText();
		
		dHp = (dHp == "") ? "0" : dHp;
		dAttack = (dAttack == "") ? "0" : dAttack;
		dRadius = (dRadius == "") ? "0" : dRadius;
		dRespawn = (dRespawn == "") ? "0" : dRespawn;
		dLevel = (dLevel == "") ? "0" : dLevel;
		dExp = (dExp == "" ) ? "0" : dExp;
		
		
		dbGame->Query("UPDATE Enemies SET name = \""+dName+"\", attack_filename = \""+dAttackFilename+"\", filename = \""+dFilename+"\", level = "+dLevel+", hp = "+dHp+" , attack = "+dAttack+", hostile = "+dHostile+", baf_radius = "+dRadius+", respawn = " + dRespawn + ", exp = "+dExp+" WHERE mesh_id = " + IntToString(db_id));
	}
}

void Game::RandomRotateEntity(SPEntity *obj) {
	M3DVector3f dir;
	dir[0] = unifRand(-1,1);
	dir[1] = 0;
	dir[2] = unifRand(-1,1);
	m3dNormalizeVector3(dir);
	obj->SetForwardVector(dir);
}

void Game::DeleteSelectedEntity() {
	if (selectedEntity != NULL && gCarot == NULL) {
		for (vector<SPEntity *>::iterator it = objects.begin();	it < objects.end(); ++it) {
			if (*it == selectedEntity) {
				objects.erase(it);
				selectedEntity = NULL;
				rotationSlider->Hide();
				heightValue->Hide();
				break;
			}
		}
	}
}
