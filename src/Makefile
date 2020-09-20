all: clean app

OUTPUT = companion
FILES = companion.c ui.c updates.c advanced.c
CFLAGS = `pkg-config --cflags gtk+-3.0 libhandy-1`
LIBS = `pkg-config --libs gtk+-3.0 libhandy-1` -lcurl
WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

app:
	glib-compile-resources companion.gresource.xml --target=ui.c --generate-source
	clang $(CFLAGS) $(FILES) $(LIBS) $(WARNINGS) $(DEBUG) $(OPTIMIZE) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)
	rm -f ui.c

run:
	./$(OUTPUT)

