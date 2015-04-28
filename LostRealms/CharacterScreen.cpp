/*
 *  CharacterScreen.cpp
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 7/8/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#define NUM_PLAYERS 4

#include "game.h"

void Game::InitCharacterScreen() {
	
//---- Initalize Player ID
	PlayerId = 0;
	
//---- UI
	int sideLength = 300;
	char_canvas = new UICanvas(UIRect(0,0,gWidth,gHeight));
	UIWindow *form = new UIWindow(char_canvas);
	form->SetBounds(UIRect(gWidth - sideLength - 10 , 50, sideLength, 500));
	form->SetTitle("Characters");
	
	lstCharacters = new UIListbox(form);
	lstCharacters->SetViewCount(10);
	lstCharacters->SetBounds(UIRect(10,50,sideLength - 20));
	
	btnHost = new UIButton(form, "Host");
	btnHost->SetBounds(UIRect(10, 300,sideLength/2 - 15,30));
	
	btnJoin = new UIButton(form, "Join");
	btnJoin->SetBounds(UIRect(20 + sideLength/2 - 15 , 300,sideLength/2 - 15,30));
	
	btnNewChar = new UIButton(form, "New Character");
	btnNewChar->SetBounds(UIRect(10, 350,sideLength - 20,30));
	
	btnDelete = new UIButton(form, "Delete Character");
	btnDelete->SetBounds(UIRect(10, 450,sideLength - 20,20));
	
//---- Load Characters from Database
	if(!dbPlayers->Query("Select player_id, class, name from Players;")) {
		cout << "Failed to find players" << dbPlayers->GetError();
	}
	
	for (int i = 0; i < dbPlayers->total_rows; ++i) {
		player_ids.push_back(dbPlayers->results[i][0]);
		class_ids.push_back(dbPlayers->results[i][1]);
		names.push_back(dbPlayers->results[i][2]);
		//lstCharacters->AddItem(dbPlayers->results[i][2]);
		
		lstCharacters->AddItemWithIcon(dbPlayers->results[i][2], &tClasses[dbPlayers->GetInteger(i, 1) -1]);
	}
	
//----Load Character Title Screen Image
	characterBackground = new sTexture();
	characterBackground->loadTexture("charscreen.tga");
	charBG.Move(0.0f,0.0f);
	charBG.SetSide(2048.0f);
	charBG.SetTexture(characterBackground);
	
}
void Game::DeleteCharacterScreen() {
	if (current_screen == CharacterScreen) {
		delete char_canvas;	
	}
	delete characterBackground;
	player_ids.clear();
	class_ids.clear();
	names.clear();	
}
void Game::UpdateCharacterScreen() {
	
	char_canvas->Update();
	
	if (gMouse == btnHost) {
		//check if there is something selected;
		if (lstCharacters->GetCount() > 0) {
			
			// Initalize Host as Server
			nHost = new NetworkHost(nServer,NUM_PLAYERS);
			
			// Get Data from Player selected  
			int selected_index = lstCharacters->GetSelectedIndex();
			player_name = names[selected_index];
			db_player_id = player_ids[selected_index];
			dbPlayers->Query("Select posx,posy,posz,level,class, hp, mp, exp,exp_max from Players where player_id = " + db_player_id);
			std::string class_id = dbPlayers->results[0][4];
			
			//Get Class Mesh Data from Player selected
			dbGame->Query("Select mesh_id, attack_mesh_id from Classes where class_id = " + class_id);
			int mesh_id = dbGame->GetInteger(0,0);
			int attack_mesh_id = dbGame->GetInteger(0, 1);
			
			//Set Host player (0) with our Player data
			players[0].connected = true;
			players[0].level = atoi(dbPlayers->results[0][3].c_str());
			players[0].SetObject(meshes[mesh_id - 1]);
			players[0].SetAttackingObject(meshes[attack_mesh_id - 1]);

			players[0].object->SetOrigin(atof(dbPlayers->results[0][0].c_str()),atof(dbPlayers->results[0][1].c_str()),atof(dbPlayers->results[0][2].c_str()));
			float height;
			land->GetHeightAtPos(players[0].object->GetOriginX(), players[0].object->GetOriginY(), players[0].object->GetOriginZ(), height );
			players[0].object->SetOriginY(height);
			
			players[0].class_id = atoi(class_id.c_str());
			players[0].SetStartingStats();
			players[0].object->GetAnimationInfo();
			players[0].attackingObject->GetAnimationInfo();
			players[0].attackingObject->ShouldRepeat(false);
			players[0].IsCurrentPlayer = true;
			players[0].player_id = 0;
			players[0].hp = dbPlayers->GetInteger(0, 5);
			players[0].mp = dbPlayers->GetInteger(0, 6);
			players[0].exp = dbPlayers->GetInteger(0, 7);
			players[0].exp_max = gLevels[atoi(dbPlayers->results[0][3].c_str())];
			players[0].db_id = db_player_id;
			UpdatePlayerStats();
			
			
			//Set our 6 Player spells according to the 2 schools of skills the player has configured with this character
			if(dbPlayers->Query("Select school1,school2 from Players where player_id == " + db_player_id)) {
				std::string primary = dbPlayers->GetValue(0,0);
				std::string secondary = dbPlayers->GetValue(0,1);
				int spellcounter =0;
				for (int i =0; i < 36; ++i) {
					if (spells[i].school == primary || spells[i].school == secondary) {
						player_spells[spellcounter++] = &spells[i];
					}
				}
				
			}
			
			//Set next screen to playscreen to start game
			next_screen = PlayScreen;
		}
	}
	
	else if (gMouse == btnJoin) {
		//check if there is something selected;
		if (lstCharacters->GetCount() > 0) {
			
			// Initialize Host as Client
			nHost = new NetworkHost(nClient);
			if (nHost->connected) {
				
				// Get Player data from database
				int selected_index = lstCharacters->GetSelectedIndex();
				player_name = names[selected_index];
				db_player_id = player_ids[selected_index];

				//Set our 6 Player spells according to the 2 schools of skills the player has configured with this character
				if(dbPlayers->Query("Select school1,school2 from Players where player_id == " + db_player_id)) {
					std::string primary = dbPlayers->GetValue(0,0);
					std::string secondary = dbPlayers->GetValue(0,1);
					int spellcounter =0;
					for (int i =0; i < 36; ++i) {
						if (spells[i].school == primary || spells[i].school == secondary) {
							player_spells[spellcounter++] = &spells[i];
						}
					}
					
				}
			}
			
			//Set next screen to playscreen to start game
			next_screen = PlayScreen;
		}
	}
	
	else if (gMouse == btnNewChar) {
		next_screen = NewCharacterScreen;
		
	}
	gMouse = NULL;
}

void Game::DrawCharacterScreen() {
	
}

void Game::Draw2DCharacterScreen() {
	glEnable(GL_TEXTURE_2D);
	charBG.Render();
	glDisable(GL_TEXTURE_2D);
	char_canvas->Render();
	
}
