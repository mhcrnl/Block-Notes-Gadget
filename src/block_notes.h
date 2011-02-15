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

#ifndef __BLOCK_NOTES_H__
#define __BLOCK_NOTES_H__

#include "../lib/libui_gtk.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define IMAGE_CLOSE "images/close.png"
#define IMAGE_MOVE "images/move.png"
#define IMAGE_SETTING "images/setting.png"
#define IMAGE_LOGO "images/logo.png"
#define XML_FILE "block-notes-gadget.xml"
#define STYLE "style/style.rc"
#define VERSION "2.1.0"

typedef struct {
	GtkWidget *button_close;
	GtkWidget *button_move;
	GtkWidget *button_setting;
	GtkWidget *window;
	GtkWidget *icon;
	GtkWidget *icon_menu;
	GtkWidget *quit_item;
	GtkWidget *about_item;
	GtkWidget *fixed;
	GtkWidget *scrolled_win;
	GtkWidget *view;
	GtkTextBuffer *buffer;

  GdkColor gadget;
  GdkColor border;
  GdkColor text;
  char *name;
  char *version;
	float border_transparency;

  /* Gadget's size */
	int width;
	int height;
  /* Gadget's position */
	int x_window;
	int y_window;
  /* Gadget's presence */
	gboolean presence;

	PangoFontDescription *font_pango;
	char *font;
	gchar *string_init;
} block_notes_core_s;

#endif /* __BLOCK_NOTES_H__ */
