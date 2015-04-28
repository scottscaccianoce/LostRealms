/*
 *  widget.cpp
 *  GUI
 *
 *  Created by Scott Scaccianoce on 6/3/12.
 *  Copyright 2012 Scotronics. All rights reserved.
 *
 */

#include "UIWidget.h"
#include <vector>


UIWidget* gMouse = NULL;
UIWidget* gCarot = NULL;
bool safe_delete = true;

void GLFWCALL onKey(int key, int action) {
	if ( action == GLFW_PRESS ) {
		char c = (char)key;
		
		if (gCarot != NULL) {
			
			std::string letter;
			letter.push_back(c);
			gCarot->AppendText(letter);
		}

	}

}
void HandleSpecialKeyInput() {
	if (glfwGetKey( GLFW_KEY_ENTER) == GLFW_PRESS ) {
		gCarot = NULL;
	}
	
	if (glfwGetKey( GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
		if (gCarot != NULL  && safe_delete) {
			gCarot->Backspace();
			safe_delete = false;
		}
	} else {
		safe_delete = true;
	}
}
UIWidget::UIWidget() {
	parent = NULL;
	BaseInit();
}

void UIWidget::AddParent(UIWidget *pParent) {
	parent = pParent;
	parent->AddChild(this);
}

UIWidget::~UIWidget() {
	int len = children.size();
	for(int i = 0; i < len; ++i) {
		delete children[i];
		children[i] = NULL;
	}
}
void UIWidget::RenderChildren() {
	unsigned int len = children.size();
	for (unsigned int i=0; i < len; ++i) {
		children[i]->Render();
	}
	
}

void UIWidget::ClearMouseActive() {
	mouse_active = false;
	unsigned int len = children.size();
	for (unsigned int i=0; i < len; ++i) {
		children[i]->ClearMouseActive();
	}
	
}

void UIWidget::SetBorderColor(const UIState &s, const UIColor &c) {
	border_color[s] = c;
}

void UIWidget::SetBgColor(const UIState &s, const UIColor &c) {
	bg_color[s] = c;
}

void UIWidget::SetCaptionColor(const UIState &s, const UIColor &c) {
	caption_color[s] = c;
}

void UIWidget::SetFontColor(const UIState &s, const UIColor &c) {
	font_color[s] = c;
}

void UIWidget::SetBounds(const UIRect &bounds) {
	rect = bounds;
}

void UIWidget::SetPos(const UIPos & pos) {
	rect.x = pos.x;
	rect.y = pos.y;
}
UIRect UIWidget::GetBounds() const {
	return rect;
}
void UIWidget::AddChild(UIWidget *wPtr) {
	children.push_back(wPtr);
}
				   
void UIWidget::RenderBg() {
	if (display_bg) {
		
		UIPos offset = GetParentPos();
		float x =  offset.x;
		float y =  offset.y;
		
		
		glBegin(GL_QUADS);
		glColor3ub (bg_color[state].r,bg_color[state].g,bg_color[state].b);

		glVertex2f(x + rect.w, y);
		glVertex2f(x, y);
		glVertex2f(x, y + rect.h);
		glVertex2f(x + rect.w, y + rect.h);
		
		glEnd();
	}
}

void UIWidget::RenderBorder() {
	if (display_border) {
		UIPos offset = GetParentPos();
		float x =  offset.x;
		float y =   offset.y;
		
		glLineWidth(1.0f);
		
		
		glBegin(GL_LINE_LOOP);
		glColor3ub(border_color[state].r,border_color[state].g,border_color[state].b);
		glVertex2f(x + rect.w + 1, y);
		glVertex2f(x, y);
		glVertex2f(x, y + rect.h);
		glVertex2f(x + rect.w, y + rect.h );
		
		glEnd();
	}
	
}

void UIWidget::BaseInit() {
	border_width = 1.0f;
	GLubyte r,g,b;
	
	r = 114;
	g = 114;
	b = 114;
	SetBgColor(kNone,UIColor(r,g,b,255));
	SetBgColor(kHover,UIColor(r,g,b,255));
	SetBgColor(kClicked,UIColor(r,g,b,255));
	
	r = 0;
	g = 0;
	b = 0;
	SetFontColor(kNone,UIColor(r,g,b,255));
	SetFontColor(kHover,UIColor(r,g,b,255));
	SetFontColor(kClicked,UIColor(r,g,b,255));
	
	r = 92;
	g = 92;
	b = 92;
	SetBorderColor(kNone,UIColor(r,g,b,255));
	SetBorderColor(kHover,UIColor(r,g,b,255));
	SetBorderColor(kClicked,UIColor(r,g,b,255));
	
	
	r = 159;
	g = 159;
	b = 159;
	SetCaptionColor(kNone,UIColor(r,g,b,255));
	SetCaptionColor(kHover,UIColor(r,g,b,255));
	SetCaptionColor(kClicked,UIColor(r,g,b,255));
	
	Show();
	state = kNone;
	mouse_active = false;
	mouse_clicked = false;
	display_border = false;
	display_bg = true;
}

void UIWidget::Show() {
	visible = true;
}

void UIWidget::Hide() {
	visible = false;
}

void UIWidget::SetDragable(bool value) {
	dragable = value;
}
bool UIWidget::GetDragable()const {
	return dragable;
}

UIPos UIWidget::GetParentPos()  {
	if (parent == NULL) {
		return UIPos(rect.x,rect.y);
	} else {
		UIPos temp = parent->GetParentPos();
		temp.x += rect.x;
		temp.y += rect.y;
		
		UIPos local_offset;
		local_offset = GetLocalOffset();
		temp.x += local_offset.x;
		temp.y += local_offset.y;
		
		
		return temp;
	}
	
}


bool UIWidget::Update() {
	
	bool ret = false;
	int myX(0), myY(0);
	glfwGetMousePos(&myX,&myY);
	if( glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
		mouse_clicked = true;
		ret = MouseHandler((float)myX, (float)myY);
	} else if (mouse_clicked == true){
	
		mouse_clicked = false;
		ClearMouseActive();
		ret = true;
	}
	return ret;
	
}

void UIWidget::BringToFront(UIWidget *ptr) {
	
	unsigned int len = children.size();
	std::vector <UIWidget *>temp;
	for( unsigned int i =0; i < len; ++i) {
		if (children[i] != ptr) {
			temp.push_back(children[i]);
		}
	}
	children.clear();
	for( unsigned int i =0; i < len - 1; ++i) {
		children.push_back(temp[i]);
	}
	children.push_back(ptr);
}

bool UIWidget::IsMouseActive()const {
	return mouse_active == true ;
}

void UIWidget::SetText( const std::string &s){	
}

void UIWidget::AppendText(const std::string &s) {}
void UIWidget::Backspace() {}


UIPos UIWidget::GetLocalOffset() {
	return UIPos(0.0f,0.0f);
}
