#include "advanced.h"

char arch[6];

char fileToFlash[4096];

// Flash file chooser dialog
GObject *flashFileChooser;

// Buttons
GObject *btnFlashBootloader;
GObject *btnFlashInfinitime;
GObject *btnFlashWeb;
GObject *btnFlashFile;

// Confirmation message dialog
GObject *confirmFlash;

// Input address dialog
GObject *flashGetAddress;
GObject *inpAddress;

// Initialize the UI and all event handlers under the Advanced page
void initAdvanced()
{
	// place system arch in arch string
	switch(getArch())
	{
		case 0: printf("ERROR: getArch() failed\n"); break;
		case 1: strcpy(arch,"amd64"); break;
		case 2: strcpy(arch,"arm64"); break;
		case 3: strcpy(arch,"arm32"); break;
	}
	
	// Connect objects in the UI to our objects
	btnFlashBootloader	= gtk_builder_get_object(builder,"btnFlashBootloader");
	btnFlashInfinitime	= gtk_builder_get_object(builder,"btnFlashInfinitime");
	btnFlashWeb		= gtk_builder_get_object(builder,"btnFlashWeb");
	btnFlashFile		= gtk_builder_get_object(builder,"btnFlashFile");
	flashFileChooser	= gtk_builder_get_object(builder,"flashFileChooser");
	confirmFlash		= gtk_builder_get_object(builder,"confirmFlash");
	flashGetAddress		= gtk_builder_get_object(builder,"flashGetAddress");
	inpAddress		= gtk_builder_get_object(builder,"inpAddress");
	
	// Connect button press events to our functions
	g_signal_connect(btnFlashBootloader,"clicked",G_CALLBACK(flashBootloader),NULL);
	g_signal_connect(btnFlashInfinitime,"clicked",G_CALLBACK(flashInfinitime),NULL);
	g_signal_connect(btnFlashWeb,"clicked",G_CALLBACK(flashWeb),NULL);
	g_signal_connect(btnFlashFile,"clicked",G_CALLBACK(flashFile),NULL);
}

/*
Get CPU architecture
return 1 = amd64
return 2 = arm64
return 3 = arm32
*/
int getArch()
{
	struct utsname *systemData = malloc(sizeof(struct utsname));
	uname(systemData);
	char arch[8];
	strcpy(arch,systemData->machine);
	free(systemData);
	printf("System architecture: %s\n",arch);
	
	int retVal = 0;
	
	if(strcmp(arch,"x86_64") == 0) retVal = 1;
	// TODO: Get arm64 and arm32 architectures
	
	return retVal;
}

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
		printf("%s\n",command);
		
		// Execute
		system(command);
		
		// Reload udev rules, FIXME: this means two root password requests in a row, fix?
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
	char path[4107];
	getcwd(path,sizeof(path));
	
	snprintf(path,sizeof(path),"%s%s",
		"/",basename(url)
	);
	
	// TODO: Don't run wget, use curl library instead:
	// https://curl.haxx.se/libcurl/c/
	// https://curl.haxx.se/libcurl/c/curlgtk.html
	
	// Construct wget command
	char command[4118];
	snprintf(command,sizeof(command),"%s%s%s%s",
		"wget -q",url," -O ",path
	);
	printf("%s\n",command);
	
	// Run wget command. Don't just use system() because we need the output
	FILE *p = popen(command, "r");
	// FIXME: What's a good buffer size for this?
	char buffer[4095];
	fgets(buffer,sizeof(buffer),p);
	pclose(p);
	
	// Copy the destination path to fileToFlash so flash() can find it
	
	strcpy(fileToFlash,path);
}

// Flash file in location fileToFlash to a given address on the Pinetime
void flash(char address[])
{
	// TODO: "Flashing..." dialog for showing OpenOCD's log

	printf("Flashing %s to address %s\n",fileToFlash,address);
	
	// Construct openocd command
	char command[4215];
	snprintf(command,sizeof(command),"%s%s%s%s%s%s%s%s%s%s%s",
		"openocd/",arch,"/bin/openocd ",
		"-c 'set filename ",fileToFlash," ' ",
		"-c 'set address ",address," ' ",
		"-f scripts/swd-stlink.ocd ",
		"-f scripts/flash-program.ocd"
	);
	printf("%s\n",command);
	
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

// Handle input from flash confirmation dialog, used for flash*() functions
int flashConfirm(char name[],char address[])
{
	int retVal;
	
	// Set the dialog text
	// FIXME: Proper string size? This one doesn't put out a warning when it's too small
	char message[1024];
	snprintf(message,sizeof(message),"%s%s%s%s%s",
		"Are you sure you want to flash ",name," to address ",address,"?"
	);
	gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(confirmFlash),message);
	
	// Return whether the user has pressed yes or no
	int result = gtk_dialog_run(GTK_DIALOG(confirmFlash));
	switch(result)
	{
		case GTK_RESPONSE_YES:
			retVal = 1;
			break;
		default:
			retVal = 0;
			break;
	}
	
	// Close the widget
	gtk_widget_hide(GTK_WIDGET(confirmFlash));
	
	return retVal;
}

void flashBootloader()
{
	char address[] = "0x0000";
	char url[] = "https://github.com/lupyuen/pinetime-rust-mynewt/releases/latest/download/mynewt_nosemi.elf.bin";
	
	// If user clicks yes in the confirmation dialog, commence the flashing
	if(flashConfirm("the MCUBoot bootloader",address) == 1)
	{
		setUdev();
		downloadBinary(url);
		flash(address);
	}
}

void flashInfinitime()
{
	char address[] = "0x8000";
	// FIXME: This is not guaranteed to be the latest version
	char url[] = "https://github.com/JF002/Pinetime/releases/download/0.7.1/pinetime-mcuboot-app.img";
	
	if(flashConfirm("InfiniTime",address) == 1)
	{
		setUdev();
		downloadBinary(url);
		flash(address);
	}
}

void flashWeb()
{
	char url[1024];
	
	// TODO: Dialog for entering a URL
	
	// Get the address from the getAddress dialog
	char address[6];
	int addressSet = 0;
	int addressResponse = gtk_dialog_run(GTK_DIALOG(flashGetAddress));
	switch(addressResponse)
	{
		case GTK_RESPONSE_OK:
			strcpy(address,gtk_entry_get_text(GTK_ENTRY(inpAddress)));
			printf("Address to flash to: %s\n",address);
			addressSet = 1;
			break;
		default:
			break;
	}
	gtk_widget_hide(GTK_WIDGET(flashGetAddress));
	
	// Check if the address is set
	if(addressSet)
	{
		// Do a last confirmation with the user
		if(flashConfirm("URL FILE??",address) == 1)
		{
			// Set udev rules, download the file and flash
			setUdev();
			downloadBinary(url);
			flash(address);
		}
	}
}

void flashFile()
{
	// Get filename from file chooser dialog
	char *filename;
	int filenameSet = 0;
	int fileChooserResponse = gtk_dialog_run(GTK_DIALOG(flashFileChooser));
	switch(fileChooserResponse)
	{
		case GTK_RESPONSE_ACCEPT:
			filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(flashFileChooser));
			printf("File to flash: %s\n",filename);
			filenameSet = 1;
			break;
		default:
			break;
	}
	gtk_widget_hide(GTK_WIDGET(flashFileChooser));
	
	// Get the address from the getAddress dialog
	char address[6];
	int addressSet = 0;
	int addressResponse = gtk_dialog_run(GTK_DIALOG(flashGetAddress));
	switch(addressResponse)
	{
		case GTK_RESPONSE_OK:
			strcpy(address,gtk_entry_get_text(GTK_ENTRY(inpAddress)));
			printf("Address to flash to: %s\n",address);
			addressSet = 1;
			break;
		default:
			break;
	}
	gtk_widget_hide(GTK_WIDGET(flashGetAddress));
	
	// Check if both the filename and the address are set
	if(filenameSet && addressSet)
	{
		// Do a last confirmation with the user
		if(flashConfirm(filename,address) == 1)
		{
			// Set udev rules and flash
			setUdev();
			strcpy(fileToFlash,filename);
			flash(address);
		}
	}
}

