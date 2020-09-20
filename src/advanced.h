#ifndef ADVANCED_H
#define ADVANCED_H

#include "companion.h"

// For uname() in getArch()
#include <sys/utsname.h>

// For downloading files from the internet
#include <curl/curl.h>

// Multithreading, for running commands AND the UI at the same time
#include <pthread.h>

extern char arch[];

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
void flashBootloader();
void flashInfinitime();
void flashWeb();
void flashFile();
int getArch();
int setUdev();
void downloadBinary(char url[]);
void flash(char address[]);
int flashConfirm(char name[],char address[]);


#endif
