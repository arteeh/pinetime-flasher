#include "advanced.h"

char arch[6];

char fileToFlash[4096];
char filePath[4096];
char url[4096];
int address = 0x0404;
int urlSet = 0;
int addressSet = 0;
int filePathSet = 0;
int downloadDone = 0;
int flashDone = 0;
int confirmed = 0;
int udevSet = 0;

// Windows
GObject *advancedWindows;
GObject *advancedWindowMain;
GObject *advancedWindowGetUrl;
GObject *advancedWindowGetAddress;
GObject *advancedWindowConfirm;
GObject *advancedWindowDownloading;
GObject *advancedWindowFlashing;

// advancedWindowMain
GObject *btnFlashBootloader;
GObject *btnFlashInfinitime;
GObject *btnFlashWeb;
GObject *btnFlashFile;

// advancedWindowGetUrl
GObject *inpGetUrl;
GObject *btnGetUrlCancel;
GObject *btnGetUrlContinue;

// advancedWindowGetAddress
GObject *inpGetAddress;
GObject *btnGetAddressCancel;
GObject *btnGetAddressContinue;

// advancedWindowConfirm
GObject *lblConfirm;
GObject *btnConfirmCancel;
GObject *btnConfirmContinue;

// Flash native file chooser dialog
GObject *getFileChooser;

// Initialize the UI and all event handlers under the Advanced page
void initAdvanced()
{
	setArch();
	
	// Connect objects in the UI to our GObjects
	advancedWindows			= gtk_builder_get_object(builder,"advancedWindows");
	advancedWindowMain		= gtk_builder_get_object(builder,"advancedWindowMain");
	advancedWindowGetUrl		= gtk_builder_get_object(builder,"advancedWindowGetUrl");
	advancedWindowGetAddress	= gtk_builder_get_object(builder,"advancedWindowGetAddress");
	advancedWindowConfirm		= gtk_builder_get_object(builder,"advancedWindowConfirm");
	advancedWindowDownloading	= gtk_builder_get_object(builder,"advancedWindowDownloading");
	advancedWindowFlashing		= gtk_builder_get_object(builder,"advancedWindowFlashing");
	btnFlashBootloader		= gtk_builder_get_object(builder,"btnFlashBootloader");
	btnFlashInfinitime		= gtk_builder_get_object(builder,"btnFlashInfinitime");
	btnFlashWeb			= gtk_builder_get_object(builder,"btnFlashWeb");
	btnFlashFile			= gtk_builder_get_object(builder,"btnFlashFile");
	inpGetUrl			= gtk_builder_get_object(builder,"inpGetUrl");
	btnGetUrlCancel			= gtk_builder_get_object(builder,"btnGetUrlCancel");
	btnGetUrlContinue		= gtk_builder_get_object(builder,"btnGetUrlContinue");
	inpGetAddress			= gtk_builder_get_object(builder,"inpGetAddress");
	btnGetAddressCancel		= gtk_builder_get_object(builder,"btnGetAddressCancel");
	btnGetAddressContinue		= gtk_builder_get_object(builder,"btnGetAddressContinue");
	lblConfirm			= gtk_builder_get_object(builder,"lblConfirm");
	btnConfirmCancel		= gtk_builder_get_object(builder,"btnConfirmCancel");
	btnConfirmContinue		= gtk_builder_get_object(builder,"btnConfirmContinue");	
	getFileChooser			= gtk_builder_get_object(builder,"getFileChooser");
	
	// Connect button press events to our functions
	g_signal_connect(btnFlashBootloader,"clicked",G_CALLBACK(_btnFlashBootloader),NULL);
	g_signal_connect(btnFlashInfinitime,"clicked",G_CALLBACK(_btnFlashInfinitime),NULL);
	g_signal_connect(btnFlashWeb,"clicked",G_CALLBACK(_btnFlashWeb),NULL);
	g_signal_connect(btnFlashFile,"clicked",G_CALLBACK(_btnFlashFile),NULL);
	g_signal_connect(btnGetUrlCancel,"clicked",G_CALLBACK(_btnGetUrlCancel),NULL);
	g_signal_connect(btnGetUrlContinue,"clicked",G_CALLBACK(_btnGetUrlContinue),NULL);
	g_signal_connect(btnGetAddressCancel,"clicked",G_CALLBACK(_btnGetAddressCancel),NULL);
	g_signal_connect(btnGetAddressContinue,"clicked",G_CALLBACK(_btnGetAddressContinue),NULL);
	g_signal_connect(btnConfirmCancel,"clicked",G_CALLBACK(_btnConfirmCancel),NULL);
	g_signal_connect(btnConfirmContinue,"clicked",G_CALLBACK(_btnConfirmContinue),NULL);
}

void clean()
{
	gtk_stack_set_visible_child(GTK_STACK(advancedWindows),GTK_WIDGET(advancedWindowMain));
	strcpy(url,"");
	strcpy(fileToFlash,"");
	strcpy(filePath,"");
	address = 0x0404;
	flashDone = 0;
	urlSet = 0;
	addressSet = 0;
	filePathSet = 0;
	downloadDone = 0;
	flashDone = 0;
	confirmed = 0;
	udevSet = 0;
}

void setArch()
{
	struct utsname *systemData = malloc(sizeof(struct utsname));
	uname(systemData);
	strcpy(arch,systemData->machine);
	free(systemData);
	
	// TODO: Test on arm64 and arm32 architectures
	if(strcmp(arch,"x86_64") == 0) strcpy(arch,"amd64");
	else if(strcmp(arch,"arm64") == 0) strcpy(arch,"arm64");
	else if(strcmp(arch,"arm32") == 0) strcpy(arch,"arm32");
	else printf("ERROR: setArch() failed\n");
	
	printf("System architecture: %s\n",arch);
}

void _btnFlashBootloader()
{
	// FIXME: This is not guaranteed to be the latest version
	strcpy(url,"https://github.com/lupyuen/pinetime-rust-mynewt/releases/download/v5.0.4/mynewt.elf.bin");
	urlSet = 1;
	address = 0x0000;
	addressSet = 1;
	
	flashConfirm("the MCUBoot bootloader");
	if(confirmed == 1) setUdev();
	if(confirmed == 1 && udevSet && urlSet && addressSet)
	{
		downloadBinary();
		flash(1);
	}
	clean();
}

void _btnFlashInfinitime()
{
	// FIXME: This is not guaranteed to be the latest version
	strcpy(url,"https://github.com/JF002/Pinetime/releases/download/0.8.2/image-0.8.2.bin");
	address = 0x8000;
	
	flashConfirm("InfiniTime");
	if(confirmed == 1) setUdev();
	if(confirmed == 1 && udevSet && urlSet && addressSet)
	{
		downloadBinary();
		flash(1);
	}
	clean();
}

void _btnFlashWeb()
{
	gtk_stack_set_visible_child(GTK_STACK(advancedWindows),GTK_WIDGET(advancedWindowGetUrl));
	while(urlSet == 0) gtk_main_iteration_do(0);
	if(urlSet == -1) return;
	gtk_stack_set_visible_child(GTK_STACK(advancedWindows),GTK_WIDGET(advancedWindowGetAddress));
	while(addressSet == 0) gtk_main_iteration_do(0);
	if(addressSet == -1) return;
	
	flashConfirm(url);
	if(confirmed == 1) setUdev();
	if(confirmed == 1 && udevSet && urlSet && addressSet)
	{
		downloadBinary();
		flash(1);
	}
	clean();
}

void _btnFlashFile()
{
	// Get filePath from file chooser dialog
	int fileChooserResponse = gtk_native_dialog_run(GTK_NATIVE_DIALOG(getFileChooser));
	if(fileChooserResponse == GTK_RESPONSE_ACCEPT)
	{
		strcpy(filePath,gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(getFileChooser)));
		printf("File to flash: %s\n",filePath);
		strcpy(fileToFlash,filePath);
		filePathSet = 1;
	}
	else
	{
		printf("File chooser cancelled\n");
		filePathSet = -1;
		return;
	}
	gtk_widget_hide(GTK_WIDGET(getFileChooser));
	
	gtk_stack_set_visible_child(GTK_STACK(advancedWindows),GTK_WIDGET(advancedWindowGetAddress));
	while(addressSet == 0) gtk_main_iteration_do(0);
	if(addressSet == -1) printf("Address set failed\n"); clean(); return;
	
	flashConfirm(filePath);
	if(confirmed == 1) setUdev();
	if(confirmed == 1 && udevSet && filePathSet && addressSet) flash(1);
	clean();
}

void _btnGetUrlCancel()
{
	gtk_stack_set_visible_child(GTK_STACK(advancedWindows),GTK_WIDGET(advancedWindowMain));
	urlSet = -1;
	printf("URL input cancelled\n");
}

void _btnGetUrlContinue()
{
	strcpy(url,gtk_entry_get_text(GTK_ENTRY(inpGetUrl)));
	urlSet = 1;
	printf("URL to download from: %s\n",url);
}

void _btnGetAddressCancel()
{
	gtk_stack_set_visible_child(GTK_STACK(advancedWindows),GTK_WIDGET(advancedWindowMain));
	addressSet = -1;
	printf("Address input cancelled\n");
}

void _btnGetAddressContinue()
{
	strcpy(address,gtk_entry_get_text(GTK_ENTRY(inpGetAddress)));
	addressSet = 1;
	printf("Address to flash to: 0x%i\n",address);
}

void _btnConfirmCancel()
{
	gtk_stack_set_visible_child(GTK_STACK(advancedWindows),GTK_WIDGET(advancedWindowMain));
	confirmed = -1;
	printf("Confirmation cancelled\n");
}

void _btnConfirmContinue()
{
	confirmed = 1;
	printf("Confirmation continued\n");
}

void flashConfirm(char name[])
{
	char message[1024];
	char shortname[512];
	strcpy(shortname,basename(name));
		
	snprintf(message,sizeof(message),"%s%s%s%i%s",
		"<span size=\"large\" font_weight=\"bold\">Are you sure you want to flash ",
		shortname,
		" to address 0x",
		address,
		"?</span>"
	);
	gtk_label_set_markup(GTK_LABEL(lblConfirm),message);
	gtk_stack_set_visible_child(GTK_STACK(advancedWindows),GTK_WIDGET(advancedWindowConfirm));
	while(confirmed == 0) gtk_main_iteration_do(0);
	if(confirmed == 1) printf("Confirmed\n");
	else if(confirmed == -1) printf("Cancelled\n");
}

// Copy OpenOCD Udev rules to /etc/udev/rules.d/
// TODO: How to get this to work in flatpak without asking for full filesystem permissions?
void setUdev()
{
	// If the udev rule file already exists, skip
	if(access("/etc/udev/rules.d/60-openocd.rules",F_OK) != -1)
	{
		printf("OpenOCD udev rule already installed, skipping\n");
		udevSet = 1;
	}
	else
	{
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
		else printf("ERROR: Can't find /etc/udev/rules.d/\n");
		closedir(dirExists);
		
		// Check again to make sure it installed properly
		if(access("/etc/udev/rules.d/60-openocd.rules",F_OK) != -1)
		{
			printf("OpenOCD udev rule installed\n");
			udevSet = 2;
		}
		else
		{
			printf("ERROR: OpenOCD udev rule failed to install\n");
			udevSet = -1;
		}
	}
}

// We create a thread in downloadBinary, which runs the following code of downloading the file
void *downloadBinaryThread(void * arg)
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
		if(res != CURLE_OK) printf("Download failed: %s\n",curl_easy_strerror(res));
		fclose(destinationFile);
		curl_easy_cleanup(curl);
	}
	
	// Copy the destination path to fileToFlash so flash() can find it
	strcpy(fileToFlash,path);
	
	// Notify the main function that downloading is finished and it can continue
	downloadDone = 1;
	
	pthread_exit(NULL);
}

// Download a binary from the internet
void downloadBinary()
{
	gtk_stack_set_visible_child(GTK_STACK(advancedWindows),GTK_WIDGET(advancedWindowDownloading));
	
	// Create the downloading thread
	pthread_t curlThread;
	pthread_create(&curlThread,NULL,downloadBinaryThread,NULL);
	
	// Let GTK do its thing until downloading is done
	while(downloadDone == 0) gtk_main_iteration_do(0);
	downloadDone = 0;
}

void *flashThread(void * removeAfter)
{
	// Get current working directory
	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	
	// Construct openocd command
	char command[512];
	snprintf(command,sizeof(command),"%s%s%s%s%s%s%s%i%s%s%s",
		"openocd/",arch,"/bin/openocd ",
		"-c 'set filePath ",fileToFlash," ' ",
		"-c 'set address 0x",address," ' ",
		"-f scripts/swd-stlink.ocd ",
		"-f scripts/flash-program.ocd"
	);
	printf("%s\n",command);
	
	// Run openocd
	system(command);
	
	// Clean up leftovers
	if(removeAfter) remove(fileToFlash);
	strcpy(fileToFlash,"");
	
	// Tell the main thread that flashing is done
	flashDone = 1;
	
	pthread_exit(NULL);
}

// Flash file in location fileToFlash to a given address on the Pinetime
void flash(int removeAfter)
{
	gtk_stack_set_visible_child(GTK_STACK(advancedWindows),GTK_WIDGET(advancedWindowFlashing));
	
	pthread_t fThread;
	pthread_create(&fThread,NULL,flashThread,(void *)removeAfter);
	
	// TODO: Get OpenOCD's output and place it on the screen
	
	// Let GTK do its thing until flashing is done
	while(flashDone == 0) gtk_main_iteration_do(0);
}

