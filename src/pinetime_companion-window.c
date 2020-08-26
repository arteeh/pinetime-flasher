#include "pinetime_companion-config.h"
#include "pinetime_companion-window.h"

struct _PinetimeCompanionWindow
{
  HdyApplicationWindow  parent_instance;
};

G_DEFINE_TYPE (PinetimeCompanionWindow, pinetime_companion_window, HDY_TYPE_APPLICATION_WINDOW)

static void
pinetime_companion_window_class_init (PinetimeCompanionWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/com/arteeh/PineTime-Companion/pinetime_companion-window.ui");
}

static void
pinetime_companion_window_init (PinetimeCompanionWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}
