#include "advanced.h"

/*
Set which architecture of openocd to use. Choose between "amd64", "arm32" and "arm64"
FIXME: Don't hardcode this? Set in makefile or something?
Or get arch from the system in initAdvanced and set this variable?
*/
char arch[] = "amd64";

// FIXME: Don't hardcode this either. Settings dialog where user can change this?
char bootloaderUrl[] = "https://github.com/lupyuen/pinetime-rust-mynewt/releases/latest/download/mynewt_nosemi.elf.bin";
// FIXME: Can't just grab the latest release because there are only pre-releases right now. Fix later
char infinitimeUrl[] = "https://github.com/JF002/Pinetime/releases/download/0.7.1/pinetime-mcuboot-app.img";

char fileToFlash[4096];

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

// Initialize the UI and all event handlers under the Advanced page
void initAdvanced()
{
	// Add advanced.ui to the builder
	gtk_builder_add_from_file(builder,"advanced.ui",NULL);
	
	// Connect objects in the UI to our objects
	btnFlashBootloader	= gtk_builder_get_object(builder,"btnFlashBootloader");
	btnFlashInfinitime	= gtk_builder_get_object(builder,"btnFlashInfinitime");
	btnFlashWeb		= gtk_builder_get_object(builder,"btnFlashWeb");
	btnFlashFile		= gtk_builder_get_object(builder,"btnFlashFile");
	
	// Connect button press events to our functions
	g_signal_connect(btnFlashBootloader,"clicked",G_CALLBACK(flashBootloader),NULL);
	g_signal_connect(btnFlashInfinitime,"clicked",G_CALLBACK(flashInfinitime),NULL);
	g_signal_connect(btnFlashWeb,"clicked",G_CALLBACK(flashWeb),NULL);
	g_signal_connect(btnFlashFile,"clicked",G_CALLBACK(flashFile),NULL);
}

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
		// Get current working directory, because as root we won't know where to find it
		char path[PATH_MAX];
		getcwd(path,sizeof(path));
		
		// Construct the command we're going to execute.
		// pkexec: run a command as root user
		char command[4164];
		snprintf(command,sizeof(command),"%s%s%s%s%s",
			"pkexec cp ",path,"/openocd/",arch,
			"/contrib/60-openocd.rules /etc/udev/rules.d/"
		);
		g_print("%s\n",command);
		
		// Execute
		system(command);
		
		// Reload udev rules, FIXME: this means two root password requests in a row, fix?
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
	char path[4107];
	getcwd(path,sizeof(path));
	
	snprintf(path,sizeof(path),"%s%s",
		"/",basename(url)
	);
	
	// Construct wget command
	char command[4118];
	snprintf(command,sizeof(command),"%s%s%s%s",
		"wget -q",url," -O ",path
	);
	g_print("%s\n",command);
	
	// TODO: Check if wget is installed. Maybe bundle wget with flatpak?
	
	// Run wget command. Don't just use system() because we need the output
	FILE *p = popen(command, "r");
	// FIXME: What's a good buffer size for this?
	char buffer[4096];
	fgets(buffer,sizeof(buffer),p);
	pclose(p);
	
	// Copy the destination path to fileToFlash so flash() can find it
	
	strcpy(fileToFlash,path);
}

// Flash file in location fileToFlash to a given address on the Pinetime
void flash(char address[])
{
	// TODO: "Flashing..." dialog for showing OpenOCD's log

	g_print("Flashing %s to address %s\n",fileToFlash,address);
	
	// Construct openocd command
	char command[4215];
	snprintf(command,sizeof(command),"%s%s%s%s%s%s%s%s%s%s%s",
		"openocd/",arch,"/bin/openocd ",
		"-c 'set filename ",fileToFlash," ' ",
		"-c 'set address ",address," ' ",
		"-f scripts/swd-stlink.ocd ",
		"-f scripts/flash-program.ocd"
	);
	g_print("%s\n",command);
	
	// Run it. Don't just use system() because we need the output
	FILE *p = popen(command, "r");
	// FIXME: What's a good buffer size for this?
	char buffer[4096];
	fgets(buffer,sizeof(buffer),p);
	pclose(p);
	
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
	char url[200];
	
	// TODO: Dialog for entering a URL
	
	// TODO: Dialog for specifying the address to flash to
	
	if(flashDialog(confirmFlashGeneric,"confirmFlashGeneric") == 1)
	{
		setUdev();
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
	
	// TODO: Dialog for specifying the address to flash to
	
	if(flashDialog(confirmFlashGeneric,"confirmFlashGeneric") == 1)
	{
		setUdev();
		strcpy(fileToFlash,filename);
		flash("0x8000");
	}
}

