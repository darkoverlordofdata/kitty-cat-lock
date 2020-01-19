void die(const char *errstr, ...);
int file_exists(char* filename);

#ifndef WITH_BSD_AUTH
const char * get_password();
#endif

int image_filename(char* user, char* theme, char* name, char* type, char* ret);
int char_is_hex(char hexch);
int string_is_hex(char* hexst);
