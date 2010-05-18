
objects = main.o block_notes_misc.o block_notes_install.o block_notes_conf.o block_notes_error_dialog.o block_notes_setting.o

%.o: src/%.c
	gcc -c $< -o $@ -g `pkg-config --cflags gtk+-2.0`

compile: $(objects)
	gcc $(objects) -o block-notes `pkg-config --libs gtk+-2.0`

clean:
	rm *.o -fv
