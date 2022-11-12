# https://github.com/Taiko2k/GTK4PythonTutorial/blob/main/README.md
# https://amolenaar.github.io/pgi-docgen/
# UI porting progress: https://github.com/arteeh/pinetime-flasher/blob/master/src/pinetime-flasher.ui#L259

import setup
import options
import sys
import gi

gi.require_version('Gtk', '4.0')
gi.require_version('Adw', '1')

from gi.repository import Gtk, Adw

class MainApp(Adw.Application):

	def __init__(self, **kwargs):
		super().__init__(**kwargs)
		self.props.application_id		= "com.arteeh.Flasher"
		self.connect('activate', self.on_activate)

	def on_activate(self, app):
		self.mainWindow					= MainWindow()
		self.mainWindow.present()

class MainWindow(Gtk.ApplicationWindow):

	def __init__(self, *args, **kwargs):
		super().__init__(*args, **kwargs)
		self.props.application			= app
		self.props.title				= "Pinetime Flasher"
		self.props.default_height		= 400
		self.props.default_width		= 400
		self.mainStack					= MainStack()
		self.set_child(self.mainStack)

class MainStack(Gtk.Stack):

	def __init__(self, *args, **kwargs):
		super().__init__(*args, **kwargs)
		self.props.transition_type		= Gtk.StackTransitionType(6) # slide_left_right
		self.setupScreen				= setup.SetupScreen()
		self.optionsScreen				= options.OptionsScreen()
		self.add_named(self.setupScreen, 'setupScreen')
		self.add_named(self.optionsScreen, 'optionsScreen')

app = MainApp()
app.run(sys.argv)