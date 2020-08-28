#include "pinetime-companion.h"

GtkBuilder *builder;
GObject *window;

int main(int argc,char *argv[])
{
	gtk_init(&argc,&argv);
	hdy_init();
	
	// Construct a GtkBuilder instance to load app.glade
	builder = gtk_builder_new_from_file("pinetime-companion.ui");
	
	// Connect objects in the UI to our objects
	window = gtk_builder_get_object(builder,"window");
	
	// Close app when close button is pressed
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	
	// Set up other pages
	initAdvanced();
	
	// Show the window
	gtk_widget_show_all(GTK_WIDGET(window));
	
	gtk_main();
	
	return 0;
}

