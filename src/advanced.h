#ifndef ADVANCED_H
#define ADVANCED_H

#include "companion.h"

// String to number
#include <stdlib.h>

// For uname() in getArch()
#include <sys/utsname.h>

// For downloading files from the internet
#include <curl/curl.h>

// Multithreading, for running commands AND the UI at the same time
#include <pthread.h>

extern char arch[];
extern char fileToFlash[];

// Windows
extern GObject *advancedWindows;
extern GObject *advancedWindowMain;
extern GObject *advancedWindowGetUrl;
extern GObject *advancedWindowGetAddress;
extern GObject *advancedWindowConfirm;
extern GObject *advancedWindowDownloading;
extern GObject *advancedWindowFlashing;

// advancedWindowMain
extern GObject *btnFlashBootloader;
extern GObject *btnFlashInfinitime;
extern GObject *btnFlashWeb;
extern GObject *btnFlashFile;

// advancedWindowGetUrl
extern GObject *inpGetUrl;
extern GObject *btnGetUrlCancel;
extern GObject *btnGetUrlContinue;

// advancedWindowGetAddress
extern GObject *inpGetAddress;
extern GObject *btnGetAddressCancel;
extern GObject *btnGetAddressContinue;

// advancedWindowConfirm
extern GObject *lblConfirm;
extern GObject *btnConfirmCancel;
extern GObject *btnConfirmContinue;

// Flash native file chooser dialog
extern GObject *getFileChooser;

// Function declarations
void initAdvanced();
void clean();
void setArch();
void _btnFlashBootloader();
void _btnFlashInfinitime();
void _btnFlashWeb();
void _btnFlashFile();
void _btnGetUrlCancel();
void _btnGetUrlContinue();
void _btnGetAddressCancel();
void _btnGetAddressContinue();
void _btnConfirmCancel();
void _btnConfirmContinue();
void flashConfirm(char name[]);
void setUdev();
void *downloadBinaryThread();
void downloadBinary();
void *flashThread(void * removeAfter);
void flash(int removeAfter);

#endif
