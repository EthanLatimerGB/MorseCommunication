morse_desktop_sources = [
  'main.c',
  'morse-desktop-application.c',
  'morse-desktop-window.c',
]

morse_desktop_deps = [
  dependency('gtk4'),
  dependency('libadwaita-1', version: '>= 1.2'),
]

morse_desktop_sources += gnome.compile_resources('morse-desktop-resources',
  'morse-desktop.gresource.xml',
  c_name: 'morse_desktop'
)

executable('morse-desktop', morse_desktop_sources,
  dependencies: morse_desktop_deps,
       install: true,
)
