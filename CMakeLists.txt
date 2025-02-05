check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources
        #test/libiec61850-ns3-examples-test-suite.cc
        )
endif()

build_lib(
    LIBNAME libiec61850-ns3
    SOURCE_FILES model/libiec61850-ns3.cc
                 model/libiec61850/hal/time/ns3/time.cc

                 helper/libiec61850-ns3-helper.cc
    HEADER_FILES model/libiec61850-ns3.h
                 helper/libiec61850-ns3-helper.h

    PRIVATE_HEADER_FILES model/libiec61850/hal/inc/hal_base.h


    LIBRARIES_TO_LINK ${libcore}
    TEST_SOURCES test/libiec61850-ns3-test-suite.cc
                 ${examples_as_tests_sources}
)
