project('pinetime-companion', 'c',
          version: '0.1.0',
    meson_version: '>= 0.50.0',
  default_options: [ 'warning_level=2',
                     'c_std=gnu11',
                   ],
)

libhandy_dep = dependency('libhandy-1', version: '>= 0.91.0', required: false)
if not libhandy_dep.found()
  libhandy = subproject(
    'libhandy',
    install: false,
    default_options: [
      'examples=false',
      'package_subdir=my-project-name',
      'tests=false',
    ]
  )
  libhandy_dep = libhandy.get_variable('libhandy_dep')
endif

i18n = import('i18n')

config_h = configuration_data()
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())
config_h.set_quoted('GETTEXT_PACKAGE', 'pinetime-companion')
config_h.set_quoted('LOCALEDIR', join_paths(get_option('prefix'), get_option('localedir')))
configure_file(
  output: 'pinetime_companion-config.h',
  configuration: config_h,
)
add_project_arguments([
  '-I' + meson.build_root(),
], language: 'c')

subdir('data')
subdir('src')
subdir('po')

meson.add_install_script('build-aux/meson/postinstall.py')
