
#ifndef __UIWIDGET__
#define __UIWIDGET__


#ifdef __APPLE__
#include "GL/glfw.h"
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#import <GLUT/GLUT.h>
#else
#include "gl/glew.h"
#include "GL/glfw.h"

#endif

#include <vector>
#include <deque>
#include <string>




using namespace std;

struct UIColor {
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
	
	UIColor(GLubyte red = 0,GLubyte green = 0,GLubyte blue = 0,GLubyte alpha = 255):r(red),g(green),b(blue),a(alpha){}	
};

struct UIPos {
	float x;
	float y;
	
	UIPos(float xcord = 0 ,float ycord = 0): x(xcord), y(ycord){}
};

struct UIRect {
	float x;
	float y;
	float w;
	float h;
	UIRect(float xcord = 0,float ycord = 0, float width = 10, float height = 10): x(xcord), y(ycord), w(width),h(height){}
	bool Intersects(float x2, float y2) {
		if ( x2 >= x && x2 <= (x + w) ) {
			if ( y2 >= y && y2 <= (y + h) ) {
				return true;
			}
		}
		return false;
	}
};

enum UIState{kNone=0,kHover,kClicked};


// BASE WIDGET CLASS

class UIWidget {
	
public:
	
	UIWidget();
	void AddParent(UIWidget *parent);
	virtual ~UIWidget();
	virtual void Render() = 0;
	virtual bool MouseHandler(float x, float y) = 0; // positions relative to widget
	virtual UIPos GetLocalOffset();
	
	void RenderChildren();
	void SetBorderColor(const UIState &, const UIColor &);
	void SetBgColor(const UIState &, const UIColor &);
	virtual void SetFontColor(const UIState &, const UIColor &);
	void SetCaptionColor(const UIState &, const UIColor &);
	
	
	virtual void SetBounds(const UIRect &bounds);
	void SetPos(const UIPos & pos);
	UIRect GetBounds() const;
	void AddChild(UIWidget *wPtr);
	bool Update();
	void RenderBg();
	void RenderBorder();
	void Show();
	void Hide();
	void SetDragable(bool);
	bool GetDragable() const;
	 UIPos GetParentPos() ;
	virtual void ClearMouseActive();
	virtual void BringToFront(UIWidget *wPtr);
	
	bool IsMouseActive()const;
	
	//virtual not required to implement
	virtual void SetText(const std::string &s);
	virtual void AppendText(const std::string &s);
	virtual void Backspace();
	
	void ShowBorder(bool value){display_border = value;}
	void ShowBackground(bool value){display_bg = value;}
	bool IsVisable(){ return visible;}
	
protected:
	bool mouse_active;
	void BaseInit();
	
	UIWidget *parent;
	const std::vector<UIWidget *>& GetChildren() const {return children;}
	UIRect rect;
	bool visible;
	bool dragable;
	bool active;
	bool display_border;
	bool display_bg;
	UIState state;
	bool mouse_clicked;
	//styling
	int border_width;
	UIColor border_color[3]; //border[state][rgba]
	UIColor caption_color[3]; //border[state][rgba]
	
	UIColor bg_color[3];
	UIColor font_color[3];
	std::vector<UIWidget *> children;
	
private:
	
};


//Global click variable
extern UIWidget *gMouse;
extern UIWidget *gCarot;

extern void GLFWCALL onKey(int key, int action);
extern void HandleSpecialKeyInput();

#endif