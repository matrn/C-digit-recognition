#ifndef RECOG_GUI_H
#define RECOG_GUI_H

#include <gtk/gtk.h>
#include "../../lib/ceural/include/ceural.h"
#include "../../lib/lag/include/matrix.h"

#define GUI_MAIN_WINDOW_NAME "C digit recognition"
#define GUI_OUT_WINDOW_NAME "Cropped"



typedef uint8_t byte;


// from: https://code.woboq.org/gtk/gtk/examples/drawing.c.html
/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;
static int surface_width = -1;
static int surface_height = -1;


static GtkWidget *image;



int crecog_gui_setup(int argc, char **argv);



#endif