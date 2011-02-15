/*
 * Copyright © 2010
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "block_notes.h"


/*
 Reads and gets previosly data saved
*/
gboolean
block_notes_conf_get_data (block_notes_core_s *block_notes_core)
{
	FILE *file;
	char *ptr;
	char string[512];	
	file = fopen (g_strdup_printf("%s/%s", block_notes_core->cur_directory, DATA_FILE), "r");
	if (file != NULL){
		while (fgets (string, sizeof (string) - 1, file) != NULL)
			block_notes_core->string_init = g_strdup_printf ("%s%s", block_notes_core->string_init, string);

		fclose (file);
		return TRUE;
	}
	else 
		return FALSE;
}


/*
 Reads conf file and sets gadget init data
*/
gboolean
block_notes_conf_get_gadget_info (block_notes_core_s *block_notes_core)
{
	FILE *file;
	char *ptr;
	char string[512];
	
	file = fopen (g_strdup_printf("%s/%s", block_notes_core->cur_directory, GADGET_CONF_FILE), "r");
	if (file != NULL){
		while (fgets (string, sizeof(string) - 1, file) != NULL){
			if (strncmp (string, "WIDTH:", 6) == 0){
				ptr = (char *)double_split (string, ' ', '\n', 1);
				block_notes_core->width = atoi (ptr);
			}
			if (strncmp (string, "HEIGHT:", 7) == 0){
				ptr = (char *) double_split (string, ' ', '\n', 1);
				block_notes_core->height = atoi (ptr);
			}
			if (strncmp (string, "GADGET-RED:", 11) == 0){
				ptr = (char *) double_split (string, ' ', '\n', 1);
				block_notes_core->gadget_red = atof (ptr);
			}
			else if (strncmp (string, "GADGET-GREEN:", 13) == 0){
				ptr = (char *) double_split (string, ' ', '\n', 1);
				block_notes_core->gadget_green = atof (ptr);
			}
			else if (strncmp (string, "GADGET-BLUE:", 12) == 0){
				ptr = (char *) double_split (string, ' ', '\n', 1);
				block_notes_core->gadget_blue = atof (ptr);
			}
			else if (strncmp (string, "BORDER-RED:", 11) == 0){
				ptr = (char *) double_split (string, ' ', '\n', 1);
				block_notes_core->border_red = atof (ptr);
			}
			else if (strncmp (string, "BORDER-GREEN:", 13) == 0){
				ptr = (char *) double_split (string, ' ', '\n', 1);
				block_notes_core->border_green = atof (ptr);
			}
			else if (strncmp (string, "BORDER-BLUE:", 12) == 0){
				ptr = (char *) double_split (string, ' ', '\n', 1);
				block_notes_core->border_blue = atof (ptr);
			}
			else if (strncmp (string, "BORDER-TRANSPARENCY:", 20) == 0){
				ptr = (char *) double_split (string, ' ', '\n', 1);
				block_notes_core->border_transparency = atof (ptr);
			}
			else if (strncmp (string, "TEXT-RED:", 9) == 0){
				ptr = (char *) double_split (string, ' ', '\n', 1);
				block_notes_core->text_red = atof (ptr);
			}
			else if (strncmp (string, "TEXT-GREEN:", 11) == 0){
				ptr = (char *) double_split (string, ' ', '\n', 1);
				block_notes_core->text_green = atof (ptr);
			}
			else if (strncmp (string, "TEXT-BLUE:", 10) == 0){
				ptr = (char *) double_split (string, ' ', '\n', 1);
				block_notes_core->text_blue = atof (ptr);
			}
			else if (strncmp (string, "FONT:", 5) == 0){
				ptr = (char *) double_split (string, ' ', '\n', 1);
				block_notes_core->font_pango = pango_font_description_from_string(ptr);
				block_notes_core->font = pango_font_description_to_string(block_notes_core->font_pango);
			}
		}
		fclose (file);
		return TRUE;
	}
	else 
		return FALSE;
}


/*
 Reads conf file and sets initial data position
*/ 
block_notes_conf_set_gadget_position (block_notes_core_s *block_notes_core)
{
	FILE *file;
	char *ptr;
	char string[512];
	
	file = fopen(g_strdup_printf("%s/%s", block_notes_core->cur_directory, POSITION_CONF_FILE), "r");
	if (file != NULL){
		while (fgets (string, sizeof(string) - 1, file) != NULL){
			if (strncmp (string, "X-WINDOW:", 9) == 0){
				ptr = (char *) double_split (string, ' ', '\n', 1);
				block_notes_core->x_window = atoi (ptr);
			}
			if (strncmp (string, "Y-WINDOW:", 9) == 0){
				ptr = (char *) double_split (string, ' ', '\n', 1);
				block_notes_core->y_window = atoi(ptr);
			}
		}
		fclose (file);
		return TRUE;
	}
	else 
		return FALSE;
}
