pinetime_companion_sources = [
  'main.c',
  'advanced.c',
  'pinetime_companion-window.c',
]

pinetime_companion_deps = [
  dependency('gio-2.0', version: '>= 2.50'),
  dependency('gtk+-3.0', version: '>= 3.22'),
  dependency('libhandy-1'),
]

gnome = import('gnome')

pinetime_companion_sources += gnome.compile_resources('pinetime_companion-resources',
  'pinetime_companion.gresource.xml',
  c_name: 'pinetime_companion'
)

executable('pinetime-companion', pinetime_companion_sources,
  dependencies: pinetime_companion_deps,
  install: true,
)
