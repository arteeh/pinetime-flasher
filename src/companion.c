#include "companion.h"

GtkBuilder *builder;
GObject *window;

int main(int argc,char *argv[])
{
	gtk_init(&argc,&argv);
	hdy_init();
	
	// Construct a GtkBuilder instance and fill it with the main UI
	builder = gtk_builder_new_from_resource("/com/arteeh/PineTime-Companion/companion.ui");
	
	// Connect window object in the builder to our own window object
	window = gtk_builder_get_object(builder,"window");
	
	// Close app when the close button is pressed
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	
	// Set up other pages
	//initUpdates();
	initAdvanced();
	
	// Show the window
	gtk_widget_show_all(GTK_WIDGET(window));
	
	gtk_main();
	
	return 0;
}

