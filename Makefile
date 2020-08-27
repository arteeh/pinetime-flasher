FILES = app.c

app:
	gcc `pkg-config --cflags gtk+-3.0 libhandy-1` -o app $(FILES) `pkg-config --libs gtk+-3.0 libhandy-1`

clean:
	rm app
