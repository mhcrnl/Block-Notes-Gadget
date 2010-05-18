/*
 * Copyright © 2010 Andrea Costa <nukemup@hotmail.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 * or write to the Free Software Foundation, Inc., 51 Franklin St
 * Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <cairo.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdlib.h>
#include <glib.h>
#include <gdk/gdk.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "block_notes.h"

#if !GTK_CHECK_VERSION(2,9,0)
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#include <gdk/gdkx.h>
#endif


GdkPixbuf *create_pixbuf(const gchar * filename);											
int icon_popup_menu(GtkStatusIcon *icon, gint button, gint time, GtkMenu*menu);	
static gboolean on_expose_event (GtkWidget *widget, GdkEventExpose *event, block_notes_core_s *block_notes_core);


/*
 Shows or hides the gadget keeping it alive
*/
int
block_notes_show_hide_gadget (GtkWidget *widget, block_notes_core_s *block_notes_core)
{
	if (block_notes_core->presence == TRUE){
		gtk_window_get_position (GTK_WINDOW(block_notes_core->window), &block_notes_core->x_window, &block_notes_core->y_window);
		gtk_widget_hide_all (block_notes_core->window);
		block_notes_core->presence = FALSE;
	}
	else if (block_notes_core->presence == FALSE){
		gtk_widget_show_all (block_notes_core->window);
		gtk_widget_set_uposition (block_notes_core->window, block_notes_core->x_window, block_notes_core->y_window);
		block_notes_core->presence = TRUE;
	}
	return TRUE;
}


/*
 Moves the gadget around the screen
*/
gboolean
block_notes_move_gadget (GtkWidget* pWidget,GdkEventButton* pButton,GdkWindowEdge edge)
{
	gtk_window_begin_move_drag (GTK_WINDOW (gtk_widget_get_toplevel (pWidget)), pButton->button, pButton->x_root, pButton->y_root, pButton->time);
	return TRUE;
}


/*
 Creates main window
*/
void
block_notes_save_data (GtkWidget *widget, block_notes_core_s *block_notes_core)
{
	GtkTextIter start;
	GtkTextIter end;
	gchar *string;
	FILE *file;
	gtk_text_buffer_get_start_iter (block_notes_core->buffer, &start);
	gtk_text_buffer_get_end_iter (block_notes_core->buffer, &end);
	string = gtk_text_buffer_get_text (block_notes_core->buffer, &start, &end, FALSE);

	file = fopen (g_strdup_printf("%s/%s", block_notes_core->cur_directory, DATA_FILE), "w");
	if (file != NULL){
		fprintf (file, "%s", string);
		fclose (file);
	}
}


/*
 Saves gadget's position
*/
gboolean
launcher_save_position (GtkWidget* pWidget, GdkEventCrossing* pButton, block_notes_core_s *block_notes_core)
{
	FILE *file;
	
	gtk_window_get_position (GTK_WINDOW(block_notes_core->window), &block_notes_core->x_window, &block_notes_core->y_window);
	file = fopen (g_strdup_printf("%s/%s", block_notes_core->cur_directory, POSITION_CONF_FILE), "w");
	if (file != NULL){
		fprintf (file, "X-WINDOW: %d\n", block_notes_core->x_window);
		fprintf (file, "Y-WINDOW: %d\n", block_notes_core->y_window);
		fclose (file);
	}
	return FALSE;
}


/*
 Refresh gadget's background if needed
*/
static gboolean
refresh_background (block_notes_core_s *block_notes_core)
{	
	GdkColor color;
	cairo_t *cr;

	double x0      = 5.0;
	double y0      = 15.0;
	double rect_width  = block_notes_core->width - 10;
	double rect_height = block_notes_core->height - 25;
	double radius = 160;
	double x1,y1;

	cr = gdk_cairo_create (block_notes_core->window->window);

	/* Gadget background transparent */
	cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.0);
	cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
	cairo_paint (cr);

	x1 = x0 + rect_width;
	y1 = y0 + rect_height;

	if (!rect_width || !rect_height)
		return;

	cairo_move_to  (cr, x0, (y0 + y1)/2);
	cairo_curve_to (cr, x0 ,y0, x0, y0, (x0 + x1)/2, y0);
	cairo_curve_to (cr, x1, y0, x1, y0, x1, (y0 + y1)/2);
	cairo_curve_to (cr, x1, y1, x1, y1, (x1 + x0)/2, y1);
	cairo_curve_to (cr, x0, y1, x0, y1, x0, (y0 + y1)/2);

	cairo_close_path (cr);

	cairo_set_source_rgba (cr,
	    					block_notes_core->gadget_red,
	    					block_notes_core->gadget_green,
	    					block_notes_core->gadget_blue,
	    					1.0);
	
	cairo_fill_preserve (cr);
	
	cairo_set_source_rgba (cr,
	    					block_notes_core->border_red,
	    					block_notes_core->border_green,
	    					block_notes_core->border_blue,
	    					block_notes_core->border_transparency);

	cairo_set_line_width (cr, 5.0);
	cairo_stroke (cr);
	cairo_destroy (cr);


	/* Reset textview color */
	color.red = block_notes_core->gadget_red * 65535;
	color.green = block_notes_core->gadget_green * 65535;
	color.blue = block_notes_core->gadget_blue * 65535;
	gtk_widget_modify_base (block_notes_core->view, GTK_STATE_NORMAL, &color);

	/* Reset textview font */
	gtk_widget_modify_font (block_notes_core->view, block_notes_core->font_pango);

	/* Reset font color */
	color.red = block_notes_core->text_red * 65535;
	color.green = block_notes_core->text_green * 65535;
	color.blue = block_notes_core->text_blue * 65535;
	gtk_widget_modify_text (block_notes_core->view, GTK_STATE_NORMAL, &color);
	
	/* Resize window */
	gtk_window_resize (GTK_WINDOW(block_notes_core->window), block_notes_core->width, block_notes_core->height);

	/* Reset buttons */
	gtk_fixed_move (GTK_FIXED(block_notes_core->fixed), block_notes_core->button_setting, block_notes_core->width - 28, 4);
	gtk_fixed_move (GTK_FIXED(block_notes_core->fixed), block_notes_core->button_move, block_notes_core->width - 19, 4);
	gtk_fixed_move (GTK_FIXED(block_notes_core->fixed), block_notes_core->button_close, block_notes_core->width - 10, 4);

	if ((block_notes_core->width * block_notes_core->height) <= 40000){
		gtk_widget_set_size_request (block_notes_core->view, block_notes_core->width - 28, block_notes_core->height - 70);
		gtk_fixed_move (GTK_FIXED(block_notes_core->fixed), block_notes_core->scrolled_win, 15, 35);
	}
	else if (((block_notes_core->width * block_notes_core->height) > 40000) && ((block_notes_core->width * block_notes_core->height) <= 60000)){
		gtk_widget_set_size_request (block_notes_core->view, block_notes_core->width - 40, block_notes_core->height - 70);
		gtk_fixed_move (GTK_FIXED(block_notes_core->fixed), block_notes_core->scrolled_win, 20, 35);
	}
	else {
		gtk_widget_set_size_request (block_notes_core->view, block_notes_core->width - 43, block_notes_core->height - 80);
		gtk_fixed_move (GTK_FIXED(block_notes_core->fixed), block_notes_core->scrolled_win, 22, 40);
	}

	return FALSE;
}


/*
 Opens setting dialog
 */
static void
block_notes_setting (GtkButton *button, block_notes_core_s *block_notes_core)
{	
	block_notes_setting_dialog_run (block_notes_core);
	
	/* Get data about the gadget */
	block_notes_conf_get_gadget_info (block_notes_core);
	
	gtk_widget_queue_draw (block_notes_core->window);
	refresh_background (block_notes_core);
}


int main (int argc, char *argv[]){

	gtk_init(&argc, &argv);	

	GtkWidget *label;
	GtkWidget *h_box;
	int i;

	block_notes_core_s *block_notes_core;
	block_notes_core = g_new0 (block_notes_core_s, 1);

	g_set_application_name ("block-notes-gadget");
	block_notes_core->cur_directory = g_strdup_printf ("%s/.block-notes-gadget", g_get_home_dir ());

	/* Check if conf folders and files are available, if not create them */
	if (block_notes_check_directory (block_notes_core) == FALSE) {
		char *error;
		if (block_notes_install_create_conf_folders_and_files (error) == FALSE) {
			g_print ("block_notes_install_create_conf_folders_and_files FAILED\n");
			block_notes_dialog_error (error);
			return 0;
		}
	}
	
	/* Get init data about the gadget */
	if (block_notes_conf_get_gadget_info (block_notes_core) != TRUE) {
		g_print ("Unable to get init gadget data\n");
		block_notes_core->width = 180;
		block_notes_core->height = 200;
		block_notes_core->gadget_red = 1.0;
		block_notes_core->gadget_green = 1.0;
		block_notes_core->gadget_blue = 1.0;
		block_notes_core->border_red = 0.85;
		block_notes_core->border_green = 0.85;
		block_notes_core->border_blue = 0.85;
		block_notes_core->border_transparency = 0.85;
		block_notes_core->text_red = 0.0;
		block_notes_core->text_green = 0.0;
		block_notes_core->text_blue = 0.0;
		block_notes_core->font_pango = pango_font_description_from_string("Phetsarath OT 8");
		block_notes_core->font = "Phetsarath OT 8";
	}
	
	/* Style widget */
	gtk_rc_parse (g_strdup_printf("%s/%s", block_notes_core->cur_directory, STYLE));

	/* leggo il contenuto di CONF_DATA e salvo tutto in string_init */
	block_notes_core->string_init = "";
	block_notes_core->presence = TRUE;
	if (block_notes_conf_get_data (block_notes_core) != TRUE) {
		g_print ("Unable to get init data from data.txt\n");
	}

	/* Create widgets */
	block_notes_create_window (block_notes_core);
	block_notes_create_icon (block_notes_core, g_strdup_printf("%s/%s", block_notes_core->cur_directory, IMAGE_LOGO), "Block Notes 2.0");
	block_notes_create_icon_menu (block_notes_core);
	block_notes_core->button_close = (GtkWidget *) block_notes_create_button (g_strdup_printf("%s/%s", block_notes_core->cur_directory, IMAGE_CLOSE));
	block_notes_core->button_move = (GtkWidget *) block_notes_create_button (g_strdup_printf("%s/%s", block_notes_core->cur_directory, IMAGE_MOVE));
	block_notes_core->button_setting = (GtkWidget *) block_notes_create_button (g_strdup_printf("%s/%s", block_notes_core->cur_directory, IMAGE_SETTING));
	
	/* Get position gadget data */
	if (block_notes_conf_set_gadget_position (block_notes_core) != TRUE) {
		g_print ("Unable to set init gadget position\n");
		gtk_window_set_position (GTK_WINDOW(block_notes_core->window), GTK_WIN_POS_CENTER);
	}
	else
		gtk_widget_set_uposition (GTK_WIDGET(block_notes_core->window), block_notes_core->x_window, block_notes_core->y_window);
	
	/* Create fixed area */	
	block_notes_core->fixed = gtk_fixed_new();

	gtk_fixed_put (GTK_FIXED(block_notes_core->fixed),
	    			block_notes_core->button_close,
				block_notes_core->width - 20,
				0);

	gtk_fixed_put (GTK_FIXED(block_notes_core->fixed),
	    			block_notes_core->button_move,
				block_notes_core->width - 30,
				0);
	gtk_fixed_put (GTK_FIXED(block_notes_core->fixed),
				block_notes_core->button_setting,
				block_notes_core->width - 40,
				0);

	block_notes_create_textview_buffer (block_notes_core);
	/* inserisco il text view */

	gtk_container_add( GTK_CONTAINER(block_notes_core->window), block_notes_core->fixed);
	gtk_widget_show (block_notes_core->fixed);
	
	/* signal */
	g_signal_connect (G_OBJECT(block_notes_core->window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect (G_OBJECT(block_notes_core->window), "expose-event", G_CALLBACK(on_expose_event), block_notes_core);
	g_signal_connect (G_OBJECT (block_notes_core->button_close), "clicked", G_CALLBACK (block_notes_show_hide_gadget), block_notes_core);
	g_signal_connect (G_OBJECT(block_notes_core->button_move), "button-press-event", G_CALLBACK (block_notes_move_gadget), block_notes_core);
	g_signal_connect (G_OBJECT(block_notes_core->button_move), "leave-notify-event", G_CALLBACK (launcher_save_position), block_notes_core);	
	g_signal_connect (G_OBJECT (block_notes_core->button_setting), "clicked", G_CALLBACK (block_notes_setting), block_notes_core);	
	g_signal_connect (G_OBJECT (block_notes_core->buffer), "changed", G_CALLBACK (block_notes_save_data), block_notes_core);
	gtk_signal_connect (GTK_OBJECT(block_notes_core->quit_item), "activate", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect (block_notes_core->icon, "popup-menu",G_CALLBACK (icon_popup_menu), block_notes_core->icon_menu);
	g_signal_connect(block_notes_core->icon, "activate",G_CALLBACK(block_notes_show_hide_gadget), block_notes_core);
	
	gtk_widget_show_all (block_notes_core->window);
 	gtk_main ();
	
	return 0;
}

/*
 Sets gadget's background
*/
static
gboolean on_expose_event (GtkWidget *widget, GdkEventExpose *event, block_notes_core_s *block_notes_core)
{
	cairo_t *cr;
	
	double x0      = 5.0;
	double y0      = 15.0;
	double rect_width  = block_notes_core->width - 10;
	double rect_height = block_notes_core->height - 25;
	double radius = 160;
	double x1,y1;
	
	cr = gdk_cairo_create (widget->window);
	
	/* settaggi per rendere trasparente lo sfondo */
	cairo_set_source_rgba (cr, 1.0f, 1.0f, 1.0f, 0.0f);
	cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
	cairo_paint (cr);

	x1=x0+rect_width;
	y1=y0+rect_height;
	
	if (!rect_width || !rect_height)
		return;
	if (rect_width/2<radius) {
		cairo_move_to  (cr, x0, (y0 + y1)/2);
		cairo_curve_to (cr, x0 ,y0, x0, y0, (x0 + x1)/2, y0);
		cairo_curve_to (cr, x1, y0, x1, y0, x1, (y0 + y1)/2);
		cairo_curve_to (cr, x1, y1, x1, y1, (x1 + x0)/2, y1);
		cairo_curve_to (cr, x0, y1, x0, y1, x0, (y0 + y1)/2);
	} else {
		cairo_move_to  (cr, x0, y0 + radius);
		cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
		cairo_line_to (cr, x1 - radius, y0);
		cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
		cairo_line_to (cr, x1 , y1 - radius);
		cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
		cairo_line_to (cr, x0 + radius, y1);
		cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);
	}

	cairo_close_path (cr);

	cairo_set_source_rgba (cr,
	    					block_notes_core->gadget_red,
	    					block_notes_core->gadget_green,
	    					block_notes_core->gadget_blue,
	    					1.0);
	
	cairo_fill_preserve (cr);
	
	cairo_set_source_rgba (cr,
	    					block_notes_core->border_red,
	    					block_notes_core->border_green,
	    					block_notes_core->border_blue,
	    					block_notes_core->border_transparency);

	cairo_set_line_width (cr, 5.0);
	cairo_stroke (cr);
	cairo_destroy (cr);

	return FALSE;
}


/*
 Shows menu when you click in GtkStatusIcon
*/
int
icon_popup_menu (GtkStatusIcon *icon, gint button, gint time, GtkMenu*menu)
{
	gtk_menu_popup (menu, NULL, NULL, gtk_status_icon_position_menu, icon, button, time);
	return TRUE;
}
