all: clean app

OUTPUT = pinetime-flasher
FILES = pinetime-flasher.c ui.c
CFLAGS = `pkg-config --cflags gtk+-3.0 libhandy-1` -rdynamic
LIBS = `pkg-config --libs gtk+-3.0 libhandy-1` -lcurl
WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

app:
	glib-compile-resources pinetime-flasher.gresource.xml --target=ui.c --generate-source
	clang $(CFLAGS) $(FILES) $(LIBS) $(WARNINGS) $(DEBUG) $(OPTIMIZE) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)
	rm -f ui.c

run:
	./$(OUTPUT)

