# -*- Meson -*-

project('ae-whocc', ['cpp'],
        license         : 'MIT',
        version         : '1',
        default_options : [
          'warning_level=3',
          'cpp_std=c++2a', # g++-11 does not support c++20
          'buildtype=debugoptimized',
          'werror=false'
        ],
       )

if meson.get_compiler('cpp').get_id() == 'clang'
  add_project_arguments(
    '-Weverything',
    '-Wno-weak-vtables', '-Wno-padded', '-Wno-c++98-compat', '-Wno-c++98-c++11-compat', '-Wno-c++98-compat-pedantic',
    '-Wno-ambiguous-reversed-operator', # ISO C++20 considers use of overloaded operator '<=' to be ambiguous despite there being a unique best viable function
    '-Wno-zero-as-null-pointer-constant', # clang13 somtimes with operator<=>
    language : 'cpp')
  libomp_dir = ['-L/usr/local/lib'] # for -lomp, otherwise not found
else
  # add_project_arguments(
  #   '-Wno-unknown-pragmas',     # xlnt: ignoring ‘#pragma clang diagnostic’
  #   '-Wno-volatile',            # alglib: ‘++’ expression of ‘volatile’-qualified type is deprecated
  #   language : 'cpp')
  libomp_dir = []
endif

# ----------------------------------------------------------------------
# external dependencies
# ----------------------------------------------------------------------

subproject('xlnt')
xlnt = dependency('xlnt')

subproject('pybind11')
pybind11 = dependency('pybind11')

subproject('fmt')
fmt = dependency('fmt')

subproject('range_v3')
range_v3 = dependency('range_v3')

zlib = dependency('zlib', version : '>=1.2.8')
xz = dependency('liblzma')
bzip2 = meson.get_compiler('cpp').find_library('bz2', required : false)

include_cc = include_directories('./cc')

# ----------------------------------------------------------------------
# ae
# ----------------------------------------------------------------------

sources_py = [
  'cc/py/module.cc',
]

sources_ae_whocc = [
  'cc/xlsx/sheet.cc', 'cc/xlsx/sheet-extractor.cc', 'cc/xlsx/csv-parser.cc',
  'cc/utils/file.cc', 'cc/ext/date.cc',
]

# ----------------------------------------------------------------------
# python module
# https://github.com/mesonbuild/meson/issues/4677
# ----------------------------------------------------------------------

python3 = import('python').find_installation('python3')

ae_whocc_py_lib = python3.extension_module(
  'ae_whocc',
  sources : sources_py + sources_ae_whocc,
  include_directories : include_cc,
  dependencies : [dependency('python3'), xlnt, pybind11, fmt, range_v3, bzip2, zlib, xz],
  install : true)

# https://gabmus.org/posts/python-unittest-meson/
# envdata = environment()
# python_paths = [join_paths(meson.current_build_dir(), '..')]
# envdata.append('PYTHONPATH', python_paths)
# envdata.append('TESTS_BUILDDIR', meson.current_build_dir())
# test('pyb test 1', python3,
#      args : ['-V'],
#      # args: ['-m', 'unittest', 'tests'],
#      # env: envdata
#     )

# ----------------------------------------------------------------------
