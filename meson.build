project('42sh', 'c',
    version : '0,1', 
    default_options : [
        'warning_level=3',
        'c_std=c99',
        'strip=false', 
        'default_library=shared',
        ])

cc = meson.get_compiler('c')
c_args = ['-std=c99', '-D_POSIX_C_SOURCE=200809L', '-pedantic', '-Werror', '-Wall', '-Wextra', '-Wvla']

# Debug with -Werror flag (usage : "meson setup buildir --buildtype=debug")
if get_option('buildtype') == 'debug'
    c_args += ['-g', '-O0']
    c_args += ['-D DEBUG']
endif

dep = []
link_args = []
c_args += ['-fsanitize=address']
link_args += ['-fsanitize=address']
sources = []
subdir('src')
inc = include_directories('include')



if get_option('enable_tests') == true
    # Tests main
    subdir('tests')

    executable('42sh_tests_main', sources + ['tests/main.c'], include_directories: inc, c_args: c_args, dependencies: dep, link_args: link_args)

    # Tests
    executable('42sh_tests', sources, include_directories: inc, c_args: c_args, dependencies: dep, link_args: link_args)
else
   # 42sh
   executable('42sh', sources + ['src/main.c'], include_directories: inc, c_args: c_args, dependencies: dep, link_args: link_args)


endif
