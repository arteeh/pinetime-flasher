all: clean app

OUTPUT = pinetime-companion
FILES = pinetime-companion.c updates.c advanced.c
CFLAGS = `pkg-config --cflags gtk+-3.0 libhandy-1`
LIBS = `pkg-config --libs gtk+-3.0 libhandy-1` -lcurl
WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

app:
	clang $(CFLAGS) $(FILES) $(LIBS) $(WARNINGS) $(DEBUG) $(OPTIMIZE) -o $(OUTPUT)

flatpak:
	rm -f $(OUTPUT)
	clang $(CFLAGS) $(FILES) $(LIBS) $(WARNINGS) $(DEBUG) $(OPTIMIZE) -o $(OUTPUT)
	cp pinetime-companion /app/bin/pinetime-companion
	cp pinetime-companion.ui /app/bin/pinetime-companion.ui
	cp -r openocd /app/bin/openocd
	cp -r scripts /app/bin/scripts

clean:
	rm -f $(OUTPUT)

run:
	./$(OUTPUT)

