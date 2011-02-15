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
  Refresh gadget's background if needed
*/
gboolean
refresh_background (block_notes_core_s *core)
{	
  GdkColor color;
  cairo_t *cr;
  double x0      = 5.0;
  double y0      = 12.0;
  double rect_width  = core->width - 10;
  double rect_height = core->height - 12;
  double radius = 40;
  double x1,y1;

  cr = gdk_cairo_create (core->window->window);

  /* Set gadget's background transparent */
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
                         (float) core->gadget.red / 65535.0,
                         (float) core->gadget.green / 65535.0,
                         (float) core->gadget.blue / 65535.0,
                         1.0);
	
  cairo_fill_preserve (cr);
	
  cairo_set_source_rgba (cr,
                         (float) core->border.red / 65535.0,
                         (float) core->border.green / 65535.0,
                         (float) core->border.blue / 65535.0,
                         core->border_transparency);

  cairo_set_line_width (cr, 3.0);
  cairo_stroke (cr);
  cairo_destroy (cr);


  gtk_widget_modify_base (core->view, GTK_STATE_NORMAL, &core->gadget);

  /* Reset textview font */
  gtk_widget_modify_font (core->view, core->font_pango);

  /* Reset font color */
  gtk_widget_modify_text (core->view, GTK_STATE_NORMAL, &core->text);

  /* Resize window */
  gtk_window_resize (GTK_WINDOW(core->window), core->width, core->height);

  /* Reset buttons */
  gtk_fixed_move (GTK_FIXED(core->fixed), core->button_setting, core->width - 40, 0);
  gtk_fixed_move (GTK_FIXED(core->fixed), core->button_move, core->width - 30, 0);
  gtk_fixed_move (GTK_FIXED(core->fixed), core->button_close, core->width - 20, 0);
  gtk_widget_set_size_request (core->view, core->width - 27, core->height - 30);

  return FALSE;
}


/*
  Set gadget's background
*/
gboolean
on_expose_event (GtkWidget *widget, GdkEventExpose *event, block_notes_core_s *core)
{
  cairo_t *cr;	
  double x0      = 5.0;
  double y0      = 12.0;
  double rect_width  = core->width - 10;
  double rect_height = core->height - 15;
  double radius = 30;
  double x1,y1;
	
  cr = gdk_cairo_create (widget->window);
	
  /* settaggi per rendere trasparente lo sfondo */
  cairo_set_source_rgba (cr, 1.0f, 1.0f, 1.0f, 0.0f);
  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint (cr);

  x1 = x0 + rect_width;
  y1 = y0 + rect_height;
	
  if (!rect_width || !rect_height)
    return;

  cairo_move_to  (cr, x0, y0 + radius);
  cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
  cairo_line_to (cr, x1 - radius, y0);
  cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
  cairo_line_to (cr, x1 , y1 - radius);
  cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
  cairo_line_to (cr, x0 + radius, y1);
  cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);

  cairo_close_path (cr);



  cairo_set_source_rgba (cr,
                         (float) core->gadget.red / 65535.0,
                         (float) core->gadget.green / 65535.0,
                         (float) core->gadget.blue / 65535.0,
                         1.0);

  cairo_fill_preserve (cr);

  cairo_set_source_rgba (cr,
                         1.0,
                         1.0,
                         1.0,
                         0.7);

	cairo_set_source_rgba (cr,
                         (float) core->border.red / 65535.0,
                         (float) core->border.green / 65535.0,
                         (float) core->border.blue / 65535.0,
                         core->border_transparency);

  cairo_set_line_width (cr, 3.0);
  cairo_stroke (cr);
  cairo_destroy (cr);

  return FALSE;
}

