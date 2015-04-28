/*
 *  targeting.cpp
 *  LostRealms
 *
 *  Created by Scott Scaccianoce on 12/29/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "game.h"
void Game::ClearTargets() {
	TargetEnemy = NULL;
	TargetAlly = NULL;
	
}

void Game::CheckForTargetClick(M3DVector3f pos, M3DVector3f dir) {
	int size  = enemy_objects.size();
	float dist;
	float max_dist = 100;
	bool found = false;
	
	//Check for Enemies
	for (int i = 0; i < size; ++i) {
		if (enemy_objects[i]->hp > 0 &&enemy_objects[i]->RayIntersect(pos,dir,dist)) {
			if (dist < max_dist) {
				ClearTargets();
				TargetEnemy = enemy_objects[i];
				found = true;
				TargetIndex = i;
				break;
			}
		}
	}
	
	//Check for Players
	for (int i = 0; i < 4; ++i) {
		if (players[i].connected && !players[i].IsCurrentPlayer && players[i].object->RayIntersect(pos,dir,dist)) {
			if (dist < max_dist) {
				ClearTargets();
				TargetAlly = &players[i];
				found = true;
				AllyIndex = i;
				break;
			}
		}
	}
	
	if (!found) {
		ClearTargets();
	}
}

void Game::UpdateTargeting() {
	if (TargetEnemy != NULL) {
		target_win->Show();
		target_hp->SetPercentage((float)TargetEnemy->hp / (float)TargetEnemy->hp_max);
		target_level->SetText( IntToString(TargetEnemy->level));
		target_name->SetText(TargetEnemy->name);
		target_pic->SetTexture(&tEnemyIcon);
		
	} else if (TargetAlly != NULL) {
		target_win->Show();
		target_hp->SetPercentage((float)TargetAlly->hp / (float)TargetAlly->hp_max);
		target_level->SetText( IntToString(TargetAlly->level));
		target_name->SetText(TargetAlly->name);
		target_pic->SetTexture(&tClasses[players[AllyIndex].class_id -1]);
	} else {
		target_win->Hide();
	}

	
}
