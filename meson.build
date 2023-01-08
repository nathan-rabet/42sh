project('42sh', 'c', 
    version : '0,1', 
    default_options : [
        'warning_level=3',
        'c_std=c99',
        'strip=false', 
        'default_library=shared',
        ])

cc = meson.get_compiler('c')

# Debug with -Werror flag (usage : "meson setup buildir --buildtype=debug")
if get_option('buildtype') == 'debug'
    c_args = ['-std=c99', '-D_POSIX_C_SOURCE=200809L', '-pedantic', '-Werror', '-Wall', '-Wextra', '-Wvla']
else
    c_args = ['-std=c99', '-D_POSIX_C_SOURCE=200809L', '-pedantic', '-Wall', '-Wextra', '-Wvla']
endif

dep = []
sources = ['src/utils/xalloc.c']
inc = include_directories('include')
program = ''

# Tests
if get_option('enable_tests') == true
    program = '42sh_tests'
    subdir('tests')
else
    sources += ['src/main.c']
    program = '42sh'
endif
    exe = executable(program, sources, include_directories: inc, c_args: c_args, dependencies: dep)

