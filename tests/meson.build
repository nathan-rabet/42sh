# meson.build for tests
c_args += ['-DDEBUG']

# Link criterion
criterion_dep = dependency('criterion', required: true)
criterion_lib = criterion_dep.get_pkgconfig_variable('libdir')
criterion_lib = criterion_lib + '/libcriterion.so'
criterion_lib = cc.find_library('criterion', dirs: criterion_lib)
dep += [criterion_lib]

# Adding ASAN
c_args += ['-fsanitize=address']
link_args += ['-fsanitize=address']

subdir('parser')
subdir('execution')
