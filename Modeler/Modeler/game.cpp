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
#include "WinDir.h"
#include <iostream>
#include <sstream>

using namespace std;


Game::Game(int w, int h, bool fullscreen) : App(w,h,fullscreen) {

	gWidth = w;
	gHeight = h;
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
	glFogf(GL_FOG_START, 50.0f);             // Fog Start Depth
	glFogf(GL_FOG_END, 170.0f);  
	
	
	sVector3 p = sVector3(0.0f,0.0f,0.0f);
	land  = new Terrain(32,32,2.0f,2.0f,p);
	land->RenderWireframe(true);
		
	gCamera.viewHeight =gHeight;
	gCamera.viewWidth = gWidth;
	gCamera.SetOrigin(0.0f, 25.0f, 25.0f);
	
	//gCamera.RotateWorld(-1.0f, 1,0,0);
	M3DVector3f cdir;
	M3DVector3f origin = {0.0f, 0.0f, 0.0f};
	M3DVector3f corigin;
	M3DVector3f localX, vUp, vForward;
	gCamera.GetUpVector(vUp);
	gCamera.GetForwardVector(vForward);
	
	// Get the local X axis
	m3dCrossProduct3(localX, vForward, vUp);
	
	gCamera.GetOrigin(corigin);
	
	
	m3dSubtractVectors3(cdir, origin, corigin );
	m3dNormalizeVector3(cdir);
	gCamera.SetForwardVector(cdir);
	
	
	// Get the local X axis
	m3dCrossProduct3(vUp, localX, cdir);
	
	
	gCamera.SetUpVector(vUp);
	
	gFrustum.SetPerspective(60,(GLfloat)gWidth / (GLfloat)gHeight, 0.3f, gCamera.GetFarClippingDistance() );
	
	
	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]= { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat LightPosition[]= { 0.0f, 1000.0f, 2000.0f, 1.0f };   
	
	glLightfv(GL_LIGHT0,GL_AMBIENT,LightAmbient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,LightDiffuse);
	glLightfv(GL_LIGHT0,GL_POSITION,LightPosition);
	glEnable(GL_LIGHT0);
	mouseDownLock = false;
	
	
	//-------testing----
	//mesh.ImportMesh("test.spm", "../../../");
	
	
	//------------------
	
	
	object = new SPEntity(&mesh);
	LoadedEntity = new SPEntity(&LoadedMesh);
	M3DVector3f loadedPos;
	loadedPos[0] = 0.0f;
	loadedPos[1] = 0.0f;
	loadedPos[2] = -10.0f;
	
	LoadedEntity->SetOrigin(loadedPos);
	imported = false;
	
	ofstream myfile;
	myfile.open ("example.txt");
	myfile << "Writing this to a file.\n";
	myfile.close();
	
#ifdef __APPLE__
	DIR *dh;
	struct dirent *dirp;
	
	
	if((dh = opendir("../../../")) != NULL){
		while((dirp = readdir(dh)) != NULL){
			// do my stuff
			string filename = string(dirp->d_name);
			size_t found = filename.find(".obj");
			if (found != string::npos) {
				lstFiles->AddItem(filename);
			}
		}
		closedir(dh);
	}

#else

	std::vector<std::string> results;
	GetFilesInDir(results);
	for	(int i =0; i < results.size(); ++i) {
		lstFiles->AddItem(results[i]);
	}

#endif
	
	
}

Game::~Game() {
	delete object;
	delete LoadedEntity;
	delete canvas;
	delete land;
}

void Game::Init() {
	

	}

void Game::Draw() {
	
	
			
	glViewport(0,0, viewWidth, viewHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	gCamera.Render();
	land->Render();
	
	if (lstFrames->GetCount() > 0) {
		object->Render();
	}
	
	if (imported) {
		LoadedEntity->Render();
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
	
	char buffer[256];
	sprintf(buffer, "[-] [+] Keys move imported SPModel");
	gFont.DrawText(buffer, 10, 10, 1, 0.5, 0);
	
	char quadsbuffer[256];
	sprintf(quadsbuffer, "[W/A/S/D]: Move Camera ");
	gFont.DrawText(quadsbuffer, 10, 25, 1, 0.5, 0);
	
	char buffer2[256];
	sprintf(buffer2, "[Up/Down/Left/Right]: Rotate Camera ");
	gFont.DrawText(buffer2, 10, 40, 1, 0.5, 0);
	
	canvas->Render();
	
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
	HandleInput(dt);
	
	object->Update(dt);
	//gCamera.MoveForward(dt * 3.0f);
	
	if (imported) {
		LoadedEntity->Update(dt);
	}
	

}
void Game::HandleInput(float dt ) {
	
	if (gCarot == NULL) {
		
	
		if(glfwGetKey('W')){
			gCamera.MoveForward(15 *dt);
		}
		
		if(glfwGetKey('S')){
			gCamera.MoveForward(-15 *dt);
		}
		if(glfwGetKey('D')){
			gCamera.MoveRight(-15.0f * dt);
		}
		if(glfwGetKey('A')){
			gCamera.MoveRight(+15.0f * dt);
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
		if(glfwGetKey('-')){
			LoadedEntity->MoveForward(-4.5 * dt);
		}
		if(glfwGetKey('=')){
			LoadedEntity->MoveForward(4.5 * dt);		}
	}
	
	if (gCarot != NULL) {
		if (gCarot == txtSteps) {
			mesh.SetStepCount(lstFrames->GetSelectedIndex(), txtSteps->GetTextAsFloat());
			object->GetAnimationInfo();
			
		}
		else if (gCarot == txtSpeed) {
			mesh.speed = txtSpeed->GetTextAsFloat();
			object->GetAnimationInfo();
		}
		else if (gCarot == txtScale) {
			mesh.scale[0] = txtScale->GetTextAsFloat();
			mesh.scale[1] = txtScale->GetTextAsFloat();
			mesh.scale[2] = txtScale->GetTextAsFloat();
			
			object->GetAnimationInfo();
		}
	}
	
	if(canvas->Update()) {
		
		
		
		if (gMouse != NULL) {
			if (gMouse == btnExport) {
				ExportMesh();
				dialog->Show();
			}
			if (gMouse == btnOk) {
				dialog->Hide();
			}
			if (gMouse == chkRepeat) {
				mesh.repeat = chkRepeat->GetChecked();
				object->GetAnimationInfo();
			}
			if (gMouse == btnLoadFile) {
				if (lstFiles->GetCount() > 0) {
					
					if (lstFrames->GetCount() == 0) {
#ifdef __APPLE__
						mesh.AddObjModel(lstFiles->GetSelectedItem().c_str(),"../../../");
#else
						mesh.AddObjModel(lstFiles->GetSelectedItem().c_str(),"");
#endif
						lstFrames->AddItem(lstFiles->GetSelectedItem());
					} else {
						mesh.AddObjModel(lstFiles->GetSelectedItem().c_str(),"../../../", false);
						lstFrames->AddItem(lstFiles->GetSelectedItem());
						object->HasAnimation(true);
						
						float stepcount = mesh.GetStepCount(lstFrames->GetCount() -1);
						stringstream ss (stringstream::in | stringstream::out);
						ss << stepcount;
						txtSteps->SetText(ss.str().c_str());
						
						object->GetAnimationInfo();
					}
				}
			}
			if (gMouse == lstFrames) {
				float stepcount = mesh.GetStepCount(lstFrames->GetSelectedIndex());
				stringstream ss (stringstream::in | stringstream::out);
				ss << stepcount;
				txtSteps->SetText(ss.str().c_str());
			}
			if (gMouse == btnPlay) {
				object->StartAnimation(true);
			}
			else if (gMouse == btnStop) {
				object->StopAnimation();
			}
			if (gMouse == btnImport) {
				if (!imported) {
					//Import
					imported = true;
#ifdef __APPLE__
					LoadedMesh.ImportMesh(txtImport->GetText(),"../../../");
#else
					LoadedMesh.ImportMesh(txtImport->GetText(),"");
#endif
					LoadedEntity->RefreshMesh();
					gCarot = NULL;
				}
			}
			
		gMouse = NULL;
		}
	//------------------------------------	
	} else {
		
	HandleSpecialKeyInput();
	}
	
	
}

void Game::CreateUI() {
	//1280 - 800
	canvas = new UICanvas(UIRect(0,0,gWidth,gHeight));
	UIWidget *form;
	UIWidget *label;
	UIWidget *list;
	UIWidget *button;
	UIWidget *txt;
	UIWidget *chk;
	
	int window_x_offset = 1200;
	//int subwindow_height = 750;
	
	form = new UIWindow(canvas);
	form->SetBounds(UIRect(window_x_offset,0,238,100));
	reinterpret_cast<UIWindow*>( form)->SetTitle("Export Model");
	form->SetDragable(false);

	label = new UILabel(form);
	label->SetPos(UIPos(10,20));
	label->SetText("FileName:");
	
	txt = new UITextbox(form);
	txtFile = reinterpret_cast<UITextbox*> (txt);
	txtFile->SetText("");
	txtFile->SetBounds(UIRect(100,20,120));
	
	button = new UIButton(form, "Export Model");
	btnExport = reinterpret_cast<UIButton*> (button);
	btnExport->SetBounds(UIRect(100, 50, 120, 20));
	//====================================
	
	
	
	///--------
	form = new UIWindow(canvas);
	mainForm = reinterpret_cast<UIWindow*>(form);
	mainForm->SetBounds(UIRect(window_x_offset,100,238,430));
	mainForm->SetDragable(false);
	mainForm->SetTitle("Frame Info");
	
	
	label = new UILabel(form);
	label->SetPos(UIPos(10,5));
	label->SetText("Files");
	
	list = new UIListbox(form);
	lstFiles = reinterpret_cast<UIListbox*> (list);
	lstFiles->SetBounds(UIRect(10,30, 200));
	lstFiles->SetViewCount(10);
	
	button = new UIButton(form, "Load Selected Model");
	btnLoadFile = reinterpret_cast<UIButton*> (button);
	btnLoadFile->SetBounds(UIRect(10,190,200,20));
	
	
	
	label = new UILabel(form);
	label->SetPos(UIPos(10,210));
	label->SetText("---------Frames----------");

	
	list = new UIListbox(form);
	lstFrames = reinterpret_cast<UIListbox*> (list);
	lstFrames->SetBounds(UIRect(10,230, 200));
	lstFrames->SetViewCount(8);
	
	
		
	label = new UILabel(form);
	label->SetPos(UIPos(10,350));
	label->SetText("Steps in Selected Frame");
	
	
	txt = new UITextbox(form);
	txtSteps = reinterpret_cast<UITextbox*> (txt);
	txtSteps->SetText("");
	txtSteps->SetBounds(UIRect(10,370,200));
	
	///-----------------------------------------------
	form = new UIWindow(canvas);
	form->SetBounds(UIRect(window_x_offset,530,238,300));
	reinterpret_cast<UIWindow*>( form)->SetTitle("Global Options");
	form->SetDragable(false);
	
	chk = new UICheckbox(form, "Loop Animation");
	chkRepeat = reinterpret_cast<UICheckbox*> (chk);
	chkRepeat->SetBounds(UIRect(10, 10));
	chkRepeat->SetValue(false);
	
	label = new UILabel(form);
	label->SetPos(UIPos(10,30));
	label->SetText("Scale");
	
	txt = new UITextbox(form);
	txtScale = reinterpret_cast<UITextbox*> (txt);
	txtScale->SetText("1.0");
	txtScale->SetBounds(UIRect(10,50,200));	

	
	
	label = new UILabel(form);
	label->SetPos(UIPos(10,70));
	label->SetText("Delay (Global value)");
	
	txt = new UITextbox(form);
	txtSpeed = reinterpret_cast<UITextbox*> (txt);
	txtSpeed->SetText("2");
	txtSpeed->SetBounds(UIRect(10,90,200));	
	
	
	
	
	button = new UIButton(form, "Play");
	btnPlay = reinterpret_cast<UIButton*> (button);
	btnPlay->SetBounds(UIRect(10,220,80,40));
	
	button = new UIButton(form, "Stop");
	btnStop = reinterpret_cast<UIButton*> (button);
	btnStop->SetBounds(UIRect(120,220,80,40));
	
	
	form = new UIWindow(canvas);
	dialog = reinterpret_cast<UIWindow*>(form);
	dialog->SetBounds(UIRect(550,100,300,60));
	dialog->SetDragable(false);
	dialog->SetTitle("Export Complete");
	dialog->Hide();
	
	button = new UIButton(form, "Ok");
	btnOk = reinterpret_cast<UIButton*> (button);
	btnOk->SetBounds(UIRect(240,15,50,25));
	
	
	importForm = new UIWindow(canvas);
	importForm->SetBounds(UIRect(890,0,300,60));
	importForm->SetDragable(false);
	importForm->SetTitle("Import SPModel");
	
	txt = new UITextbox(importForm);
	txtImport = reinterpret_cast<UITextbox*> (txt);
	txtImport->SetText("");
	txtImport->SetBounds(UIRect(20,10,150));	
	
	button = new UIButton(importForm, "Import");
	btnImport = reinterpret_cast<UIButton*> (button);
	btnImport->SetBounds(UIRect(180,5,70,25));
	
}


void Game::ExportMesh() {
	string filename = txtFile->GetText();
#ifdef __APPLE__
	string fullpath = "../../../" + filename;
#else
	string fullpath = filename;
#endif
	mesh.ExportMesh(fullpath);
	
	

}