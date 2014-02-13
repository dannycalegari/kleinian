/*******************************************************
 * graphics.cc - sets up window stuff for main program *
 *                                                     *
 * uses standard Xlib stuff, because I'm too perverse  *
 * to use a GUI toolkit. the main reason is that       *
 * they keep upgrading to newer and newer libraries    *
 * which are not backward compatible, so that my       *
 * programs always start to break. boohoohoo.          *
 *                                                     *
 * Danny Calegari 12/17/2000                           *
 *******************************************************/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

Display *display;
int screen_num;
unsigned int display_width, display_height;
XEvent report;
GC gc;
Window win;
int border_width = 4;
unsigned int width, height;
XFontStruct * font;

void setup_font(void){
    const char * fontname = "-*-georgia-*-r-*-*-14-*-*-*-*-*-*-*";
 //   const char * fontname = "-*-times-*-r-*-*-16-*-*-*-*-*-*-*";

    font = XLoadQueryFont (display, fontname);
    /* If the font could not be loaded, revert to the "fixed" font. */
    if (! font) {
        font = XLoadQueryFont (display, "fixed");
        cout << "couldn't find font!\n";
    }
    XSetFont (display, gc, font->fid);
}

void setup_graphics(void){
	display=XOpenDisplay(NULL);
	display_width = DisplayWidth(display, screen_num);
	display_height = DisplayHeight(display, screen_num);
	screen_num = DefaultScreen(display);  
	width = 1000;
	height = 860;
	win = XCreateSimpleWindow(display, RootWindow(display, screen_num), 0, 0, width, 
		height, border_width, BlackPixel(display, screen_num), WhitePixel(display, screen_num));
	XSelectInput(display, win, ExposureMask | KeyPressMask | ButtonPressMask | PointerMotionMask);
	gc = DefaultGC(display, screen_num);
	XSetForeground(display, gc, BlackPixel(display, screen_num));
	XSetBackground(display, gc, WhitePixel(display, screen_num));
  	XMapWindow(display, win);
  	setup_font();
}

void erase_field(void){
	XClearWindow(display, win);
}

point mouse_location(){
//    Bool result;
    Window window_returned;
    int root_x, root_y;
    int win_x, win_y;
    unsigned int mask_return;
    point p;
    
	XQueryPointer(display, win, &window_returned,
                &window_returned, &root_x, &root_y, &win_x, &win_y,
                &mask_return);
    p.x=win_x;
    p.y=win_y;
    return(p);
};

void draw_point(point p, long col){
    XSetForeground(display, gc, col);
	XDrawPoint(display, win, gc, p.x, p.y);
};

void draw_line(point p1, point p2, long col){
    XSetForeground(display, gc, col);
    XSetLineAttributes(display, gc, 2, LineSolid, 1, 1);
    XDrawLine(display, win, gc, p1.x, p1.y, p2.x, p2.y);
};

void draw_triangle(point p1, point p2, point p3, long col){
	XPoint points[3];
	points[0].x=p1.x;
	points[0].y=p1.y;
	points[1].x=p2.x;
	points[1].y=p2.y;
	points[2].x=p3.x;
	points[2].y=p3.y;
	XSetForeground(display, gc, col);
	XSetFillStyle(display, gc, FillSolid);
	XFillPolygon(display, win, gc, points, 3, Convex, CoordModeOrigin);
};

void draw_faint_line(point p1, point p2, long col){
    XSetForeground(display, gc, (long) 0xDDDDDD);
	XSetLineAttributes(display, gc, 1, LineOnOffDash, 1, 1);
    XDrawLine(display, win, gc, p1.x, p1.y, p2.x, p2.y);
};

void erase_circle(point p, int r){
	XSetForeground(display, gc, 0xFFFFFF);
    XSetLineAttributes(display, gc, 1, LineOnOffDash, 1, 1);
	XSetFillStyle(display, gc, FillSolid);
    XFillArc(display, win, gc, p.x-r, p.y-r, 2*r, 2*r, 0, 23040);
};

void draw_circle(point p, int r, long col){
    XSetForeground(display, gc, col);
    XSetLineAttributes(display, gc, 1, LineSolid, 1, 1);
    XSetFillStyle(display, gc, FillSolid);
    XDrawArc(display, win, gc, p.x-r, p.y-r, 2*r, 2*r, 0, 23040);
};

void draw_concentric_circles(point p, int r, long col){
	int s;
    for(s=3;s>0;s--){
	    XSetForeground(display, gc, col*s);
    	XSetLineAttributes(display, gc, 1, LineSolid, 1, 1);
    	XSetFillStyle(display, gc, FillSolid);
	    XDrawArc(display, win, gc, p.x-r*s+1, p.y-r*s+1, 2*r*s-2, 2*r*s-2, 0, 23040);
	};
};

void draw_path(vector<point> L, long col){
	int i;
	for(i=1;i<(int) L.size();i++){
		draw_line(L[i-1],L[i],col);
	};
};

void draw_text(point p, stringstream &T, long col){
    string S;
    XSetForeground(display, gc, col);
    S=T.str();
	XDrawString(display,win,gc,p.x,p.y,S.c_str(),strlen(S.c_str()));
};

void draw_label(point p, int i, long col){
    XSetForeground(display, gc, col);
	stringstream T;
	T << i;
	p.x=p.x+5;
	p.y=p.y-5;
	draw_text(p,T,col);
};