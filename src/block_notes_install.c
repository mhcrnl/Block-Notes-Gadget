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
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "block_notes.h"


/*
 Checks the presence of the conf folder
 */
gboolean
block_notes_check_directory (block_notes_core_s *block_notes_core)
{
	struct stat sbuf;
	gchar *path;

	path = g_strdup_printf ("%s/.block-notes-gadget", g_get_home_dir ());
	
	if (lstat (path, &sbuf) == -1)
		return FALSE;

	if (S_ISDIR (sbuf.st_mode))
		return TRUE;
}


/*
 Creates a new folder in the home directory
 Creates a conf folders and files
 Copys all the images
 */
gboolean
block_notes_install_create_conf_folders_and_files ()
{
	int result;
	FILE *file;
	FILE *file_copy;
	gchar *current_dir = g_get_current_dir ();
	gchar *file_name;
	gchar *conf_path;
	size_t len = 0 ;
	char *buffer;
	long lSize;

	g_print ("Create .block-notes-gadget\n");
	conf_path = g_strdup_printf ("%s/.block-notes-gadget", g_get_home_dir ());
	if (mkdir(conf_path, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
		g_print ("Block Notes 2.0 is not able to create the folder .block-notes-gadget\n in the home directory\n");
		return FALSE;
	}

	g_print ("Create .block-notes-gadget/data.txt\n");
	file_name = g_strdup_printf ("%s/.block-notes-gadget/data.txt", g_get_home_dir ());
	file = fopen (file_name, "w");
	if (file == NULL) {
		g_print ("Block Notes 2.0 is not able to create\n .block-notes-gadget/data.txt in the home directory\n");
		return FALSE;		
	}
	else {
		fprintf (file, "\n");
		fclose (file);
	}

	g_print ("Create .block-notes-gadget/version.txt\n");
	file_name = g_strdup_printf ("%s/.block-notes-gadget/version.txt", g_get_home_dir ());
	file = fopen (file_name, "w");
	if (file == NULL) {
	g_print ("Block Notes 2.0 is not able to create\n .block-notes-gadget/version.txt in the home directory\n");
		return FALSE;
	}
	else {
		fprintf (file, "VERSION: %f\n\n", VERSION);
		fclose (file);
	}
	
	g_print ("Create .block-notes-gadget/conf\n");
	conf_path = g_strdup_printf ("%s/.block-notes-gadget/conf", g_get_home_dir ());
	if (mkdir(conf_path, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
		g_print ("Block Notes 2.0 is not able to create the folder .block-notes-gadget/conf\n in the home directory\n");
		return FALSE;
	}

	g_print ("Create .block-notes-gadget/style\n");
	conf_path = g_strdup_printf ("%s/.block-notes-gadget/style", g_get_home_dir ());
	if (mkdir(conf_path, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
		g_print ("Block Notes 2.0 is not able to create the folder .block-notes-gadget/style in the home directory");
		return FALSE;
	}

	g_print ("Create .block-notes-gadget/style/style.rc.conf\n");
	/* Create style file */
	file_name = g_strdup_printf ("%s/.block-notes-gadget/style/style.rc", g_get_home_dir ());
	file = fopen (file_name, "w");
	if (file != NULL) {
		fprintf (file, "style \"tooltip\"{\n");
		fprintf (file, "	 bg[NORMAL] = \"#FFFFFF\"\n");
		fprintf (file, "	 fg[NORMAL] = \"#000000\"\n");
		fprintf (file, "	 fg[NORMAL] = \"#000000\"\n");
		fprintf (file, "}\n\n");
		fprintf (file, "style \"default\"{\n");
		fprintf (file, "	 #GtkButton::xthickness = 0\n");
		fprintf (file, "	 #GtkButton::ythickness = 0\n");
		fprintf (file, "	 GtkButton::focus-padding = 0\n");
		fprintf (file, "	 GtkButton::default-border = { 0, 0, 0, 0 }\n");
		fprintf (file, "	 GtkButton::focus-line-width = 0\n");
		fprintf (file, "	 GtkButton::inner-border = { 0, 0, 0, 0 }\n");
		fprintf (file, "	 font_name = \"helvetica 9\"\n");
		fprintf (file, "	 GtkComboBox::focus-padding = 0\n");
		fprintf (file, "	 GtkComboBox::default-border = { 0, 0, 0, 0 }\n");
		fprintf (file, "	 GtkComboBox::focus-line-width = 0\n");
		fprintf (file, "	 GtkComboBox::inner-border = { 0, 0, 0, 0 }\n");
		fprintf (file, "}\n\n");
		fprintf (file, "style \"my-scrolledwindow\"{\n");
		fprintf (file, "	 xthickness = 0\n");
		fprintf (file, "	 ythickness = 0\n");
		fprintf (file, "	 GtkScrolledWindow::scrollbars-within-bevel = 0\n");
		fprintf (file, "	 GtkScrolledWindow::scrollbar-spacing = 0\n");
		fprintf (file, "	 GtkScrolledWindow::stepper-size = 5\n");
		fprintf (file, "}\n\n");
		fprintf (file, "style \"my-scrollbar\"{\n");
		fprintf (file, "	 GtkScrollbar::stepper-size = 7\n");
		fprintf (file, "	 GtkScrollbar::slider-width = 7\n");
		fprintf (file, "}\n\n");
		fprintf (file, "widget \"GtkScrolledWindow\" style \"my-scrolledwindow\"\n");
		fprintf (file, "class \"GtkScrollbar\" style \"my-scrollbar\"\n");
		fprintf (file, "widget \"*gtk-tooltip*\" style \"tooltip\"\n");
		fprintf (file, "class \"GtkWidget\" style \"default\"\n");
		fclose  (file);
	}
	else {
		g_print ("Block Notes 2.0 is not able to create\n .block-notes-gadget/style/style.rc in the home directory\n");
		return FALSE;		
	}

	g_print ("Create .block-notes-gadget/images\n");
	conf_path = g_strdup_printf ("%s/.block-notes-gadget/images", g_get_home_dir ());
	if (mkdir(conf_path, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
		g_print ("Block Notes 2.0 is not able to create the folder .block-notes-gadget/images in the home direcortory");
		return FALSE;
	}

	g_print ("Copy %s/images/logo.png\n", current_dir);
	file = fopen (g_strdup_printf ("%s/images/logo.png", current_dir), "rb");
	file_copy = fopen (g_strdup_printf ("%s/logo.png", conf_path), "wb");
	if ((file != NULL) && (file_copy != NULL)) {
		/* Obtain file size */
		fseek (file , 0 , SEEK_END);
		lSize = ftell (file);
		rewind (file);

		/* Allocate memory to contain the whole file */
		buffer = (char*) malloc (sizeof(char)*lSize);

		result = fread (buffer, 1, lSize, file);
		fwrite (buffer, lSize, 1, file_copy);

		fclose (file);
		fclose (file_copy);
		free (buffer);
	}
	else {
		g_print ("Block Notes 2.0 is not able to copy /images/logo.png\n");
		return FALSE;
	}	
	g_print ("Copy %s/images/close.png\n", current_dir);
	file = fopen (g_strdup_printf ("%s/images/close.png", current_dir), "rb");
	file_copy = fopen (g_strdup_printf ("%s/close.png", conf_path), "wb");
	if ((file != NULL) && (file_copy != NULL)) {
		/* Obtain file size */
		fseek (file , 0 , SEEK_END);
		lSize = ftell (file);
		rewind (file);

		/* Allocate memory to contain the whole file */
		buffer = (char*) malloc (sizeof(char)*lSize);

		result = fread (buffer, 1, lSize, file);
		fwrite (buffer, lSize, 1, file_copy);

		fclose (file);
		fclose (file_copy);
		free (buffer);
	}
	else {
		g_print ("Block Notes 2.0 is not able to copy /images/close.png\n");
		return FALSE;
	}
	
	g_print ("Copy %s/images/move.png\n", current_dir);
	file = fopen (g_strdup_printf ("%s/images/move.png", current_dir), "rb");
	file_copy = fopen (g_strdup_printf ("%s/move.png", conf_path), "wb");
	if ((file != NULL) && (file_copy != NULL)) {
		/* Obtain file size */
		fseek (file , 0 , SEEK_END);
		lSize = ftell (file);
		rewind (file);

		/* Allocate memory to contain the whole file */
		buffer = (char*) malloc (sizeof(char)*lSize);

		result = fread (buffer, 1, lSize, file);
		fwrite (buffer, lSize, 1, file_copy);

		fclose (file);
		fclose (file_copy);
		free (buffer);
	}
	else {
		g_print ("Block Notes 2.0 is not able to copy /images/move.png\n");
		return FALSE;
	}
	
	if ((file != NULL) && (file_copy != NULL)) {
		g_print ("Copy %s/images/setting.png\n", current_dir);
		file = fopen (g_strdup_printf ("%s/images/setting.png", current_dir), "rb");
		file_copy = fopen (g_strdup_printf ("%s/setting.png", conf_path), "wb");

		/* Obtain file size */
		fseek (file , 0 , SEEK_END);
		lSize = ftell (file);
		rewind (file);

		/* Allocate memory to contain the whole file */
		buffer = (char*) malloc (sizeof(char)*lSize);

		result = fread (buffer, 1, lSize, file);
		fwrite (buffer, lSize, 1, file_copy);

		fclose (file);
		fclose (file_copy);
		free (buffer);
	}
	else {
		g_print ("Block Notes 2.0 is not able to copy /images/setting.png\n");
		return FALSE;
	}
}
