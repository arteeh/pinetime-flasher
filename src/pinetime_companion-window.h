#pragma once

#include <gtk/gtk.h>
#include <libhandy-1/handy.h>

G_BEGIN_DECLS

#define PINETIME_COMPANION_TYPE_WINDOW (pinetime_companion_window_get_type())

G_DECLARE_FINAL_TYPE (PinetimeCompanionWindow, pinetime_companion_window, PINETIME_COMPANION, WINDOW, HdyApplicationWindow)

G_END_DECLS
