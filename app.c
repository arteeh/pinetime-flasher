#include <gtk/gtk.h>

GtkBuilder *builder;
GError *error = NULL;

// Init objects to load builder objects into

// Main window
GObject *window;

// Flash file chooser dialog
GObject *flashFileChooser;
GObject *btnFlashFileChooserSelect;

// Buttons
GObject *btnFlashBootloader;
GObject *btnFlashInfinitime;
GObject *btnFlashWeb;
GObject *btnFlashFile;

// Confirmation message dialogs
GObject *confirmFlashBootloader;
GObject *confirmFlashInfinitime;
GObject *confirmFlashGeneric;


const char bootloaderUrl[] = "https://github.com/lupyuen/pinetime-rust-mynewt/releases/latest/download/mynewt_nosemi.elf.bin";
const char infinitimeUrl[] = "https://github.com/JF002/Pinetime/releases/download/0.7.1/pinetime-mcuboot-app.img";

// Generically handle input from flash confirmation dialogues, used for flash*() functions
int flashDialog(GObject *confirmFlashx, char confirmFlashxName[])
{
	int retVal;
	confirmFlashx = gtk_builder_get_object(builder,confirmFlashxName);
	int result = gtk_dialog_run(GTK_DIALOG(confirmFlashx));
	switch(result)
	{
		case GTK_RESPONSE_YES:
			g_print("Flashing...\n");
			retVal = 1;
			break;
		default:
			g_print("Cancelling flash...\n");
			retVal = 0;
			break;
	}
	gtk_widget_hide(GTK_WIDGET(confirmFlashx));
	return retVal;
}

void flashBootloader()
{
	flashDialog(confirmFlashBootloader,"confirmFlashBootloader");
}

void flashInfinitime()
{
	flashDialog(confirmFlashInfinitime,"confirmFlashInfinitime");
}

void flashWeb()
{
	//input or sumn
	flashDialog(confirmFlashGeneric,"confirmFlashGeneric");
}

void flashFile()
{
	// Get filename from file chooser dialog
	char *filename;
	flashFileChooser = gtk_builder_get_object(builder,"flashFileChooser");
	btnFlashFileChooserSelect = gtk_builder_get_object(builder,"btnFlashFileChooserSelect");
	int response = gtk_dialog_run(GTK_DIALOG(flashFileChooser));
	if(response == GTK_RESPONSE_ACCEPT)
	{
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(flashFileChooser));
	}
	else return;
	gtk_widget_hide(GTK_WIDGET(flashFileChooser));
	
	flashDialog(confirmFlashGeneric,"confirmFlashGeneric");
}

int main(int argc,char *argv[])
{
	gtk_init(&argc,&argv);
	
	// Construct a GtkBuilder instance to load app.glade
	builder = gtk_builder_new();
	if (gtk_builder_add_from_file(builder, "app.glade", &error) == 0)
	{
		g_printerr ("Error loading file: %s\n", error->message);
		g_clear_error (&error);
		return 1;
	}
	
	// Connect window in the UI to our window object
	window = gtk_builder_get_object(builder,"window");
	
	// Close app when close button is pressed
	g_signal_connect (window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	
	// Connect buttons in the UI to our button objects
	btnFlashBootloader = gtk_builder_get_object(builder,"btnFlashBootloader");
	btnFlashInfinitime = gtk_builder_get_object(builder,"btnFlashInfinitime");
	btnFlashWeb = gtk_builder_get_object(builder,"btnFlashWeb");
	btnFlashFile = gtk_builder_get_object(builder,"btnFlashFile");
	
	// Connect button press events to our functions
	g_signal_connect(btnFlashBootloader,"clicked",G_CALLBACK(flashBootloader),NULL);
	g_signal_connect(btnFlashInfinitime,"clicked",G_CALLBACK(flashInfinitime),NULL);
	g_signal_connect(btnFlashWeb,"clicked",G_CALLBACK(flashWeb),NULL);
	g_signal_connect(btnFlashFile,"clicked",G_CALLBACK(flashFile),NULL);
	
	gtk_main();
	
	return 0;
}
