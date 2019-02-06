#include <dlfcn.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "libui/ui.h"

typedef struct CInitOptionsS {
  char* args;
  char* url;
  uint32_t width;
  uint32_t height;
  float density;
  bool enable_subpixel_text_antialiasing;
} CInitOptions;

typedef struct CHostCallbacksS {
  void(*flush)();
  void(*make_current)();
  void(*on_load_started)();
  void(*on_load_ended)();
  void(*on_title_changed)(const char*);
  void(*on_url_changed)(const char*);
  void(*on_history_changed)(bool, bool);
  void(*on_animating_changed)(bool);
  void(*on_shutdown_complete)();
} CHostCallbacks;

typedef char* servo_version();
typedef void servo_init_with_gl(CInitOptions o, void(*fun)(), char*(*readfile)(char*), CHostCallbacks c);
typedef void servo_perform_updates();

int loadServo();

void perform_updates();
void flush();
void make_current();

static void onMouseCrossed(uiOpenGLAreaHandler *h, uiOpenGLArea *a, int left) { }
static void onDragBroken(uiOpenGLAreaHandler *h, uiOpenGLArea *a) { }
static int onKeyEvent(uiOpenGLAreaHandler *h, uiOpenGLArea *a, uiAreaKeyEvent *e) { return 0; }
