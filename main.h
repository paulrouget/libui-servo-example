#include <dlfcn.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "libui/ui.h"

int loadServo();

void perform_updates_();
void flush();
void make_current();

static void onMouseCrossed(uiOpenGLAreaHandler *h, uiOpenGLArea *a, int left) { }
static void onDragBroken(uiOpenGLAreaHandler *h, uiOpenGLArea *a) { }
static int onKeyEvent(uiOpenGLAreaHandler *h, uiOpenGLArea *a, uiAreaKeyEvent *e) { return 0; }
