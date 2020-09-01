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

// Input URL dialog
GObject *flashGetUrl;
GObject *inpUrl;

// Flashing dialog
GObject *flashingDialog;
GObject *flashingProgressBar;
GObject *flashingOutput;

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
	
	// Connect objects in the UI to our GObjects
	btnFlashBootloader	= gtk_builder_get_object(builder,"btnFlashBootloader");
	btnFlashInfinitime	= gtk_builder_get_object(builder,"btnFlashInfinitime");
	btnFlashWeb		= gtk_builder_get_object(builder,"btnFlashWeb");
	btnFlashFile		= gtk_builder_get_object(builder,"btnFlashFile");
	flashFileChooser	= gtk_builder_get_object(builder,"flashFileChooser");
	confirmFlash		= gtk_builder_get_object(builder,"confirmFlash");
	flashGetAddress		= gtk_builder_get_object(builder,"flashGetAddress");
	inpAddress		= gtk_builder_get_object(builder,"inpAddress");
	flashGetUrl		= gtk_builder_get_object(builder,"flashGetUrl");
	inpUrl			= gtk_builder_get_object(builder,"inpUrl");
	flashingDialog		= gtk_builder_get_object(builder,"flashingDialog");
	flashingProgressBar	= gtk_builder_get_object(builder,"flashingProgressBar");
	flashingOutput		= gtk_builder_get_object(builder,"flashingOutput");
	
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

void flashBootloader()
{
	char url[] = "https://github.com/lupyuen/pinetime-rust-mynewt/releases/latest/download/mynewt_nosemi.elf.bin";
	char address[] = "0x0000";
	
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
	// FIXME: This is not guaranteed to be the latest version
	char url[] = "https://github.com/JF002/Pinetime/releases/download/0.8.0-develop/pinetime-mcuboot-app-0.8.0-develop.bin";
	char address[] = "0x8000";
	
	if(flashConfirm("InfiniTime",address) == 1)
	{
		setUdev();
		downloadBinary(url);
		flash(address);
	}
}

void flashWeb()
{
	int urlSet = 0;
	int addressSet = 0;
	
	// Get the url from the getUrl dialog
	char url[1024];
	int urlResponse = gtk_dialog_run(GTK_DIALOG(flashGetUrl));
	
	if(urlResponse == GTK_RESPONSE_OK)
	{
		strcpy(url,gtk_entry_get_text(GTK_ENTRY(inpUrl)));
		printf("URL to download from: %s\n",url);
		urlSet = 1;
	}
	
	gtk_widget_hide(GTK_WIDGET(flashGetUrl));
	
	if(urlSet)
	{
		// Get the address from the getAddress dialog
		char addr[4];
		char address[6];
		
		int addressResponse = gtk_dialog_run(GTK_DIALOG(flashGetAddress));
		
		if(addressResponse == GTK_RESPONSE_OK)
		{
			strcpy(addr,gtk_entry_get_text(GTK_ENTRY(inpAddress)));
			snprintf(address,sizeof(address),"%s%s","0x",addr);
			printf("Address to flash to: %s\n",address);
			addressSet = 1;
		}
		
		
		gtk_widget_hide(GTK_WIDGET(flashGetAddress));
	}
	
	// Check if the address is set
	if(urlSet && addressSet)
	{
		// Do a last confirmation with the user
		if(flashConfirm(url,address) == 1)
		{
			// Set udev rules, download the file and flash
			setUdev();
			downloadBinary(url);
			flash(address);
		}
	}
	else if(!urlSet) printf("URL download failed\n");
	else if(!addressSet) printf("Address set failed\n");
}

void flashFile()
{
	int filenameSet = 0;
	int addressSet = 0;
	
	// Get filename from file chooser dialog
	char *filename;
	int fileChooserResponse = gtk_dialog_run(GTK_DIALOG(flashFileChooser));
	
	if(fileChooserResponse == GTK_RESPONSE_ACCEPT)
	{
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(flashFileChooser));
		printf("File to flash: %s\n",filename);
		filenameSet = 1;
	}
	
	gtk_widget_hide(GTK_WIDGET(flashFileChooser));
	
	if(filenameSet)
	{	
		// Get the address from the getAddress dialog
		char addr[8];
		char address[16];
		int addressResponse = gtk_dialog_run(GTK_DIALOG(flashGetAddress));
		
		if(addressResponse == GTK_RESPONSE_OK)
		{
			strcpy(addr,gtk_entry_get_text(GTK_ENTRY(inpAddress)));
			printf("addr: %s\n",addr);
			snprintf(address,sizeof(address),"%s%s","0x",addr);
			printf("Address to flash to: %s\n",address);
			addressSet = 1;
		}
		
		gtk_widget_hide(GTK_WIDGET(flashGetAddress));
	}
	
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
	else if(!filenameSet) printf("File picking failed\n");
	else if(!addressSet) printf("Address set failed\n");
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

// Download a binary from the internet
void downloadBinary(char url[])
{
	// Construct destination path
	char path[2048];
	char fpath[1024];
	getcwd(fpath,sizeof(fpath));
	snprintf(path,sizeof(path),"%s%s%s",
		fpath,"/",basename(url)
	);
	
	printf("Downloading url %s\n",url);
	printf("to destination  %s\n",path);
	
	// Download file using curl
	// FIXME: UI is stuck until file is done downloading, pthread?
	// Maybe dialog for progress as well?
	FILE *destinationFile;
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl)
	{
		destinationFile = fopen(path,"wb");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, destinationFile);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			printf("Download failed: %s\n",curl_easy_strerror(res));
		fclose(destinationFile);
		curl_easy_cleanup(curl);
	}
	
	// Copy the destination path to fileToFlash so flash() can find it
	strcpy(fileToFlash,path);
}

// Flash file in location fileToFlash to a given address on the Pinetime
void flash(char address[])
{
	// TODO: "Flashing..." dialog for showing OpenOCD's log
	
	gtk_dialog_run(GTK_DIALOG(flashingDialog));

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
	while(fgets(buffer,sizeof(buffer),p) != NULL)
	{
		printf("OPENOCD COMMAND BUFFER:\n%s\n",buffer);
	}
	
	pclose(p);
	
	// Clean up leftovers
	// remove(fileToFlash); FIXME: DON'T HAVE THIS UNCOMMENTED WHEN TESTING ON RANDOM FILES
	strcpy(fileToFlash,"");
	
	// Close the widget
	gtk_widget_hide(GTK_WIDGET(flashingDialog));
}

