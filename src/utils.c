#include "include/utils.h"

uint8_t *gdk_pixbuf_to_uint8_grayscale(GdkPixbuf *pixbuf) {
	int rows = gdk_pixbuf_get_height(pixbuf);
	int cols = gdk_pixbuf_get_width(pixbuf);

	assert(gdk_pixbuf_get_rowstride(pixbuf) == cols * 4);

	uint8_t *buffer = gdk_pixbuf_get_pixels(pixbuf);
	uint8_t *img_out = malloc(rows * cols * sizeof(uint8_t));
	int index = 0;
	for (int i = 0; i < rows * cols * 4; i++) {
		uint8_t r = buffer[i];
		uint8_t g = buffer[++i];
		uint8_t b = buffer[++i];
		i++;  //uint8_t a = buffer[++i];

		uint8_t gray = 255 - (r * 0.3 + g * 0.59 + b * 0.11);
		//if(gray > 0) gray = 255;
		//else gray = 0;
		img_out[index++] = gray;
	}

	free(buffer);
	return img_out;
}

GdkPixbuf *uint8_grayscale_to_gdk_pixbuf(const uint8_t *in_img, const int rows, const int cols) {
	uint8_t *rgb_img = malloc(rows * cols * 3);
	int index = 0;
	for (int i = 0; i < rows * cols; i++) {
		rgb_img[index++] = 255 - in_img[i];	 // R
		rgb_img[index++] = 255 - in_img[i];	 // G
		rgb_img[index++] = 255 - in_img[i];	 // B
	}

	GdkPixbuf *out_pixbuf = gdk_pixbuf_new_from_bytes(g_bytes_new(rgb_img, rows * cols * 3), GDK_COLORSPACE_RGB, FALSE, 8, cols, rows, cols * 3);
	free(rgb_img);

	return out_pixbuf;
}