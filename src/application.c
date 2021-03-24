#include "application.h"
#include "calendar/calendar.h"

#define BUFLEN 256

// guarded free
#define FREE_IF(ptr) if (ptr != NULL) free(ptr)

/**
 * Decode process arguments
 */
static const struct option longopts[] = {
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'V'},
    {"scrot", no_argument, NULL, 's'},
    {"calendar", required_argument, NULL, 'c'},
    {"pin", required_argument, NULL, 'p'},
    {"verbosity", required_argument, NULL, 'v'},
    {"font", required_argument, NULL, 'f'},
    {"theme", required_argument, NULL, 't'},
};

/**
 * Application Create
 * 
 * @param argc
 * @param argv
 * @returns new object
 */
Application* application_new(int argc, char **argv) {
    Application* this = calloc(1, sizeof(Application));
    this->running = true;
    this->uline = calloc(BUFLEN, sizeof(char));
    this->pline = calloc(BUFLEN, sizeof(char));
    this->tline = calloc(BUFLEN, sizeof(char));
    this->dline = calloc(BUFLEN, sizeof(char));
    this->imgfn = calloc(BUFLEN, sizeof(char));
    this->boximgfn = calloc(BUFLEN, sizeof(char));
    this->buf = calloc(BUFLEN, sizeof(char));
    this->passwd = calloc(BUFLEN, sizeof(char));
    this->scrot = false;
    this->first = true;
    struct passwd *pw = getpwuid(getuid());
    this->user_name = strdup(pw->pw_name);
    this->full_name = strdup(pw->pw_gecos);
    application_args(this, argc, argv);
    if (strcmp(this->calendar, "") == 0)
        this->holidays = NULL;
    else {
        char s[255];
        this->holidays = holidays_new(calendar_filename(s, this->user_name, this->calendar, "ics"));
    }
    this->script = (char**)malloc(4 * sizeof(char *));
    this->script[0] = strdup("/usr/local/bin/scrot");
    this->script[1] = strdup("-d");
    this->script[2] = strdup("1");
    this->script[3] = NULL;
    return this;
}

/**
 * Application Dispose
 *
 * @param this
 */
void application_dispose(Application* this) {
    FREE_IF(this->uline);
    FREE_IF(this->pline);
    FREE_IF(this->tline);
    FREE_IF(this->dline);
    FREE_IF(this->imgfn);
    FREE_IF(this->boximgfn);
    FREE_IF(this->theme_name);
    FREE_IF(this->fontname);
    FREE_IF(this->fontname_small);
    FREE_IF(this->fontname_name);
    FREE_IF(this->fontname_pwd);
    FREE_IF(this->fontname_date);
    FREE_IF(this->fontname_time);
    FREE_IF(this->buf);
    FREE_IF(this->passwd);
    FREE_IF(this->script[0]);
    FREE_IF(this->script[1]);
    FREE_IF(this->script[2]);
    FREE_IF(this->script);
    FREE_IF(this->calendar);
    // holidays_dispose(this->holidays);
    FREE_IF(this->user_name);
    FREE_IF(this->full_name);
    FREE_IF(this->pin);
    FREE_IF(this);
}

/**
 * Application Get Args
 *
 * Set defaults, inspect arguments
 *
 * @param this
 * @param argc
 * @param argv
 */
int application_args(Application* this, int argc, char **argv) {

    int longindex = -1, opt;

    int h = 0, v = 0;

    this->verbosity = 0;
    this->theme_name = strdup("badabing");
    this->calendar = strdup("");
    application_fonts(this, "bitstream vera sans-");

    if (this->verbosity > 1) { puts("process command line arguments...\n"); }

    while ((opt = getopt_long(argc, argv, "oshVt:v:f:a:c:p:", longopts, &longindex))
          != -1) {
        switch (opt) {
        case 's':
            this->scrot = true;
            break;
        case 'h':
            h = 1;
            break;
        case 'V':
            v = 1;
            break;
        case 'c':
            this->calendar = strdup(optarg);
            break;
        case 'p':
            this->pin = strdup(optarg);
            break;
        case 'f':
            application_fonts(this, optarg);
            break;
        case 't':
            FREE_IF(this->theme_name);
            this->theme_name = strdup(optarg);
            break;
        case 'v':
            this->verbosity = atoi(optarg);
            break;
        default:
            break;
        }
    }

    if (this->verbosity > 0) {
        printf("font: \"%s\"\n", this->fontname);
        printf("verbosity: %d\n", this->verbosity);
        printf("pixmap: %d\n\n", 1);
    }

    if (v == 1) {
        puts("\ncatlock v0.1.0, © 2020 Dark Overlord of Data");
        puts("inspired by metalock © 2012 Timothy Beyer");
        exit(0);
    }

    if (h == 1) {
        puts("Usage: catlock [option]\n");

        puts("-h / --help               help (this)");
        puts("-V / --version            version information");
        puts("-s / --scrot              take screen pics");
        puts("-c / --calendar           calendar app, such as \"orage\"");
        puts("-v n / --verbosity n      verbosity level (default: 0)");
        puts("-f name / --font name     X11 font name");
        puts("-t name / --theme name    theme name (default: badabing)");

        puts("\nhttps://github.com/darkoverlordofdata/kitty-cat-lock" );
        exit(0);
    }
    return 0;
  }

  /**
  * Application Generate Fonts
  *
  * Generate various font/size names
  *
  * @param this
  * @param name
  */
  void application_fonts(Application* this, char* name)
  {

    FREE_IF(this->fontname);
    FREE_IF(this->fontname_small);
    FREE_IF(this->fontname_name);
    FREE_IF(this->fontname_pwd);
    FREE_IF(this->fontname_time);
    FREE_IF(this->fontname_date);

    this->fontname = strdup(name);

    char* fontsize_small = "8";
    char* fontsize_name = "24";
    char* fontsize_pwd = "24";
    char* fontsize_date = "32";
    char* fontsize_time = "64";

    this->fontname_small = calloc(sizeof(char), strlen(this->fontname) + strlen(fontsize_small) + 1);
    this->fontname_name = calloc(sizeof(char), strlen(this->fontname) + strlen(fontsize_name) + 1);
    this->fontname_pwd  = calloc(sizeof(char), strlen(this->fontname) + strlen(fontsize_pwd) + 1);
    this->fontname_date = calloc(sizeof(char), strlen(this->fontname) + strlen(fontsize_date) + 1);
    this->fontname_time = calloc(sizeof(char), strlen(this->fontname) + strlen(fontsize_time) + 1);

    strcat(this->fontname_small, this->fontname);
    strcat(this->fontname_small, fontsize_small);

    strcat(this->fontname_name, this->fontname);
    strcat(this->fontname_name, fontsize_name);

    strcat(this->fontname_pwd, this->fontname);
    strcat(this->fontname_pwd, fontsize_pwd);

    strcat(this->fontname_date, this->fontname);
    strcat(this->fontname_date, fontsize_date);
    
    strcat(this->fontname_time, this->fontname);
    strcat(this->fontname_time, fontsize_time);

}


/**
 * Application Draw
 *
 * @param this
 */
int application_draw(Application* this) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char* instruc = "Enter PIN";

    strftime(this->tline, BUFLEN-1, "%l:%M", t);
    strftime(this->dline, BUFLEN-1, "%A, %B %d", t);

    XClearWindow(this->disp, this->active);

    /**
     * rough stab at font metrics...
     * 300 pixels wide fits about 17-18 chars at 24 pt. 
     */
    const float fac24 = 300.0f/17.0f;
    /** 
     * and 8pt is 1/3 of 24 pt... 
     */
    const float fac08 = (300.0f/17.0f)/3.0f;

    int c = (this->width-(int)(strlen(this->uline) * fac24))/2;
    int c1 = ((this->width-300)/2);
    int c2 = (this->width- (int)(strlen(instruc) * fac08))/2;

    switch (this->state) {

    case ApplicationDate:
        XftDrawString8(this->draw, &this->color, this->font_time, 40, 600, (XftChar8 *)this->tline, strlen(this->tline));
        XftDrawString8(this->draw, &this->color, this->font_date, 40, 670, (XftChar8 *)this->dline, strlen(this->dline));
        break;

    case ApplicationPassword:
        XftDrawString8(this->draw, &this->color, this->font_name, c,  480, (XftChar8 *)this->uline, strlen(this->uline));
        XftDrawRect(this->draw, &this->color, c1-1, 529, 302, 32);
        XftDrawRect(this->draw, &this->bgcolor, c1, 530, 300, 30);
        XftDrawString8(this->draw, &this->color, this->font_pwd,  c1, 560, (XftChar8 *)this->pline, strlen(this->pline));
        XftDrawString8(this->draw, &this->color, this->font_small,  c2, 660, (XftChar8 *)instruc, strlen(instruc));
        break;

    default: 
        exit(1);

    }

    return 0;
}

void fn_iterate(Holiday* this)
{
    printf("%s: %d\n", this->description, this->date);

}
/**
 * Application Run
 *
 * @param this
 * @param user_name
 */
int application_run(Application* this) {
    if (this->holidays != NULL)
        holidays_filter(this->holidays, 20210101, fn_iterate);

    /* how many characters of password to show visually */
    const int pass_num_show = 32;
    const int timeout = 1000;
    
    XEvent ev = { };
    KeySym ksym;
    int num;
    // static const char *pws = NULL;
    int ticks = 0;
    int inactive = timeout;

    /* main event loop */
    strcat(this->uline, this->full_name);
    strcpy(this->pline, "");
    application_draw(this);

    if (this->scrot) {
        if (fork() == 0) execv(this->script[0], this->script);
    }
    while(this->running) {
        if(ev.type == KeyRelease) {
            this->buf[0] = 0;
        }
        else if(ev.type == KeyPress) {
            application_event(this, ApplicationKeyPress);
            inactive = timeout;
            if (this->scrot) {
                this->scrot = false;
                if (fork() == 0) execv(this->script[0], this->script);
            }

            this->buf[0] = 0;
            num = XLookupString(&ev.xkey, this->buf, BUFLEN, &ksym, 0);

            switch(ksym) {
            case XK_Return:
                this->passwd[this->len] = 0;

                if (this->pin == NULL) { 
                    puts("No Pin!\n"); 
                    this->running = 0;
                }
                else {
                    this->running = strcmp(this->passwd, this->pin);
                }

                if (this->verbosity > 1) { puts("[return]"); }

                if (this->running != 0) {
                    XBell(this->disp, 100);
                    printf("Password failed!! Try again!\n");
                    strcpy(this->pline, "");
                    application_draw(this);
                }
                this->len = 0;
                if (this->running == 0) { puts("Unlocking Screen..."); }
                break;
            case XK_Escape:
                if (this->state == ApplicationDate) break;

                if (this->first) {
                    this->first = false;
                    break;
                }
                // this->running = false;
                this->len = 0;
                strcpy(this->pline, "");
                application_event(this, ApplicationEscape);
                break;
            case XK_BackSpace:
                if(this->len) {
                    --this->len;
                    if (this->verbosity > 1) { printf("[backspace]"); }
                    char tmp[255] = "";
                    strncpy(tmp, this->pline, strlen(tmp) + this->len);
                    strcpy(this->pline, "");
                    strcat(this->pline, tmp);
                    strcpy(tmp, "");
                    application_draw(this);
                }
                break;
            default:
                if (num && !iscntrl((int) this->buf[0]) && (this->len + num < BUFLEN)) {
                    memcpy(this->passwd + this->len, this->buf, num);
                    this->len += num;
                    if (strlen(this->pline) < pass_num_show) { strcat(this->pline, "*"); }
                    int new_pline_len = strlen(this->pline);
                    application_draw(this);

                    if (this->pin != NULL) { 
                        this->running = strcmp(this->passwd, this->pin);
                    }

                    if (this->verbosity > 1 && new_pline_len < pass_num_show) { printf("*"); }
                }
                else if (this->len + num >= BUFLEN) {
                    this->passwd[this->len] = 0;

                    XBell(this->disp, 100);
                    if (this->verbosity > 1) { puts("(terminated at 255 chars)"); }
                    printf("Specified password is *unreasonably* long!! Try again!\n");
                    this->len = 0;
                    strcpy(this->pline, "");
                    application_draw(this);
                }
                break;
            }
        }
        else if(ev.type == MotionNotify || ev.type == ButtonPress)
            application_draw(this);

        ev.type = -1;
        if (inactive) {
            inactive--;
            if (inactive <= 0) {
                this->len = 0;
                strcpy(this->pline, "");
                application_event(this, ApplicationTimeout);
            }
        }
        while(XPending(this->disp))
            XNextEvent(this->disp, &ev);
        usleep(50000);

        if (ticks++ >= (20*60)) {
            ticks = 0;
            application_draw(this);
        }

    }
    return 0;
}


/**
 * Application Init Display
 *
 * @param this
 * @param ApplicationEvent
 */
void application_event(Application* this, ApplicationEvent evt) 
{
    switch (evt) {
    case ApplicationInit:
        this->state = ApplicationDate;
        this->active = this->top;
        break;

    case ApplicationKeyPress:
        this->state = ApplicationPassword;
        this->active = this->panel;
        break;

    case ApplicationTimeout:
        this->state = ApplicationDate;
        this->active = this->top;
        XUnmapWindow(this->disp, this->panel);
        break;

    case ApplicationEscape:
        if (this->state == ApplicationDate) break;
        this->state = ApplicationDate;
        this->active = this->top;
        XUnmapWindow(this->disp, this->panel);
        break;
    }

    XDefineCursor(this->disp, this->active, this->invisible);
    XMapRaised(this->disp, this->active);
    // if the font does not exist, then fallback to fixed and give warning 
    FREE_IF(this->draw);
    this->draw = XftDrawCreate(this->disp, this->active, DefaultVisual(this->disp, this->screen), this->cm);
    // Display the pixmaps, if applicable 
    XClearWindow(this->disp, this->active);
    application_draw(this);
}

/**
 * Application Check Image Files
 *
 * verify the image file names
 * 
 * @param this
 */
void application_image_files(Application* this)
{
    if (*this->imgfn == '\0' || file_exists(this->imgfn) < 0)
        image_filename(this->imgfn, this->user_name, this->theme_name, "locked", "png");
    if (*this->imgfn == '\0' || file_exists(this->imgfn) < 0)
        image_filename(this->imgfn, this->user_name, this->theme_name, "locked", "jpg");
    if (*this->imgfn == '\0' || file_exists(this->imgfn) < 0)
        image_filename(this->imgfn, this->user_name, this->theme_name, "locked", "jpeg");
    if (*this->imgfn == '\0' || file_exists(this->imgfn) < 0)
        image_filename(this->imgfn, this->user_name, this->theme_name, "locked", "xpm");
    if (*this->imgfn == '\0' || file_exists(this->imgfn) < 0)
        image_filename(this->imgfn, this->user_name, this->theme_name, "locked", "xpm.gz");

    if (this->verbosity > 1)
        printf("background image filename: %s\n", this->imgfn);

    if (*this->boximgfn == '\0' || file_exists(this->boximgfn) < 0)
        image_filename(this->boximgfn, this->user_name, this->theme_name, "authorize", "png");
    if (*this->boximgfn == '\0' || file_exists(this->boximgfn) < 0)
        image_filename(this->boximgfn, this->user_name, this->theme_name, "authorize", "jpg");
    if (*this->boximgfn == '\0' || file_exists(this->boximgfn) < 0)
        image_filename(this->boximgfn, this->user_name, this->theme_name, "authorize", "jpeg");
    if (*this->boximgfn == '\0' || file_exists(this->boximgfn) < 0)
        image_filename(this->boximgfn, this->user_name, this->theme_name, "authorize", "xpm");
    if (*this->boximgfn == '\0' || file_exists(this->boximgfn) < 0)
        image_filename(this->boximgfn, this->user_name, this->theme_name, "authorize", "xpm.gz");

    if (this->verbosity > 1)
        printf("box image filename: %s\n\n", this->boximgfn);

    if (file_exists(this->imgfn) < 0) {
        printf("image %s does not exist\n", this->imgfn);
        exit(0);
    }

    if (file_exists(this->boximgfn) < 0) {
        printf("image %s does not exist\n", this->boximgfn);
        exit(0);
    }
}