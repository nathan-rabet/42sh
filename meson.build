project('42sh', 'c', 
    version : '0,1', 
    default_options : [
        'warning_level=3',
        'c_std=c99',
        'c_args=-D_POSIX_C_SOURCE=200809L',
        'strip=false', 
        'default_library=shared'])

cc = meson.get_compiler('c')

# Buildtype = debug
#if get_option('buildtype') == 'debug'
#   add_project_arguments('Werror', language:'c')
#endif

# instructs Meson to enter into the specified directory and execute the meson.build file in that subdirectory
subdir('src/dep')

# Tests
if get_option('enable_tests') == true
    subdir('tests')
endif

# Add main.c as an executable
sources = ['src/main.c']

#take the dep_lib from /src/dep meson.build and link it to our main sources
exe = executable('42sh', sources, link_with:dep_lib)
