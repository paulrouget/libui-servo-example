#include "main.h"

static uiOpenGLArea* sArea = NULL;

void perform_updates_() {
  perform_updates();
}

void flush() {
  printf("calling flush\n");
  uiOpenGLAreaSwapBuffers(sArea);
}

void make_current() {
  printf("calling make_current\n");
  uiOpenGLAreaMakeCurrent(sArea);
}

static void onMouseEvent(uiOpenGLAreaHandler *h, uiOpenGLArea *a, uiAreaMouseEvent *e) {
}

static void onInitGL(uiOpenGLAreaHandler *h, uiOpenGLArea *a) {
  uiOpenGLAreaSetVSync(a, 1);
  uiOpenGLAreaMakeCurrent(a);
  sArea = a;
  loadServo();
}

static void onDrawGL(uiOpenGLAreaHandler *h, uiOpenGLArea *a, double width, double height) {
  make_current();
  /* perform_updates(); */
  /* flush(); */
  /* uiOpenGLAreaQueueRedrawAll(a); */
}

static uiOpenGLAreaHandler AREA_HANDLER = { onDrawGL, onMouseEvent, onMouseCrossed, onDragBroken, onKeyEvent, onInitGL };

static int render(void *d) {
  perform_updates();
  /* uiOpenGLAreaQueueRedrawAll(d); */
  return 1;
}

void wakeup() {
}

static int onClosing(uiWindow *w, void *data)
{
  uiControlDestroy(uiControl(w));
  uiQuit();
  return 0;
}

static int shouldQuit(void *data)
{
  uiControlDestroy((uiControl *)data);
  return 1;
}

int main(void)
{
  uiInitOptions o = { 0 };
  const char *err = uiInit(&o);
  if (err != NULL) {
    fprintf(stderr, "error initializing ui: %s\n", err);
    uiFreeInitError(err);
    return 1;
  }

  uiWindow *mainwin = uiNewWindow("libui OpenGL Example", 900, 900, 1);
  uiWindowSetMargined(mainwin, 1);
  uiWindowOnClosing(mainwin, onClosing, NULL);
  uiOnShouldQuit(shouldQuit, mainwin);

  uiBox *b = uiNewVerticalBox();
  uiBoxSetPadded(b, 1);
  uiWindowSetChild(mainwin, uiControl(b));

  uiBoxAppend(b, uiControl(uiNewLabel("hi")), 0);

  uiOpenGLAttributes *attribs = uiNewOpenGLAttributes();
	uiOpenGLAttributesSetAttribute(attribs, uiOpenGLAttributeMajorVersion, 3);
	uiOpenGLAttributesSetAttribute(attribs, uiOpenGLAttributeMinorVersion, 3);
	uiOpenGLAttributesSetAttribute(attribs, uiOpenGLAttributeCompatProfile, 0);

  uiOpenGLArea *glarea = uiNewOpenGLArea(&AREA_HANDLER, attribs);

  uiBoxAppend(b, uiControl(glarea), 1);

  uiTimer(1000/60, render, glarea);

  uiControlShow(uiControl(mainwin));

  uiMain();

  uiFreeOpenGLAttributes(attribs);

  uiUninit();
  return 0;
}

void on_load_started() {}
void on_load_ended() {}
void on_title_changed() {}
void on_url_changed() {}
void on_history_changed(bool back, bool fwd) {}
void on_animating_changed(bool animating) {}
void on_shutdown_complete() {}

void loadServo() {
  CInitOptions o;
  o.args = "[\"https://example.com\"]";
  o.url = "https://servo.org";
  o.width = 400;
  o.height = 400;
  o.density = 1.0;
  o.enable_subpixel_text_antialiasing = true;

  CHostCallbacks c;
  c.flush = &flush;
  c.make_current = &make_current;
  c.on_load_started = &on_load_started;
  c.on_load_ended = &on_load_ended;
  c.on_title_changed = &on_title_changed;
  c.on_url_changed = &on_url_changed;
  c.on_history_changed = &on_history_changed;
  c.on_animating_changed = &on_animating_changed;
  c.on_shutdown_complete = &on_shutdown_complete;

  init_with_gl(o, &wakeup, c);
}
