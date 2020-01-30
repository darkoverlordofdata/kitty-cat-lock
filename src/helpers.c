#include "helpers.h"
/**
 * misc helper functions
 */

/**
 * print message and exit with failure
 */
void die(const char *errstr, ...) {
    va_list ap;

    va_start(ap, errstr);
    vfprintf(stderr, "catlock: ", ap);
    vfprintf(stderr, errstr, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}

/**
 * return 0 if file exists, otherwise return negative number
 */
int file_exists(char* filename) {
    FILE *fn;
    fn = fopen (filename, "r");
    if (fn) { fclose(fn); return 0; }
    else { return -1; }
}

/**
 * get the use account info
 *
 * only run as root
 */
const char * get_password() {
    const char *rval;
    struct passwd *pw;

    if(geteuid() != 0) {
        die("cannot retrieve password entry (make sure to suid catlock)\n");
    }
    pw = getpwuid(getuid());
    endpwent();
    rval =  pw->pw_passwd;

    /* drop privileges */
    if(setgid(pw->pw_gid) < 0 || setuid(pw->pw_uid) < 0)
        die("cannot drop privileges\n");
    return rval;
}

/**
 * generate the full path to image file
 *
 * ~/.local/share/catlock/themes/$theme/$name.$type
 */
char* image_filename(char* str, char* user, char* theme, char* name, char* type) {
    strcpy(str, "");
    strcat(str, "/home/");
    strcat(str, user);
    strcat(str, "/.local/share/catlock/themes/");
    strcat(str, theme);
    strcat(str, "/");
    strcat(str, name);
    strcat(str, ".");
    strcat(str, type);
    return str;

}

/**
 * generate the full path to calendar file
 *
 *  ex:    char* fn = calendar_filename("darko", "orage", "ics", &str);
 *
 *  ~/.local/share/orage/orage.ics
 */
char* calendar_filename(char* str, char* user, char* name, char* type) {
    strcpy(str, "");
    strcat(str, "/home/");
    strcat(str, user);
    strcat(str, "/.local/share/");
    strcat(str, name);
    strcat(str, "/");
    strcat(str, name);
    strcat(str, ".");
    strcat(str, type);
    return str;

}

/**
 * check if hex char
 */
int char_is_hex(char hexch) {
    if (hexch == '0' || hexch == '1' || hexch == '2' || hexch == '3' ||
        hexch == '4' || hexch == '5' || hexch == '6' || hexch == '7' ||
        hexch == '8' || hexch == '9' || hexch == 'a' || hexch == 'A' ||
        hexch == 'b' || hexch == 'B' || hexch == 'c' || hexch == 'C' ||
        hexch == 'd' || hexch == 'D' || hexch == 'e' || hexch == 'E' ||
        hexch == 'f' || hexch == 'F') { return 0; }
    else { return -1; }
}

/**
 * check if hex string
 */
int string_is_hex(char* hexst) {
    if (strlen(hexst) == 7 && hexst[0] == '#') {
        if (char_is_hex(hexst[1]) == 0 && char_is_hex(hexst[2]) == 0 &&
            char_is_hex(hexst[3]) == 0 && char_is_hex(hexst[4]) == 0 &&
            char_is_hex(hexst[5]) == 0 && char_is_hex(hexst[6]) == 0) { return 0; }
        else { return -1; } }
    else if((strlen(hexst) == 8 && hexst[0] == '0' && hexst[1] == 'x') ||
            (strlen(hexst) == 8 && hexst[0] == '0' && hexst[1] == 'X')) {
        if (char_is_hex(hexst[2] == 0) && char_is_hex(hexst[3]) == 0 &&
            char_is_hex(hexst[4] == 0) && char_is_hex(hexst[5]) == 0 &&
            char_is_hex(hexst[6] == 0) && char_is_hex(hexst[7]) == 0) { return 0; }
        else { return -1; } }
    else { return -1; }
}

