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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "block_notes.h"


GdkPixbuf *create_pixbuf(const gchar * filename){
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if(!pixbuf) {
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}
	return pixbuf;
}


void
on_alpha_screen_changed(GtkWidget* pWidget, GdkScreen* pOldScreen,GtkWidget* pLabel)
{
	GdkScreen* pScreen = gtk_widget_get_screen(pWidget);
	GdkColormap* pColormap = gdk_screen_get_rgba_colormap(pScreen);
	if (!pColormap) pColormap = gdk_screen_get_rgb_colormap(pScreen);
		gtk_widget_set_colormap(pWidget, pColormap);
}


/*
 Creates main window
*/
void
block_notes_create_window (block_notes_core_s *block_notes_core)
{
	block_notes_core->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW(block_notes_core->window), block_notes_core->width, block_notes_core->height);
	gtk_window_set_resizable (GTK_WINDOW(block_notes_core->window), TRUE);
	gtk_widget_set_app_paintable (block_notes_core->window, TRUE);
	gtk_window_set_decorated (GTK_WINDOW(block_notes_core->window), FALSE);
	on_alpha_screen_changed (block_notes_core->window, NULL, NULL);
	gtk_window_stick (GTK_WINDOW(block_notes_core->window));
	gtk_window_set_keep_below (GTK_WINDOW(block_notes_core->window), TRUE);
	gtk_window_set_modal (GTK_WINDOW(block_notes_core->window), FALSE);
	gtk_window_set_type_hint (GTK_WINDOW(block_notes_core->window), GDK_WINDOW_TYPE_HINT_DOCK);
}


/*
 Creates GtkStatusIcon and tooltip
*/
void
block_notes_create_icon (block_notes_core_s *block_notes_core, char *image, char *text)
{
	block_notes_core->icon = (GtkWidget *) gtk_status_icon_new_from_pixbuf (create_pixbuf(image));
	gtk_status_icon_set_tooltip_text (GTK_STATUS_ICON(block_notes_core->icon), text);
	gtk_widget_set_has_tooltip (block_notes_core->window, TRUE);
}


/*
 Creates GtkStatusIcon menu
*/
void
block_notes_create_icon_menu (block_notes_core_s *block_notes_core)
{
	block_notes_core->icon_menu = gtk_menu_new ();
    block_notes_core->quit_item = gtk_menu_item_new_with_label ("Quit");
	gtk_menu_shell_append (GTK_MENU_SHELL(block_notes_core->icon_menu), block_notes_core->quit_item);
	gtk_widget_show_all (block_notes_core->icon_menu);
}	


/*
  Creates button
 */
extern
GtkWidget* block_notes_create_button (char *id_image)
{
	GtkWidget *widget;
	GtkWidget *image;
	
	widget = gtk_button_new();
	
	image = gtk_image_new_from_pixbuf(create_pixbuf(id_image));
	gtk_button_set_image (GTK_BUTTON(widget), image);
	gtk_button_set_relief (GTK_BUTTON(widget), GTK_RELIEF_NONE);
	gtk_button_set_focus_on_click (GTK_BUTTON(widget), FALSE); 
	gtk_button_set_image_position (GTK_BUTTON(widget), GTK_POS_LEFT);
	return widget;
}


/*
 Creates main textview
*/
void
block_notes_create_textview_buffer (block_notes_core_s *block_notes_core)
{
	GdkColor color;
	block_notes_core->view = gtk_text_view_new ();
	gtk_widget_modify_font (block_notes_core->view, block_notes_core->font_pango);

	/* Set textview color*/
	color.red = block_notes_core->gadget_red * 65535;
	color.green = block_notes_core->gadget_green * 65535;
	color.blue = block_notes_core->gadget_blue * 65535;
	gtk_widget_modify_base (block_notes_core->view, GTK_STATE_NORMAL, &color);
	gtk_widget_grab_focus (block_notes_core->view);

	gtk_text_view_set_editable (GTK_TEXT_VIEW (block_notes_core->view), TRUE);
	gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (block_notes_core->view), TRUE);

	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (block_notes_core->view), GTK_WRAP_WORD_CHAR);
	gtk_text_view_set_justification (GTK_TEXT_VIEW (block_notes_core->view), GTK_JUSTIFY_LEFT);

	/* Set space between two rows */
	gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW (block_notes_core->view), 1);
	gtk_text_view_set_pixels_below_lines (GTK_TEXT_VIEW (block_notes_core->view), 1);

	/* Set borders */
	gtk_text_view_set_left_margin (GTK_TEXT_VIEW (block_notes_core->view), 2);
	gtk_text_view_set_right_margin (GTK_TEXT_VIEW (block_notes_core->view), 2);

	color.red = block_notes_core->text_red * 65535;
	color.green = block_notes_core->text_green * 65535;
	color.blue = block_notes_core->text_blue * 65535;
	gtk_widget_modify_text (block_notes_core->view, GTK_STATE_NORMAL, &color);

	gtk_widget_show (block_notes_core->view);

	block_notes_core->buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(block_notes_core->view));
	gtk_text_buffer_set_text (block_notes_core->buffer, block_notes_core->string_init, -1);

	block_notes_core->scrolled_win = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (block_notes_core->scrolled_win), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER(block_notes_core->scrolled_win), block_notes_core->view);

	if ((block_notes_core->width * block_notes_core->height) <= 40000) {
		gtk_widget_set_size_request (block_notes_core->view, block_notes_core->width - 28, block_notes_core->height - 70);
		gtk_fixed_put (GTK_FIXED(block_notes_core->fixed), block_notes_core->scrolled_win, 15, 35);
	}
	else if (((block_notes_core->width * block_notes_core->height) > 40000) && ((block_notes_core->width * block_notes_core->height) <= 60000)) {
		gtk_widget_set_size_request (block_notes_core->view, block_notes_core->width - 40, block_notes_core->height - 70);
		gtk_fixed_put (GTK_FIXED(block_notes_core->fixed), block_notes_core->scrolled_win, 20, 35);
	}
	else {
		gtk_widget_set_size_request (block_notes_core->view, block_notes_core->width - 43, block_notes_core->height - 80);
		gtk_fixed_put (GTK_FIXED(block_notes_core->fixed), block_notes_core->scrolled_win, 22, 40);
	}
}


/*
 Splits a string
*/
char *
split(str, split, ret)
     char *str;
     char split;
     int ret;
{
    char *ptr;
    int a, b, c = 0;

    /* Find the first split-char */
    if (ret != 0) {
        for (a = 0; a <= strlen(str); a++) {
            if (str[a] == split && ++c == ret) {
                a++;
                break;
            }
            if (a == strlen(str))
                return NULL;
        }
    } else {
        a = 0;
    }

    /* Find the second split-char */
    for (b = a; b <= strlen(str); b++)
        if (str[b] == split)
            break;

    ptr = (char *)malloc(sizeof(char) * (b - a + 1));
    strncpy(ptr, &str[a], b - a);
    ptr[b - a] = 0x0;

    return (char *)ptr;
}


/*
 Splits a string
*/
char *
double_split(str, split1, split2, ret)
     char *str;
     char split1;
	 char split2;
     int ret;
{
    char *ptr;
    int a, b, c = 0;

    /* Find the first split-char */
    if (ret != 0) {
        for (a = 0; a <= strlen(str); a++) {
            if (str[a] == split1 && ++c == ret) {
                a++;
                break;
            }
            if (a == strlen(str))
                return NULL;
        }
    } else {
        a = 0;
    }

    /* Find the second split-char */
    for (b = a; b <= strlen(str); b++)
        if (str[b] == split2)
            break;

    ptr = (char *)malloc(sizeof(char) * (b - a + 1));
    strncpy(ptr, &str[a], b - a);
    ptr[b - a] = 0x0;

    return (char *)ptr;
}
