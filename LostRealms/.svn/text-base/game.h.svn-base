/*
 *  game.h
 *  triangle
 *
 *  Created by Scott Scaccianoce on 3/4/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */
#ifndef GAME_H
#define GAME_H

#ifdef __APPLE__

//#include "GLTools.h"
//#include "GLShaderManager.h"

#include "GL/glfw.h"
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#import <GLUT/GLUT.h>

#else
#include "gl/glew.h"

#endif


#include "app.h"
#include "font.h"
#include "UIWidget.h"
#include "UICanvas.h"
#include "UIWindow.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UICheckbox.h"
#include "UIListbox.h"
#include "UISlider.h"
#include "UIStepSlider.h"
#include "UITextbox.h"
#include "UIImage.h"
#include "UIBar.h"

#include "texture.h"
#include "Camera.h"
#include "terrain.h"
#include "spMesh.h"
#include "SPEntity.h"
#include "skyplane.h"
#include "quadterrain.h"
#include "Frustum.h"
#include "Water.h"
#include "util.h"
#include "Database.h"
#include "sqlite3.h"
#include "Network.h"
#include "PacketManager.h"
#include "Player.h"
#include "Sprite.h"
#include "Spell.h"
#include "Buff.h"
#include "item.h"
#include "particle_system.h"
#include "emitters.h"
#include "enet/enet.h"

enum gamescreens{CharacterScreen=0,NewCharacterScreen,PlayScreen};
class Game : public App {

public:
	Game(int w, int h, bool fullscreen);
	~Game();
	void Init();
	void Draw();
	void Draw2D();
	void Update();

private:
	

//SCREENS-----------------------------------------------------
// Character Screen ----
	sTexture *characterBackground;
	Sprite charBG;
	void InitCharacterScreen();
	void UpdateCharacterScreen();
	void DeleteCharacterScreen();
	void DrawCharacterScreen();
	void Draw2DCharacterScreen();
	UICanvas *char_canvas;
	UIListbox *lstCharacters;
	UIButton *btnHost;
	UIButton *btnJoin;
	UIButton *btnNewChar;
	UIButton *btnDelete;
	vector<string> player_ids;
	vector<string> class_ids;
	vector<string> names;
	
// New Character Screen ----
	void InitNewCharacterScreen();
	void DeleteNewCharacterScreen();
	void UpdateNewCharacterScreen();
	void DrawNewCharacterScreen();
	void Draw2DNewCharacterScreen();
	void ToggleGroup(UICheckbox *options[], int index, int size );
	std::map<int,string> classes;
	std::map<int,string> roles;
	std::vector<int> db_ids;
	
	//<class_id, name of school
	std::map<int, string> school1;
	std::map<int, string> school2;
	std::map<int, string> school3;
	
	UICanvas *new_canvas;
	UIWindow *new_form;
	std::vector<UIButton *> class_buttons;
	UIButton *btnWarrior;
	UIButton *btnWizard;
	UIButton *btnBard;
	UIButton *btnHealer;
	UITextbox *txtName;
	string newClass;
	UIButton *btnCreate;
	UILabel *classSelected;
	UILabel *lblRoll;
	UIWindow *createDialog;
	UIButton *btnOk;
	UICheckbox *primary[3];
	UICheckbox *secondary[3];
	
//Player Screen-------------------------
	//Vital window
	UIWindow *vitals;
	UIImage *vital_pic;
	UIBar *vital_hp;
	UIBar *vital_mp;
	UIBar *vital_xp;
	sTexture tClasses[4];
	sTexture tEnemyIcon;
	
	//target window
	UIWindow *target_win;
	UIImage *target_pic;
	UIBar *target_hp;
	UIBar *target_mp;
	UILabel *target_name;
	UILabel *target_level;
	
	//Spellbar window
	UIWindow *spellBar;
	UIImage *spellSlots[6];
	Spell *player_spells[6];
	UIButton *spell_assign[6];
	int selectedSpellIndex;
	std::map<int, int> spellMapping; //<spellSlot index, playerSpell index
	UIImage *spellWindowSlots[6];
	UILabel *recastTimers[6];
	Sprite bufficons[16];
	int castingIndex;
	
	UIBar *castBar;
	int castSelected;
	void ResetCurrentCasting(); 
	void StartCasting(); 
	void UpdateCasting(float dt);
	void RenderRecast();
	void CreateSpellEmitter(int sId, int tIndex, int aIndex, int spell_array_index);
	bool HandleTargetEnemySpell(int sId, Enemy *tIndex, int pIndex, int amount, int tIndex);
	void HandleTargetAllySpell(int sId, Player *tIndex, int amount);
	void HandleTargetGroupSpell(int sId, int pid, int amount);
	void UpdateBuffs(float dt);
	void RenderBuffs();
	
	//Chat Window
	UITextbox *txtPrompt;
	UIListbox *log;
	UICanvas *play_canvas;
	UIWindow *chat_form;
	
	//Combat log
	
	UIWindow *combatWindow;
	
	//Game Windows (inventory, quests, spells etc)
	sTexture tSettings;
	sTexture tInventory;
	sTexture tEquip;
	sTexture tSpells;
	sTexture tQuests;
	sTexture tGroup;
	sTexture tMap;
	sTexture tStats;
	
	UIImage *wSettings;
	UIImage *wInventory;
	UIImage *wEquip;
	UIImage *wSpells;
	UIImage *wQuests;
	UIImage *wGroup;
	UIImage *wMap;
	UIImage *wStats;
	
	UIWindow *fSettings;
	UIWindow *fInventory;
	UIWindow *fEquip;
	UIWindow *fSpells;
	UIWindow *fQuests;
	UIWindow *fGroup;
	UIWindow *fMap;
	UIWindow *fStats;	
	
	void HideForms();
	
	//Stats
	UILabel *statHp;
	UILabel *statHpMax;
	UILabel *statName;
	UILabel *statMp;
	UILabel *statMpMax;
	UILabel *statXp;
	UILabel *statLevel;
	UILabel *statVit;
	UILabel *statDefense;
	UILabel *statStr;
	UILabel *statInt;
	UILabel *statAgi;
	UILabel *statWis;
	UILabel *statDodge;
	UILabel *statAbsorb;
	
	
	
	
	//Settings window
	UICheckbox *cReflection;
	UICheckbox *cRefraction;
	UILabel *selfIP;
	UISlider *renderDistance;
	
	//Equipment Window
	sTexture *tHead;
	sTexture *tChest;
	sTexture *tHands;
	sTexture *tLegs;
	sTexture *tFeet;
	sTexture *tRing;
	sTexture tTrinket;
	sTexture *tNecklace;
	sTexture tMainWeapon[4];
	sTexture tOffWeapon[4];
	UIImage *uHead;
	UIImage *uChest;
	UIImage *uHands;
	UIImage *uLegs;
	UIImage *uFeet;
	UIImage *uRing;
	UIImage *uTrinket;
	UIImage *uNecklace;
	UIImage *uMainWeapon;
	UIImage *uOffWeapon;
	UIListbox *lstEquip;
	UILabel *eqpValue;
	UILabel *eqpLevel;
	UILabel *eqpAttack;
	UILabel *eqpClass;
	UILabel *eqpInt;
	UILabel *eqpStr;
	UILabel *eqpAgi;
	UILabel *eqpWis;
	UILabel *eqpVit;
	UILabel *eqpDef;
	UILabel *eqpCurrentInt;
	UILabel *eqpCurrentStr;
	UILabel *eqpCurrentAgi;
	UILabel *eqpCurrentWis;
	UILabel *eqpCurrentVit;
	UILabel *eqpCurrentDef;
	UILabel *eqpCurrentAttack;
	UIButton *btnEquip;
	UILabel *equipNames[9];
	
	//Inventory Window
	UIListbox *lstInventory;
	UILabel *invValue;
	UILabel *invLevel;
	UILabel *invAttack;
	UILabel *invClass;
	UILabel *invInt;
	UILabel *invStr;
	UILabel *invAgi;
	UILabel *invWis;
	UILabel *invVit;
	UILabel *invDef;

	//Group Window
	UILabel *GroupInviteName;
	UILabel *GroupInviteAccept;
	UILabel *GroupInviteDecline;
	int GroupPlayerIdInvite;
	
	UIButton *GroupSendInvite;
	UIButton *GroupLeave;
	
	
	
	//Group Bar
	UIWindow *GroupBarWindow;
	UIWindow *GroupMemberFrames[4];
	UIImage	*GroupMemberIcons[4];
	UILabel *GroupMemberNames[4];
	UIBar *GroupMemberHp[4];
	UIBar *GroupMemberMp[4];
	
	
	
	
	
	
	
	
//End Screens --------------------------
	
	// Abstract Functions
	void InitPlayScreen();
	void DeletePlayScreen();
	void UpdatePlayScreen(float dt);
	void DrawPlayScreen();
	void Draw2DPlayScreen();

	
	//Player
	string db_player_id;
	string player_name;
	
	
	//Scotronics Classes
	Font gFont;
	QuadTerrain *land;
	SkyPlane *sky;
	Water *water;
	bool safe_delete;
	
	//Networking
	

	bool sentRequstForNumber;
	void ProcessPackets();
	float elapsed_player_update;
	
	//Database controllers
	Database *dbPlayers;
	Database *dbGame;
	
	//Screen variables
	gamescreens current_screen;
	gamescreens next_screen;
		
	//Compass variables
	sTexture tCompass;
	Sprite	sCompass;
	float compassRot;
	
	//Lists

	
	
	
	//UI
	UIWidget *canvas;
	bool mouseDownLock;
	
	//Chat
	std::string key_pressed;
	bool chat_toggle;
	bool starting_toggle;
	bool ending_toggle;
	
	//Debug Information
	float mFPS;
	int old_mouse_x;
	int old_mouse_y;
	
	//Setting Flags
	bool renderRefraction;
	bool renderReflection;
	
	
	//Player Movement UI Flags
	bool moving_forward;
	bool moving_back;
	bool turning_right;
	bool turning_left;
	
	//Inventory Functions
	void GetInventoryFromDatabase();
	void AddItemToInventory(int item_id);
	void AddNewItemToInventory(int item_id);
	void RefreshInventoryWindow();
	void RefreshInventoryStats();
	void RemoveItemFromInventory(int item_id);
	std::vector<Item*> inventory;
	
	//Equipment Functions
	void GetEquipFromDatabase();
	void AddItemToEquip(int item_id);
	void RefreshEquipWindow();
	void RefreshEquipStats();
	void RefreshEquipCurrentStats();
	void HighlightSelectedEquipmentPosition();
	void ClearEquipBorders();
	void RefreshEquipmentNames();
	void EquipItem();
	int selectedEquipIndex;
	std::vector<Item*> EqpToInv;
	
	// Player Stats
	void UpdatePlayerStats();
	void UpdateVitalWindow();
	void UpdateStatsWindow();
	//Classid to name hash
	std::map<int,std::string> classIdsToName;
	
	//Camera Rotate variables
	int MouseLastX;
	int MouseLastY;
	float RotateCameraX;
	float RotateCameraY;
	bool RotatingCamera;
	float cameraDist;
	int oldMouseWheelValue;
	
	//Targeting Functions
	
	void ClearTargets();
	void CheckForTargetClick(M3DVector3f pos, M3DVector3f dir);
	void UpdateTargeting();
	
	bool PlayerToggledAttack;//Used so we only toggle once
	
	
	
	Frustum objectFrustum;
};

//global
extern int PlayerId; //id of player in game
extern std::vector<Player> gPlayers;
extern std::vector<SPMesh*> meshes;
extern std::vector<SPMesh*> enemy_meshes;
extern std::vector<SPMesh*> enemy_attack_meshes;

extern std::vector<sTexture*> item_textures;
extern vector<Enemy*> enemy_objects;

extern vector<SPEntity*> objects;
extern Item equipment[9];
extern UIListbox *combatLog;

//TARGET STUFF
extern Enemy *TargetEnemy;
extern int TargetIndex;
extern Player *TargetAlly;
extern int AllyIndex;


extern NetworkHost *nHost;
extern Spell *castingSpell;

//players
extern Player players[4];
extern std::vector<int> GroupedAllies;
extern 	bool connected[4];

extern UIColor RED;
extern UIColor TEAL;
extern UIColor YELLOW;
extern UIColor GREEN;


//Particles
extern std::vector<Emitter*> gEmitters;

//Buffs


extern int currentCastingPlayerIndex;
extern int currentCastingEnemyIndex;

extern std::map <int, int> gLevels;

#endif