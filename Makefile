lib =  libui_gtk_dialog_error.o libui_gtk_gadget_move.o libui_gtk_gadget_window.o libui_gtk_button.o libui_gtk_image.o libui_gtk_label.o libui_gtk_entry.o

%.o: lib/%.c
	gcc -c $< -o $@ -g `pkg-config --cflags gtk+-2.0` -I/usr/include

objects = main.o block_notes_misc.o block_notes_error_dialog.o block_notes_setting.o block_notes_cairo.o block_notes_xml.o block_notes_about.o

%.o: src/%.c
	gcc -c $< -o $@ -g `pkg-config --cflags gtk+-2.0` `xml2-config --cflags`

compile: $(lib) $(objects)
	gcc $(lib) $(objects) -o block-notes `pkg-config --libs gtk+-2.0` `xml2-config --libs` -I/usr/include

clean:
	rm *.o -fv
