#ifndef COMPANION_H
#define COMPANION_H

// time() for debugging
#include <sys/time.h>
// strlen()
#include <string.h>
// For system(), to run executables
#include <unistd.h>
// For basename() in downloadBinary()
#include <libgen.h>
// GTK library
#include <gtk/gtk.h>
// Libhandy
#include <handy.h>

#include "updates.h"
#include "advanced.h"

extern GtkBuilder *builder;
extern GObject *window;
extern GObject *btnAbout;
extern GObject *windowAbout;

int main(int argc,char *argv[]);
void _btnAbout();

#endif
