#include <gtk/gtk.h>

void flashBootloader()
{
	g_print("Flashing bootloader...\n");
}

void flashInfinitime()
{
	g_print("Flashing Infinitime...\n");
}

void flashWeb()
{
	g_print("Flashing Web URL...\n");
}

void flashFile()
{
	g_print("Flashing selected file...\n");
}

int main(int argc,char *argv[])
{
	GtkBuilder *builder;
	GError *error = NULL;
	
	GObject *window;
	GObject *buttonFlashBootloader;
	GObject *buttonFlashInfinitime;
	GObject *buttonFlashWeb;
	GObject *buttonFlashFile;
	
	gtk_init(&argc,&argv);
	
	// Construct a GtkBuilder instance and load our UI description
	builder = gtk_builder_new();
	
	if (gtk_builder_add_from_file(builder, "app.glade", &error) == 0)
	{
		g_printerr ("Error loading file: %s\n", error->message);
		g_clear_error (&error);
		return 1;
	}
	
	// Connect signal handlers to the constructed widgets.
	window = gtk_builder_get_object(builder,"window");
	
	// Close app when x is pressed
	g_signal_connect (window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	
	buttonFlashBootloader = gtk_builder_get_object(builder,"buttonFlashBootloader");
	buttonFlashInfinitime = gtk_builder_get_object(builder,"buttonFlashInfinitime");
	buttonFlashWeb = gtk_builder_get_object(builder,"buttonFlashWeb");
	buttonFlashFile = gtk_builder_get_object(builder,"buttonFlashFile");
	
	// Connect button presses to functions
	g_signal_connect(buttonFlashBootloader,"clicked",G_CALLBACK(flashBootloader),NULL);
	g_signal_connect(buttonFlashInfinitime,"clicked",G_CALLBACK(flashInfinitime),NULL);
	g_signal_connect(buttonFlashWeb,"clicked",G_CALLBACK(flashWeb),NULL);
	g_signal_connect(buttonFlashFile,"clicked",G_CALLBACK(flashFile),NULL);
	
	gtk_main();
	
	return 0;
}
