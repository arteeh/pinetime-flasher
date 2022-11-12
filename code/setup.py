import main
import options
import sys
import gi

gi.require_version('Gtk', '4.0')
gi.require_version('Adw', '1')

from gi.repository import Gtk, Adw

class SetupScreen(Gtk.Grid):
	def __init__(self, *args, **kwargs):
		super().__init__(*args, **kwargs)
		self.setupLabel1			= SetupLabel1()
		self.setupLabel2			= SetupLabel2()
		self.setupLabel3			= SetupLabel3()
		self.setupLabel4			= SetupLabel4()
		self.box					= SetupBox()
		self.attach(self.setupLabel1, 0, 0, 1, 1)
		self.attach(self.setupLabel2, 0, 1, 1, 1)
		self.attach(self.setupLabel3, 0, 2, 1, 1)
		self.attach(self.setupLabel4, 0, 3, 1, 1)
		self.attach(self.box, 0, 4, 1, 1)

class SetupLabelCommon(Gtk.Label):
	def __init__(self, *args, **kwargs):
		super().__init__(*args, **kwargs)
		self.props.wrap				= True
		self.props.use_markup		= True
		self.props.margin_start		= 5
		self.props.margin_end		= 5
		self.props.margin_top		= 5
		self.props.margin_bottom	= 5
		self.props.valign			= Gtk.Align(1) # start

class SetupLabel1(SetupLabelCommon):
	def __init__(self, *args, **kwargs):
		super().__init__(*args, **kwargs)
		self.props.vexpand			= True
		self.props.valign			= Gtk.Align(2) # end
		self.set_markup("""&lt;span size="large" font_weight="bold"&gt;Before this app can access your ST-Link adapter, a Udev rule should be installed on this device.&lt;/span&gt; Press the Download button to download this rule. Then, place this file in the following folder:""")

class SetupLabel2(SetupLabelCommon):
	def __init__(self, *args, **kwargs):
		super().__init__(*args, **kwargs)
		self.set_markup("&lt;tt&gt;/etc/udev/rules.d/&lt;/tt&gt;")

class SetupLabel3(SetupLabelCommon):
	def __init__(self, *args, **kwargs):
		super().__init__(*args, **kwargs)
		self.set_markup("After that, you should run the following command in your terminal:")

class SetupLabel4(SetupLabelCommon):
	def __init__(self, *args, **kwargs):
		super().__init__(*args, **kwargs)
		self.set_markup("&lt;tt&gt;udevadm control --reload-rules&lt;/tt&gt;")

class SetupBox(Gtk.Box):
	def __init__(self, *args, **kwargs):
		super().__init__(*args, **kwargs)
		self.props.margin_start		= 5
		self.props.margin_end		= 5
		self.props.margin_top		= 10
		self.props.margin_bottom	= 10
		self.props.hexpand			= True
		self.props.vexpand			= True
		#self.props.layout_style = Gtk.ButtonBoxStyle(1) # spread
		self.setupButtonDownload	= SetupButtonDownload()
		self.append(self.setupButtonDownload)

class SetupButtonDownload(Gtk.Button):
	def __init__(self, *args, **kwargs):
		super().__init__(*args, **kwargs)
		self.props.label			= "Download"
		self.props.receives_default	= True
		self.connect('clicked', self.OnClicked)
	
	def OnClicked():
		yes = True

class SetupButtonContinue(Gtk.Button):
	def __init__(self, *args, **kwargs):
		super().__init__(*args, **kwargs)
		self.props.label			= "Continue"
		# TODO: suggested-action button CSS
	
	def OnClicked():
		yes = true