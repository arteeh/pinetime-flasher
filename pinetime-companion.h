#ifndef PINETIME_COMPANION_H
#define PINETIME_COMPANION_H

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

#include "advanced.h"

extern GtkBuilder *builder;
extern GObject *window;

#endif
