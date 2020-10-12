#include "companion.h"

GtkBuilder *builder;

GObject *window;
GObject *btnAbout;
GObject *windowAbout;

int main(int argc,char *argv[])
{
	gtk_init(&argc,&argv);
	hdy_init();
	
	// Construct a GtkBuilder instance and fill it with the main UI
	builder = gtk_builder_new_from_resource("/com/arteeh/Companion/companion.ui");
	
	// Connect window object in the builder to our own window object
	window = gtk_builder_get_object(builder,"window");
	btnAbout = gtk_builder_get_object(builder,"btnAbout");
	windowAbout = gtk_builder_get_object(builder,"windowAbout");
	
	// Connect all the signal handlers in the ui file
	gtk_builder_connect_signals(builder,NULL);
	
	// Set up other pages
	initUpdates();
	initAdvanced();
	
	// Show the window
	gtk_widget_show_all(GTK_WIDGET(window));
	
	gtk_main();
	
	return 0;
}

void _btnAbout()
{
	gtk_widget_show(GTK_WIDGET(windowAbout));
}
