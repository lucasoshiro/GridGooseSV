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

set(libiec61850_source_files
        model/libiec61850/hal/time/ns3/time.cc
        model/libiec61850/hal/thread/ns3/thread.cc
        model/libiec61850/hal/memory/lib_memory.cc
        model/libiec61850/hal/filesystem/ns3/filesystem.cc
)

set(libiec61850_header_files
        model/libiec61850/hal/inc/hal_base.h
        model/libiec61850/hal/inc/hal_time.h
        model/libiec61850/hal/inc/hal_thread.h
        model/libiec61850/hal/inc/lib_memory.h
        model/libiec61850/hal/inc/hal_filesystem.h
        model/libiec61850/hal/inc/hal_socket.h
)

set(libiec61850_test_sources
        test/model/libiec61850/hal/time/test-time.cc
        test/model/libiec61850/hal/thread/test-thread.cc
        test/model/libiec61850/hal/filesystem/test-filesystem.cc
)

set(helper_source_files
        helper/libiec61850-ns3-helper.cc
        helper/mms-server-helper.cc
)

set(helper_header_files
        helper/libiec61850-ns3-helper.h
        helper/mms-server-helper.h
)

set(model_source_files
        model/libiec61850-ns3.cc
        model/mms-server.cc
)

set(model_header_files
        model/libiec61850-ns3.h
        model/mms-server.h
)

set(util_source_files
        util/device-util.cc
        util/address-util.cc
)

set(util_header_files
        util/device-util.h
        util/address-util.h
)

set(util_test_sources
        test/util/test-device-util.cc
        test/util/test-address-util.cc
)

set(source_files
        ${helper_source_files}
        ${model_source_files}
        ${libiec61850_source_files}
        ${util_source_files}
)

set(header_files
        ${helper_header_files}
        ${model_header_files}
)

set(private_header_files
        ${libiec61850_header_files}
        ${util_header_files}
)

set(test_sources
        test/libiec61850-ns3-test-suite.cc
        ${util_test_sources}
        ${libiec61850_test_sources}
)

build_lib(
    LIBNAME libiec61850-ns3
    SOURCE_FILES ${source_files}
    HEADER_FILES ${header_files}
    PRIVATE_HEADER_FILES ${private_header_files}
    LIBRARIES_TO_LINK ${libcore}
    TEST_SOURCES ${test_sources}
                 ${examples_as_tests_sources}
)
