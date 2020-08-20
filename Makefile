FILES = app.c

app:
	gcc `pkg-config --cflags gtk+-3.0` -o app $(FILES) `pkg-config --libs gtk+-3.0`

clean:
	rm app
