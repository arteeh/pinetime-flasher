
app:
	gcc `pkg-config --cflags gtk+-3.0` -o app app.c `pkg-config --libs gtk+-3.0`

clean:
	rm app
