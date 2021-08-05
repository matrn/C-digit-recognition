#ifndef RECOG_GUI_H
#define RECOG_GUI_H

#include <assert.h>
#include <ceural.h>
#include <gtk/gtk.h>
#include <lag.h>

#include "nn.h"

#define GUI_APP_NAME "org.gtk.c-digit-recognition"
#define GUI_MAIN_WINDOW_NAME "C digit recognition"
#define GUI_RESULT_WINDOW_NAME "Output"

#define DRAW_SIZE 20  // width & height of brush rectangle

GtkWidget *main_window, *result_window;
GtkTextBuffer *result_buffer;

int crecog_gui_setup(int argc, char **argv);

#endif