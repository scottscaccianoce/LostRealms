/*
 *  vitals.cpp
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 12/29/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */


#include "game.h"
#include "PacketManager.h"

void Game::UpdatePlayerStats() {

	if(players[PlayerId].UpdateStats()) {
		//We updated the Max value stats significantly, so we send pack to update everyone's UI
		
	}
}

void Game::UpdateVitalWindow() {
	vital_hp->SetPercentage((float)players[PlayerId].hp / (float)players[PlayerId].hp_max);
	vital_mp->SetPercentage((float)players[PlayerId].mp / (float)players[PlayerId].mp_max);
	vital_xp->SetPercentage((float)players[PlayerId].exp / (float)players[PlayerId].exp_max);
	
}

void Game::UpdateStatsWindow() {
	players[PlayerId].UpdateStats();
	statLevel->SetText( IntToString(players[PlayerId].level));
	statHp->SetText(  IntToString(players[PlayerId].hp) + " / " + IntToString(players[PlayerId].hp_max));
	statMp->SetText(  IntToString(players[PlayerId].mp) + " / " + IntToString(players[PlayerId].mp_max));
	statXp->SetText(  IntToString(players[PlayerId].exp) + " / " + IntToString(players[PlayerId].exp_max));
	statInt->SetText(  IntToString(players[PlayerId].Int) );
	statStr->SetText(  IntToString(players[PlayerId].Str) );
	statAgi->SetText(  IntToString(players[PlayerId].Agi) );
	statVit->SetText(  IntToString(players[PlayerId].Vit) );
	statWis->SetText(  IntToString(players[PlayerId].Wis) );
	statDefense->SetText(  IntToString(players[PlayerId].Def) );
	
	int dodge = players[PlayerId].Agi / 5;
	std::string strDodge = IntToString(dodge) + " %";
	statDodge->SetText( strDodge );
	
	int absorb = players[PlayerId].Def / 4;
	std::string strAbsorb = IntToString(absorb) + " %";
	statAbsorb->SetText( strAbsorb);
	
	
	
	
	
	
}