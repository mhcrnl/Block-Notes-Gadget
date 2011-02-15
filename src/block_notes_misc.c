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


/*
  Create GtkStatusIcon menu
*/
void
block_notes_create_icon_menu (block_notes_core_s *core)
{
  core->icon_menu = gtk_menu_new ();
  core->about_item = gtk_menu_item_new_with_label ("About");
  core->quit_item = gtk_menu_item_new_with_label ("Quit");
  gtk_menu_shell_append (GTK_MENU_SHELL(core->icon_menu), core->about_item);
  gtk_menu_shell_append (GTK_MENU_SHELL(core->icon_menu), core->quit_item);
  gtk_widget_show_all (core->icon_menu);
}	


/*
  Create main textview
*/
void
block_notes_create_textview_buffer (block_notes_core_s *core)
{
  core->view = gtk_text_view_new ();
  gtk_widget_modify_font (core->view, core->font_pango);

  gtk_widget_modify_base (core->view, GTK_STATE_NORMAL, &core->gadget);
  gtk_widget_grab_focus (core->view);

  gtk_text_view_set_editable (GTK_TEXT_VIEW (core->view), TRUE);
  gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (core->view), TRUE);

  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (core->view), GTK_WRAP_WORD_CHAR);
  gtk_text_view_set_justification (GTK_TEXT_VIEW (core->view), GTK_JUSTIFY_LEFT);

  /* Set space between two rows */
  gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW (core->view), 1);
  gtk_text_view_set_pixels_below_lines (GTK_TEXT_VIEW (core->view), 1);

  /* Set borders */
  gtk_text_view_set_left_margin (GTK_TEXT_VIEW (core->view), 2);
  gtk_text_view_set_right_margin (GTK_TEXT_VIEW (core->view), 2);

  gtk_widget_modify_text (core->view, GTK_STATE_NORMAL, &core->text);

  gtk_widget_set_can_focus (core->view, TRUE);
  gtk_widget_grab_focus (core->view);

  gtk_widget_show (core->view);

  core->buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(core->view));
  gtk_text_buffer_set_text (core->buffer, core->string_init, -1);

  core->scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (core->scrolled_win), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add (GTK_CONTAINER(core->scrolled_win), core->view);

  gtk_widget_set_size_request (core->view, core->width - 27, core->height - 30);
  gtk_fixed_put (GTK_FIXED(core->fixed), core->scrolled_win, 15, 20);
}
