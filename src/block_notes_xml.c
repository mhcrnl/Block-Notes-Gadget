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

#include <libxml/parser.h>
#include <libxml/xinclude.h>
#include <libxml/tree.h>
#include <libxml/xmlIO.h>
#include <string.h>


/*
  Get general info data
*/
static void
block_notes_xml_parse_info (xmlDocPtr doc, xmlNodePtr cur, block_notes_core_s *core)
{
  xmlChar *key;
  cur = cur->xmlChildrenNode;
  while (cur != NULL)
  {
    /* Save app's name - Max 20 char */
    if ((!xmlStrcmp (cur->name, (const xmlChar *) "name")))
    {
      key = xmlNodeListGetString (doc, cur->xmlChildrenNode, 1);
      core->name = (char *) malloc (20);
      snprintf (core->name, 20, "%s", (char *) key);
      xmlFree (key);
    }
    /* Save app's version - Max 10 char */
    else if ((!xmlStrcmp (cur->name, (const xmlChar *) "version")))
    {
			core->version = (char *) malloc (10);
      key = xmlNodeListGetString (doc, cur->xmlChildrenNode, 1);
      snprintf (core->version, 10, "%s", (char *) key);
      xmlFree (key);
    }

    cur = cur->next;
  }
  return;
}


/*
 Get all XML configuration data
*/
int
block_notes_xml_parse (block_notes_core_s *core,
                       char *sidegadget_xml_file,
                       gboolean get_info,
                       gboolean get_author,
                       gboolean get_graphics,
                       gboolean get_setting)
{
  xmlDocPtr doc;
  xmlNodePtr cur;

  doc = xmlParseFile (sidegadget_xml_file);
	
  if (doc == NULL)
    return -1;

  cur = xmlDocGetRootElement (doc);

  if (cur == NULL)
  {
    xmlFreeDoc (doc);
    return -1;
  }
	
  if (xmlStrcmp (cur->name, (const xmlChar *) "block-notes-gadget"))
  {
    xmlFreeDoc (doc);
    return -1;
  }

  core->width = -1;
  core->height = -1;
  core->x_window = -1;
  core->y_window = -1;

  cur = cur->xmlChildrenNode;

  while (cur != NULL)
  {
    if ((!xmlStrcmp (cur->name, (const xmlChar *) "info")))
    {
      if (get_info == TRUE)
  			block_notes_xml_parse_info (doc, cur, core);
    }
		else if ((!xmlStrcmp (cur->name, (const xmlChar *) "graphics")))
    {
      if (get_graphics == TRUE)
      {
        core->x_window = atoi ((char *) xmlGetProp (cur, "x_win"));
        core->y_window = atoi ((char *) xmlGetProp (cur, "y_win"));
        core->width    = atoi ((char *) xmlGetProp (cur, "width"));
        core->height   = atoi ((char *) xmlGetProp (cur, "height"));
      }
		}
		else if ((!xmlStrcmp (cur->name, (const xmlChar *) "setting")))
    {
      if (get_setting == TRUE)
      {
        /* Gadget's red */
        core->gadget.red = atoi ((char *) xmlGetProp (cur, "gadget_red"));
        /* Gadget's green */
        core->gadget.green = atoi ((char *) xmlGetProp (cur, "gadget_green"));
        /* Gadget's blue */
        core->gadget.blue = atoi ((char *) xmlGetProp (cur, "gadget_blue"));

        /* Border's red */
        core->border.red = atoi ((char *) xmlGetProp (cur, "border_red"));
        /* Border's green */
        core->border.green = atoi ((char *) xmlGetProp (cur, "border_green"));
        /* Border's blue */
        core->border.blue = atoi ((char *) xmlGetProp (cur, "border_blue"));
        /* Border's blue */
        core->border_transparency = atof ((char *) xmlGetProp (cur, "border_transparency"));

        /* Text's red */
        core->text.red = atoi ((char *) xmlGetProp (cur, "text_red"));
        /* Text's green */
        core->text.green = atoi ((char *) xmlGetProp (cur, "text_green"));
        /* Text's blue */
        core->text.blue = atoi ((char *) xmlGetProp (cur, "text_blue"));

        /* App's font */
  			core->font = (char *) malloc (30);
        snprintf (core->font, 30, "%s", (char *) xmlGetProp (cur, "font"));
        core->font_pango = pango_font_description_from_string ((char *) xmlGetProp (cur, "font"));

        /* App's text */
        core->string_init = (char *) xmlGetProp (cur, "init_text");
      }
		}

		cur = cur->next;
	}

  /* Print app's info */
  g_print ("*  version: %s\n", core->version);
  g_print ("*  default settings:\n");
  g_print ("*   gadget: (r) %d, (g) %d, (b) %d\n", core->gadget.red, core->gadget.green, core->gadget.blue);
  g_print ("*   border: (r) %d, (g) %d, (b) %d\n", core->border.red, core->border.green, core->border.blue);
  g_print ("*   text  : (r) %d, (g) %d, (b) %d\n", core->text.red, core->text.green, core->text.blue);
  g_print ("*   font  : %s\n", core->font);

	return 0;
}


/*
 Update XML configuration file
*/
int
block_notes_xml_update_position (char *sidegadget_xml_file, int x_win, int y_win)
{
  xmlDocPtr doc;
  xmlNodePtr cur;

  doc = xmlParseFile (sidegadget_xml_file);
	
  if (doc == NULL )
  {
    libui_gtk_dialog_error ("XML Configuration - Error parsing xml file.");
    return -1;
  }

	cur = xmlDocGetRootElement (doc);

  if (cur == NULL)
  {
    libui_gtk_dialog_error ("XML Configuration - File empty.");
    xmlFreeDoc (doc);
    return -1;
  }
	
  if (xmlStrcmp (cur->name, (const xmlChar *) "block-notes-gadget"))
  {
    libui_gtk_dialog_error ("XML Configuration\nRoot node \"block-notes-gadget\" is missing.");
    xmlFreeDoc (doc);
    return -1;
  }

  cur = cur->xmlChildrenNode;

  while (cur != NULL)
  {

    if ((!xmlStrcmp(cur->name, (const xmlChar *) "graphics")))
    {
      xmlSetProp (cur, "x_win", g_strdup_printf ("%d", x_win));
      xmlSetProp (cur, "y_win", g_strdup_printf ("%d", y_win));
    }
    cur = cur->next;
  }

  if (doc != NULL)
  {
    xmlSaveFormatFile (sidegadget_xml_file, doc, 0);
    xmlFreeDoc (doc);
  }
	return 0;
}


/*
 Update XML configuration file
*/
int
block_notes_xml_update_setting (char *sidegadget_xml_file, block_notes_core_s *core)
{
  xmlDocPtr doc;
  xmlNodePtr cur;

  doc = xmlParseFile (sidegadget_xml_file);
	
  if (doc == NULL )
    return -1;

	cur = xmlDocGetRootElement (doc);

  if (cur == NULL)
  {
    xmlFreeDoc (doc);
    return -1;
  }
	
  if (xmlStrcmp (cur->name, (const xmlChar *) "block-notes-gadget"))
  {
    xmlFreeDoc (doc);
    return -1;
  }

  cur = cur->xmlChildrenNode;

  while (cur != NULL)
  {
    if ((!xmlStrcmp(cur->name, (const xmlChar *) "graphics")))
    {
      xmlSetProp (cur, "width", g_strdup_printf ("%d", core->width));
      xmlSetProp (cur, "height", g_strdup_printf ("%d", core->height));
    }
    else if ((!xmlStrcmp(cur->name, (const xmlChar *) "setting")))
    {

      xmlSetProp (cur, "gadget_red", g_strdup_printf ("%d", core->gadget.red));
      xmlSetProp (cur, "gadget_green", g_strdup_printf ("%d", core->gadget.green));
      xmlSetProp (cur, "gadget_blue", g_strdup_printf ("%d", core->gadget.blue));

      xmlSetProp (cur, "border_red", g_strdup_printf ("%d", core->border.red));
      xmlSetProp (cur, "border_green", g_strdup_printf ("%d", core->border.green));
      xmlSetProp (cur, "border_blue", g_strdup_printf ("%d", core->border.blue));
      xmlSetProp (cur, "border_transparency", g_strdup_printf ("%f", core->border_transparency));

      xmlSetProp (cur, "text_red", g_strdup_printf ("%d", core->text.red));
      xmlSetProp (cur, "text_green", g_strdup_printf ("%d", core->text.green));
      xmlSetProp (cur, "text_blue", g_strdup_printf ("%d", core->text.blue));
      xmlSetProp (cur, "font", g_strdup_printf ("%s", core->font));
    }
    cur = cur->next;
  }

  if (doc != NULL)
  {
    xmlSaveFormatFile (sidegadget_xml_file, doc, 0);
    xmlFreeDoc (doc);
  }
	return 0;
}


/*
 Save data
*/
int
block_notes_xml_save_data (char *sidegadget_xml_file, char *text)
{
  xmlDocPtr doc;
  xmlNodePtr cur;

  doc = xmlParseFile (sidegadget_xml_file);
	
  if (doc == NULL )
    return -1;

	cur = xmlDocGetRootElement (doc);

  if (cur == NULL)
  {
    xmlFreeDoc (doc);
    return -1;
  }
	
  if (xmlStrcmp (cur->name, (const xmlChar *) "block-notes-gadget"))
  {
    xmlFreeDoc (doc);
    return -1;
  }

  cur = cur->xmlChildrenNode;

  while (cur != NULL)
  {
    if ((!xmlStrcmp(cur->name, (const xmlChar *) "setting")))
      xmlSetProp (cur, "init_text", g_strdup_printf ("%s", text));

    cur = cur->next;
  }

  if (doc != NULL)
  {
    xmlSaveFormatFile (sidegadget_xml_file, doc, 0);
    xmlFreeDoc (doc);
  }
	return 0;
}


