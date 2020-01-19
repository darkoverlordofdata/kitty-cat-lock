/**
 * wallpaper-lock
 *
 */
/* See LICENSE file for license details. */
#include "main.h"
#include "helpers.h"

int main(int argc, char **argv) {

    // struct passwd *pw = getpwuid(getuid());
    Application* app = application_new(argc, argv);

    if(!(app->disp = XOpenDisplay(0))) { die("cannot open display\n"); }
    app->screen = DefaultScreen(app->disp);
    int depth = DefaultDepth(app->disp, app->screen);
    Window root = RootWindow(app->disp, app->screen);

    app->width = DisplayWidth(app->disp, app->screen);
    app->height = DisplayHeight(app->disp, app->screen);

    app->cm = DefaultColormap(app->disp, app->screen);

    XColor bg = { 0, 0, 0, 0 }; // black
    XColor fg = { 0, 65535, 65535, 65535 }; // white

    XAllocColor(app->disp, app->cm, &bg);
    XAllocColor(app->disp, app->cm, &fg);

    /* Create the top level window */
    XSetWindowAttributes wa = { .override_redirect = 1, .background_pixel = bg.pixel };
    Visual *vis = DefaultVisual(app->disp, app->screen);
    app->top = XCreateWindow(app->disp, root, 0, 0, app->width, app->height,
                      0, depth, CopyFromParent,
                      vis, CWOverrideRedirect | CWBackPixel, &wa);
    

    imlib_context_set_dither(1);
    imlib_context_set_display(app->disp);
    imlib_context_set_visual(vis);
    application_image_files(app);
    
    /* Load the top level background image */
    Imlib_Image image = imlib_load_image(app->imgfn);
    imlib_context_set_image(image);
    int width = imlib_image_get_width();
    int height = imlib_image_get_height();

    /* Set the top level background image */
    Pixmap pixm = XCreatePixmap(app->disp, app->top, width, height, depth);
    imlib_context_set_drawable(pixm);
    imlib_render_image_on_drawable(0, 0);
    XSetWindowBackgroundPixmap(app->disp, app->top, pixm);
    XFreePixmap(app->disp, pixm);

    /* Load the panel background image */
    Imlib_Image box_image = imlib_load_image(app->boximgfn);
    imlib_context_set_image(box_image);
    int panel_width = imlib_image_get_width();
    int panel_height = imlib_image_get_height();

    app->active = app->panel = XCreateSimpleWindow(app->disp, app->top, 0, 0, panel_width, panel_height,
                          0, fg.pixel, bg.pixel);

    Pixmap pix = XCreatePixmap(app->disp, app->panel, panel_width, panel_height, depth);
    imlib_context_set_drawable(pix);
    imlib_render_image_on_drawable(0, 0);
    XSetWindowBackgroundPixmap(app->disp, app->panel, pix);
    XFreePixmap(app->disp, pix);

    // // set color to white 
    XColor black, dummy;
    XRenderColor xrb = {.red = bg.red, .green = bg.green, .blue = bg.blue, .alpha = 0xffff };
    XRenderColor xrc = {.red = fg.red, .green = fg.green, .blue = fg.blue, .alpha = 0xffff };
    XftColorAllocValue(app->disp, DefaultVisual(app->disp, app->screen), app->cm, &xrb, &app->bgcolor);
    XftColorAllocValue(app->disp, DefaultVisual(app->disp, app->screen), app->cm, &xrc, &app->color);
    XAllocNamedColor(app->disp, app->cm, "black", &black, &dummy);

    char curs[] = {0, 0, 0, 0, 0, 0, 0, 0};
    Pixmap pmap = XCreateBitmapFromData(app->disp, app->top, curs, 8, 8);
    app->invisible = XCreatePixmapCursor(app->disp, pmap, pmap, &black, &black, 0, 0);

    for(app->len = 1000; app->len; app->len--) {
        if(XGrabPointer(app->disp, root, False,
                      ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
                      GrabModeAsync, GrabModeAsync, None, app->invisible, CurrentTime)
            == GrabSuccess)
            break;
        usleep(100);
    }
    if((app->running = app->running && (app->len > 0))) {
        for(app->len = 1000; app->len; app->len--) {
            if(XGrabKeyboard(app->disp, root, True, GrabModeAsync, GrabModeAsync,
                        CurrentTime) == GrabSuccess)
                break;
            usleep(100);
        }
        app->running = (app->len > 0);
    }
    app->len = 0;
    XSync(app->disp, False);

    app->font_name = XftFontOpenName(app->disp, app->screen, app->fontname_name);
    if (app->font_name == NULL) {
        printf("font \"%s\" does not exist\n", app->fontname);
        application_fonts(app, "6x10-");
        app->font_name = XftFontOpenName(app->disp, app->screen, app->fontname_name);
    }
  
    app->font_small = XftFontOpenName(app->disp, app->screen, app->fontname_small);
    app->font_pwd = XftFontOpenName(app->disp, app->screen, app->fontname_pwd);
    app->font_time = XftFontOpenName(app->disp, app->screen, app->fontname_time);
    app->font_date = XftFontOpenName(app->disp, app->screen, app->fontname_date);

    application_event(app, ApplicationInit);
    /* Set initial Username / Password State */
    // struct passwd *pw = getpwuid(getuid());

    /* main loop */
    application_run(app);
    /** teardown */

    XUngrabPointer(app->disp, CurrentTime);
    XFreePixmap(app->disp, pmap);
    XftColorFree(app->disp, DefaultVisual(app->disp, app->screen), app->cm, &app->color);
    XftDrawDestroy(app->draw);
    XDestroyWindow(app->disp, app->top);
    XCloseDisplay(app->disp);
    application_dispose(app);
    return 0;
}

