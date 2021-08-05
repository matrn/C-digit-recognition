#include "include/gui.h"



void show_image(GdkPixbuf *data) {
	GdkPixbuf *pixbuf = gdk_pixbuf_scale_simple(data, 250, 250, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf((GtkImage *)image, pixbuf);
}



uint8_t * gdk_pixbuf_to_uint8_grayscale(GdkPixbuf * pixbuf){
	int rows = gdk_pixbuf_get_height(pixbuf);
	int cols = gdk_pixbuf_get_width(pixbuf);
	//printf("%dx%d, sride. %d\n", rows, cols, gdk_pixbuf_get_rowstride(pixbuf));
	assert(gdk_pixbuf_get_rowstride(pixbuf) == cols*4);
	
	uint8_t *buffer = gdk_pixbuf_get_pixels(pixbuf);
   	uint8_t * img_out = malloc(rows*cols*sizeof(uint8_t));
	int index = 0;
	for (int i = 0; i < rows*cols*4; i++) {	
		uint8_t r = buffer[i];
		uint8_t g = buffer[++i];
		uint8_t b = buffer[++i];
		i++;   //uint8_t a = buffer[++i];

		uint8_t gray = 255 - (r * 0.3 + g * 0.59 + b * 0.11);
		//if(gray > 0) gray = 255;
		//else gray = 0;
		img_out[index++] = gray;
	}

	free(buffer);
	return img_out;
}

static void clear_surface(void) {
	cairo_t *cr;
	cr = cairo_create(surface);
	cairo_set_source_rgb(cr, 1, 1, 1);
	//cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_paint(cr);
	cairo_destroy(cr);
}

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, gpointer data) {
	if (surface) cairo_surface_destroy(surface);

	surface_width = gtk_widget_get_allocated_width(widget);
	surface_height = gtk_widget_get_allocated_height(widget);
	surface = gdk_window_create_similar_image_surface(gtk_widget_get_window(widget), CAIRO_FORMAT_ARGB32, surface_width, surface_height, 0);
	//surface = cairo_image_surface_create(CAIRO_FORMAT_A8, surface_width, surface_height);

	int width = cairo_image_surface_get_width(surface);
	int height = cairo_image_surface_get_height(surface);
	printf("config: w x h: %dx%d\n", width, height);

	/* Initialize the surface to white */
	clear_surface();

	/* We've handled the configure event, no need for further processing. */
	return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data) {
	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_paint(cr);
	return FALSE;
}



/* Draw a rectangle on the surface at the given position */
static void draw_brush(GtkWidget *widget, gdouble x, gdouble y) {
	cairo_t *cr;
	/* Paint to the surface, where we store our state */
	cr = cairo_create(surface);
	cairo_rectangle(cr, x - DRAW_WIDTH/2, y - DRAW_WIDTH/2, DRAW_WIDTH, DRAW_WIDTH);
	cairo_fill(cr);
	cairo_destroy(cr);
	/* Now invalidate the affected region of the drawing area. */
	gtk_widget_queue_draw(widget);
}

/* Handle button press events by either drawing a rectangle
 * or clearing the surface, depending on which button was pressed.
 * The ::button-press signal handler receives a GdkEventButton
 * struct which contains this information.
 */
static gboolean button_press_event_cb(GtkWidget *widget, GdkEventButton *event, gpointer data) {
	/* paranoia check, in case we haven't gotten a configure event */
	if (surface == NULL) return FALSE;
	if (event->button == GDK_BUTTON_PRIMARY) {
		draw_brush(widget, event->x, event->y);
	} else if (event->button == GDK_BUTTON_SECONDARY) {
		clear_surface();
		gtk_widget_queue_draw(widget);
	}
	/* We've handled the event, stop processing */
	return TRUE;
}

/* Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
static gboolean motion_notify_event_cb(GtkWidget *widget, GdkEventMotion *event, gpointer data) {
	/* paranoia check, in case we haven't gotten a configure event */
	if (surface == NULL) return FALSE;

	if (event->state & GDK_BUTTON1_MASK) draw_brush(widget, event->x, event->y);

	/* We've handled it, stop processing */
	return TRUE;
}

static void close_window(void) {
	if (surface) cairo_surface_destroy(surface);
	gtk_main_quit();
	nn_free();
}



static void recognise(GtkWidget *widget, gpointer data) {
	g_print("Recognise\n");

	cairo_surface_flush(surface);
	cairo_surface_t *img_surface = surface;	 //cairo_surface_create_similar_image(surface, cairo_surface_get_type(surface), surface_width, surface_height);

	//cairo_surface_t * img_surface = cairo_image_surface_create_for_data(img_surface_src, cairo_image_surface_get_format(img_surface_src), surface_width, surface_height, cairo_format_stride_for_width(img_surface_src));

	unsigned char *img_data = cairo_image_surface_get_data(img_surface);
	int width = cairo_image_surface_get_width(img_surface);
	int height = cairo_image_surface_get_height(img_surface);

	cairo_format_t format = cairo_image_surface_get_format(img_surface);
	

	
	int crop_x, crop_y, crop_x2, crop_y2;
	//cropped = matrix_1ubyteMat_crop_edges(&crop_rows, &crop_cols, img, height, width);
	const uint8_t null_values[] = {255,255,255,255};
	//matrix_1ubyteMat_calculate_crop_old(&crop_x, &crop_x2, &crop_y, &crop_y2, img, height, width);
	matrix_1ubyteMat_calculate_crop(&crop_x, &crop_x2, &crop_y, &crop_y2, img_data, height, width, 4, null_values);
	int crop_w = crop_x2 - crop_x + 1;
	int crop_h = crop_y2 - crop_y + 1;
	int max_size = max(crop_w, crop_h);
	crop_x -= (max_size-crop_w)/2;
	crop_y -= (max_size-crop_h)/2;

	crop_w = crop_h = max_size;

	if(crop_x + crop_w > width) crop_x -= crop_x+crop_w-width;
	if(crop_x < 0) crop_x = 0;

	if(crop_y+crop_h > height) crop_y -= crop_y+crop_h-height;
	if(crop_y < 0) crop_y = 0;
	
	printf("Cropped size: %dx%d\n", crop_w, crop_h);
	printf("Crop position: x=%d, y=%d\n", crop_x, crop_y);
	//printf("at 0: %d\n", cropped[0]);
	//if(cropped == NULL) puts("NULLL");
	//puts("ENDCROP");
	/*
	for(int row = 0; row < crop_rows; row ++){
		for(int col = 0; col < crop_cols; col ++){
			printf("%4d ", cropped[row*crop_cols+col]);
		}
		printf("\n");
	}*/
	GBytes *img_bytes = g_bytes_new(img_data, width * height * 4);
	//GBytes * img_bytes = g_bytes_new_from_bytes(img_data, 0, width*height*4);
	GdkPixbuf *subpixbuf = gdk_pixbuf_new_subpixbuf(gdk_pixbuf_new_from_bytes(img_bytes, GDK_COLORSPACE_RGB, true, 8, width, height, cairo_image_surface_get_stride(surface)), crop_x, crop_y, crop_w, crop_h);
	//GdkPixbuf * pixbuf = gdk_pixbuf_new_from_bytes(cropped, GDK_COLORSPACE_RGB, false, 8, crop_cols, crop_rows, crop_cols*3);
	//subpixbuf = gdk_pixbuf_new_from_bytes(img_bytes, GDK_COLORSPACE_RGB, true, 8, width, height, cairo_image_surface_get_stride(surface));
	GdkPixbuf *pixbuf = gdk_pixbuf_scale_simple(subpixbuf, 18, 18, GDK_INTERP_BILINEAR);
	show_image(pixbuf);

	uint8_t * img = gdk_pixbuf_to_uint8_grayscale(pixbuf);
	int rows, cols;

	uint8_t * img2 =matrix_1ubyteMat_add_frame(&rows, &cols, img, 18, 18, 5, 5, 5, 5);
	printf("new_size: %dx%d\n", rows, cols);
	for(int row = 0; row < rows; row ++){
		for(int col = 0; col < cols; col ++){
			if(img2[row*cols+col] == 0) printf(" ");
			else printf("█");
		}
		puts("");
	}
	printf("RESULT: %d\n", nn_recognise(img2));

	free(img);
	free(img2);
	//free(cropped);

	puts("");
	/*
	printf("stride: %d\n", cairo_image_surface_get_stride(surface));
	printf("w x h: %dx%d\n", width, height);
	printf("format: %d\n", format);

	switch (format) {
		case CAIRO_FORMAT_INVALID:
			puts("CAIRO_FORMAT_INVALID");
			break;

		case CAIRO_FORMAT_ARGB32:
			puts("CAIRO_FORMAT_ARGB32");
			break;

		case CAIRO_FORMAT_RGB24:
			puts("CAIRO_FORMAT_RGB24");
			break;

		case CAIRO_FORMAT_A8:
			puts("CAIRO_FORMAT_A8");
			break;

		case CAIRO_FORMAT_A1:
			puts("CAIRO_FORMAT_A1");
			break;

		case CAIRO_FORMAT_RGB16_565:
			puts("CAIRO_FORMAT_RGB16_565");
			break;

		case CAIRO_FORMAT_RGB30:
			puts("CAIRO_FORMAT_RGB30");
			break;
	}*/
	/*
	cairo_pattern_t *pattern = cairo_pattern_create_for_surface (img_surface);
	cairo_t * cr;
	cairo_rectangle (cr, 0, 0, width, height);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_operator (cr, CAIRO_OPERATOR_HSL_SATURATION);
	cairo_mask (cr, pattern);
	cairo_pattern_destroy (pattern);
	*/
}

static void crecog_gui_activate(GtkApplication *app, gpointer user_data) {
	GtkWidget *window;
	GtkWidget *frame;
	GtkWidget *drawing_area;
	GtkWidget *button;
	GtkWidget *button_box;
	GtkWidget *vbox;


	nn_init();
	//nn_test();


	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), GUI_MAIN_WINDOW_NAME);
	gtk_container_set_border_width(GTK_CONTAINER(window), 8);
	g_signal_connect(window, "destroy", G_CALLBACK(close_window), NULL);
	

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	//frame = gtk_frame_new(NULL);
	//gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
	//gtk_container_add(GTK_CONTAINER(window), frame);
	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(drawing_area, 400, 400);   // set a minimum size
	gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

	
	//gtk_container_add(GTK_CONTAINER(frame), drawing_area);

	//button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	//gtk_container_add(GTK_CONTAINER(window), button_box);

	button = gtk_button_new_with_label("Recognise");
	gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);
	g_signal_connect(button, "clicked", G_CALLBACK(recognise), NULL);

	/* Signals used to handle the backing surface */
	g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_cb), NULL);
	g_signal_connect(drawing_area, "configure-event", G_CALLBACK(configure_event_cb), NULL);

	/* Event signals */
	g_signal_connect(drawing_area, "motion-notify-event", G_CALLBACK(motion_notify_event_cb), NULL);
	g_signal_connect(drawing_area, "button-press-event", G_CALLBACK(button_press_event_cb), NULL);

	/* Ask to receive events the drawing area doesn't normally
     * subscribe to. In particular, we need to ask for the
     * button press and motion notify events that want to handle.
    */
	gtk_widget_set_events(drawing_area, gtk_widget_get_events(drawing_area) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);
	gtk_widget_show_all(window);
	//gtk_window_get_position(GTK_WINDOW(window), &main_x, &main_y);
	gtk_window_move(GTK_WINDOW(window), 300, 300);
	
	

	/* Set up the UI */
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), GUI_OUT_WINDOW_NAME);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	image = gtk_image_new();
	gtk_widget_set_size_request(image, 200, 200);
	gtk_box_pack_start(GTK_BOX(vbox), image, TRUE, TRUE, 0);

	/* Exit when the window is closed */
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show_all(window);
	gtk_window_move(GTK_WINDOW(window), 800, 300);
}


int crecog_gui_setup(int argc, char **argv) {
	GtkApplication *app;
	int status;
	app = gtk_application_new("org.gtk.c-digit-recognition", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(crecog_gui_activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}