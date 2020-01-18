#pragma once
#define _XOPEN_SOURCE 500
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>
#include <X11/extensions/dpms.h>
#include <X11/keysym.h>
#include <Imlib2.h>
#include <Xft/Xft.h>

/**
 * Application interface
 */
typedef struct Application Application;

Application* application_new(int argc, char **argv);
void    application_dispose(Application* this);
int     application_args(Application* app, int argc, char **argv);
void    application_fonts(Application* app, char* name);
int     application_draw(Application* app);
int     application_run(Application* app, char *user_name);
void    application_set_display(Application* app, Window window);
void    application_image_files(Application* app);

/**
 * Application data
 */
struct Application
{
    /* X11 values */
    Display *disp;    // application display
    int width;
    int height;
    Window active;    // the active window
    Window top;       // top level window
    Window panel;     // drawing panel
    XftDraw *draw;    // drawable
    XftColor color;   // color tp draw
    XftColor bgcolor;   // color tp draw

    Bool flag1;
    Bool flag2;
    char** script;
    /* Application values */
    int verbosity;
    char* fontname;
    char* fontname_small;
    char* fontname_name;
    char* fontname_pwd;
    char* fontname_time;
    char* fontname_date;
    char* theme_name;
    XftFont *font_small;
    XftFont *font_name;
    XftFont *font_pwd;
    XftFont *font_time;
    XftFont *font_date;
    Colormap cm;
    Cursor invisible;
    int screen;
    Bool running;
    unsigned int len;
    // const char *pws;
    int state;
    // char *line;
    char* uline;
    char* pline;
    char* tline;
    char* dline;
    char* imgfn;
    char* boximgfn;
    char* passwd;
    char* buf;

};

