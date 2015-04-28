/*
 *  UIListbox.cpp
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/10/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "UIListbox.h"

UIListbox::UIListbox(UIWidget *ptr) {
	AddParent(ptr);
	BaseInit();
	char_width = 8;
	char_height = 13;
	SetViewCount(3);
	SetBounds(UIRect(10,10,100,100));
	item_height = 15;
	selected_text = UIColor(0,0,0);
	selected_bg = UIColor(100,142,208);
	SetBgColor(kNone, UIColor(39,39,39));
	SetFontColor(kNone, UIColor(253,253,253));
	button_color = UIColor(159,159,159);
	view_pos = 0;
	selected_index = 0;
	tri_offsetx = 4;
	tri_offsety = 3;
	tri_length = 8;
	chat_mode = false;
	max_lines = 100;
	
	//icon = new UIImage(NULL);
}

UIListbox::~UIListbox() {
	//delete icon;
}

void UIListbox::Render() {
	if (!visible) {
		return;
	}
	
	RenderBg();
	RenderBorder();
	UIPos pos = GetParentPos();
	
	//Render selected background
	
	//Render each item
	for (int i = 0; i < view_count; ++i) {
		float x = pos.x;
		float y = pos.y + (i * item_height);
		int index = view_pos + i;
		if (index < items.size()) {
			if (selected_index == index  && !chat_mode) {
				
				float local_y = pos.y + (( selected_index - view_pos) * item_height);
				glColor3ub (selected_bg.r,selected_bg.g,selected_bg.b);
				glBegin(GL_QUADS);
				glVertex2f(x  + rect.w - 1, local_y + 1);
				glVertex2f(x + 2, local_y + 1);
				glVertex2f(x + 2, local_y + item_height - 1);
				glVertex2f(x + rect.w - 1, local_y + item_height - 1);
				glEnd();
			}
		
			std::string text = (items[index].length() > max_chars) ? items[index].substr(0,max_chars) : items[index];
			float textOffset = 0;
			if (icons[index] != NULL) {
				textOffset = 20;
				
				icons[index]->bindTexture();
				glEnable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
				glColor3f(1.0f, 1.0f, 1.0f);
				float iconx, icony, iconw;
				iconx = x + 1;
				icony = y ;
				iconw = 15;
				glTexCoord2f(1.0f, 1.0f); glVertex3f(iconx  + iconw , icony ,0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(iconx , icony ,0.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(iconx , icony +iconw ,0.0f);
				glTexCoord2f(1.0f, 0.0f);glVertex3f(iconx + iconw , icony + iconw ,0.0f);
				
				glEnd();	
				glDisable(GL_TEXTURE_2D);
			
			}

			// Instead of using font_color, we use the color stored in the row of the element.
			// So we can have multicolored line chat boxes if we want.
			// font_color is the default if you add an item without a color though.
			font.DrawTextb(text.c_str(), x + 1 + textOffset,y + char_height, colors[index].r, colors[index].g, colors[index].b, 0);
		}
	}
	
	//Render the scrollbar
	if (items.size() > view_count) {
		
		//Render top box
		float x = upbox.x + pos.x;
		float y = upbox.y + pos.y;
		
		//Background
		glColor3ub (button_color.r,button_color.g,button_color.b);
		glBegin(GL_QUADS);
		glVertex2f(x  + upbox.w , y);
		glVertex2f(x , y );
		glVertex2f(x , y + upbox.h);
		glVertex2f(x + upbox.w, y + upbox.h);
		glEnd();
		
		//Border
		glColor3ub (border_color[kNone].r,border_color[kNone].g,border_color[kNone].b);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x  + upbox.w , y);
		glVertex2f(x , y );
		glVertex2f(x , y + upbox.h);
		glVertex2f(x + upbox.w, y + upbox.h);
		glEnd();
		
		//Triangle
		
		glColor3ub (39,39,39);
		glBegin(GL_TRIANGLES);
		glVertex2f(x  + tri_offsetx + tri_length/2  , y + tri_offsety);
		glVertex2f(x + tri_offsetx , y + tri_length + tri_offsety);
		glVertex2f(x +tri_offsetx + tri_length, y + tri_offsety + tri_length);
		glEnd();
		
		//======================================================
		//Render bottom box
		x = downbox.x + pos.x;
		y = downbox.y + pos.y;
		
		//Background
		glColor3ub (button_color.r,button_color.g,button_color.b);
		glBegin(GL_QUADS);
		glVertex2f(x  + downbox.w , y);
		glVertex2f(x , y );
		glVertex2f(x , y + downbox.h);
		glVertex2f(x + downbox.w, y + downbox.h);
		glEnd();
		
		//Border
		glColor3ub (border_color[kNone].r,border_color[kNone].g,border_color[kNone].b);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x  + downbox.w , y);
		glVertex2f(x , y );
		glVertex2f(x , y + downbox.h);
		glVertex2f(x + downbox.w, y + downbox.h);
		glEnd();
		
		
		//Triangle
		
		glColor3ub (39,39,39);
		glBegin(GL_TRIANGLES);
		glVertex2f(x  + tri_offsetx + tri_length/2  , y + tri_offsety + tri_length);
		glVertex2f(x + tri_offsetx , y + tri_offsety);
		glVertex2f(x +tri_offsetx + tri_length, y + tri_offsety);
		glEnd();
		
		//draw bar
		glColor3ub (39,39,39);
		glBegin(GL_QUADS);
		glVertex2f(bar.x  + bar.w + pos.x , bar.y + pos.y);
		glVertex2f(bar.x + pos.x , bar.y + pos.y );
		glVertex2f(bar.x  + pos.x, bar.y + bar.h + pos.y);
		glVertex2f(bar.x + bar.w + pos.x, bar.y + bar.h + pos.y);
		glEnd();
		
		
		//draw slider box
		total_views = (int)items.size() - view_count;
		if ( total_views < 0 ) {
			total_views = 0;
		}
		total_views++; //Add one so if we have 1 view change, we have 2 spots
		float box_height = bar.h / total_views;
		if ( box_height < 1.0f ) {
			box_height = 1.0f;
		}
		UIPos box_pos = UIPos(bar.x, bar.y + (view_pos *box_height));
		float width = bar.w + 0.5f;
		
		glColor3ub (219,219,219);
		glBegin(GL_QUADS);
		glVertex2f(box_pos.x  + width + pos.x -1 , box_pos.y + pos.y);
		glVertex2f(box_pos.x + pos.x , box_pos.y + pos.y );
		glVertex2f(box_pos.x  + pos.x, box_pos.y + box_height + pos.y);
		glVertex2f(box_pos.x +width + pos.x -1, box_pos.y + box_height + pos.y);
		glEnd();
		
	}
	
}

bool UIListbox::MouseHandler(float x, float y) {
	UIPos pos = GetParentPos();
	
	if (items.size() > view_count) {
		//UP BOX
		UIRect up = UIRect( upbox.x + pos.x, upbox.y +pos.y, upbox.w, upbox.h);
		if (up.Intersects(x,y)) {
			//if first click, register it as a click
			if (!mouse_active) {
				mouse_active = true;
				ChangeView(-1);
				
			}
			return true;
		}
		
		//down
		UIRect down = UIRect( downbox.x + pos.x, downbox.y +pos.y, downbox.w, downbox.h);
		if (down.Intersects(x,y)) {
			//if first click, register it as a click
			if (!mouse_active) {
				mouse_active = true;
				ChangeView(1);
				
			}
			return true;
		}
	}
	//Check for selected
	UIRect temp  = UIRect(  pos.x, pos.y, rect.w, rect.h);
	
	if (temp.Intersects(x,y)) {
		//if first click, register it as a click
		if (!mouse_active) {
			mouse_active = true;
			gMouse = this;
			
			float new_y = y -temp.y;
			int old_selected_index = selected_index;
			for (int i = 0; i < view_count; ++i) {
				if (new_y <= item_height * (i +1)) {
					selected_index = view_pos + i;
					break;
				}
			}
			
			//Check to make sure our list box is populated that much
			if (items.size() -1 < selected_index) {
				selected_index = old_selected_index;
			}

		}
		return true;
	}
	
	
	return false;
}


void UIListbox::AddItem(const std::string &item) {
	/*items.push_back(item);
	
	if (chat_mode) {
		
		ChangeView(max_lines);
	}
	 */
	sTexture *nullTex = NULL;
	
	AddItemWithIcon(item, nullTex);
}

void UIListbox::AddItemWithIcon(const std::string &item, sTexture *tex ) {
	items.push_back(item);
	icons.push_back(tex);
	colors.push_back(font_color[kNone]);
	
	if (chat_mode) {
		
		ChangeView(max_lines);
	}
	
	
}

void UIListbox::AddItem(const std::string &item, UIColor c) {

	sTexture *nullTex = NULL;
	
	AddItemWithIcon(item, nullTex, c);
}

void UIListbox::AddItemWithIcon(const std::string &item, sTexture *tex, UIColor c ) {
	items.push_back(item);
	icons.push_back(tex);
	colors.push_back(c);
	
	if (chat_mode) {
		
		ChangeView(max_lines);
	}
	
	
}

bool UIListbox::RemoveItem(int index) {
	bool retval = false;
	if (items.size() > index + 1) {
		items.erase(items.begin() + index);
		retval =  true;
	}
	
	if (icons.size() > index + 1) {
		icons.erase(icons.begin() + index);
		retval =  true;
	}
	
	if (colors.size() > index + 1) {
		colors.erase(colors.begin() + index);
		retval =  true;
	}
	
	return retval;
}

bool UIListbox::RemoveItem(const std::string &item) {
	for(int i =0; i < items.size(); ++i) {
		if (items[i] == item) {
			items.erase(items.begin() +i);
			icons.erase(icons.begin() +i);
			colors.erase(colors.begin() + i);
			return true;
		}
	}
	return false;
}

int UIListbox::GetSelectedIndex() const {
	//check to make sure that there is an item in the selected index
	return selected_index;
}

std::string UIListbox::GetSelectedItem() {
	return ( items.size() > 0) ? items[selected_index] : "";
}

void UIListbox::SetViewCount(int count) {

	//Minimum of 3 always
	if (count < 3) {
		view_count = 3;
	} else {
		view_count = count;
	}
	CalculateResize();
}

void UIListbox::SetBounds(const UIRect &bounds) {
	rect = bounds;
	CalculateResize();
}
			  
void UIListbox::SetPos(UIPos pos ) {
	rect.x = pos.x;
	rect.y = pos.y;
	CalculateResize();
}

void UIListbox::CalculateResize() {
	max_chars = (int)rect.w / char_width;
	rect.h = view_count * item_height;
	
	//position the buttons
	
	//up
	upbox.x = rect.w + 1;
	upbox.y = 0;
	upbox.w = 15;
	upbox.h = 15;
	
	//down
	downbox.x = rect.w + 1;
	downbox.y = rect.h - 15;
	downbox.w = 15;
	downbox.h = 15;
	
	bar.x = rect.w + 2;
	bar.y = 15;
	bar.w = 14.5f;
	bar.h = downbox.y - (upbox.y + upbox.h);
	
	
}
void UIListbox::ChangeView(int scroll_num) {
	if (items.size() <= view_count) {
		return;
	}
	view_pos += scroll_num;
	if (view_pos < 0) {
		view_pos = 0;
		return;
	}
	
	if (view_pos > items.size() - view_count) {
		view_pos = items.size() - view_count;
	}
	if (view_pos < 0) {
		view_pos = 0;
	}
}

int UIListbox::GetCount() {
	return items.size();
}

void UIListbox::Clear() {
	items.clear();
	icons.clear();
	colors.clear();
	selected_index = 0;
}

bool UIListbox::SetChatMode(bool value) {
	chat_mode = value;
	return true;
}

void UIListbox::RenderBg() {
	if (display_bg) {
		
		float boxoffset = 0;
		if (items.size() <= view_count) {
			boxoffset += upbox.w;
		}
		
		UIPos offset = GetParentPos();
		float x =  offset.x;
		float y =  offset.y;
		
		
		glBegin(GL_QUADS);
		glColor3ub (bg_color[state].r,bg_color[state].g,bg_color[state].b);
		
		glVertex2f(x + rect.w +boxoffset, y);
		glVertex2f(x, y);
		glVertex2f(x, y + rect.h);
		glVertex2f(x + rect.w + boxoffset, y + rect.h);
		
		glEnd();
	}
}
