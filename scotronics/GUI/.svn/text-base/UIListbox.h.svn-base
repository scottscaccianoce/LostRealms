/*
 *  UIListbox.h
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/10/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

//Scroll bar is another 10 pixels wide

#ifndef __UILISTBOX__
#define __UILISTBOX__

#include "UIWidget.h"
#include <vector>
#include <string>
#include "font.h"
#include "texture.h"
#include "UIImage.h"

class UIListbox : public UIWidget {
public:
	
	UIListbox(UIWidget *ptr);
	~UIListbox();
	void Render();
	bool MouseHandler(float x, float y);
	void SetBounds(const UIRect &bounds);
	void SetPos(UIPos pos);
	void AddItem(const std::string &item);
	void AddItem(const std::string &item, UIColor color);
	
	void AddItemWithIcon(const std::string &item, sTexture *tex );
	void AddItemWithIcon(const std::string &item, sTexture *tex, UIColor color );
	
	bool RemoveItem(int index); //Remove by index
	bool RemoveItem(const std::string &item); //Remove by value
	int GetSelectedIndex() const;
	std::string GetSelectedItem();
	void SetViewCount(int count);
	int GetCount();
	void Clear();
	
	bool SetChatMode(bool value);
	
private:
	void RenderBg(); //override widget function
	
	Font font;
	std::vector<std::string> items;
	std::vector<sTexture*> icons;
	std::vector<UIColor> colors;
	//UIImage *icon;
	
	
	int view_count; //How many items are shown at one time
	int item_height;
	int char_height;
	int char_width;
	int max_chars;
	UIRect bar;
	UIRect upbox;
	UIRect downbox;
	float tri_offsetx;
	float tri_offsety;
	float tri_length;
	int selected_index;
	void CalculateResize();
	void ChangeView(int scroll_num);
	int view_pos;
	int total_views;
	UIColor selected_text;
	UIColor selected_bg;
	UIColor button_color;
	
	bool chat_mode;
	int max_lines;
};

#endif