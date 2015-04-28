/*
 *  NewCharacterScreen.cpp
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 7/8/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */
#include "game.h"

void Game::InitNewCharacterScreen() {
	
//---- Load Classes from Database
	string sql = "SELECT class_id,name,role FROM classes";
	if (dbGame->Query(sql)) {
		for ( int i =0; i < dbGame->total_rows; ++i) {
			classes[dbGame->GetInteger(i, 0)] = dbGame->GetValue(i, 1);
			roles[dbGame->GetInteger(i, 0)] = dbGame->GetValue(i, 2);
			db_ids.push_back(dbGame->GetInteger(i,0));
		}
	}
	
//---- Spell Schools
	// Create a hash of all classes schools
	// school1[class] = school_name
	// school2[class] = school_name
	// school3[class] = school_name
	if (dbGame->Query("SELECT distinct school_id,class_id,school FROM Spells")) {
		for ( int i =0; i < dbGame->total_rows; ++i) {
			std::string school_id = dbGame->GetValue(i,0);
			if(school_id == "1") {
				school1[dbGame->GetInteger(i, 1)] = dbGame->GetValue(i, 2);
			} else if(school_id == "2") {
				school2[dbGame->GetInteger(i, 1)] = dbGame->GetValue(i, 2);
			} else if(school_id == "3") {
				school3[dbGame->GetInteger(i, 1)] = dbGame->GetValue(i, 2);
			}
			
		}
	}
	
//---- UI
	int sideLength = 300;
	int button_height = 30;
	
	new_canvas = new UICanvas(UIRect(0,0,gWidth,gHeight));
	
	new_form = new UIWindow(new_canvas);
	new_form->SetBounds(UIRect(gWidth - sideLength - 10 , 50, sideLength, 700));
	new_form->SetTitle("Choose Class");
	
	UIWidget *label = new UILabel(new_form);
	label->SetText("Name");
	label->SetPos(UIPos(20, 20));
	
	txtName = new UITextbox(new_form);
	txtName->SetBounds(UIRect(20, 40, sideLength-40));
	
	UIWidget *button;
	for (int i =0; i < db_ids.size(); ++i) {
		button = new UIButton(new_form, classes[db_ids[i]]);
		class_buttons.push_back(reinterpret_cast<UIButton*>(button));
		button->SetBounds(UIRect(20, 100 + (40 * i), sideLength-40, button_height ));
	}
	
	label = new UILabel(new_form);
	label->SetText("Selected:");
	label->SetPos(UIPos(20, 270));
	
	classSelected = new UILabel(new_form);
	classSelected->SetText("");
	classSelected->SetPos(UIPos(100, 270));
	classSelected->SetFontColor(kNone, UIColor(0,255,0,255));
	
	label = new UILabel(new_form);
	label->SetText("Roll:");
	label->SetPos(UIPos(20, 290));
	
	lblRoll = new UILabel(new_form);
	lblRoll->SetFontColor(kNone, UIColor(155,250,200,255));
	lblRoll->SetText("");
	lblRoll->SetPos(UIPos(70, 290));
	
	button = new UIButton(new_form, "Create Character");
	btnCreate = reinterpret_cast<UIButton*> (button);
	btnCreate->SetBounds(UIRect(20, 660,sideLength-40,button_height));
	
	label = new UILabel(new_form);
	label->SetText("Primary School:");
	label->SetPos(UIPos(20, 320));
	
	primary[0] = new UICheckbox(new_form,"");
	primary[0]->SetBounds(UIRect(20,350));
	
	primary[1] = new UICheckbox(new_form,"");
	primary[1]->SetBounds(UIRect(20,370));
	primary[1]->SetValue(false);
	
	primary[2] = new UICheckbox(new_form,"");
	primary[2]->SetBounds(UIRect(20,390));
	primary[2]->SetValue(false);
	
	label = new UILabel(new_form);
	label->SetText("Secondary School:");
	label->SetPos(UIPos(20, 420));
	
	secondary[0] = new UICheckbox(new_form,"");
	secondary[0]->SetBounds(UIRect(20,450));
	secondary[0]->SetValue(false);
	
	secondary[1] = new UICheckbox(new_form,"");
	secondary[1]->SetBounds(UIRect(20,470));
	
	secondary[2] = new UICheckbox(new_form,"");
	secondary[2]->SetBounds(UIRect(20,490));
	secondary[2]->SetValue(false);
	
	// Pop Up Window
	createDialog = new UIWindow(new_canvas);
	createDialog->SetTitle("Character Created");
	createDialog->SetBounds(UIRect((gWidth - 300) /2, 100, 300, 40));
	
	btnOk = new UIButton(createDialog,"Ok");
	btnOk->SetBounds(UIRect(250,10,40,20));
	
	createDialog->Hide();
	
//---- Selected Class Id
	newClass = "0";
}

void Game::ToggleGroup(UICheckbox *options[], int index, int size ) {
	for (int i =0; i < size; ++i) {
		options[i]->SetValue(false);
	}
	options[index]->SetValue(true);
	
}

void Game::DeleteNewCharacterScreen() {
	if (new_canvas != NULL) {
		delete new_canvas;
	}
}

void Game::UpdateNewCharacterScreen() {
	new_canvas->Update();
	for (int i =0; i < class_buttons.size(); ++i) {
		if (gMouse == class_buttons[i] ) {
			classSelected->SetText(classes[db_ids[i]]);
			lblRoll->SetText(roles[db_ids[i]]);
			newClass = IntToString(db_ids[i]);
			primary[0]->SetText(school1[atoi(newClass.c_str())]);
			primary[1]->SetText(school2[atoi(newClass.c_str())]);
			primary[2]->SetText(school3[atoi(newClass.c_str())]);
			
			secondary[0]->SetText(school1[atoi(newClass.c_str())]);
			secondary[1]->SetText(school2[atoi(newClass.c_str())]);
			secondary[2]->SetText(school3[atoi(newClass.c_str())]);
			
			
		}
	}
	
	//Check for toggle on checkbox groups
	for (int i =0; i < 3; ++i) {
		if (gMouse == primary[i]) {
			ToggleGroup(primary, i, 3);
		} else if (gMouse == secondary[i]) {
			ToggleGroup(secondary, i, 3);
		}
		
	
	}

	
	if (gMouse == btnCreate) {
		
		std::string school1_title, school2_title;
		for (int i =0; i < 3; ++i) {
			if (primary[i]->GetChecked()) {
				if (i == 0) {
					school1_title = school1[atoi(newClass.c_str())];
				} else if (i == 1) {
					school1_title = school2[atoi(newClass.c_str())];
				} else if (i == 2) {
					school1_title = school3[atoi(newClass.c_str())];
				}
				
			}
			
			if (secondary[i]->GetChecked()) {
				if (i == 0) {
					school2_title = school1[atoi(newClass.c_str())];
				} else if (i == 1) {
					school2_title = school2[atoi(newClass.c_str())];
				} else if (i == 2) {
					school2_title = school3[atoi(newClass.c_str())];
				}
				
			}
		}
		
		if (txtName->GetText() != "" && newClass != "0" && school1_title != school2_title) {
			string sql = "INSERT INTO Players(class,name,level, exp, exp_max,posx, posy,posz, school1, school2, hp, mp) VALUES (" + newClass + ",\"" + txtName->GetText() + "\",1,0,10,-13,4.7,22,\""+ school1_title + "\",\"" + school2_title + "\",1000,100)";
			dbPlayers->Query(sql);
			new_form->Hide();
			createDialog->Show();
			gCarot = NULL;
		}
	}
	else if (gMouse == btnOk) {
		next_screen = CharacterScreen;
	}
	gMouse = NULL;
	
}

void Game::DrawNewCharacterScreen() {
	
}

void Game::Draw2DNewCharacterScreen() {
	new_canvas->Render();
}