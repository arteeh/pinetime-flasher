// LIBRARIES

// For booleans
#include <stdbool.h>
// For system(), to run executables
#include <unistd.h>
// For basename() in downloadBinary()
#include <libgen.h>
// String to number
#include <stdlib.h>
// Multithreading, for running commands AND the UI at the same time
#include <pthread.h>
// For downloading files from the internet
#include <curl/curl.h>
// GTK library
#include <gtk/gtk.h>
// Libhandy
#include <handy.h>

// DEFINITIONS

#define DO_NOT_REMOVE_AFTER 0
#define DO_REMOVE_AFTER 1

#define ABORT -1
#define WAIT 0
#define CONTINUE 1

// VARIABLES

char url[4096];
char address[8];
char filePath[4096];
char fileToFlash[4096];
bool downloadDone = false;
bool filePathSet = false;
bool flashDone = false;
int confirmed = WAIT;
int addressSet = WAIT;
int udevSet = WAIT;
int urlSet = WAIT;

// GTK OBJECTS

// Main
GtkBuilder *builder;
GObject *window;
GObject *btnAbout;
GObject *windowAbout;

// Windows
GObject *windows;
GObject *windowMain;
GObject *windowConfirm;
GObject *windowDownloading;
GObject *windowFlashing;
GObject *windowGetAddress;
GObject *windowGetUrl;

// windowMain
GObject *btnFlashBootloader;
GObject *btnFlashFile;
GObject *btnFlashInfinitime;
GObject *btnFlashWeb;

// windowGetUrl
GObject *btnGetUrlCancel;
GObject *btnGetUrlContinue;
GObject *inpGetUrl;

// windowGetAddress
GObject *btnGetAddressCancel;
GObject *btnGetAddressContinue;
GObject *inpGetAddress;

// windowConfirm
GObject *btnConfirmCancel;
GObject *btnConfirmContinue;
GObject *lblConfirm;

// Flash native file chooser dialog
GObject *getFileChooser;

// FUNCTION DECLARATIONS

void init();
void clean();
void setUdev();
void _btnAbout();
void _btnConfirmCancel();
void _btnConfirmContinue();
void _btnFlashBootloader();
void _btnFlashFile();
void _btnFlashInfinitime();
void _btnFlashWeb();
void _btnGetAddressCancel();
void _btnGetAddressContinue();
void _btnGetUrlCancel();
void _btnGetUrlContinue();
void flashConfirm(char name[]);
void *downloadBinaryThread(void * arg);
void downloadBinary();
void *flashThread(void * removeAfter);
void flash(bool removeAfter);

// FUNCTIONS

// Initialize the UI and all event handlers
void init()
{	
	// Construct a GtkBuilder instance and fill it with the main UI
	builder = gtk_builder_new_from_resource("/com/arteeh/Flasher/pinetime-flasher.ui");
	
	// Connect objects in the UI to our GObjects
	window				= gtk_builder_get_object(builder,"window");
	windows				= gtk_builder_get_object(builder,"windows");
	windowMain			= gtk_builder_get_object(builder,"windowMain");
	windowAbout			= gtk_builder_get_object(builder,"windowAbout");
	windowConfirm			= gtk_builder_get_object(builder,"windowConfirm");
	windowDownloading		= gtk_builder_get_object(builder,"windowDownloading");
	windowFlashing			= gtk_builder_get_object(builder,"windowFlashing");
	windowGetAddress		= gtk_builder_get_object(builder,"windowGetAddress");
	windowGetUrl			= gtk_builder_get_object(builder,"windowGetUrl");
	btnAbout			= gtk_builder_get_object(builder,"btnAbout");
	btnConfirmCancel		= gtk_builder_get_object(builder,"btnConfirmCancel");
	btnConfirmContinue		= gtk_builder_get_object(builder,"btnConfirmContinue");	
	btnFlashBootloader		= gtk_builder_get_object(builder,"btnFlashBootloader");
	btnFlashFile			= gtk_builder_get_object(builder,"btnFlashFile");
	btnFlashInfinitime		= gtk_builder_get_object(builder,"btnFlashInfinitime");
	btnFlashWeb			= gtk_builder_get_object(builder,"btnFlashWeb");
	btnGetAddressCancel		= gtk_builder_get_object(builder,"btnGetAddressCancel");
	btnGetAddressContinue		= gtk_builder_get_object(builder,"btnGetAddressContinue");
	btnGetUrlCancel			= gtk_builder_get_object(builder,"btnGetUrlCancel");
	btnGetUrlContinue		= gtk_builder_get_object(builder,"btnGetUrlContinue");
	getFileChooser			= gtk_builder_get_object(builder,"getFileChooser");
	inpGetAddress			= gtk_builder_get_object(builder,"inpGetAddress");
	inpGetUrl			= gtk_builder_get_object(builder,"inpGetUrl");
	lblConfirm			= gtk_builder_get_object(builder,"lblConfirm");
	
	// Connect all the signal handlers in the ui file
	gtk_builder_connect_signals(builder,NULL);
	
	// Show the window
	gtk_widget_show_all(GTK_WIDGET(window));
}

void clean()
{
	gtk_stack_set_visible_child(GTK_STACK(windows),GTK_WIDGET(windowMain));
	strcpy(url,"");
	strcpy(fileToFlash,"");
	strcpy(filePath,"");
	strcpy(address,"0x0404");
	flashDone = false;
	
	filePathSet = false;
	downloadDone = false;
	flashDone = false;
	addressSet = WAIT;
	urlSet = WAIT;
	udevSet = WAIT;
	confirmed = WAIT;
}

void _btnAbout()
{
	gtk_widget_show(GTK_WIDGET(windowAbout));
}

bool _closeAbout()
{
	gtk_widget_hide(GTK_WIDGET(windowAbout));
	return true;
}

void _btnFlashBootloader()
{
	// FIXME: This is not guaranteed to be the latest version
	strcpy(url,"https://github.com/lupyuen/pinetime-rust-mynewt/releases/download/v5.0.4/mynewt.elf.bin");
	urlSet = true;
	strcpy(address,"0x0000");
	addressSet = CONTINUE;
	
	flashConfirm("the MCUBoot bootloader");
	//if(confirmed == CONTINUE) setUdev();
	if(confirmed == CONTINUE && urlSet == CONTINUE && addressSet == CONTINUE /*&& udevSet*/)
	{
		downloadBinary();
		flash(DO_REMOVE_AFTER);
	}
	clean();
}

void _btnFlashInfinitime()
{
	// FIXME: This is not guaranteed to be the latest version
	strcpy(url,"https://github.com/JF002/Pinetime/releases/download/0.8.2/image-0.8.2.bin");
	urlSet = true;
	strcpy(address,"0x8000");
	addressSet = CONTINUE;
	
	flashConfirm("InfiniTime");
	//if(confirmed == CONTINUE) setUdev();
	if(confirmed == CONTINUE && urlSet == CONTINUE && addressSet == CONTINUE /*&& udevSet*/)
	{
		downloadBinary();
		flash(DO_REMOVE_AFTER);
	}
	clean();
}

void _btnFlashWeb()
{
	gtk_stack_set_visible_child(
		GTK_STACK(windows),
		GTK_WIDGET(windowGetUrl));
	while(urlSet == WAIT) gtk_main_iteration_do(0);
	if(urlSet == ABORT) return;
	gtk_stack_set_visible_child(
		GTK_STACK(windows),
		GTK_WIDGET(windowGetAddress));
	while(addressSet == WAIT) gtk_main_iteration_do(0);
	if(addressSet == ABORT) return;
	
	flashConfirm(url);
	//if(confirmed == CONTINUE) setUdev();
	if(confirmed == CONTINUE && urlSet == CONTINUE && addressSet == CONTINUE /*&& udevSet*/)
	{
		downloadBinary();
		flash(DO_REMOVE_AFTER);
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
		filePathSet = CONTINUE;
	}
	else
	{
		printf("File chooser cancelled\n");
		filePathSet = ABORT;
		return;
	}
	
	gtk_stack_set_visible_child(
		GTK_STACK(windows),
		GTK_WIDGET(windowGetAddress));
	while(addressSet == WAIT) gtk_main_iteration_do(0);
	if(addressSet == ABORT)
	{
		printf("Address set failed\n");
		clean();
		return;
	}
		
	flashConfirm(filePath);
	//if(confirmed == CONTINUE) setUdev();
	if(confirmed == CONTINUE && urlSet == CONTINUE && addressSet == CONTINUE /*&& udevSet*/)
	{
		flash(DO_NOT_REMOVE_AFTER);
	}
	clean();
}

void _btnGetUrlCancel()
{
	gtk_stack_set_visible_child(
		GTK_STACK(windows),
		GTK_WIDGET(windowMain));
	urlSet = ABORT;
	printf("URL input cancelled\n");
}

void _btnGetUrlContinue()
{
	strcpy(url,gtk_entry_get_text(GTK_ENTRY(inpGetUrl)));
	urlSet = CONTINUE;
	printf("URL to download from: %s\n",url);
}

void _btnGetAddressCancel()
{
	gtk_stack_set_visible_child(
		GTK_STACK(windows),
		GTK_WIDGET(windowMain));
	addressSet = ABORT;
	printf("Address input cancelled\n");
}

void _btnGetAddressContinue()
{
	snprintf(address,sizeof(address),"%s%s",
		"0x",
		gtk_entry_get_text(GTK_ENTRY(inpGetAddress))
	);
	addressSet = CONTINUE;
	printf("Address to flash to: %s\n",address);
}

void _btnConfirmCancel()
{
	gtk_stack_set_visible_child(
		GTK_STACK(windows),
		GTK_WIDGET(windowMain));
	confirmed = ABORT;
	printf("Confirmation cancelled\n");
}

void _btnConfirmContinue()
{
	confirmed = CONTINUE;
	printf("Confirmation continued\n");
}

void flashConfirm(char name[])
{
	printf("entering flashConfirm\n");
	char message[1024];
	char shortname[512];
	strcpy(shortname,basename(name));
		
	snprintf(message,sizeof(message),"%s%s%s%s%s",
		"<span size=\"large\" font_weight=\"bold\">Are you sure you want to flash ",
		shortname,
		" to address ",
		address,
		"?</span>"
	);
	gtk_label_set_markup(GTK_LABEL(lblConfirm),message);
	gtk_stack_set_visible_child(
		GTK_STACK(windows),
		GTK_WIDGET(windowConfirm));
	while(confirmed == WAIT) gtk_main_iteration_do(0);
	if(confirmed == CONTINUE) printf("Confirmed\n");
	else if(confirmed == ABORT) printf("Cancelled\n");
	
	printf("leaving flashConfirm\n");
}

// Copy OpenOCD Udev rules to /etc/udev/rules.d/
// TODO: This is broken in flatpak because flatpak doesn't allow udev changes.
// As a workaround, ask the user to place 60-openocd.rules in /etc/udev/rules.d/ and then run "udevadm control --reload-rules"
// rules file web location: https://sourceforge.net/p/openocd/code/ci/master/tree/contrib/60-openocd.rules?format=raw

/*
void setUdev()
{
	// If the udev rule file already exists, skip
	if(access("/etc/udev/rules.d/60-openocd.rules",F_OK) != -1)
	{
		printf("OpenOCD udev rule already installed, skipping\n");
		udevSet = CONTINUE;
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
			udevSet = CONTINUE;
		}
		else
		{
			printf("ERROR: OpenOCD udev rule failed to install\n");
			udevSet = ABORT;
		}
	}
}
*/

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
	downloadDone = true;
	
	pthread_exit(NULL);
}

// Download a binary from the internet
void downloadBinary()
{
	gtk_stack_set_visible_child(
		GTK_STACK(windows),
		GTK_WIDGET(windowDownloading));
	
	// Create the downloading thread
	pthread_t curlThread;
	pthread_create(&curlThread,NULL,downloadBinaryThread,NULL);
	
	// Let GTK do its thing until downloading is done
	while(!downloadDone) gtk_main_iteration_do(0);
	downloadDone = false;
}

void *flashThread(void * removeAfter)
{
	// Get current working directory
	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	
	// Construct openocd command
	char command[4222];
	snprintf(command,sizeof(command),"%s%s%s%s%s%s%s%s%s",
		"openocd ",
		"-c 'set filename ",fileToFlash," ' ",
		"-c 'set address ",address," ' ",
		"-f /app/bin/swd-stlink.ocd ",
		"-f /app/bin/flash-program.ocd"
	);
	printf("%s\n",command);
	
	// Run openocd
	system(command);
	
	// Clean up leftovers
	if(removeAfter) remove(fileToFlash);
	strcpy(fileToFlash,"");
	
	// Tell the main thread that flashing is done
	flashDone = true;
	
	pthread_exit(NULL);
}

// Flash file in location fileToFlash to a given address on the Pinetime
void flash(bool removeAfter)
{
	gtk_stack_set_visible_child(
		GTK_STACK(windows),
		GTK_WIDGET(windowFlashing));
	
	pthread_t fThread;
	pthread_create(&fThread,NULL,flashThread,(void *) (size_t) removeAfter);
	
	// TODO: Get OpenOCD's output and place it on the screen
	
	// Let GTK do its thing until flashing is done
	while(!flashDone) gtk_main_iteration_do(0);
}	

int main(int argc,char *argv[])
{
	gtk_init(&argc, &argv);
	hdy_init();
	init();
	
	gtk_main();
}

