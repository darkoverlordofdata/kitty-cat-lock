#include "main.h"
#include "helpers.h"

#define BUFLEN 256
/**
 * Decode process arguments
 */
static const struct option longopts[] = {
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'V'},
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
    this->running = True;
    this->uline = calloc(BUFLEN, sizeof(char));
    this->pline = calloc(BUFLEN, sizeof(char));
    this->tline = calloc(BUFLEN, sizeof(char));
    this->dline = calloc(BUFLEN, sizeof(char));
    this->imgfn = calloc(BUFLEN, sizeof(char));
    this->boximgfn = calloc(BUFLEN, sizeof(char));
    this->buf = calloc(BUFLEN, sizeof(char));
    this->passwd = calloc(BUFLEN, sizeof(char));
    application_args(this, argc, argv);

    this->flag1 = False;//True;
    this->flag2 = False;//True;
    this->script = (char**)malloc(4 * sizeof(char *));
    this->script[0] = strdup("/usr/local/bin/scrot");
    this->script[1] = strdup("-d");
    this->script[2] = strdup("1");


            // if (fork() == 0) execv("/usr/local/bin/scrot", "-d 1");


    return this;
}

/**
 * Application Dispose
 *
 * @param this
 */
void application_dispose(Application* this) {
    free(this->uline);
    free(this->pline);
    free(this->tline);
    free(this->dline);
    free(this->imgfn);
    free(this->boximgfn);
    free(this->theme_name);
    free(this->fontname);
    free(this->fontname_small);
    free(this->fontname_name);
    free(this->fontname_pwd);
    free(this->fontname_date);
    free(this->fontname_time);
    free(this->buf);
    free(this->passwd);
    free(this);
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

    int longindex = -1, opt, j;

    int h = 0, o = 0, c = 0, v = 0;

    this->verbosity = 0;
    this->theme_name = strdup(DEFAULT_THEME);
    application_fonts(this, "bitstream vera sans-");

    if (this->verbosity > 1) { puts("process command line arguments...\n"); }

    while ((opt = getopt_long(argc, argv, "hVt:v:f:", longopts, &longindex))
          != -1) {
        j = 0;
        switch (opt) {
        case 'h':
            h = 1;
            break;
        case 'V':
            v = 1;
            break;
        case 'c':
            c = 1;
            break;
        case 'C':
            o = 1;
            break;
        case 'f':
            application_fonts(this, optarg);
            break;
        case 't':
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
        printf("wallpaperlock-%s, © 2020 Dark Overlord of Data\n", VERSION);
        printf("inspired by metalock-0.8.1, © 2012 Timothy Beyer\n");
        exit(0);
    }

    if (h == 1) {
        puts("Usage: binglock [option]\n");

        puts("-h / --help               help (this)");
        puts("-V / --version            version information");
        puts("-v n / --verbosity n      verbosity level (default: 0)");
        puts("-f str / --font str       X11 quoted font name string");
        puts("-t name / --theme name    theme name (default: " DEFAULT_THEME ")");
        puts("\nmail bug reports and suggestions to " EMAIL);
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

    if (this->fontname) free(this->fontname);
    if (this->fontname_small) free(this->fontname_small);
    if (this->fontname_name) free(this->fontname_name);
    if (this->fontname_name) free(this->fontname_pwd);
    if (this->fontname_time) free(this->fontname_time);
    if (this->fontname_date) free(this->fontname_date);

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
    char* instruc = "Enter password";

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

    if (this->state) {

        int c = (this->width-(int)(strlen(this->uline) * fac24))/2;
        int c1 = ((this->width-300)/2);
        int c2 = (this->width- (int)(strlen(instruc) * fac08))/2;

        XftDrawString8(this->draw, &this->color, this->font_name, c,  480, (XftChar8 *)this->uline, strlen(this->uline));
        XftDrawRect(this->draw, &this->color, c1-1, 529, 302, 32);
        XftDrawRect(this->draw, &this->bgcolor, c1, 530, 300, 30);
        XftDrawString8(this->draw, &this->color, this->font_pwd,  c1, 560, (XftChar8 *)this->pline, strlen(this->pline));
        XftDrawString8(this->draw, &this->color, this->font_small,  c2, 660, (XftChar8 *)instruc, strlen(instruc));
    } else {
        XftDrawString8(this->draw, &this->color, this->font_time, 40, 600, (XftChar8 *)this->tline, strlen(this->tline));
        XftDrawString8(this->draw, &this->color, this->font_date, 40, 670, (XftChar8 *)this->dline, strlen(this->dline));
    }

    return 0;
}

/**
 * Application Run
 *
 * @param this
 * @param user_name
 */
int application_run(Application* this, char *user_name) {

    /* how many characters of password to show visually */
    const int pass_num_show = 32;
    const int timeout = 20;
    XEvent ev;
    KeySym ksym;
    int num;
    static const char *pws = NULL;
    int ticks = 0;
    int inactive = timeout;

    /* main event loop */
    strcat(this->uline, user_name);
    strcpy(this->pline, "");
    application_draw(this);

    if (this->flag1) {
        this->flag1 = False;
        if (fork() == 0) execv(this->script[0], this->script);
    }
    while(this->running) {
        if(ev.type == KeyRelease) {
            this->buf[0] = 0;
        }
        else if(ev.type == KeyPress) {
            this->state = 1;
            application_set_display(this, this->panel);
            inactive = timeout;
            application_draw(this);
            if (this->flag2) {
                this->flag2 = False;
                if (fork() == 0) execv(this->script[0], this->script);
            }

            this->buf[0] = 0;
            num = XLookupString(&ev.xkey, this->buf, BUFLEN, &ksym, 0);

            switch(ksym) {
            case XK_Return:
                this->passwd[this->len] = 0;

                if (pws == NULL) pws = get_password();
                this->running = strcmp(crypt(this->passwd, pws), pws);

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
                // testing failsafe:
                // this->running = 0;
                this->len = 0;
                strcpy(this->pline, "");
                application_set_display(this, this->top);
                application_draw(this);
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

        // if (inactive) {
        //     inactive--;
        //     if (inactive <= 0) {
        //         printf("RESET\n");
        //         active = 0;
        //         this->active = this->top;
        //         application_set_display(this);            
        //         draw_box(this);
        //     }
        // }
        ev.type = -1;
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
 */
void application_set_display(Application* this, Window window)
{

    this->active = window;
    XDefineCursor(this->disp, this->active, this->invisible);
    XMapRaised(this->disp, this->active);
    // if the font does not exist, then fallback to fixed and give warning 
    this->draw = XftDrawCreate(this->disp, this->active, DefaultVisual(this->disp, this->screen), this->cm);
    // Display the pixmaps, if applicable 
    XClearWindow(this->disp, this->active);
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
        image_filename(this->theme_name, "bg", "png", this->imgfn);
    if (*this->imgfn == '\0' || file_exists(this->imgfn) < 0)
        image_filename(this->theme_name, "bg", "jpg", this->imgfn);
    if (*this->imgfn == '\0' || file_exists(this->imgfn) < 0)
        image_filename(this->theme_name, "bg", "jpeg", this->imgfn);
    if (*this->imgfn == '\0' || file_exists(this->imgfn) < 0)
        image_filename(this->theme_name, "bg", "xpm", this->imgfn);
    if (*this->imgfn == '\0' || file_exists(this->imgfn) < 0)
        image_filename(this->theme_name, "bg", "xpm.gz", this->imgfn);

    if (this->verbosity > 1)
        printf("background image filename: %s\n", this->imgfn);

    if (*this->boximgfn == '\0' || file_exists(this->boximgfn) < 0)
        image_filename(this->theme_name, "box", "png", this->boximgfn);
    if (*this->boximgfn == '\0' || file_exists(this->boximgfn) < 0)
        image_filename(this->theme_name, "box", "jpg", this->boximgfn);
    if (*this->boximgfn == '\0' || file_exists(this->boximgfn) < 0)
        image_filename(this->theme_name, "box", "jpeg", this->boximgfn);
    if (*this->boximgfn == '\0' || file_exists(this->boximgfn) < 0)
        image_filename(this->theme_name, "box", "xpm", this->boximgfn);
    if (*this->boximgfn == '\0' || file_exists(this->boximgfn) < 0)
        image_filename(this->theme_name, "box", "xpm.gz", this->boximgfn);

    if (this->verbosity > 1)
        printf("box image filename: %s\n\n", this->boximgfn);

    if (file_exists(this->imgfn) < 0 || file_exists(this->boximgfn) < 0) {
        printf("image %s does not exist\n", this->imgfn);
        exit(0);

    }
}