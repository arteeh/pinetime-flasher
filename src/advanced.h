#ifndef ADVANCED_H
#define ADVANCED_H

#include "pinetime-companion.h"

// For uname() in getArch()
#include <sys/utsname.h>

// For downloading files from the internet
//#include <curl/curl.h>

extern char arch[];

extern char bootloaderUrl[];
extern char infinitimeUrl[];

extern char fileToFlash[];

// Flash file chooser dialog
extern GObject *flashFileChooser;
extern GObject *btnFlashFileChooserSelect;

// Buttons
extern GObject *btnFlashBootloader;
extern GObject *btnFlashInfinitime;
extern GObject *btnFlashWeb;
extern GObject *btnFlashFile;

// Confirmation message dialogs
extern GObject *confirmFlashBootloader;
extern GObject *confirmFlashInfinitime;
extern GObject *confirmFlashGeneric;

// Function declarations
void initAdvanced();
int getArch();
void setUdev();
void downloadBinary(char url[]);
void flash(char address[]);
int flashDialog(GObject *confirmFlashx, char confirmFlashxName[]);
void flashBootloader();
void flashInfinitime();
void flashWeb();
void flashFile();

#endif
