all: clean app

OUTPUT = pinetime-companion
FILES = pinetime-companion.c ui.c updates.c advanced.c
CFLAGS = `pkg-config --cflags gtk+-3.0 libhandy-1`
LIBS = `pkg-config --libs gtk+-3.0 libhandy-1` -lcurl
WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

app:
	glib-compile-resources pinetime-companion.gresource.xml --target=ui.c --generate-source
	clang $(CFLAGS) $(FILES) $(LIBS) $(WARNINGS) $(DEBUG) $(OPTIMIZE) -o $(OUTPUT)

clean:
	rm -f ui.c
	rm -f $(OUTPUT)

run:
	./$(OUTPUT)

