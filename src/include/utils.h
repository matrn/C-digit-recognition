#ifndef GUI_UTILS_H
#define GUI_UTILS_H

#include <assert.h>
#include <gtk/gtk.h>

uint8_t *gdk_pixbuf_to_uint8_grayscale(GdkPixbuf *pixbuf);
GdkPixbuf *uint8_grayscale_to_gdk_pixbuf(const uint8_t *in_img, const int rows, const int cols);

#endif