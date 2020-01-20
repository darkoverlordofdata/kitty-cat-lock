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
#include "util/options.h"
#include "util/font.h"
#include "util/image.h"
#include "util/display.h"

/**
 * Application interface
 */
typedef struct Application Application;
typedef enum  { ApplicationDate, ApplicationPassword } ApplicationState;
typedef enum  { ApplicationInit, ApplicationKeyPress, ApplicationEscape, ApplicationTimeout } ApplicationEvent;



Application* application_new(int argc, char **argv);
void    application_dispose(Application* this);
int     application_args(Application* app, int argc, char **argv);
void    application_fonts(Application* app, char* name);
int     application_draw(Application* app);
int     application_run(Application* app);
void    application_event(Application* app, ApplicationEvent evt);
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
    Colormap cm;
    Cursor invisible;
    int screen;

    /* Application values */
    char** script;
    char* user_name;
    char* full_name;
    char* passwd;
    unsigned int len;
    char* buf;
    Bool running;
    ApplicationState state;
    LockOptions* options;
    LockFont* font;
    LockImage* image0;
    LockImage* image1;
    LockDisplay* display;

    /* Application values */
    Bool scrot;
    Bool one_time;
    int verbosity;
    char* theme_name;
    char* fontname;
    char* fontname_small;
    char* fontname_name;
    char* fontname_pwd;
    char* fontname_time;
    char* fontname_date;
    XftFont *font_small;
    XftFont *font_name;
    XftFont *font_pwd;
    XftFont *font_time;
    XftFont *font_date;
    char* uline;
    char* pline;
    char* tline;
    char* dline;
    char* imgfn;
    char* boximgfn;

};

