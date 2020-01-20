#include "font.h"

struct LockFont {
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
};
