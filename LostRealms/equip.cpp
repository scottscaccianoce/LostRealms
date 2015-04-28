/*
 *  equip.cpp
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 12/28/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "item.h"
#include "game.h"


void Game::RefreshEquipWindow() {
	lstEquip->Clear();
	EqpToInv.clear();
	
	for (int i =0; i < inventory.size(); ++i) {
		if (inventory[i]->req_level >= players[PlayerId].level &&
			 (inventory[i]->class_id == players[PlayerId].class_id || inventory[i]->class_id == 0) &&
			  inventory[i]->position == selectedEquipIndex) 
		{
			lstEquip->AddItemWithIcon(inventory[i]->name, item_textures[inventory[i]->icon_id - 1]);
			EqpToInv.push_back(inventory[i]);
		}
		
	}
	RefreshEquipmentNames();
	RefreshEquipStats();
	RefreshEquipCurrentStats();
	
}

void Game::RefreshEquipStats() {
	if (lstEquip->GetCount() > 0) {
		int selected = lstEquip->GetSelectedIndex();
		eqpInt->SetText(IntToString(EqpToInv[selected]->mInt));
		eqpStr->SetText(IntToString(EqpToInv[selected]->mStr));
		eqpAgi->SetText(IntToString(EqpToInv[selected]->mAgi));
		eqpVit->SetText(IntToString(EqpToInv[selected]->mVit));
		eqpWis->SetText(IntToString(EqpToInv[selected]->mWis));
		eqpDef->SetText(IntToString(EqpToInv[selected]->mDef));
		eqpAttack->SetText(IntToString(EqpToInv[selected]->attack));			
	} else {
		eqpInt->SetText("");
		eqpStr->SetText("");
		eqpAgi->SetText("");
		eqpVit->SetText("");
		eqpWis->SetText("");
		eqpDef->SetText("");
		eqpAttack->SetText("");
	}

	
}

void Game::ClearEquipBorders() {
	
	UIColor itemborder = UIColor(195,187,172);
	uHead->SetBorderColor(kNone,itemborder);
	uChest->SetBorderColor(kNone, itemborder);
	uHands->SetBorderColor(kNone, itemborder);
	uLegs->SetBorderColor(kNone, itemborder);
	uFeet->SetBorderColor(kNone, itemborder);
	uNecklace->SetBorderColor(kNone, itemborder);
	uRing->SetBorderColor(kNone, itemborder);
	uMainWeapon->SetBorderColor(kNone, itemborder);
	uOffWeapon->SetBorderColor(kNone, itemborder);
}
void Game::HighlightSelectedEquipmentPosition() {
	if (gMouse == uHead) {
		selectedEquipIndex = 1;
		ClearEquipBorders();
		uHead->SetBorderColor(kNone, UIColor(255,0,0));
		RefreshEquipWindow();
	} else if (gMouse == uChest) {
		selectedEquipIndex = 2;
		ClearEquipBorders();
		uChest->SetBorderColor(kNone, UIColor(255,0,0));
		RefreshEquipWindow();
	} else if (gMouse == uHands) {
		selectedEquipIndex = 3;
		ClearEquipBorders();
		uHands->SetBorderColor(kNone, UIColor(255,0,0));
		RefreshEquipWindow();
	} else if (gMouse == uLegs) {
		selectedEquipIndex = 4;
		ClearEquipBorders();
		uLegs->SetBorderColor(kNone, UIColor(255,0,0));
		RefreshEquipWindow();
	} else if (gMouse == uFeet) {
		selectedEquipIndex = 5;
		ClearEquipBorders();
		uFeet->SetBorderColor(kNone, UIColor(255,0,0));
		RefreshEquipWindow();
	} else if (gMouse == uNecklace) {
		selectedEquipIndex = 6;
		ClearEquipBorders();
		uNecklace->SetBorderColor(kNone, UIColor(255,0,0));
		RefreshEquipWindow();
	} else if (gMouse == uRing) {
		selectedEquipIndex = 7;
		ClearEquipBorders();
		uRing->SetBorderColor(kNone, UIColor(255,0,0));
		RefreshEquipWindow();
	} else if (gMouse == uMainWeapon) {
		selectedEquipIndex = 8;
		ClearEquipBorders();
		uMainWeapon->SetBorderColor(kNone, UIColor(255,0,0));
		RefreshEquipWindow();
	} else if (gMouse == uOffWeapon) {
		selectedEquipIndex = 9;
		ClearEquipBorders();
		uOffWeapon->SetBorderColor(kNone, UIColor(255,0,0));
		RefreshEquipWindow();
	}

}

void Game::RefreshEquipCurrentStats() {
	Item *ptr = &equipment[selectedEquipIndex - 1];
	eqpCurrentInt->SetText(IntToString(ptr->mInt));
	eqpCurrentStr->SetText(IntToString(ptr->mStr));
	eqpCurrentAgi->SetText(IntToString(ptr->mAgi));
	eqpCurrentVit->SetText(IntToString(ptr->mVit));
	eqpCurrentWis->SetText(IntToString(ptr->mWis));
	eqpCurrentDef->SetText(IntToString(ptr->mDef));
	eqpCurrentAttack->SetText(IntToString(ptr->attack));		
	
}

void Game::RefreshEquipmentNames() {
	
	for (int i =0; i < 9; ++i) {
		if (equipment[i].db_id != "0") {
			equipNames[i]->SetText(equipment[i].name);
		}
	}
	
}


void Game::EquipItem() {
	
	
	
	//Check if an item is already equipped in that position
	int equipmentIndex = selectedEquipIndex - 1;
	int selected = lstEquip->GetSelectedIndex();
	
	if (lstEquip->GetCount() > 0) {
		
		if (equipment[equipmentIndex].db_id != "0" ) {
			//Add it back to the inventory
			AddNewItemToInventory(atoi(equipment[equipmentIndex].db_id.c_str()));
		}
		
		//update Equip table and array
		dbPlayers->Query("INSERT OR REPLACE INTO Equiped (player_id,item_id,Position) VALUES("+db_player_id+"," + EqpToInv[selected]->db_id + "," + IntToString(EqpToInv[selected]->position)  + ")");
		
		//set up Equipment
		equipment[equipmentIndex].db_id = EqpToInv[selected]->db_id;
		equipment[equipmentIndex].mInt	= EqpToInv[selected]->mInt;
		equipment[equipmentIndex].mStr = EqpToInv[selected]->mStr;
		equipment[equipmentIndex].mAgi = EqpToInv[selected]->mAgi;
		equipment[equipmentIndex].mVit = EqpToInv[selected]->mVit;
		equipment[equipmentIndex].mWis = EqpToInv[selected]->mWis;
		equipment[equipmentIndex].mDef = EqpToInv[selected]->mDef;
		equipment[equipmentIndex].attack = EqpToInv[selected]->attack;
		equipment[equipmentIndex].name = EqpToInv[selected]->name;
		equipment[equipmentIndex].class_id = EqpToInv[selected]->class_id;
		equipment[equipmentIndex].value	= EqpToInv[selected]->value;
		equipment[equipmentIndex].req_level = EqpToInv[selected]->req_level;
		
		RemoveItemFromInventory(atoi(EqpToInv[selected]->db_id.c_str()));
		RefreshEquipWindow();
		UpdatePlayerStats();
	}
}

void Game::GetEquipFromDatabase() {
	if (dbPlayers->Query("Select item_id, position from Equiped where player_id = " + db_player_id)) {
		for (int i =0; i < dbPlayers->total_rows; ++i) {
			int item_id = dbPlayers->GetInteger(i, 0);
			int position = dbPlayers->GetInteger(i, 1) - 1;
			
			if (dbGame->Query("Select * from Items where item_id = " + IntToString(item_id))) {
				
			
				
				equipment[position].db_id = dbGame->GetValue(0, 0);
				equipment[position].req_level = dbGame->GetInteger(0, 1);
				equipment[position].name = dbGame->GetValue(0, 2);
				equipment[position].value	= dbGame->GetInteger(0, 3);
				equipment[position].icon_id = dbGame->GetInteger(0, 4);
				equipment[position].position = dbGame->GetInteger(0, 5);
				equipment[position].attack = dbGame->GetFloat(0, 6);
				equipment[position].class_id = dbGame->GetInteger(0, 7);
				
				
				equipment[position].mInt = dbGame->GetInteger(0, 8);
				equipment[position].mStr = dbGame->GetInteger(0, 9);
				equipment[position].mAgi = dbGame->GetInteger(0, 10);
				equipment[position].mVit = dbGame->GetInteger(0, 11);
				equipment[position].mWis = dbGame->GetInteger(0, 12);
				equipment[position].mDef = dbGame->GetInteger(0, 13);
				
				
			}
		}
	}
	
}
