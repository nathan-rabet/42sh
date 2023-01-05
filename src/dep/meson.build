# meson build to produce the library with all the src files on this directory
dep_src = files([
    'dep.c',
    'dep.h',
])

dep_inc = include_directories('.')

dep_lib = static_library('dep', dep_src)

list_dep = declare_dependency(
    link_with : dep_lib,
    include_directories : dep_inc,
)