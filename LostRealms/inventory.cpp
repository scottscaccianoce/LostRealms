/*
 *  inventory.cpp
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 12/27/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */


#include "game.h"
#include "item.h"

void Game::GetInventoryFromDatabase() {

	if (dbPlayers->Query("Select item_id from Inventory where player_id = " + db_player_id)) {
		for (int i =0; i < dbPlayers->total_rows; ++i) {
			int item_id = dbPlayers->GetInteger(i, 0);
			AddItemToInventory(item_id);
		}
	}
}

// This will add an item to your inventory but not the database.  Good for intial startup and helper function to new item function below
void Game::AddItemToInventory(int item_id) {
	if ( dbGame->Query("Select * from items where item_id = " + IntToString(item_id)) ) {
		if (dbGame->total_rows > 0) {
			
			Item *item = new Item();
			item->db_id = dbGame->GetValue(0,0);
			item->req_level = dbGame->GetInteger(0, 1);
			item->name = dbGame->GetValue(0, 2);
			item->value = dbGame->GetInteger(0, 3);
			item->icon_id = dbGame->GetInteger(0,4);
			item->position = dbGame->GetInteger(0, 5);
			item->attack = dbGame->GetFloat(0, 6);
			item->class_id = dbGame->GetInteger(0, 7);
			item->mInt = dbGame->GetInteger(0, 8);
			item->mStr = dbGame->GetInteger(0, 9);
			item->mAgi = dbGame->GetInteger(0, 10);
			item->mVit = dbGame->GetInteger(0, 11);
			item->mWis = dbGame->GetInteger(0, 12);
			item->mDef = dbGame->GetInteger(0, 13);
			
			inventory.push_back(item);
		}
	}
}

// When you aquire an item from a quest or mob you need to call this funtion.  It will add item to your inventory and database
void Game::AddNewItemToInventory(int item_id) {
	AddItemToInventory(item_id);
	dbPlayers->Query("INSERT INTO Inventory (item_id, player_id) VALUES (" + IntToString(item_id) + "," +db_player_id + ")");
}


// We sync the window with your inventory
void Game::RefreshInventoryWindow() {
	lstInventory->Clear();
	
	for (int i =0; i < inventory.size(); ++i) {
		lstInventory->AddItemWithIcon(inventory[i]->name, item_textures[inventory[i]->icon_id - 1]);
	}
	
	RefreshInventoryStats();

}

//Update the stats when you click on an item
void Game::RefreshInventoryStats() {
	if (lstInventory->GetCount() > 0) {
		int selected = lstInventory->GetSelectedIndex();
		invValue->SetText(IntToString(inventory[selected]->value));
		invClass->SetText(classIdsToName[inventory[selected]->class_id]);
		invLevel->SetText(IntToString(inventory[selected]->req_level));
		invInt->SetText(IntToString(inventory[selected]->mInt));
		invStr->SetText(IntToString(inventory[selected]->mStr));
		invAgi->SetText(IntToString(inventory[selected]->mAgi));
		invVit->SetText(IntToString(inventory[selected]->mVit));
		invWis->SetText(IntToString(inventory[selected]->mWis));
		invDef->SetText(IntToString(inventory[selected]->mDef));
		invAttack->SetText(IntToString(inventory[selected]->attack));		


		
		
		
	}
}

void Game::RemoveItemFromInventory(int item_id) {

	int size = inventory.size();
	int index = -1;
	
	for (int i =0; i < size; ++i) {
		if (atoi(inventory[i]->db_id.c_str()) == item_id ) {
			index = i;
			break;
		}
	}
	
	if (index != -1) {
		if (inventory.size() >= index + 1) {
			std::string sql = "Delete from Inventory where inventory_id in (SELECT inventory_id from Inventory where item_id =   " + inventory[index]->db_id + " limit 1)";
			dbPlayers->Query(sql);
			inventory.erase(inventory.begin() + index);
		}
	}
	
	
}