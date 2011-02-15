/*
 * Copyright © 2011
 * Andrea Costa <nukemup@hotmail.com>
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


#include "block_notes.h"
#include "block_notes_cairo.h"

extern void block_notes_about (block_notes_core_s *core);


/*
  Show or hide the gadget keeping it alive
*/
int
block_notes_show_hide_gadget (GtkWidget *widget, block_notes_core_s *core)
{
  if (core->presence == TRUE)
  {
    gtk_window_get_position (GTK_WINDOW(core->window), &core->x_window, &core->y_window);
    gtk_widget_hide_all (core->window);
    core->presence = FALSE;
  }
  else if (core->presence == FALSE)
  {
    gtk_widget_show_all (core->window);
    gtk_widget_set_uposition (core->window, core->x_window, core->y_window);
    core->presence = TRUE;
  }
  return TRUE;
}


/*
  Show menu when you click in GtkStatusIcon
*/
int
icon_popup_menu (GtkStatusIcon *icon, gint button, gint time, GtkMenu*menu)
{
  gtk_menu_popup (menu, NULL, NULL, gtk_status_icon_position_menu, icon, button, time);
  return TRUE;
}


/*
 Saves textview data
*/
void
block_notes_save_data (GtkWidget *widget, block_notes_core_s *core)
{
  GtkTextIter start;
  GtkTextIter end;
  gchar *string;
  FILE *file;
  gtk_text_buffer_get_start_iter (core->buffer, &start);
  gtk_text_buffer_get_end_iter (core->buffer, &end);
  string = gtk_text_buffer_get_text (core->buffer, &start, &end, FALSE);

  block_notes_xml_save_data (XML_FILE, string);
}


/*
  Save gadget's position
*/
gboolean
block_notes_gadget_save_position (GtkWidget* pWidget, GdkEventCrossing* pButton, block_notes_core_s *core)
{
  int x, y;
  gtk_window_get_position (GTK_WINDOW(core->window), &x, &y);
  block_notes_xml_update_position (XML_FILE, x, y);
	return FALSE;
}


/*
  Open setting dialog
*/
static void
block_notes_setting (GtkButton *button, block_notes_core_s *block_notes_core)
{	
  block_notes_setting_dialog_run (block_notes_core);
  gtk_widget_queue_draw (block_notes_core->window);
  refresh_background (block_notes_core);
}


/*
  Quit all - ALT+F4
*/
void
block_notes_quit (GtkWidget *widget, block_notes_core_s *core)
{
	gtk_widget_destroy (core->window);
  g_free (core);
  gtk_main_quit ();
}


/*
  Quit all - Quit item
*/
void
block_notes_quit_menu (GtkMenuItem *menuitem, block_notes_core_s *core)
{
  g_free (core);
  gtk_main_quit ();
}


int
main (int argc, char *argv[])
{
	gtk_init (&argc, &argv);	

  GtkWidget *label;
  GtkWidget *h_box;
  int i;

  /* Create app's data structure */
  block_notes_core_s *core;
  core = g_new0 (block_notes_core_s, 1);

  /* Set app's name */
  g_set_application_name ("block-notes-gadget");

  /* Print app's header */
  g_print ("\n* BLOCK NOTES GADGET\n");

  /* Set app's presence */
  core->presence = TRUE;
	
  /* Parse the XML conf. file */
  if ((block_notes_xml_parse (core, XML_FILE, TRUE, TRUE, TRUE, TRUE) == -1) || (core->width == -1) || (core->height == -1))
    libui_gtk_dialog_error ("Impossibilie avviare l'applicazione\n. XML file missing!");

	/* Style widget */
  gtk_rc_parse (STYLE);

	/* leggo il contenuto di CONF_DATA e salvo tutto in string_init */
//	core->string_init = "";

	/* Create widgets */
	/* Create gadget's main window */
  core->window = libui_gtk_gadget_window_new (core->width,
                                              core->height,
                                              TRUE,
                                              TRUE,
                                              FALSE,
                                              TRUE,
                                              TRUE,
                                              FALSE,
                                              GDK_WINDOW_TYPE_HINT_DOCK);

	/* Create gadget's icon */
  core->icon = (GtkWidget *) libui_gtk_gadget_window_icon_new (core->window,
                                                               IMAGE_LOGO,
                                                               "Block Notes 2.1.0");

	block_notes_create_icon_menu (core);

	/* Create button close */
  core->button_close = libgadget_button_new_with_image (FALSE,
                                                        0,
                                                        0,
                                                        TRUE,
                                                        7,
                                                        7,
                                                        GTK_POS_TOP,
                                                        IMAGE_CLOSE);

	/* Create button setting */
  core->button_setting = libgadget_button_new_with_image (FALSE,
                                                          0,
                                                          0,
                                                          TRUE,
                                                          7,
                                                          7,
                                                          GTK_POS_TOP,
                                                          IMAGE_SETTING);

	/* Create button move */
  core->button_move = libgadget_button_new_with_image (FALSE,
                                                       0,
                                                       0,
                                                       TRUE,
                                                       7,
                                                       7,
                                                       GTK_POS_TOP,
                                                       IMAGE_MOVE);
	
  /* Set gadget's position */
  if ((core->x_window == -1) || (core->y_window == -1))
  {
    g_print ("Unable to set init gadget position\n");
    gtk_window_set_position (GTK_WINDOW(core->window), GTK_WIN_POS_CENTER);
  }
  else
    gtk_widget_set_uposition (GTK_WIDGET(core->window), core->x_window, core->y_window);
	
  /* Create fixed area */	
  core->fixed = gtk_fixed_new();

	gtk_fixed_put (GTK_FIXED(core->fixed),
                 core->button_close,
                 core->width - 20,
                 0);

	gtk_fixed_put (GTK_FIXED(core->fixed),
                 core->button_move,
                 core->width - 30,
                 0);

  gtk_fixed_put (GTK_FIXED(core->fixed),
                 core->button_setting,
                 core->width - 40,
                 0);

	/* Create textview */
	block_notes_create_textview_buffer (core);

	gtk_container_add (GTK_CONTAINER(core->window), core->fixed);
	gtk_widget_show_all (core->fixed);

  /* signal */
  g_signal_connect (G_OBJECT(core->window), "destroy", G_CALLBACK (block_notes_quit), core);
	gtk_signal_connect (GTK_OBJECT(core->quit_item), "activate", G_CALLBACK (block_notes_quit_menu), core);
	gtk_signal_connect (GTK_OBJECT(core->about_item), "activate", G_CALLBACK (block_notes_about), core);
  g_signal_connect (G_OBJECT(core->window), "expose-event", G_CALLBACK(on_expose_event), core);
  g_signal_connect (G_OBJECT (core->button_close), "clicked", G_CALLBACK (block_notes_show_hide_gadget), core);
  g_signal_connect (G_OBJECT(core->button_move), "button-press-event", G_CALLBACK (libui_gtk_gadget_move), core);
  g_signal_connect (G_OBJECT(core->button_move), "leave-notify-event", G_CALLBACK (block_notes_gadget_save_position), core);
  g_signal_connect (G_OBJECT (core->button_setting), "clicked", G_CALLBACK (block_notes_setting), core);
  g_signal_connect (G_OBJECT (core->buffer), "changed", G_CALLBACK (block_notes_save_data), core);
  g_signal_connect (core->icon, "popup-menu",G_CALLBACK (icon_popup_menu), core->icon_menu);
  g_signal_connect (core->icon, "activate",G_CALLBACK (block_notes_show_hide_gadget), core);
	
  gtk_widget_show_all (core->window);
  gtk_main ();
	
	return 0;
}

