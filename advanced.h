#ifndef ADVANCED_H
#define ADVANCED_H

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
extern void setUdev();
extern void downloadBinary(char url[]);
extern void flash(char address[]);
extern int flashDialog(GObject *confirmFlashx, char confirmFlashxName[]);
extern void flashBootloader();
extern void flashInfinitime();
extern void flashWeb();
extern void flashFile();

#endif
