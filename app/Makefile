all: clean app

OUTPUT = pinetime-flasher
FILES = ../src/pinetime-flasher.c ../src/resources.c
CFLAGS = `pkg-config --cflags gtk+-3.0 libhandy-1` -rdynamic
LIBS = `pkg-config --libs gtk+-3.0 libhandy-1` -lcurl
WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

app:
	cd ../src; \
	glib-compile-resources \
		pinetime-flasher.gresource.xml \
		--target=resources.c --generate-source
	gcc $(CFLAGS) $(FILES) $(LIBS) $(WARNINGS) $(DEBUG) $(OPTIMIZE) -o $(OUTPUT);
	cd ../src; \
	rm -f resources.c

clean:
	rm -f $(OUTPUT)
	cd ../src; \
	rm -f resources.c

run:
	./$(OUTPUT)
