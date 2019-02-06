// 28 may 2016

#include <dlfcn.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "libui/ui.h"
#include "main.h"

static uiOpenGLArea* sArea = NULL;
static servo_perform_updates* perform_updates_internal = NULL;

void perform_updates() {
  if (perform_updates_internal != NULL) {
    perform_updates_internal();
  }
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
  flush();
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
	uiOpenGLAttributesSetAttribute(attribs, uiOpenGLAttributeDoubleBuffer, 1);
	uiOpenGLAttributesSetAttribute(attribs, uiOpenGLAttributeRedBits, 32);
	uiOpenGLAttributesSetAttribute(attribs, uiOpenGLAttributeGreenBits, 32);
	uiOpenGLAttributesSetAttribute(attribs, uiOpenGLAttributeBlueBits, 32);
	uiOpenGLAttributesSetAttribute(attribs, uiOpenGLAttributeAlphaBits, 8);
	uiOpenGLAttributesSetAttribute(attribs, uiOpenGLAttributeStencilBits, 8);

	uiOpenGLAttributesSetAttribute(attribs, uiOpenGLAttributeMajorVersion, 3);
	uiOpenGLAttributesSetAttribute(attribs, uiOpenGLAttributeMinorVersion, 2);

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

typedef struct CFileContentS {
  char* content;
  uint32_t size;
} CFileContent;

CFileContent readfile2(char* filename) {
  printf("Reading %s\n", filename);
  /* declare a file pointer */
  FILE    *infile;
  char    *buffer;
  long    numbytes;

  /* open an existing file for reading */
  infile = fopen(filename, "r");

  /* quit if the file does not exist */
  if(infile == NULL) {
    exit(1);
  }

  /* Get the number of bytes */
  fseek(infile, 0L, SEEK_END);
  numbytes = ftell(infile);

  fseek(infile, 0L, SEEK_SET);	
  buffer = (char*)calloc(numbytes, sizeof(char));	
  if(buffer == NULL) {
    exit(1);
  }

  fread(buffer, sizeof(char), numbytes, infile);
  fclose(infile);

  CFileContent c;

  c.content = buffer;
  c.size = numbytes;

  return c;
}

CFileContent readfile(char* name) {
  printf("readfile: %s\n", name);
  char buf[256]; // FIXME. Don't.
  snprintf(buf, sizeof buf, "%s%s", "/tmp/resources/", name);
  CFileContent file = readfile2(buf);
  return file;
}

int loadServo() {
  void* handle = dlopen("/tmp/libsimpleservo.dylib", RTLD_LAZY);
  if (handle == NULL) {
    fprintf(stderr, "Could not open libsimpleservo: %s\n", dlerror());
    return 1;
  }

  servo_init_with_gl* init = dlsym(handle, "init_with_gl");
  if (init == NULL) {
    fprintf(stderr, "Could not find init_with_gl symbol: %s\n", dlerror());
    return 1;
  }

  perform_updates_internal = dlsym(handle, "perform_updates");
  if (perform_updates_internal == NULL) {
    fprintf(stderr, "Could not find perform_updates symbol: %s\n", dlerror());
    return 1;
  }

  /** --- init --- **/

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

  init(o, &wakeup, &readfile, c);

  /* if (dlclose(handle) != 0) { */
  /*   fprintf(stderr, "Could not close libsimpleservo: %s\n", dlerror()); */
  /*   return 1; */
  /* } */
  return 0;
}

