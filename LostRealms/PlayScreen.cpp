/*
 *  PlayScreen.cpp
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 7/8/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "game.h"

void Game::InitPlayScreen(){
	
	
	
//---- UI Color Settings
	UIColor formbg = UIColor(99,88,74);
	UIColor formcaption = UIColor(206,189,170);
	UIColor formborder = UIColor(87,73,41);
	UIColor formText = UIColor(255,255,255);
	

	play_canvas = new UICanvas(UIRect(0,0,gWidth,gHeight));
	
//---- Chat Form
	starting_toggle =false;
	chat_toggle = false;
	ending_toggle = false;
	int chatHeight = 185;
	int chatWidth = 500;
	chat_form = new UIWindow(play_canvas);
	chat_form->SetBounds(UIRect(2 , gHeight - chatHeight - 10, chatWidth, chatHeight +10));
	chat_form->SetTitle("Chat");
	chat_form->DisplayTitle(false);
	chat_form->SetBgColor(kNone, formbg);
	chat_form->SetCaptionColor(kNone, formcaption);
	chat_form->SetBorderColor(kNone, formborder);
	txtPrompt = new UITextbox(chat_form);
	txtPrompt->SetBounds(UIRect(5, chatHeight - 14, chatWidth - 10));
	txtPrompt->SetBgColor(kNone, UIColor(0,0,0));
	txtPrompt->SetFontColor(kNone, UIColor(255,255,255));
	txtPrompt->SetBorderColor(kNone, UIColor(163,182,230));
	log = new UIListbox(chat_form);
	log->SetViewCount(10);
	log->SetBounds(UIRect(5,17,chatWidth - 25 ));
	log->SetChatMode(true);
	
	int combatHeight = 115;
	combatWindow = new UIWindow(play_canvas);
	combatWindow->SetBounds(UIRect(gWidth - chatWidth , gHeight - combatHeight - 5, chatWidth, combatHeight +2));
	combatWindow->SetTitle("Combat");
	combatWindow->DisplayTitle(false);
	combatWindow->SetBgColor(kNone, formbg);
	combatWindow->SetCaptionColor(kNone, formcaption);
	combatWindow->SetBorderColor(kNone, formborder);
	combatLog = new UIListbox(combatWindow);
	combatLog->SetViewCount(6);
	combatLog->SetBounds(UIRect(5,17,chatWidth - 25 ));
	combatLog->SetChatMode(true);
	
	
	
	key_pressed = "";
	sentRequstForNumber = false;
	int iconsize = 32;
	
//-- TOP Menu Icons
	wSettings = new UIImage(play_canvas, &tSettings);
	wSettings->SetBounds(UIRect(gWidth - iconsize,0,iconsize,iconsize));
	wSettings->ShowBorder(true);
	
	wGroup = new UIImage(play_canvas, &tGroup);
	wGroup->SetBounds(UIRect(gWidth - (iconsize * 2) ,0,iconsize,iconsize));
	wGroup->ShowBorder(true);
	
	wMap = new UIImage(play_canvas, &tMap);
	wMap->SetBounds(UIRect(gWidth - (iconsize * 3),0,iconsize,iconsize));
	wMap->ShowBorder(true);
	
	wQuests = new UIImage(play_canvas, &tQuests);
	wQuests->SetBounds(UIRect(gWidth - (iconsize * 4),0,iconsize,iconsize));
	wQuests->ShowBorder(true);
	
	wSpells = new UIImage(play_canvas, &tSpells);
	wSpells->SetBounds(UIRect(gWidth - (iconsize * 5),0,iconsize,iconsize));
	wSpells->ShowBorder(true);
	
	wEquip = new UIImage(play_canvas, &tEquip);
	wEquip->SetBounds(UIRect(gWidth - (iconsize * 6),0,iconsize,iconsize));
	wEquip->ShowBorder(true);
	
	wInventory = new UIImage(play_canvas, &tInventory);
	wInventory->SetBounds(UIRect(gWidth - (iconsize * 7),0,iconsize,iconsize));
	wInventory->ShowBorder(true);
	
	wStats = new UIImage(play_canvas, &tStats);
	wStats->SetBounds(UIRect(gWidth - (iconsize * 8),0,iconsize,iconsize));
	wStats->ShowBorder(true);
	
	

	//FORMS
	int sideLength = 254;
	int sideHeight = 600;
	UIColor fontColor = UIColor(255,255,255);
	UIColor chkColor = UIColor(154,211,255);
	
//---- Settings
	fSettings = new UIWindow(play_canvas);
	fSettings->SetBounds(UIRect(gWidth - sideLength -2 , 33, sideLength, sideHeight));
	fSettings->SetBgColor(kNone, formbg);
	fSettings->SetCaptionColor(kNone, formcaption);
	fSettings->SetBorderColor(kNone, formborder);
	fSettings->SetTitle("Settings");
	fSettings->SetDragable(false);
	fSettings->Hide();
	
	/*
	UILabel *temp = new UILabel(fSettings, "Address");
	temp->SetPos(UIPos(5,30));
	temp->SetFontColor(kNone, fontColor);
	selfIP = new UILabel(fSettings, "192.168.1.1");
	selfIP->SetPos(UIPos(100,30));
	selfIP->SetFontColor(kNone, fontColor);
	*/
	
	cReflection = new UICheckbox(fSettings, "Water Reflection");
	cReflection->SetBounds(UIRect(5,60));
	cReflection->SetFontColor(kNone, fontColor);
	cReflection->SetBorderColor(kNone, chkColor);
	
	UILabel *temp = new UILabel(fSettings, "Render Distance");
	temp->SetPos(UIPos(5,90));
	temp->SetFontColor(kNone, fontColor);
	renderDistance = new UISlider(fSettings);
	renderDistance->SetBounds(UIRect(140,97,100));
	
	
//---- Group
	fGroup = new UIWindow(play_canvas);
	fGroup->SetBounds(UIRect(gWidth - sideLength -2 , 33, sideLength, sideHeight));
	fGroup->SetBgColor(kNone, formbg);
	fGroup->SetCaptionColor(kNone, formcaption);
	fGroup->SetBorderColor(kNone, formborder);
	fGroup->SetTitle("Group");
	fGroup->SetDragable(false);
	fGroup->Hide();
	
//---- Map
	fMap = new UIWindow(play_canvas);
	fMap->SetBounds(UIRect(gWidth - sideLength -2 , 33, sideLength, sideHeight));
	fMap->SetBgColor(kNone, formbg);
	fMap->SetCaptionColor(kNone, formcaption);
	fMap->SetBorderColor(kNone, formborder);
	fMap->SetTitle("Map");
	fMap->SetDragable(false);
	fMap->Hide();
	
//---- Quests
	fQuests = new UIWindow(play_canvas);
	fQuests->SetBounds(UIRect(gWidth - sideLength -2 , 33, sideLength, sideHeight));
	fQuests->SetBgColor(kNone, formbg);
	fQuests->SetCaptionColor(kNone, formcaption);
	fQuests->SetBorderColor(kNone, formborder);
	fQuests->SetTitle("Quest Log");
	fQuests->SetDragable(false);
	fQuests->Hide();

//----- Spells
	fSpells = new UIWindow(play_canvas);
	fSpells->SetBounds(UIRect(gWidth - sideLength -2 , 33, sideLength, sideHeight));
	fSpells->SetBgColor(kNone, formbg);
	fSpells->SetCaptionColor(kNone, formcaption);
	fSpells->SetBorderColor(kNone, formborder);
	fSpells->SetTitle("Spellbook");
	fSpells->SetDragable(false);
	fSpells->Hide();
	
	UILabel *spellLabel;
	int spelloffset = 7;
	int spelliconwidth = 32;
	for (int i =0; i < 6; ++i) {
		spellWindowSlots[i] = new UIImage(fSpells, player_spells[i]->icon);
		spellWindowSlots[i]->SetBounds(UIRect(spelloffset, spelloffset + (i * spelloffset) + (i * spelliconwidth), spelliconwidth,spelliconwidth));
		spellWindowSlots[i]->ShowBorder(true);
		spellWindowSlots[i]->SetBorderColor(kNone, UIColor(0,0,0));
		spellLabel = new UILabel(fSpells, player_spells[i]->name);
		spellLabel->SetPos(UIPos(spelliconwidth + spelloffset * 2, spelloffset + (i * spelloffset) + (i * spelliconwidth) + 12)); //24 is to center the text
		spellLabel->SetFontColor(kNone, fontColor);
		
		
	}
	
	int startingAssignYPos = spelloffset + (6 * spelloffset) + (6 * spelliconwidth);
	spellLabel = new UILabel(fSpells, "Assign Selected to Position:");
	spellLabel->SetPos(UIPos(spelloffset, startingAssignYPos + spelloffset)); 
	spellLabel->SetFontColor(kNone, fontColor);
	
	for (int i =0; i < 6; ++i) {
		spell_assign[i] = new UIButton(fSpells, IntToString(i + 1));
		spell_assign[i]->SetBounds(UIRect(spelloffset + ((spelloffset) * i) + i * 30, startingAssignYPos + 30,30,30));
	}
	selectedSpellIndex = 0;
	spellWindowSlots[selectedSpellIndex]->SetBorderColor(kNone, UIColor(255,0,0));	
	
//---- Equipment

	fEquip = new UIWindow(play_canvas);
	fEquip->SetBounds(UIRect(gWidth - sideLength -2 , 33, sideLength, sideHeight));
	fEquip->SetBgColor(kNone, formbg);
	fEquip->SetCaptionColor(kNone, formcaption);
	fEquip->SetBorderColor(kNone, formborder);
	fEquip->SetTitle("Equipment");
	fEquip->SetDragable(false);
	fEquip->Hide();
	fEquip->SetDragable(false);
	int equipOffset = 7;
	int imagesize = 32;
	int spacing = 7;
	UIColor itemborder = UIColor(195,187,172);
	tHead = item_textures[0];
	tChest = item_textures[1];
	tHands = item_textures[2];
	tLegs = item_textures[4];
	tFeet = item_textures[3];
	tNecklace = item_textures[5];
	tRing = item_textures[6];
	
	
	for (int i =0; i < 9; ++i) {
		equipNames[i] = new UILabel(fEquip,"None");
		equipNames[i]->SetPos(UIPos(equipOffset + imagesize + equipOffset, (imagesize * i) + (spacing * i) + 13));
		equipNames[i]->SetFontColor(kNone, formText);
	}
	
	uHead = new UIImage(fEquip,tHead);
	uHead->SetBounds(UIRect(equipOffset,equipOffset + (imagesize * 0) + (spacing * 0) , imagesize,imagesize));
	uHead->SetBorderColor(kNone, UIColor(255,0,0)); // Because this is always the first we we have selected
	uHead->ShowBorder(true);
	
	uChest = new UIImage(fEquip,tChest);
	uChest->SetBounds(UIRect(equipOffset,equipOffset + (imagesize * 1) + (spacing * 1) , imagesize,imagesize));
	uChest->SetBorderColor(kNone,itemborder);
	uChest->ShowBorder(true);
	
	uHands = new UIImage(fEquip,tHands);
	uHands->SetBounds(UIRect(equipOffset,equipOffset + (imagesize * 2) + (spacing * 2) , imagesize,imagesize));
	uHands->SetBorderColor(kNone, itemborder);
	uHands->ShowBorder(true);
	
	uLegs = new UIImage(fEquip,tLegs);
	uLegs->SetBounds(UIRect(equipOffset,equipOffset + (imagesize * 3) + (spacing * 3) , imagesize,imagesize));
	uLegs->SetBorderColor(kNone, itemborder);
	uLegs->ShowBorder(true);
	
	uFeet = new UIImage(fEquip,tFeet);
	uFeet->SetBounds(UIRect(equipOffset,equipOffset + (imagesize * 4) + (spacing * 4) , imagesize,imagesize));
	uFeet->SetBorderColor(kNone,itemborder);
	uFeet->ShowBorder(true);
	
	uNecklace = new UIImage(fEquip,tNecklace);
	uNecklace->SetBounds(UIRect(equipOffset,equipOffset + (imagesize * 5) + (spacing * 5) , imagesize,imagesize));
	uNecklace->SetBorderColor(kNone, itemborder);
	uNecklace->ShowBorder(true);
	
	uRing = new UIImage(fEquip,tRing);
	uRing->SetBounds(UIRect(equipOffset,equipOffset + (imagesize * 6) + (spacing * 6) , imagesize,imagesize));
	uRing->SetBorderColor(kNone, itemborder);
	uRing->ShowBorder(true);
	
	uMainWeapon = new UIImage(fEquip,&tMainWeapon[players[PlayerId].class_id - 1]);
	uMainWeapon->SetBounds(UIRect(equipOffset,equipOffset + (imagesize * 7) + (spacing * 7) , imagesize,imagesize));
	uMainWeapon->SetBorderColor(kNone, itemborder);
	uMainWeapon->ShowBorder(true);
	
	uOffWeapon = new UIImage(fEquip,&tOffWeapon[players[PlayerId].class_id - 1]);
	uOffWeapon->SetBounds(UIRect(equipOffset,equipOffset + (imagesize * 8) + (spacing * 8) , imagesize,imagesize));
	uOffWeapon->SetBorderColor(kNone, itemborder);
	uOffWeapon->ShowBorder(true);
		
	UIWindow *currentEquipStats = new UIWindow(fEquip);
	currentEquipStats->SetBounds(UIRect(equipOffset, equipOffset + (imagesize * 9) + (spacing * 9),sideLength-(equipOffset * 2),74));
	currentEquipStats->ShowBorder(true);
	currentEquipStats->SetBorderColor(kNone, itemborder);
	currentEquipStats->DisplayTitle(false);	
	currentEquipStats->SetBgColor(kNone, formbg);
	currentEquipStats->SetFontColor(kNone, formText);
	
	lstEquip = new UIListbox(fEquip);
	lstEquip->SetBounds(UIRect(equipOffset,equipOffset + (imagesize * 11) + (spacing * 11), sideLength - (equipOffset * 4) ));
	lstEquip->SetViewCount(4);
	
	btnEquip = new UIButton(fEquip,"Equip");
	btnEquip->SetBounds(UIRect(equipOffset,500,sideLength - (equipOffset * 2),25 ));
	
	int starting_eqp_stats = 495;
	int eqp_stats_offset = 60;
	int eqp_spaccing = 2;
	
	
	UILabel *eqpTemp = new UILabel(fEquip, "Int");
	eqpTemp->SetFontColor(kNone, formText);
	eqpTemp->SetPos(UIPos(8,starting_eqp_stats + (2 * 15) + (2 * eqp_spaccing)));
	eqpInt = new UILabel(fEquip, "");
	eqpInt->SetFontColor(kNone, formText);
	eqpInt->SetPos(UIPos(eqp_stats_offset,starting_eqp_stats + (2 * 15) + (2 * eqp_spaccing)));
	
	eqpTemp = new UILabel(fEquip, "Str");
	eqpTemp->SetFontColor(kNone, formText);
	eqpTemp->SetPos(UIPos(8,starting_eqp_stats + (3 * 15) + (3 * eqp_spaccing)));
	eqpStr = new UILabel(fEquip, "");
	eqpStr->SetFontColor(kNone, formText);
	eqpStr->SetPos(UIPos(eqp_stats_offset,starting_eqp_stats + (3 * 15) + (3 * eqp_spaccing)));
	
	eqpTemp = new UILabel(fEquip, "Agi");
	eqpTemp->SetFontColor(kNone, formText);
	eqpTemp->SetPos(UIPos(8,starting_eqp_stats + (4 * 15) + (4 * eqp_spaccing)));
	eqpAgi = new UILabel(fEquip, "");
	eqpAgi->SetFontColor(kNone, formText);
	eqpAgi->SetPos(UIPos(eqp_stats_offset,starting_eqp_stats + (4 * 15) + (4 * eqp_spaccing)));
	
	int eqp_second_column = 120;
	eqpTemp = new UILabel(fEquip, "Vit");
	eqpTemp->SetFontColor(kNone, formText);
	eqpTemp->SetPos(UIPos(eqp_second_column,starting_eqp_stats + (2 * 15) + (2 * eqp_spaccing)));
	eqpVit = new UILabel(fEquip, "");
	eqpVit->SetFontColor(kNone, formText);
	eqpVit->SetPos(UIPos(eqp_stats_offset + eqp_second_column,starting_eqp_stats + (2 * 15) + (2 * eqp_spaccing)));
	
	eqpTemp = new UILabel(fEquip, "Wis");
	eqpTemp->SetFontColor(kNone, formText);
	eqpTemp->SetPos(UIPos(eqp_second_column,starting_eqp_stats + (3 * 15) + (3 * eqp_spaccing)));
	eqpWis = new UILabel(fEquip, "");
	eqpWis->SetFontColor(kNone, formText);
	eqpWis->SetPos(UIPos(eqp_stats_offset + eqp_second_column,starting_eqp_stats + (3 * 15) + (3 * eqp_spaccing)));
	
	eqpTemp = new UILabel(fEquip, "Def");
	eqpTemp->SetFontColor(kNone, formText);
	eqpTemp->SetPos(UIPos(eqp_second_column,starting_eqp_stats + (4 * 15) + (4 * eqp_spaccing)));
	eqpDef = new UILabel(fEquip, "");
	eqpDef->SetFontColor(kNone, formText);
	eqpDef->SetPos(UIPos(eqp_stats_offset + eqp_second_column,starting_eqp_stats + (4 * 15) + (4 * eqp_spaccing)));
	
	eqpTemp = new UILabel(fEquip, "Atk");
	eqpTemp->SetFontColor(kNone, formText);
	eqpTemp->SetPos(UIPos(8,starting_eqp_stats + (5 * 15) + (5 * eqp_spaccing)));
	eqpAttack = new UILabel(fEquip, "");
	eqpAttack->SetFontColor(kNone, formText);
	eqpAttack->SetPos(UIPos(eqp_stats_offset,starting_eqp_stats + (5 * 15) + (5 * eqp_spaccing)));
	
	//  Do the current equiped
	
	eqpTemp = new UILabel(currentEquipStats, "Int");
	eqpTemp->SetFontColor(kNone, formText);
	eqpTemp->SetPos(UIPos(8,(0 * 15) + (0 * eqp_spaccing)));
	eqpCurrentInt = new UILabel(currentEquipStats, "");
	eqpCurrentInt->SetFontColor(kNone, formText);
	eqpCurrentInt->SetPos(UIPos(eqp_stats_offset,  (0 * 15) + (0 * eqp_spaccing)));
	
	eqpTemp = new UILabel(currentEquipStats, "Str");
	eqpTemp->SetFontColor(kNone, formText);
	eqpTemp->SetPos(UIPos(8, (1 * 15) + (2 * eqp_spaccing)));
	eqpCurrentStr = new UILabel(currentEquipStats, "");
	eqpCurrentStr->SetFontColor(kNone, formText);
	eqpCurrentStr->SetPos(UIPos(eqp_stats_offset,  (1 * 15) + (1 * eqp_spaccing)));
	
	eqpTemp = new UILabel(currentEquipStats, "Agi");
	eqpTemp->SetFontColor(kNone, formText);
	eqpTemp->SetPos(UIPos(8,  (2 * 15) + (2 * eqp_spaccing)));
	eqpCurrentAgi = new UILabel(currentEquipStats, "");
	eqpCurrentAgi->SetFontColor(kNone, formText);
	eqpCurrentAgi->SetPos(UIPos(eqp_stats_offset,  (2 * 15) + (2 * eqp_spaccing)));
	
	eqpTemp = new UILabel(currentEquipStats, "Vit");
	eqpTemp->SetFontColor(kNone, formText);
	eqpTemp->SetPos(UIPos(eqp_second_column,  (0 * 15) + (0 * eqp_spaccing)));
	eqpCurrentVit = new UILabel(currentEquipStats, "");
	eqpCurrentVit->SetFontColor(kNone, formText);
	eqpCurrentVit->SetPos(UIPos(eqp_stats_offset + eqp_second_column,  (0 * 15) + (0 * eqp_spaccing)));
	
	eqpTemp = new UILabel(currentEquipStats, "Wis");
	eqpTemp->SetFontColor(kNone, formText);
	eqpTemp->SetPos(UIPos(eqp_second_column, (1 * 15) + (1 * eqp_spaccing)));
	eqpCurrentWis = new UILabel(currentEquipStats, "");
	eqpCurrentWis->SetFontColor(kNone, formText);
	eqpCurrentWis->SetPos(UIPos(eqp_stats_offset + eqp_second_column, (1 * 15) + (1 * eqp_spaccing)));
	
	eqpTemp = new UILabel(currentEquipStats, "Def");
	eqpTemp->SetFontColor(kNone, formText);
	eqpTemp->SetPos(UIPos(eqp_second_column, (2 * 15) + (2 * eqp_spaccing)));
	eqpCurrentDef = new UILabel(currentEquipStats, "");
	eqpCurrentDef->SetFontColor(kNone, formText);
	eqpCurrentDef->SetPos(UIPos(eqp_stats_offset + eqp_second_column,  (2 * 15) + (2 * eqp_spaccing)));
	
	eqpTemp = new UILabel(currentEquipStats, "Atk");
	eqpTemp->SetFontColor(kNone, formText);
	eqpTemp->SetPos(UIPos(8,  (3 * 15) + (3 * eqp_spaccing)));
	eqpCurrentAttack = new UILabel(currentEquipStats, "");
	eqpCurrentAttack->SetFontColor(kNone, formText);
	eqpCurrentAttack->SetPos(UIPos(eqp_stats_offset,  (3 * 15) + (3 * eqp_spaccing)));
	
	
	
	
//---- Inventory
	fInventory = new UIWindow(play_canvas);
	fInventory->SetBounds(UIRect(gWidth - sideLength -2 , 33, sideLength, sideHeight));
	fInventory->SetBgColor(kNone, formbg);
	fInventory->SetCaptionColor(kNone, formcaption);
	fInventory->SetBorderColor(kNone, formborder);
	fInventory->SetTitle("Inventory");
	fInventory->SetDragable(false);
	fInventory->Hide();
	
	lstInventory = new UIListbox(fInventory);
	lstInventory->SetBounds(UIRect(equipOffset,equipOffset, sideLength - (equipOffset * 4)));
	lstInventory->SetViewCount(31);
	
	int starting_inv_stats = 475;
	int inv_stats_offset = 60;
	int inv_spaccing = 2;
	
	UILabel *invTemp = new UILabel(fInventory, "Gold");
	invTemp->SetFontColor(kNone, formText);
	invTemp->SetPos(UIPos(8,starting_inv_stats + (0 * 15) + (0 * inv_spaccing)));
	invValue = new UILabel(fInventory, "");
	invValue->SetFontColor(kNone, formText);
	invValue->SetPos(UIPos(inv_stats_offset,starting_inv_stats + (0 * 15) + (0 * inv_spaccing)));
	
	invTemp = new UILabel(fInventory, "Class");
	invTemp->SetFontColor(kNone, formText);
	invTemp->SetPos(UIPos(8,starting_inv_stats + (1 * 15) + (1 * inv_spaccing)));
	invClass = new UILabel(fInventory, "");
	invClass->SetFontColor(kNone, formText);
	invClass->SetPos(UIPos(inv_stats_offset,starting_inv_stats + (1 * 15) + (1 * inv_spaccing)));
	
	
	invTemp = new UILabel(fInventory, "Int");
	invTemp->SetFontColor(kNone, formText);
	invTemp->SetPos(UIPos(8,starting_inv_stats + (2 * 15) + (2 * inv_spaccing)));
	invInt = new UILabel(fInventory, "");
	invInt->SetFontColor(kNone, formText);
	invInt->SetPos(UIPos(inv_stats_offset,starting_inv_stats + (2 * 15) + (2 * inv_spaccing)));
	
	invTemp = new UILabel(fInventory, "Str");
	invTemp->SetFontColor(kNone, formText);
	invTemp->SetPos(UIPos(8,starting_inv_stats + (3 * 15) + (3 * inv_spaccing)));
	invStr = new UILabel(fInventory, "");
	invStr->SetFontColor(kNone, formText);
	invStr->SetPos(UIPos(inv_stats_offset,starting_inv_stats + (3 * 15) + (3 * inv_spaccing)));
	
	invTemp = new UILabel(fInventory, "Agi");
	invTemp->SetFontColor(kNone, formText);
	invTemp->SetPos(UIPos(8,starting_inv_stats + (4 * 15) + (4 * inv_spaccing)));
	invAgi = new UILabel(fInventory, "");
	invAgi->SetFontColor(kNone, formText);
	invAgi->SetPos(UIPos(inv_stats_offset,starting_inv_stats + (4 * 15) + (4 * inv_spaccing)));
	
	int second_column = 120;
	invTemp = new UILabel(fInventory, "Vit");
	invTemp->SetFontColor(kNone, formText);
	invTemp->SetPos(UIPos(second_column,starting_inv_stats + (2 * 15) + (2 * inv_spaccing)));
	invVit = new UILabel(fInventory, "");
	invVit->SetFontColor(kNone, formText);
	invVit->SetPos(UIPos(inv_stats_offset + second_column,starting_inv_stats + (2 * 15) + (2 * inv_spaccing)));
	
	invTemp = new UILabel(fInventory, "Wis");
	invTemp->SetFontColor(kNone, formText);
	invTemp->SetPos(UIPos(second_column,starting_inv_stats + (3 * 15) + (3 * inv_spaccing)));
	invWis = new UILabel(fInventory, "");
	invWis->SetFontColor(kNone, formText);
	invWis->SetPos(UIPos(inv_stats_offset + second_column,starting_inv_stats + (3 * 15) + (3 * inv_spaccing)));
	
	invTemp = new UILabel(fInventory, "Def");
	invTemp->SetFontColor(kNone, formText);
	invTemp->SetPos(UIPos(second_column,starting_inv_stats + (4 * 15) + (4 * inv_spaccing)));
	invDef = new UILabel(fInventory, "");
	invDef->SetFontColor(kNone, formText);
	invDef->SetPos(UIPos(inv_stats_offset + second_column,starting_inv_stats + (4 * 15) + (4 * inv_spaccing)));
	
	invTemp = new UILabel(fInventory, "Atk");
	invTemp->SetFontColor(kNone, formText);
	invTemp->SetPos(UIPos(8,starting_inv_stats + (5 * 15) + (5 * inv_spaccing)));
	invAttack = new UILabel(fInventory, "");
	invAttack->SetFontColor(kNone, formText);
	invAttack->SetPos(UIPos(inv_stats_offset,starting_inv_stats + (5 * 15) + (5 * inv_spaccing)));
	
	
	invTemp = new UILabel(fInventory, "Level");
	invTemp->SetFontColor(kNone, formText);
	invTemp->SetPos(UIPos(8,starting_inv_stats + (6 * 15) + (6 * inv_spaccing)));
	invLevel = new UILabel(fInventory, "");
	invLevel->SetFontColor(kNone, formText);
	invLevel->SetPos(UIPos(inv_stats_offset,starting_inv_stats + (6 * 15) + (6 * inv_spaccing)));
	
	
	
//---- Stats
	fStats = new UIWindow(play_canvas);
	fStats->SetBounds(UIRect(gWidth - sideLength -2 , 33, sideLength, sideHeight));
	fStats->SetBgColor(kNone, formbg);
	fStats->SetCaptionColor(kNone, formcaption);
	fStats->SetBorderColor(kNone, formborder);
	fStats->SetTitle("Character");
	fStats->SetDragable(false);
	fStats->Hide();
	
	int statOffset = 60;
	UILabel *stats_temp = new UILabel(fStats,player_name);
	stats_temp->SetFontColor(kNone, formText);
	stats_temp->SetPos(UIPos(5,5));

	stats_temp = new UILabel(fStats,"Level");
	stats_temp->SetFontColor(kNone, formText);
	stats_temp->SetPos(UIPos(5,30));
	statLevel = new UILabel(fStats, "");
	statLevel->SetFontColor(kNone, formText);
	statLevel->SetPos(UIPos(statOffset,30));

	
	stats_temp = new UILabel(fStats,"HP");
	stats_temp->SetFontColor(kNone, formText);
	stats_temp->SetPos(UIPos(5,45));
	statHp = new UILabel(fStats, "");
	statHp->SetFontColor(kNone, formText);
	statHp->SetPos(UIPos(statOffset,45));
	
	stats_temp = new UILabel(fStats,"MP");
	stats_temp->SetFontColor(kNone, formText);
	stats_temp->SetPos(UIPos(5,60));
	statMp = new UILabel(fStats, "2333");
	statMp->SetFontColor(kNone, formText);
	statMp->SetPos(UIPos(statOffset,60));
	
	stats_temp = new UILabel(fStats,"Exp");
	stats_temp->SetFontColor(kNone, formText);
	stats_temp->SetPos(UIPos(5,75));
	statXp = new UILabel(fStats, "2333");
	statXp->SetFontColor(kNone, formText);
	statXp->SetPos(UIPos(statOffset,75));
	
	
	
	
	statOffset = 40;
	stats_temp = new UILabel(fStats,"Int");
	stats_temp->SetFontColor(kNone, formText);
	stats_temp->SetPos(UIPos(5,100));
	statInt = new UILabel(fStats, "2333");
	statInt->SetFontColor(kNone, formText);
	statInt->SetPos(UIPos(statOffset,100));
	
	stats_temp = new UILabel(fStats,"Str");
	stats_temp->SetFontColor(kNone, formText);
	stats_temp->SetPos(UIPos(5,115));
	statStr = new UILabel(fStats, "2333");
	statStr->SetFontColor(kNone, formText);
	statStr->SetPos(UIPos(statOffset,115));
	
	stats_temp = new UILabel(fStats,"Agi");
	stats_temp->SetFontColor(kNone, formText);
	stats_temp->SetPos(UIPos(5,130));
	statAgi = new UILabel(fStats, "2333");
	statAgi->SetFontColor(kNone, formText);
	statAgi->SetPos(UIPos(statOffset,130));
	
	stats_temp = new UILabel(fStats,"Vit");
	stats_temp->SetFontColor(kNone, formText);
	stats_temp->SetPos(UIPos(5,145));
	statVit = new UILabel(fStats, "2333");
	statVit->SetFontColor(kNone, formText);
	statVit->SetPos(UIPos(statOffset,145));
	
	stats_temp = new UILabel(fStats,"Wis");
	stats_temp->SetFontColor(kNone, formText);
	stats_temp->SetPos(UIPos(5,160));
	statWis = new UILabel(fStats, "2333");
	statWis->SetFontColor(kNone, formText);
	statWis->SetPos(UIPos(statOffset,160));
	
	stats_temp = new UILabel(fStats,"Def");
	stats_temp->SetFontColor(kNone, formText);
	stats_temp->SetPos(UIPos(5,175));
	statDefense = new UILabel(fStats, "2333");
	statDefense->SetFontColor(kNone, formText);
	statDefense->SetPos(UIPos(statOffset,175));
	
	stats_temp = new UILabel(fStats,"Dodge");
	stats_temp->SetFontColor(kNone, formText);
	stats_temp->SetPos(UIPos(5,200));
	statDodge = new UILabel(fStats, "0");
	statDodge->SetFontColor(kNone, formText);
	statDodge->SetPos(UIPos(60,200));
	
	stats_temp = new UILabel(fStats,"Absorb");
	stats_temp->SetFontColor(kNone, formText);
	stats_temp->SetPos(UIPos(5,215));
	statAbsorb = new UILabel(fStats, "0");
	statAbsorb->SetFontColor(kNone, formText);
	statAbsorb->SetPos(UIPos(60,215));
	
	
	
	
	
	
//---- vital window
	vitals = new UIWindow(play_canvas);
	vitals->SetBounds(UIRect(1,2 , 160, 55));
	vitals->SetBgColor(kNone, formbg);
	vitals->SetCaptionColor(kNone, formcaption);
	vitals->DisplayTitle(false);
	vitals->SetBorderColor(kNone, itemborder);
	
	vital_pic = new UIImage(vitals, &tClasses[0]);
	if (nHost->GetType() == nServer ) {
		//we need to load correct texture  because we are server
		vital_pic->SetTexture(&tClasses[players[0].class_id -1]);
	}
	vital_pic->SetBounds(UIRect(2,20,32,32));
	vital_pic->SetBorderColor(kNone, itemborder);
	vital_pic->ShowBorder(true);
	 
	temp = new UILabel(vitals,"HP");
	temp->SetFontColor(kNone, formText);
	temp->SetPos(UIPos(38,14));
	
	temp = new UILabel(vitals,"MP");
	temp->SetFontColor(kNone, formText);
	temp->SetPos(UIPos(38,26));
				  
	temp = new UILabel(vitals,"XP");
	temp->SetFontColor(kNone, formText);
	temp->SetPos(UIPos(38,38));
	
	UILabel *charname = new UILabel(vitals,player_name);
	charname->SetFontColor(kNone, formText);
	charname->SetPos(UIPos(2,1));
	
	vital_hp = new UIBar(vitals);
	vital_hp->SetBounds(UIRect(56,20,100,6));
	vital_hp->SetBarColor(UIColor(176,20,38));
	vital_hp->SetBorderColor(kNone, itemborder);
	
	vital_mp = new UIBar(vitals);
	vital_mp->SetBounds(UIRect(56,32,100,6));
	vital_mp->SetBarColor(UIColor(28,112,198));
	vital_mp->SetBorderColor(kNone, itemborder);
	
	vital_xp = new UIBar(vitals);
	vital_xp->SetBounds(UIRect(56,44,100,6));
	vital_xp->SetBarColor(UIColor(240,232,104)); //yellow
	vital_xp->SetBorderColor(kNone, itemborder);
	

//---- Target window 
	target_win = new UIWindow(play_canvas);
	target_win->SetBounds(UIRect(170,2 , 160, 43));
	target_win->SetBgColor(kNone, formbg);
	target_win->SetCaptionColor(kNone, formcaption);
	target_win->DisplayTitle(false);
	target_win->SetBorderColor(kNone, itemborder);
	
	target_pic = new UIImage(target_win, &tEnemyIcon);
	target_pic->SetBounds(UIRect(2,6,32,32));
	target_pic->SetBorderColor(kNone, itemborder);
	target_pic->ShowBorder(true);
	
	temp = new UILabel(target_win,"HP");
	temp->SetFontColor(kNone, formText);
	temp->SetPos(UIPos(38,14));
	
	temp = new UILabel(target_win,"Level");
	temp->SetFontColor(kNone, formText);
	temp->SetPos(UIPos(38,26));
	target_level = new UILabel(target_win, "");
	target_level->SetPos(UIPos(100,26));
	target_level->SetFontColor(kNone, formText);
	
	target_name = new UILabel(target_win,"Monster");
	target_name->SetFontColor(kNone, formText);
	target_name->SetPos(UIPos(38,1));
	
	target_hp = new UIBar(target_win);
	target_hp->SetBounds(UIRect(56,20,100,6));
	target_hp->SetBarColor(UIColor(176,20,38));
	target_hp->SetBorderColor(kNone, itemborder);
	
	
//----SPELL BAR
	
	spellBar = new UIWindow(play_canvas);
	float spellBarlength = 410;
	spellBar->SetBounds(UIRect((gWidth - spellBarlength)/2.0f,gHeight-70 -2 , spellBarlength,70));
	spellBar->SetBgColor(kNone, formbg);
	spellBar->SetBorderColor(kNone, itemborder);
	spellBar->DisplayTitle(false);
	
	float spellOffset = 4.0f;
	for(int i =0; i < 6; ++i) {
		spellSlots[i] = new UIImage(spellBar, &tSettings);
		spellSlots[i]->SetBounds(UIRect(spellOffset + (i * 64) + (i * spellOffset) ,4,64,64));
		spellSlots[i]->Hide();
		
		//recastTimer Spots
		recastTimers[i] = new UILabel(spellBar, IntToString(i));
		recastTimers[i]->SetPos(UIPos(spellOffset + (i * 64) + (i * spellOffset) + 22, 32));
		recastTimers[i]->SetFontColor(kNone, formText);
		
		recastTimers[i]->Hide();
									 
	}
	
	//Load Spell Bar from Players database if there is presets
	if (dbPlayers->Query("SELECT spell_id, position from Spellbar where player_id = " + db_player_id)) {
		
		for (int i = 0; i < dbPlayers->total_rows; ++i) {
			for (int j = 0; j < 6; ++j) {
				if (player_spells[j]->db_id == dbPlayers->GetInteger(i, 0)) {
					int spot = dbPlayers->GetInteger(i, 1);
					spellMapping[spot] = j;
					spellSlots[spot]->SetTexture(player_spells[j]->icon);
					spellSlots[spot]->Show();
				}
			}
		}
	}

//---- Compass
	sCompass.SetTexture(&tCompass);
	sCompass.Move((gWidth - 256)/2, 0);
	sCompass.SetSide(256);
	
//---- Load Inventory from Database
	GetInventoryFromDatabase();
	GetEquipFromDatabase();
	UpdatePlayerStats();
	
//---- Cast Bar
	int castBarWidth =200;
	int castBarHeight =10;
	castBar = new UIBar(play_canvas);
	castBar->SetBounds(UIRect((gWidth - castBarWidth)/2,((gHeight * 1.75) - castBarHeight) /2 ,castBarWidth,castBarHeight));
	castBar->SetBarColor(UIColor(255,0,255));
	castBar->SetBorderColor(kNone, itemborder);
	castBar->SetDragable(true);
	castBar->Hide();
	
}

void Game::DeletePlayScreen(){
	if (play_canvas != NULL) {
		delete play_canvas;
	}
}

void Game::UpdatePlayScreen(float dt){
	nHost->Update();
	ProcessPackets();
	
	for (int i =0; i < 4; ++i) {
		if(!players[i].UpdateObject(dt)) {
			//We collided so we need to send stop
			GamePacket g;
			g.packet_id = 13;
			g.player_id = PlayerId;
			gPacketManager.AddPacket(g);
			nHost->SendBroadcast(g);
			
			g.packet_id = 11;
			gPacketManager.AddPacket(g);
			nHost->SendBroadcast(g);
		}
		
	}
	
	// Only Update enemies if you are server
	if (nHost->GetType() == nServer ) {
		int size = enemy_objects.size();
		for (int i =0; i < size; ++i) {
			enemy_objects[i]->AIUpdate(i,dt);
		}
	}
	
	//update animations regardless if you are host or client
	int enemysize = enemy_objects.size();
	for (int i =0; i < enemysize; ++i) {
		enemy_objects[i]->AnimationUpdate(dt);
	}
	
	//update all our buffs
	UpdateBuffs(dt);
	
	//If We are a client that just joined, we can't do stuff until we get our player_id
	if (nHost->GetType() != nServer ) {
		if (PlayerId == 0) {
			if (!sentRequstForNumber) {
				GamePacket  packet;
				strcpy (packet.name, player_name.c_str());
				packet.packet_id = 1;
				dbPlayers->Query("Select posx,posy,posz,level,class,hp,mp from Players where player_id = " + db_player_id);
				std::string class_id = dbPlayers->results[0][4];
				packet.pos[0] = atof(dbPlayers->results[0][0].c_str());
				packet.pos[1] = atof(dbPlayers->results[0][1].c_str());
				packet.pos[2] = atof(dbPlayers->results[0][2].c_str());
				packet.data1 = atoi(dbPlayers->results[0][3].c_str());
				packet.data2 = atoi(class_id.c_str());
				packet.hp = dbPlayers->GetInteger(0, 5);
				packet.mp = dbPlayers->GetInteger(0, 6);
				
				nHost->SendBroadcast(packet);
				sentRequstForNumber = true;
				vital_pic->SetTexture(&tClasses[atoi(class_id.c_str()) - 1]);
				
			}
			return;
		}
	}
	
	elapsed_player_update += dt;
	if (elapsed_player_update > 1.5) {
		elapsed_player_update = 0.0f;
		GamePacket packet;
		packet.packet_id = 3;
		packet.player_id = PlayerId;
		packet.pos[0] = players[PlayerId].object->GetOriginX();
		packet.pos[1] = players[PlayerId].object->GetOriginY();
		packet.pos[2] = players[PlayerId].object->GetOriginZ();
		packet.hp = players[PlayerId].hp;
		packet.hpmax = players[PlayerId].hp_max;
		packet.mp = players[PlayerId].mp;
		packet.mpmax = players[PlayerId].mp_max;
		
		
		players[PlayerId].object->GetForwardVector(packet.dir);
		nHost->SendBroadcast(packet);
		
	}
	//Update Vital window
	UpdateVitalWindow();
	
	//Update Targeting
	UpdateTargeting();
	
	play_canvas->Update();
	water->Update(dt);
	
	//Update Particles
	if (gEmitters.size() > 0) {
		for (int i = 0; i < gEmitters.size(); ++i) {
			gEmitters[i]->Update(dt);
			
		}
	}

	
	//Check if the user has hit the enter key to chat
	
	if (gCarot == NULL) {
				
		
		
		if((glfwGetKey(GLFW_KEY_UP) || glfwGetKey('W')) && !moving_forward) {
			moving_forward = true;
			GamePacket g;
			g.packet_id = 10;
			g.player_id = PlayerId;
			gPacketManager.AddPacket(g);
			nHost->SendBroadcast(g);
			
		}
		if(!glfwGetKey(GLFW_KEY_UP) && !glfwGetKey('W') && moving_forward) {
			moving_forward = false;
			GamePacket g;
			g.packet_id = 11;
			g.player_id = PlayerId;
			gPacketManager.AddPacket(g);
			nHost->SendBroadcast(g);
			
		}
		
		if((glfwGetKey(GLFW_KEY_DOWN)|| glfwGetKey('S')) && !moving_back) {
			moving_back = true;
			GamePacket g;
			g.packet_id = 12;
			g.player_id = PlayerId;
			gPacketManager.AddPacket(g);
			nHost->SendBroadcast(g);
			
		}
		if(!glfwGetKey(GLFW_KEY_DOWN) && !glfwGetKey('S') && moving_back) {
			moving_back = false;
			GamePacket g;
			g.packet_id = 13;
			g.player_id = PlayerId;
			gPacketManager.AddPacket(g);
			nHost->SendBroadcast(g);
			
		}
		if((glfwGetKey(GLFW_KEY_LEFT) || glfwGetKey('A') ) && !turning_left) {
			turning_left = true;
			GamePacket g;
			g.packet_id = 14;
			g.player_id = PlayerId;
			gPacketManager.AddPacket(g);
			nHost->SendBroadcast(g);
			
		}
		if(!glfwGetKey(GLFW_KEY_LEFT) && !glfwGetKey('A') && turning_left) {
			turning_left = false;
			GamePacket g;
			g.packet_id = 15;
			g.player_id = PlayerId;
			gPacketManager.AddPacket(g);
			nHost->SendBroadcast(g);
			
		}
		if(( glfwGetKey(GLFW_KEY_RIGHT) || glfwGetKey('D')) && !turning_right) {
			turning_right = true;
			GamePacket g;
			g.packet_id = 16;
			g.player_id = PlayerId;
			gPacketManager.AddPacket(g);
			nHost->SendBroadcast(g);
			
		}
		if(!glfwGetKey(GLFW_KEY_RIGHT) && !glfwGetKey('D') && turning_right) {
			turning_right = false;
			GamePacket g;
			g.packet_id = 17;
			g.player_id = PlayerId;
			gPacketManager.AddPacket(g);
			nHost->SendBroadcast(g);
			
		}
		
		if (moving_back || moving_forward) {
			ResetCurrentCasting();
		}
		
//---- Cast Spells		
		if (castingSpell == NULL) {
			if(glfwGetKey('1') && castSelected == 0 && !player_spells[spellMapping[0]]->waitingOnRecast()) {
				if (player_spells[spellMapping[0]]->CanCast(TargetEnemy, TargetAlly, players[PlayerId].mp)) {
					castingSpell = player_spells[spellMapping[0]];
					castingIndex =0;
				}
				
			} else if(glfwGetKey('2') && castSelected == 0 && !player_spells[spellMapping[1]]->waitingOnRecast()) {
				if (player_spells[spellMapping[1]]->CanCast(TargetEnemy, TargetAlly, players[PlayerId].mp)) {
					castingSpell = player_spells[spellMapping[1]];
					castingIndex =1;
				}
			} else if(glfwGetKey('3') && castSelected == 0 && !player_spells[spellMapping[2]]->waitingOnRecast()) {
				if (player_spells[spellMapping[2]]->CanCast(TargetEnemy,TargetAlly,  players[PlayerId].mp)) {
					castingSpell = player_spells[spellMapping[2]];
					castingIndex =2;
				}
			} else if(glfwGetKey('4') && castSelected == 0 && !player_spells[spellMapping[3]]->waitingOnRecast()) {
				if (player_spells[spellMapping[3]]->CanCast(TargetEnemy, TargetAlly, players[PlayerId].mp)) {
					castingSpell = player_spells[spellMapping[3]];
					castingIndex =3;
				}
			} else if(glfwGetKey('5') && castSelected == 0 && !player_spells[spellMapping[4]]->waitingOnRecast()) {
				if (player_spells[spellMapping[4]]->CanCast(TargetEnemy,TargetAlly,  players[PlayerId].mp)) {
					castingSpell = player_spells[spellMapping[4]];
					castingIndex =4;
				}
			} else if(glfwGetKey('6') && castSelected == 0 && !player_spells[spellMapping[5]]->waitingOnRecast()) {
				if (player_spells[spellMapping[5]]->CanCast(TargetEnemy, TargetAlly, players[PlayerId].mp)) {
					castingSpell = player_spells[spellMapping[5]];
					castingIndex =5;
				}
			}
			if (castingSpell != NULL) {
				//if spell type is 2 and we aren't click on anyone or on monster
				if (castingSpell->spellType == 2 && (TargetAlly == NULL || TargetEnemy != NULL) ) {
					currentCastingPlayerIndex = PlayerId;
				} else {
					currentCastingPlayerIndex = AllyIndex;
				}
				currentCastingEnemyIndex = TargetIndex;
				StartCasting();
			}
		}
		if (!glfwGetKey('1') && !glfwGetKey('2') && !glfwGetKey('3') && !glfwGetKey('4') && !glfwGetKey('5') && !glfwGetKey('6') && castSelected != 0) {
			castSelected = 0;
		}
		
		// Update Casting
		UpdateCasting(dt);
	
		
		
	}
	
	
	
	if (!PlayerToggledAttack && glfwGetKey(GLFW_KEY_SPACE)) {
		players[PlayerId].ToggleAttack();
		PlayerToggledAttack = true;
	}
	
	if (PlayerToggledAttack && !glfwGetKey(GLFW_KEY_SPACE)) {
		PlayerToggledAttack = false;
	}
	
	if(glfwGetKey(GLFW_KEY_ENTER) && !chat_toggle && !ending_toggle){
		chat_toggle = true;
		starting_toggle = true;
		gCarot = txtPrompt;		

	} 
	
	if(!glfwGetKey(GLFW_KEY_ENTER) && chat_toggle) {
		starting_toggle = false;
		
	}
	if(!glfwGetKey(GLFW_KEY_ENTER) && !chat_toggle) {
		ending_toggle = false;
	}
	if(glfwGetKey(GLFW_KEY_ENTER) &&  chat_toggle && !starting_toggle) {
		if (gCarot != NULL) {
			
			GamePacket packet;
			packet.packet_id = 0;
			packet.player_id = PlayerId;
			//packet.name = player_name.c_str();
			strcpy(&packet.name[0],(char *)player_name.c_str());
			strcpy(&packet.msg[0],(char *)reinterpret_cast<UITextbox*>(gCarot)->GetText().c_str());
			gPacketManager.AddPacket(packet);
			nHost->SendBroadcast(packet);
			
			gCarot->SetText("");
			gCarot =NULL;
			chat_toggle =false;
			ending_toggle = true;
			
			
		}
		
	}
	
	//Check for selection
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
		sVector3 mpos1, mdir, mpos2;
		GetMouseRay(mpos1, mpos2, mdir);
		M3DVector3f pos,dir,spos;
		pos[0] = mpos1.x;
		pos[1] = mpos1.y, 
		pos[2] = mpos1.z;
		dir[0] = mdir.x;
		dir[1] = mdir.y, 
		dir[2] = mdir.z;
		CheckForTargetClick( pos,  dir);
		
	}

	
	//check for camera rotation
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) && !RotatingCamera) {
		RotatingCamera = true;
		glfwGetMousePos(&MouseLastX, &MouseLastY);
		
	} else if (!glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) && RotatingCamera) {
		RotatingCamera = false;
	}
	
	if (RotatingCamera) {
		int currentx, currenty;
		glfwGetMousePos(&currentx, &currenty);
		int deltax = currentx - MouseLastX;
		int deltay = currenty - MouseLastY;
		MouseLastX = currentx;
		MouseLastY = currenty;
		
		RotateCameraX -= 0.01 * (float)deltax;
		RotateCameraY += 0.01 * (float)deltay;
		
		
		if (RotateCameraY >= 0.2) {
			RotateCameraY = 0.2;
		}
		
		if (RotateCameraY < -0.2) {
			RotateCameraY = -0.2;
		}
	}
	
	//CHeck for Camera zoom
	int mouseWheelValue = (float)glfwGetMouseWheel();
	if (mouseWheelValue != oldMouseWheelValue) {
		int diff = oldMouseWheelValue - mouseWheelValue;
		float increment = 0.2 * diff;
		cameraDist += increment;
		if (cameraDist < 1.0f) {
			cameraDist = 1.0f;
		}else if (cameraDist > 5.5f) {
			cameraDist = 5.0f;
		}
		oldMouseWheelValue = mouseWheelValue;
	}
	
	
	if (gMouse == wStats) {
		if (fStats->IsVisable()) {
			HideForms();
		} else {
			HideForms();
			fStats->Show();
			UpdateStatsWindow();
		}
		gMouse = NULL;
	} else if (gMouse == wEquip) {
		if (fEquip->IsVisable()) {
			HideForms();
		} else {
			HideForms();
			fEquip->Show();
			RefreshEquipWindow();
		}
		gMouse = NULL;
	} else if (gMouse == wSpells) {
		if (fSpells->IsVisable()) {
			HideForms();
		} else {
			HideForms();
			fSpells->Show();
		}
		gMouse = NULL;
	} else if (gMouse == wQuests) {
		if (fQuests->IsVisable()) {
			HideForms();
		} else {
			HideForms();
			fQuests->Show();
		}
		gMouse = NULL;
	} else if (gMouse == wMap) {
		if (fMap->IsVisable()) {
			HideForms();
		} else {
			HideForms();
			fMap->Show();
		}
		gMouse = NULL;
	} else if (gMouse == wGroup) {
		if (fGroup->IsVisable()) {
			HideForms();
		} else {
			HideForms();
			fGroup->Show();
		}
		gMouse = NULL;
	} else if (gMouse == wSettings) {
		
		if (fSettings->IsVisable()) {
			HideForms();
		} else {
			HideForms();
			fSettings->Show();
		}
		gMouse = NULL;
	} else if (gMouse == wInventory) {
		if (fInventory->IsVisable()) {
			HideForms();
		} else {
			HideForms();
			fInventory->Show();
			RefreshInventoryWindow();
		}
		gMouse = NULL;
	} 
	
	//Handle input on forms
	if (fSettings->IsVisable()) {
		if (gMouse == cReflection) {
			renderReflection = cReflection->GetChecked();
		}
	} else if (fEquip->IsVisable()) {
		//Determin which is selected
		HighlightSelectedEquipmentPosition();
		if (gMouse == btnEquip) {
			gMouse = NULL;
			EquipItem();
		}
		
	} else if (fSpells->IsVisable()) {
		for (int i =0; i < 6; ++i) {
			//If selected a spell icon
			if (gMouse == spellWindowSlots[i]) {
				spellWindowSlots[selectedSpellIndex]->SetBorderColor(kNone, UIColor(0,0,0));
				selectedSpellIndex = i;
				spellWindowSlots[selectedSpellIndex]->SetBorderColor(kNone, UIColor(255,0,0));
			}
			
			//if Assigned selected to a spell bar
			if (gMouse == spell_assign[i]) {
				spellMapping[i] = selectedSpellIndex;
				spellSlots[i]->SetTexture(player_spells[selectedSpellIndex]->icon);
				spellSlots[i]->Show();
				std::string sql = "INSERT OR REPLACE into Spellbar (player_id, spell_id, position) VALUES("+db_player_id+","+ IntToString(player_spells[selectedSpellIndex]->db_id)+ ","+IntToString(i)+")";
				dbPlayers->Query(sql);
			}
		}
		
	} else if (fInventory->IsVisable()) {
		if (gMouse == lstInventory) {
			gMouse = NULL;
			RefreshInventoryStats();
		}
	}
	
	//Update camera so it's behind the model
	/*M3DVector3f player_pos,player_dir, player_up;
	players[PlayerId].object->GetOrigin(player_pos);
	players[PlayerId].object->GetForwardVector(player_dir);
	players[PlayerId].object->GetUpVector(player_up);
	
	player_pos[1] += 1.4;
	gCamera.SetOrigin(player_pos);
	gCamera.SetForwardVector(player_dir);
	gCamera.SetUpVector(player_up);
	gCamera.MoveForward(-3.0f);
	*/
	
	M3DVector3f player_pos,player_dir, player_up;
	players[PlayerId].object->GetOrigin(player_pos);
	players[PlayerId].object->GetForwardVector(player_dir);
	players[PlayerId].object->GetUpVector(player_up);
	
	//player_pos[1] += 1.4;
	player_pos[1] += 0.4 + cameraDist / 5.0f;
	gCamera.SetOrigin(player_pos);
	gCamera.SetForwardVector(player_dir);
	gCamera.SetUpVector(player_up);
	gCamera.RotateLocalY(RotateCameraX);
	gCamera.RotateLocalX(RotateCameraY);
	gCamera.MoveForward(- cameraDist);
	
	
	M3DVector3f North,Forward;
	North[0] = 0.0f;
	North[1] = 0.0f;
	North[2] = -1.0f;
	gCamera.GetForwardVector(Forward);
	Forward[1] = 0;
	float angleDir = m3dRadToDeg(m3dGetAngleBetweenVectors3(Forward,North));
	
	if (Forward[0] < 0.0f) {
		angleDir = 360 - angleDir;
	}
	
	float texOffset = angleDir/90 * 64;
	float startingOffset = 158; //This gets us to the "N" for the north vector in the image
	compassRot = startingOffset + texOffset;
	
	
	//check if we exit the game
	if (!running) {
		players[PlayerId].SaveData();
	}
	
	//check for done emitters
	for (int i =0; i < gEmitters.size() ; ++i) {
		if (!gEmitters[i]->IsActive()) {
			delete gEmitters[i];
			gEmitters.erase(gEmitters.begin() + i);
		}
	}
}

void Game::DrawPlayScreen(){
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	//If We are a client that just joined, we can't do stuff until we get our player_id
	if (nHost->GetType() != nServer ) {
		if (PlayerId == 0) {
			return;
		}
	}
	
	if (renderReflection) {
	//Render the image used for water ---
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
	//--------------
	
	
	glViewport(0,0, viewWidth, viewHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	gCamera.Render();
	gFrustum.Transform(gCamera);
	objectFrustum.Transform(gCamera);

	
	sky->Render();
	land->Render();
	
	int len = objects.size();
	for(int i =0; i < len; ++i) {
		M3DVector3f point;
		objects[i]->GetOrigin(point);
		if (objectFrustum.TestSphere(point, objects[i]->GetRadius())) {
			objects[i]->Render();
			//objects[i]->RenderBB();
		}
		
	}

	len = enemy_objects.size();
	for(int i =0; i < len; ++i) {
		if (enemy_objects[i]->hp > 0) {
			M3DVector3f point;
			enemy_objects[i]->GetOrigin(point);
			if (objectFrustum.TestSphere(point, enemy_objects[i]->GetRadius())) {
				//enemy_objects[i]->Render();
				enemy_objects[i]->EnemyRender();
			}
			
		}
	}
	
	for (int i =0; i < 4; ++i) {
		players[i].Render();

	}
	
	if (renderReflection) {
		water->AdvRender();
	} else {
		water->Render();
	}
	
	//Render Particles
	if (gEmitters.size() > 0) {
		for (int i = 0; i < gEmitters.size(); ++i) {
			gEmitters[i]->Render();
		}
	}
	glDisable(GL_CULL_FACE);
}

void Game::Draw2DPlayScreen(){
	
	char buffer[256];
	float yoffset = 0;
	float xoffset = 400;
#ifdef __APPLE__
	
	sprintf(buffer, "Frames Per Second: %.2f",mFPS);
	gFont.DrawText(buffer, 10 + xoffset, 10+yoffset, 1, 0.5, 0);
	
	char quadsbuffer[256];
	sprintf(quadsbuffer, "Quads Rendered %d out of %d", QuadTerrain::counter, QuadTerrain::total);
	gFont.DrawText(quadsbuffer, 10 + xoffset, 25 + yoffset, 1, 0.5, 0);
	
	char xchange[256];
	sprintf(xchange, "y rotate %f", RotateCameraY);
	gFont.DrawText(xchange, 10 + xoffset, 40 + yoffset, 1, 0.5, 0);
#else
	sprintf(buffer, "Frames Per Second: %.2f",mFPS);
	gFont.RenderText(buffer, 10 + xoffset, 10+yoffset, 1, 0.5, 0);
	
	char quadsbuffer[256];
	sprintf(quadsbuffer, "Quads Rendered %d out of %d", QuadTerrain::counter, QuadTerrain::total);
	gFont.RenderText(quadsbuffer, 10 + xoffset, 25 + yoffset, 1, 0.5, 0);
	
	char xchange[256];
	sprintf(xchange, "y rotate %f", RotateCameraY);
	gFont.RenderText(xchange, 10 + xoffset, 40 + yoffset, 1, 0.5, 0);
#endif
	
	
	glEnable(GL_TEXTURE_2D);
	sCompass.RenderShader(compassRot,0,256,64, true);
	sCompass.RenderShader(0,64,256,64);
	RenderBuffs();
	glDisable(GL_TEXTURE_2D);	
	
	
	play_canvas->Render();
}

void Game::HideForms() {
	fStats->Hide();
	fInventory->Hide();
	fEquip->Hide();
	fSpells->Hide();
	fQuests->Hide();
	fMap->Hide();
	fGroup->Hide();
	fSettings->Hide();
	
}
