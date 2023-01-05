project('42sh', 'c',
    version : '0,1',
    default_options : [
        'warning_level=3',
        'c_std=c99',
        'c_args=-D_POSIX_C_SOURCE=200809L',
    ])

# Add the dependency subdirectory as a subproject
dep_dir = subdir('src/dep')
dep = dep_dir.static_library()

sources = ['src/main.c']


executable('42sh', sources, dependencies: dep)