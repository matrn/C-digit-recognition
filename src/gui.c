#include "include/gui.h"

/*
 * C GTK Drawing app example is taken from: https://code.woboq.org/gtk/gtk/examples/drawing.c.html
 */

const uint8_t draw_surface_null_values[] = {255, 255, 255, 255};  // R,G,B,A values which are considered as null pixel

static cairo_surface_t *surface = NULL;	 // surface to store current scribbles
static int surface_width = -1;
static int surface_height = -1;

static GtkWidget *image;

void gui_display_image(GdkPixbuf *data) {
	GdkPixbuf *pixbuf = gdk_pixbuf_scale_simple(data, 250, 250, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf((GtkImage *)image, pixbuf);
}

static void gui_display_results(const uint8_t results[], const double accuracies[]) {
	GtkTextIter iter_start, iter_stop;

	char text[200];

	gtk_text_buffer_get_iter_at_line(result_buffer, &iter_start, 0);
	gtk_text_buffer_get_iter_at_line(result_buffer, &iter_stop, 3 + 2);
	gtk_text_buffer_delete(result_buffer, &iter_start, &iter_stop);

	// 1. result
	gtk_text_buffer_insert_with_tags_by_name(result_buffer, &iter_start, "1.Result: ", -1, "result_format", "top_result", NULL);

	sprintf(text, "%d", results[0]);
	gtk_text_buffer_insert_with_tags_by_name(result_buffer, &iter_start, text, -1, "result_format", "top_result", "green", NULL);

	sprintf(text, ", accuracy: %.2f %%\n", accuracies[0]);
	gtk_text_buffer_insert_with_tags_by_name(result_buffer, &iter_start, text, -1, "result_format", "top_result", NULL);

	// 2. result
	sprintf(text, "2.Result: %d, accuracy: %.2f %%\n", results[1], accuracies[1]);
	gtk_text_buffer_insert_with_tags_by_name(result_buffer, &iter_start, text, -1, "result_format", "second_result", NULL);

	// 3. result
	sprintf(text, "3.Result: %d, accuracy: %.2f %%\n", results[2], accuracies[2]);
	gtk_text_buffer_insert_with_tags_by_name(result_buffer, &iter_start, text, -1, "result_format", "third_result", NULL);
}

void gui_close() {
	if (surface) cairo_surface_destroy(surface);
	surface = NULL;
	nn_free();

	gtk_window_close(GTK_WINDOW(main_window));
	gtk_window_close(GTK_WINDOW(result_window));
	//gtk_main_quit();
}

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

	GdkPixbuf *out_pixbuf = gdk_pixbuf_new_from_bytes(g_bytes_new(rgb_img, rows * cols * 3), GDK_COLORSPACE_RGB, false, 8, cols, rows, cols * 3);
	free(rgb_img);

	return out_pixbuf;
}

/* recognise input image and display results */
static void recognise_input(GtkWidget *widget, gpointer data) {
	GdkPixbuf *pixbuf, *subpixbuf;

	uint8_t *img, *img_final;
	matrix_size_t img_final_rows, img_final_cols;
	int crop_x, crop_y, crop_x2, crop_y2, max_crop_size;

	// for storing & processing neural network output
	matrix_t *out_mat;
	uint8_t results[3];
	double accuracies[3];

	cairo_surface_flush(surface);
	assert(cairo_image_surface_get_format(surface) == CAIRO_FORMAT_ARGB32);

	const unsigned char *const img_data = cairo_image_surface_get_data(surface);
	const int width = cairo_image_surface_get_width(surface);
	const int height = cairo_image_surface_get_height(surface);

	matrix_1ubyteMat_calculate_crop(&crop_x, &crop_x2, &crop_y, &crop_y2, img_data, height, width, 4, draw_surface_null_values);

	// calculate crop width & height
	int crop_w = crop_x2 - crop_x + 1;
	int crop_h = crop_y2 - crop_y + 1;

	// select bigger size and use it as crop size
	max_crop_size = max(crop_w, crop_h);
	crop_x -= (max_crop_size - crop_w) / 2;
	crop_y -= (max_crop_size - crop_h) / 2;

	crop_w = crop_h = max_crop_size;

	// limit X coordinates to be inside border
	if (crop_x + crop_w > width) crop_x -= crop_x + crop_w - width;
	if (crop_x < 0) crop_x = 0;

	// limit Y coordinates to be inside border
	if (crop_y + crop_h > height) crop_y -= crop_y + crop_h - height;
	if (crop_y < 0) crop_y = 0;

	dbg("Cropped size: %dx%d\n", crop_w, crop_h);
	dbg("Crop position: x=%d, y=%d\n", crop_x, crop_y);

	pixbuf = gdk_pixbuf_new_from_bytes(g_bytes_new(img_data, width * height * 4), GDK_COLORSPACE_RGB, true, 8, width, height, cairo_image_surface_get_stride(surface));	 // create pixbuf from cairo surface
	subpixbuf = gdk_pixbuf_new_subpixbuf(pixbuf, crop_x, crop_y, crop_w, crop_h);																						 // crop pixbuf using calculated coordinates
	pixbuf = gdk_pixbuf_scale_simple(subpixbuf, 20, 20, GDK_INTERP_BILINEAR);																							 // scale cropped pixbuf using bilinear interpolation

	img = gdk_pixbuf_to_uint8_grayscale(pixbuf);  // convert image to the grayscale img

	matrix_1ubyteMat_display(img, 20, 20);
	int row_center = matrix_1ubyteMat_mean(img, 20, 20, ROW_AXIS);
	int col_center = matrix_1ubyteMat_mean(img, 20, 20, COL_AXIS);
	printf("Center: %dx%d\n", row_center, col_center);

	img_final = matrix_1ubyteMat_add_frame(&img_final_rows, &img_final_cols, img, 20, 20, 4, 4, 4, 4);
	free(img);

	dbgexec(matrix_1ubyteMat_print(img_final, img_final_rows, img_final_cols));
	dbgln("---------------------------");
	matrix_1ubyteMat_submat_move(img_final, img_final_rows, img_final_cols, 20, 20, 4, 4, row_center - 10, col_center - 10);
	dbgln("---------------------------");
	dbgexec(matrix_1ubyteMat_print(img_final, img_final_rows, img_final_cols));

	puts("Move done");
	dbg("new_size: %dx%d\n", img_final_rows, img_final_cols);
	dbgexec(matrix_1ubyteMat_display(img_final, img_final_rows, img_final_cols));

	pixbuf = uint8_grayscale_to_gdk_pixbuf(img_final, img_final_rows, img_final_cols);
	gui_display_image(pixbuf);	// show preprocessed image in result window

	out_mat = matrix_new();
	nn_recognise(img_final, out_mat);

	results[0] = matrix_argmax(out_mat);
	accuracies[0] = matrix_max(out_mat);
	*matrix_at_index(out_mat, results[0]) = -1;

	results[1] = matrix_argmax(out_mat);
	accuracies[1] = matrix_max(out_mat);
	*matrix_at_index(out_mat, results[1]) = -1;

	results[2] = matrix_argmax(out_mat);
	accuracies[2] = matrix_max(out_mat);
	*matrix_at_index(out_mat, results[2]) = -1;

	dbg("RESULT: %d\n", results[0]);

	gui_display_results(results, accuracies);

	matrix_delete(out_mat);

	free(img_final);
	dbgln("-------------------");
}

/* clears cairo draw surface */
static void gui_clear_draw_surface() {
	cairo_t *cr;
	cr = cairo_create(surface);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);
	cairo_destroy(cr);
}

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean gui_surface_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, gpointer data) {
	if (surface) cairo_surface_destroy(surface);

	surface_width = gtk_widget_get_allocated_width(widget);
	surface_height = gtk_widget_get_allocated_height(widget);
	surface = gdk_window_create_similar_image_surface(gtk_widget_get_window(widget), CAIRO_FORMAT_ARGB32, surface_width, surface_height, 0);

	/* Initialize the surface to white */
	gui_clear_draw_surface();

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
static void gui_draw_brush(GtkWidget *widget, gdouble x, gdouble y) {
	cairo_t *cr;
	// paint to the surface, where we store our state
	cr = cairo_create(surface);
	cairo_rectangle(cr, x - DRAW_SIZE / 2, y - DRAW_SIZE / 2, DRAW_SIZE, DRAW_SIZE);
	cairo_fill(cr);
	cairo_destroy(cr);
	// now invalidate the affected region of the drawing area
	gtk_widget_queue_draw(widget);
}

/* Handle button press events by either drawing a rectangle
 * or clearing the surface, depending on which button was pressed.
 * The ::button-press signal handler receives a GdkEventButton
 * struct which contains this information.
 */
static gboolean gui_button_press_event_cb(GtkWidget *widget, GdkEventButton *event, gpointer data) {
	if (surface == NULL) return FALSE;	// paranoia check, in case we haven't gotten a configure event

	if (event->button == GDK_BUTTON_PRIMARY) {
		gui_draw_brush(widget, event->x, event->y);
	} else if (event->button == GDK_BUTTON_SECONDARY) {
		gui_clear_draw_surface();
		gtk_widget_queue_draw(widget);
	} else if (event->button == GDK_BUTTON_MIDDLE) {
		recognise_input(NULL, NULL);
	}

	return TRUE;  // we've handled the event, stop processing
}

/* Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
static gboolean gui_motion_notify_event_cb(GtkWidget *widget, GdkEventMotion *event, gpointer data) {
	if (surface == NULL) return FALSE;	// paranoia check, in case we haven't gotten a configure event

	if (event->state & GDK_BUTTON1_MASK) gui_draw_brush(widget, event->x, event->y);

	return TRUE;  // the event was handled, stop processing
}

static void crecog_gui_activate(GtkApplication *app, gpointer user_data) {
	GtkWidget *drawing_area;
	GtkWidget *button;
	GtkWidget *vbox;
	GtkTextIter iter;
	GtkWidget *result_view;

	puts("GUI init\n");
	fflush(stdout);

	nn_init();
	//nn_test();

	// main window setup
	main_window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(main_window), GUI_MAIN_WINDOW_NAME);
	gtk_container_set_border_width(GTK_CONTAINER(main_window), 8);
	g_signal_connect(main_window, "destroy", G_CALLBACK(gui_close), NULL);

	// vertical box
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_container_add(GTK_CONTAINER(main_window), vbox);

	// drawing area
	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(drawing_area, 400, 400);  // set a minimum size
	gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

	// recognise button
	button = gtk_button_new_with_label("Recognise");
	gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);
	g_signal_connect(button, "clicked", G_CALLBACK(recognise_input), NULL);

	// signals used to handle the backing surface
	g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_cb), NULL);
	g_signal_connect(drawing_area, "configure-event", G_CALLBACK(gui_surface_configure_event_cb), NULL);

	// event signals
	g_signal_connect(drawing_area, "motion-notify-event", G_CALLBACK(gui_motion_notify_event_cb), NULL);
	g_signal_connect(drawing_area, "button-press-event", G_CALLBACK(gui_button_press_event_cb), NULL);

	/* Ask to receive events the drawing area doesn't normally
     * subscribe to. In particular, we need to ask for the
     * button press and motion notify events that want to handle.
    */
	gtk_widget_set_events(drawing_area, gtk_widget_get_events(drawing_area) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);
	gtk_widget_show_all(main_window);
	//gtk_window_get_position(GTK_WINDOW(window), &main_x, &main_y);
	gtk_window_move(GTK_WINDOW(main_window), 300, 300);

	// result window setup
	result_window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(result_window), GUI_RESULT_WINDOW_NAME);

	// vertical box
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_container_add(GTK_CONTAINER(result_window), vbox);

	// cropped image widget
	image = gtk_image_new();
	gtk_widget_set_size_request(image, 200, 200);
	gtk_box_pack_start(GTK_BOX(vbox), image, TRUE, TRUE, 0);

	// result text widget
	result_view = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(vbox), result_view, TRUE, TRUE, 0);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(result_view), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(result_view), FALSE);
	result_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(result_view));

	gtk_text_buffer_create_tag(result_buffer, "green", "foreground", "green", "weight", PANGO_WEIGHT_BOLD, NULL);
	gtk_text_buffer_create_tag(result_buffer, "result_format", "left_margin", 10, "right_margin", 10, NULL);
	gtk_text_buffer_create_tag(result_buffer, "top_result", "font", "Monospace 15", NULL);
	gtk_text_buffer_create_tag(result_buffer, "second_result", "font", "Monospace 10", NULL);
	gtk_text_buffer_create_tag(result_buffer, "third_result", "font", "Monospace 10", NULL);

	gtk_text_buffer_get_iter_at_line(result_buffer, &iter, 0);
	gtk_text_buffer_insert_with_tags_by_name(result_buffer, &iter, "Draw a number\n", -1, "top_result", NULL);

	g_signal_connect(result_window, "destroy", G_CALLBACK(gui_close), NULL);  // exit when the window is closed
	gtk_widget_show_all(result_window);
	gtk_window_move(GTK_WINDOW(result_window), 800, 300);
}

int crecog_gui_setup(int argc, char **argv) {
	GtkApplication *app;
	int status;

	puts("Starting application setup...");
	app = gtk_application_new(GUI_APP_NAME, G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(crecog_gui_activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	puts("Dostroying application...");
	g_object_unref(app);

	return status;
}