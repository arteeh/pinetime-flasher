// time() for debugging
#include <sys/time.h>
// strlen()
#include <string.h>
// For system(), to run executables
#include <unistd.h>
// For basename() in downloadBinary()
#include <libgen.h>
// GTK library
#include <gtk/gtk.h>
// Libhandy
#include <handy.h>

const char arch[] = "amd64";

char bootloaderUrl[] = "https://github.com/lupyuen/pinetime-rust-mynewt/releases/latest/download/mynewt_nosemi.elf.bin";
char infinitimeUrl[] = "https://github.com/JF002/Pinetime/releases/download/0.7.1/pinetime-mcuboot-app.img";

char fileToFlash[PATH_MAX];

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

// Copy OpenOCD Udev rules to /etc/udev/rules.d/
void setUdev()
{
	// If the udev rule file already exists, skip
	if(access("/etc/udev/rules.d/60-openocd.rules",F_OK) != -1)
	{
		g_print("OpenOCD udev rule already installed, skipping\n");
		return;
	}
	
	g_print("Setting udev rules for ST-Link flashing\n");
	DIR *dirExists;
	dirExists = opendir("/etc/udev/rules.d/");
	if (dirExists)
	{
		// Construct the command we're going to execute
		// TODO: THIS ARBITRARY ARRAY SIZE IS SILLY AND MAY CAUSE ISSUES. FIX IT
		char command[500];
		// Pkexec calls PolicyKit to ask the user for the root password
		// ...like sudo for gui's, i guess
		strcpy(command,"pkexec cp ");
		// Get current working directory, because as root we won't know where to find it
		char path[PATH_MAX];
		getcwd(path,sizeof(path));
		strcat(command,path);
		strcat(command,"/openocd/");
		strcat(command,arch);
		strcat(command,"/contrib/60-openocd.rules /etc/udev/rules.d/");
		g_print("%s\n",command);
		system(command);
		// Reload udev rules, TODO: this means two password requests in a row, fix?
		char command2[] = "pkexec udevadm control --reload-rules";
		g_print("%s\n",command2);
		system(command2);
	}
	closedir(dirExists);
}

// Download a binary from the internet using wget
void downloadBinary(char url[])
{
	// Construct destination path
	char path[PATH_MAX];
	getcwd(path,sizeof(path));
	strcat(path,"/");
	strcat(path,basename(url));
	
	// Construct wget command
	char command[500] = "wget -q ";
	strcat(command,url);
	strcat(command," -O ");
	strcat(command,path);
	g_print("%s\n",command);
	
	// Run wget command
	system(command);
	
	// Copy the destination path to fileToFlash so flash() can find it
	strcpy(fileToFlash,path);
}

// Flash file in location fileToFlash to a given address on the Pinetime
void flash(char address[])
{
	g_print("Flashing %s to address %s\n",fileToFlash,address);
	
	// Construct openocd command
	char command[500];
	strcpy(command,"openocd/");
	strcat(command,arch);
	strcat(command,"/bin/openocd -c 'set filename ");
	strcat(command,fileToFlash);
	strcat(command," ' -c 'set address ");
	strcat(command,address);
	strcat(command," ' -f scripts/swd-stlink.ocd");
	strcat(command," -f scripts/flash-program.ocd");
	
	// Run it
	g_print("%s SIZEOF:%li\n",command,strlen(command));
	system(command);
	
	// Clean up leftovers
	remove(fileToFlash);
	strcpy(fileToFlash,"");
}

// Generically handle input from flash confirmation dialogues, used for flash*() functions
int flashDialog(GObject *confirmFlashx, char confirmFlashxName[])
{
	int retVal;
	confirmFlashx = gtk_builder_get_object(builder,confirmFlashxName);
	int result = gtk_dialog_run(GTK_DIALOG(confirmFlashx));
	switch(result)
	{
		case GTK_RESPONSE_YES:
			retVal = 1;
			break;
		default:
			retVal = 0;
			break;
	}
	gtk_widget_hide(GTK_WIDGET(confirmFlashx));
	return retVal;
}

void flashBootloader()
{
	// If user clicks yes in the confirmation dialog, commence the flashing
	if(flashDialog(confirmFlashBootloader,"confirmFlashBootloader") == 1)
	{
		setUdev();
		downloadBinary(bootloaderUrl);
		flash("0x0000");
	}
}

void flashInfinitime()
{
	if(flashDialog(confirmFlashInfinitime,"confirmFlashInfinitime") == 1)
	{
		setUdev();
		downloadBinary(infinitimeUrl);
		flash("0x8000");
	}
}

void flashWeb()
{
	//input or sumn
	char url[200];
	
	if(flashDialog(confirmFlashGeneric,"confirmFlashGeneric") == 1)
	{
		setUdev();
		// TODO: Input dialog for entering a URL
		downloadBinary(url);
		flash("0x8000");
	}
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
		gtk_widget_hide(GTK_WIDGET(flashFileChooser));
	}
	else 
	{
		gtk_widget_hide(GTK_WIDGET(flashFileChooser));
		return;
	}
	
	g_print("File to flash: %s\n",filename);
	
	if(flashDialog(confirmFlashGeneric,"confirmFlashGeneric") == 1)
	{
		setUdev();
		strcpy(fileToFlash,filename);
		flash("0x8000");
	}
}

int main(int argc,char *argv[])
{
	gtk_init(&argc,&argv);
	hdy_init();
	
	struct timeval start, stop;
	
	gettimeofday(&start, NULL);
	
	builder = gtk_builder_new_from_file("pinetime-companion.ui");
	
	gettimeofday(&stop, NULL);
	double secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
	printf("time taken %f\n",secs);
	
	// Connect objects in the UI to our objects
	window			= gtk_builder_get_object(builder,"window");
	btnFlashBootloader	= gtk_builder_get_object(builder,"btnFlashBootloader");
	btnFlashInfinitime	= gtk_builder_get_object(builder,"btnFlashInfinitime");
	btnFlashWeb		= gtk_builder_get_object(builder,"btnFlashWeb");
	btnFlashFile		= gtk_builder_get_object(builder,"btnFlashFile");
	
	// Close app when close button is pressed
	g_signal_connect (window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	
	// Connect button press events to our functions
	g_signal_connect(btnFlashBootloader,"clicked",G_CALLBACK(flashBootloader),NULL);
	g_signal_connect(btnFlashInfinitime,"clicked",G_CALLBACK(flashInfinitime),NULL);
	g_signal_connect(btnFlashWeb,"clicked",G_CALLBACK(flashWeb),NULL);
	g_signal_connect(btnFlashFile,"clicked",G_CALLBACK(flashFile),NULL);
	
	// Show the window. I hate GTK.
	gtk_widget_show_all(GTK_WIDGET(window));
	
	gtk_main();
	
	return 0;
}

