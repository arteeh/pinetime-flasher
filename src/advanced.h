#pragma once

#include "pinetime_companion-window.h"

extern const char arch[];

extern char bootloaderUrl[];
extern char infinitimeUrl[];

char fileToFlash[4096];

// Init objects to load builder objects into
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

void setUdev();
void downloadBinary(char url[]);
void flash(char address[]);
int flashDialog(GObject *confirmFlashx, char confirmFlashxName[]);
void flashBootloader();
void flashInfinitime();
void flashWeb();
void flashFile();

