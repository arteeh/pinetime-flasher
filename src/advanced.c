#include "main.h"
#include "advanced.h"

// TODO: Event handlers for buttons are broken since the move to Builder, reimplement them.

const char arch[] = "amd64";
char bootloaderUrl[] = "https://github.com/lupyuen/pinetime-rust-mynewt/releases/latest/download/mynewt_nosemi.elf.bin";
char infinitimeUrl[] = "https://github.com/JF002/Pinetime/releases/download/0.7.1/pinetime-mcuboot-app.img";

// Copy OpenOCD Udev rules to /etc/udev/rules.d/
void setUdev()
{
	// If the udev rule file already exists, skip
	if(access("/etc/udev/rules.d/60-openocd.rules",F_OK) != -1)
	{
		printf("OpenOCD udev rule already installed, skipping\n");
		return;
	}

	printf("Setting udev rules for ST-Link flashing\n");
	DIR *dirExists;
	dirExists = opendir("/etc/udev/rules.d/");
	if (dirExists)
	{
		// Construct the command we're going to execute
		// FIXME: This array size is silly and may cause issues in the future.
		char command[4096];
		// Pkexec calls PolicyKit to ask the user for the root password
		// ...like sudo for gui's, i guess
		strcpy(command,"pkexec cp ");
		// Get current working directory, because as root we won't know where to find it
		char path[4096];
		getcwd(path,sizeof(path));
		strcat(command,path);
		strcat(command,"/openocd/");
		strcat(command,arch);
		strcat(command,"/contrib/60-openocd.rules /etc/udev/rules.d/");
		printf("%s\n",command);
		system(command);
		// Reload udev rules
    // FIXME: this means two password requests in a row, fix?
		char command2[] = "pkexec udevadm control --reload-rules";
		printf("%s\n",command2);
		system(command2);
	}
	closedir(dirExists);
}

// Download a binary from the internet using wget
void downloadBinary(char url[])
{
	// Construct destination path
	char path[4096];
	getcwd(path,sizeof(path));
	strcat(path,"/");
	strcat(path,basename(url));

	// Construct wget command
	char command[500] = "wget -q ";
	strcat(command,url);
	strcat(command," -O ");
	strcat(command,path);
	printf("%s\n",command);

	// Run wget command
	system(command);

	// Copy the destination path to fileToFlash so flash() can find it
	strcpy(fileToFlash,path);
}

// Flash file in location fileToFlash to a given address on the Pinetime
void flash(char address[])
{
	printf("Flashing %s to address %s\n",fileToFlash,address);

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
	printf("%s SIZEOF:%li\n",command,strlen(command));
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
	char url[4096];

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

	printf("File to flash: %s\n",filename);

	if(flashDialog(confirmFlashGeneric,"confirmFlashGeneric") == 1)
	{
		setUdev();
		strcpy(fileToFlash,filename);
		flash("0x8000");
	}
}

