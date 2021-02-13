#pragma once
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include <pwd.h>
#include <unistd.h>

#include <getopt.h>

#include <Imlib2.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>
#include <X11/extensions/dpms.h>
#include <X11/keysym.h>
#include <Xft/Xft.h>

#include "helpers.h"
#include "calendar/calendar.h"

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
    Display *disp;      // application display
    int width;
    int height;
    Window active;      // the active window
    Window top;         // top level window
    Window panel;       // drawing panel
    XftDraw *draw;      // drawable
    XftColor color;     // color to draw
    XftColor bgcolor;   // color tp draw
    Colormap cm;
    Cursor invisible;
    int screen;
    bool first;         // ignore 1st esc key press

    /* Application values */
    char* calendar;
    Holidays* holidays;
    char** script;
    char* user_name;
    char* full_name;
    char* passwd;
    unsigned int len;
    char* buf;
    bool running;
    ApplicationState state;

    bool scrot;
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

