#pragma once

// printf()
#include <stdio.h>
// DIR
#include <dirent.h>
// strlen()
#include <string.h>
// system(), to run executables
#include <unistd.h>
// basename() in downloadBinary()
#include <libgen.h>
// ??? Builder put this here
#include <glib/gi18n.h>
// GTK
#include <gtk/gtk.h>
// Handy
#include <libhandy-1/handy.h>

extern GtkBuilder *builder;
extern GError *error;

// Main window
extern GObject *window;
