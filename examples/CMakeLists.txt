build_lib_example(
     NAME gridgoosesv-simple_sv
     SOURCE_FILES simple_sv.cc
     LIBRARIES_TO_LINK "${libcore}" "${libapplications}" "${libcsma}"
)