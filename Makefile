all: clean app run

OUTPUT = app
FILES = app.c
CFLAGS = `pkg-config --cflags gtk+-3.0 libhandy-1`
LIBS = `pkg-config --libs gtk+-3.0 libhandy-1`
WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

app:
	gcc $(CFLAGS) $(FILES) $(LIBS) $(WARNINGS) $(DEBUG) $(OPTIMIZE) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)

run:
	./$(OUTPUT)
