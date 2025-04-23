set(libraries_to_link "${libcore}" "${libapplications}" "${libcsma}")

build_lib_example(
     NAME gridgoosesv-simple_sv
     SOURCE_FILES simple_sv.cc
     LIBRARIES_TO_LINK ${libraries_to_link}
)

build_lib_example(
        NAME gridgoosesv-sv_load_test
        SOURCE_FILES sv_load_test.cc
        LIBRARIES_TO_LINK ${libraries_to_link}
)

build_lib_example(
        NAME gridgoosesv-sv_realtime
        SOURCE_FILES sv_realtime.cc
        LIBRARIES_TO_LINK ${libraries_to_link}
)

build_lib_example(
        NAME gridgoosesv-simple_goose
        SOURCE_FILES simple_goose.cc
        LIBRARIES_TO_LINK ${libraries_to_link}
)