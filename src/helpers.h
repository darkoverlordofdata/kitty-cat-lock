#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>

void die(const char *errstr, ...);
int file_exists(char* filename);

#ifndef WITH_BSD_AUTH
const char * get_password();
#endif

char* image_filename(char* str, char* user, char* theme, char* name, char* type);
char* calendar_filename(char* str, char* user, char* name, char* type);
int char_is_hex(char hexch);
int string_is_hex(char* hexst);
