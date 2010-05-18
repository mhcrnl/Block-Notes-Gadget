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

#include <glib.h>
#include <gtk/gtk.h>

#include "block_notes.h"


/*
 Creates & runs dialog setting
*/
void
block_notes_setting_dialog_run (block_notes_core_s *block_notes_core)
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
	GdkColor color_widget;
	GdkColor color_text;
	GdkColor color_border;	

	FILE *file;
	char *data;
	int i;

	dialog = gtk_dialog_new_with_buttons ("Setting", NULL, GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

	gtk_window_set_position (GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size (GTK_WINDOW(dialog), 200, 100);
	gtk_window_set_title (GTK_WINDOW(dialog), " Setting ");
	gtk_dialog_set_has_separator (GTK_DIALOG(dialog), FALSE);
	gtk_window_set_resizable (GTK_WINDOW (dialog), TRUE);

	/* Set button_colore_widget color */
	color_widget.red = block_notes_core->gadget_red * 65535;
	color_widget.green = block_notes_core->gadget_green * 65535;
	color_widget.blue = block_notes_core->gadget_blue * 65535;

	/* Set button_colore_text color */
	color_text.red = block_notes_core->text_red * 65535;
	color_text.green = block_notes_core->text_green * 65535;
	color_text.blue = block_notes_core->text_blue * 65535;

	/* Set button_colore_border color */
	color_border.red = block_notes_core->border_red * 65535;
	color_border.green = block_notes_core->border_green * 65535;
	color_border.blue = block_notes_core->border_blue * 65535;

	label = gtk_label_new ("- Gadget -");
	gtk_widget_modify_font (label, pango_font_description_from_string ("sans 8"));
	gtk_widget_show (label);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), label, FALSE, FALSE, 5);

	/* Gadget background */
	h_box = gtk_hbox_new (FALSE, 0);
		label = gtk_label_new ("Background:");
		gtk_widget_set_size_request (label, 80, 25);
		gtk_widget_modify_font (label, pango_font_description_from_string ("sans 8"));
		gtk_widget_show (label);
		gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

		button_colore_widget = gtk_color_button_new_with_color (&color_widget);
		gtk_widget_show (button_colore_widget);
		gtk_box_pack_start (GTK_BOX(h_box), button_colore_widget, FALSE, FALSE, 10);
	gtk_widget_show (h_box);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);

	/* Font */
	h_box = gtk_hbox_new (FALSE, 0);
		label = gtk_label_new ("Text:");
		gtk_widget_set_size_request (label, 80, 25);
		gtk_widget_modify_font (label, pango_font_description_from_string ("sans 8"));
		gtk_widget_show (label);
		gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

		button_font = gtk_font_button_new_with_font (block_notes_core->font);
		gtk_widget_show (button_font);
		gtk_box_pack_start (GTK_BOX(h_box), button_font, FALSE, FALSE, 10);
	gtk_widget_show (h_box);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);

	/* Font color */
	h_box = gtk_hbox_new (FALSE, 0);
		label = gtk_label_new ("Text:");
		gtk_widget_set_size_request (label, 80, 25);
		gtk_widget_modify_font (label, pango_font_description_from_string ("sans 8"));
		gtk_widget_show (label);
		gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

		button_colore_text = gtk_color_button_new_with_color (&color_text);
		gtk_widget_show (button_colore_text);
		gtk_box_pack_start (GTK_BOX(h_box), button_colore_text, FALSE, FALSE, 10);
	gtk_widget_show (h_box);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);
	
	/* Width border */
	h_box = gtk_hbox_new (FALSE, 0);
		label = gtk_label_new ("Width:");
		gtk_widget_set_size_request (label, 80, 25);
		gtk_widget_modify_font (label, pango_font_description_from_string ("sans 8"));
		gtk_widget_show (label);
		gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);
		    
		button_spin_width = gtk_spin_button_new_with_range (100, 300, 1);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON(button_spin_width), block_notes_core->width);
		gtk_widget_show (button_spin_width);
		gtk_box_pack_start (GTK_BOX(h_box), button_spin_width, FALSE, FALSE, 10);
	gtk_widget_show (h_box);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);	

	/* Height border */
	h_box = gtk_hbox_new (FALSE, 0);
		label = gtk_label_new ("Height:");
		gtk_widget_set_size_request (label, 80, 25);
		gtk_widget_modify_font (label, pango_font_description_from_string ("sans 8"));
		gtk_widget_show (label);
		gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);
		    
		button_spin_height = gtk_spin_button_new_with_range (150.0, 300.0, 1);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON(button_spin_height), block_notes_core->height);
		gtk_widget_show (button_spin_height);
		gtk_box_pack_start (GTK_BOX(h_box), button_spin_height, FALSE, FALSE, 10);
	gtk_widget_show (h_box);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);	
	
	label = gtk_label_new ("- Border -");
	gtk_widget_modify_font (label, pango_font_description_from_string ("sans 8"));
	gtk_widget_show (label);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), label, FALSE, FALSE, 5);

	/* Border background */
	h_box = gtk_hbox_new (FALSE, 0);
		label = gtk_label_new ("Background:");
		gtk_widget_set_size_request (label, 80, 25);
		gtk_widget_modify_font (label, pango_font_description_from_string ("sans 8"));
		gtk_widget_show (label);
		gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);
	
		button_colore_border = gtk_color_button_new_with_color (&color_border);
		gtk_widget_show (button_colore_border);
		gtk_box_pack_start (GTK_BOX(h_box), button_colore_border, FALSE, FALSE, 10);
	gtk_widget_show (h_box);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);

	/* Border transparency */
	h_box = gtk_hbox_new (FALSE, 0);
		label = gtk_label_new ("Transparency:");
		gtk_widget_set_size_request (label, 80, 25);
		gtk_widget_modify_font (label, pango_font_description_from_string ("sans 8"));
		gtk_widget_show (label);
		gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);
		    
		button_spin_border = gtk_spin_button_new_with_range (0.0, 1.0, 0.05);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON(button_spin_border), block_notes_core->border_transparency);
		gtk_widget_show (button_spin_border);
		gtk_box_pack_start (GTK_BOX(h_box), button_spin_border, FALSE, FALSE, 10);
	gtk_widget_show (h_box);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);


	
	/* apro il dialog */
	gint result = gtk_dialog_run (GTK_DIALOG (dialog));

	switch (result){
		case GTK_RESPONSE_DELETE_EVENT:
			gtk_widget_destroy (dialog);
			break;
		case GTK_RESPONSE_CANCEL:
			gtk_widget_destroy (dialog);
			break;	
		case GTK_RESPONSE_OK:
			/*** LEGGO TUTTE LE INFORMAZIONI E LE SALVO NEL FILE DI CONFIGURAZIONE ***/
			gtk_color_button_get_color (GTK_COLOR_BUTTON(button_colore_widget), &color_widget);
			gtk_color_button_get_color (GTK_COLOR_BUTTON(button_colore_text), &color_text);
			gtk_color_button_get_color (GTK_COLOR_BUTTON(button_colore_border), &color_border);
			data = (char *) gtk_font_button_get_font_name (GTK_FONT_BUTTON(button_font));
			block_notes_core->font_pango = pango_font_description_from_string (data);
			block_notes_core->font = pango_font_description_to_string (block_notes_core->font_pango);

			/* Gadget color */			
			block_notes_core->gadget_red = (float) color_widget.red / 65535.0;
			block_notes_core->gadget_green = (float) color_widget.green / 65535.0;
			block_notes_core->gadget_blue = (float) color_widget.blue / 65535.0;
			
			/* Text color */
			block_notes_core->text_red = (float) color_text.red / 65535.0;
			block_notes_core->text_green = (float) color_text.green / 65535.0;
			block_notes_core->text_blue = (float) color_text.blue / 65535.0;
			
			/* Border color */
			block_notes_core->border_red = (float) color_border.red / 65535.0;
			block_notes_core->border_green = (float) color_border.green / 65535.0;
			block_notes_core->border_blue = (float) color_border.blue / 65535.0;
			
			block_notes_core->border_transparency = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_spin_border));
			block_notes_core->width = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_spin_width));
			block_notes_core->height = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_spin_height));
			
			/* Recreate  GADGET_CONF_FILE */
			file = fopen (g_strdup_printf("%s/%s", block_notes_core->cur_directory, GADGET_CONF_FILE), "w");
			if (file != NULL){
				fprintf (file, "WIDTH: %d\n", block_notes_core->width);
				fprintf (file, "HEIGHT: %d\n", block_notes_core->height);
				fprintf (file, "GADGET-RED: %f\n", block_notes_core->gadget_red);
				fprintf (file, "GADGET-GREEN: %f\n", block_notes_core->gadget_green);
				fprintf (file, "GADGET-BLUE: %f\n", block_notes_core->gadget_blue);
				fprintf (file, "BORDER-RED: %f\n", block_notes_core->border_red);
				fprintf (file, "BORDER-GREEN: %f\n", block_notes_core->border_green);
				fprintf (file, "BORDER-BLUE: %f\n", block_notes_core->border_blue);
				fprintf (file, "BORDER-TRANSPARENCY: %f\n", block_notes_core->border_transparency);
				fprintf (file, "FONT: %s\n", block_notes_core->font);
				fprintf (file, "TEXT-RED: %f\n", block_notes_core->text_red);
				fprintf (file, "TEXT-GREEN: %f\n", block_notes_core->text_green);
				fprintf (file, "TEXT-BLUE: %f\n", block_notes_core->text_blue);
				fclose (file);
			}				
			gtk_widget_destroy (dialog);
			break;
	} 
	return;
}
