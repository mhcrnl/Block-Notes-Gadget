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


#include <glib.h>
#include <gtk/gtk.h>

#include "block_notes.h"


/*
  Create & run dialog setting
*/
void
block_notes_setting_dialog_run (block_notes_core_s *core)
{	
  GtkWidget *dialog;
  GtkWidget *label, *h_box;
  GtkWidget *button_colore_widget;
  GtkWidget *button_colore_text;
  GtkWidget *button_colore_border;
  GtkWidget *button_font;
  GtkWidget *button_spin_border;
  GtkWidget *button_spin_height;
  GtkWidget *button_spin_width;
  GdkColor gadget;
  GdkColor text;
  GdkColor border;	

  FILE *file;
  char *data;
  int i;

  dialog = gtk_dialog_new_with_buttons ("Setting",
                                        NULL,
                                        GTK_DIALOG_MODAL,
                                        GTK_STOCK_OK,
                                        GTK_RESPONSE_OK,
                                        GTK_STOCK_CANCEL,
                                        GTK_RESPONSE_CANCEL,
                                        NULL);

  gtk_window_set_position (GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size (GTK_WINDOW(dialog), 200, 100);
  gtk_window_set_title (GTK_WINDOW(dialog), " Setting ");
  gtk_dialog_set_has_separator (GTK_DIALOG(dialog), FALSE);
  gtk_window_set_resizable (GTK_WINDOW (dialog), TRUE);

  /* Set gadget's color */
  gadget.red   = core->gadget.red;
  gadget.green = core->gadget.green;
  gadget.blue  = core->gadget.blue;

  /* Set text's color */
  text.red   = core->text.red;
  text.green = core->text.green;
  text.blue  = core->text.blue;

  /* Set border's color */
  border.red   = core->border.red;
  border.green = core->border.green;
  border.blue  = core->border.blue;

  label = libui_gtk_label_new_with_descriptor ("- Gadget -", "sans bold 8", FALSE, 0, 0, FALSE, NULL);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), label, FALSE, FALSE, 5);

  /* Gadget background */
  h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Background:", "sans 8", TRUE, 80, 25, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

    button_colore_widget = gtk_color_button_new_with_color (&gadget);
    gtk_widget_show (button_colore_widget);
    gtk_box_pack_start (GTK_BOX(h_box), button_colore_widget, FALSE, FALSE, 10);
  gtk_widget_show (h_box);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);

  /* Font */
  h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Text:", "sans 8", TRUE, 80, 25, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

    button_font = gtk_font_button_new_with_font (core->font);
    gtk_box_pack_start (GTK_BOX(h_box), button_font, FALSE, FALSE, 10);
  gtk_widget_show_all (h_box);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);

  /* Font color */
  h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Text:", "sans 8", TRUE, 80, 25, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

    button_colore_text = gtk_color_button_new_with_color (&text);
    gtk_box_pack_start (GTK_BOX(h_box), button_colore_text, FALSE, FALSE, 10);
  gtk_widget_show_all (h_box);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);
	
  /* Width border */
  h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Width:", "sans 8", TRUE, 80, 25, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);
      
    button_spin_width = gtk_spin_button_new_with_range (100, 300, 1);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(button_spin_width), core->width);
    gtk_box_pack_start (GTK_BOX(h_box), button_spin_width, FALSE, FALSE, 10);
  gtk_widget_show_all (h_box);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);	

  /* Height border */
  h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Height:", "sans 8", TRUE, 80, 25, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);
		    
    button_spin_height = gtk_spin_button_new_with_range (150.0, 300.0, 1);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(button_spin_height), core->height);
    gtk_box_pack_start (GTK_BOX(h_box), button_spin_height, FALSE, FALSE, 10);
  gtk_widget_show_all (h_box);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);	

  label = libui_gtk_label_new_with_descriptor ("- Border -", "sans bold 8", FALSE, 0, 0, FALSE, NULL);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), label, FALSE, FALSE, 5);

  /* Border background */
  h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Background:", "sans 8", TRUE, 80, 25, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);
	
    button_colore_border = gtk_color_button_new_with_color (&border);
    gtk_box_pack_start (GTK_BOX(h_box), button_colore_border, FALSE, FALSE, 10);
    gtk_widget_show_all (h_box);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);

  /* Border transparency */
  h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Transparency:", "sans 8", TRUE, 80, 25, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);
		    
    button_spin_border = gtk_spin_button_new_with_range (0.0, 1.0, 0.05);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(button_spin_border), core->border_transparency);
    gtk_box_pack_start (GTK_BOX(h_box), button_spin_border, FALSE, FALSE, 10);
    gtk_widget_show_all (h_box);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);
  gtk_widget_show_all (GTK_DIALOG(dialog)->vbox);

  /* Run it */
  gint result = gtk_dialog_run (GTK_DIALOG (dialog));

  switch (result)
  {
    case GTK_RESPONSE_DELETE_EVENT:
      gtk_widget_destroy (dialog);
      break;
    case GTK_RESPONSE_CANCEL:
      gtk_widget_destroy (dialog);
      break;	
    case GTK_RESPONSE_OK:
      /* Read data and save all in the XML conf file */
      gtk_color_button_get_color (GTK_COLOR_BUTTON(button_colore_widget), &gadget);
      gtk_color_button_get_color (GTK_COLOR_BUTTON(button_colore_text), &text);
      gtk_color_button_get_color (GTK_COLOR_BUTTON(button_colore_border), &border);
      data = (char *) gtk_font_button_get_font_name (GTK_FONT_BUTTON(button_font));
      core->font_pango = pango_font_description_from_string (data);
      snprintf (core->font, 30, "%s", pango_font_description_to_string (core->font_pango));

      /* Gadget's color */			
      core->gadget.red   = gadget.red;
      core->gadget.green = gadget.green;
      core->gadget.blue  = gadget.blue;

      /* Text's color */
      core->text.red   = text.red;
      core->text.green = text.green;
      core->text.blue  = text.blue;

      /* Border's color */
      core->border.red   = border.red;
      core->border.green = border.green;
      core->border.blue  = border.blue;

      core->border_transparency = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_spin_border));
      core->width = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_spin_width));
      core->height = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_spin_height));

			/* Recreate XML_FILE */
      block_notes_xml_update_setting (XML_FILE, core);

      gtk_widget_destroy (dialog);
      break;
  } 
  return;
}
