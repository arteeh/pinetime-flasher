import main
import setup
import sys
import gi

gi.require_version('Gtk', '4.0')
gi.require_version('Adw', '1')

from gi.repository import Gtk, Adw

class OptionsScreen(Gtk.Grid):
	def __init__(self, *args, **kwargs):
		super().__init__(*args, **kwargs)
		# halign?
		# valign?
		self.props.orientation	= Gtk.Orientation(1) # vertical