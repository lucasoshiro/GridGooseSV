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

########################### LIBIEC61850 SOURCE FILES ###########################

set(libiec61850_hal_source_files
        model/libiec61850/hal/time/ns3/time.cc
        model/libiec61850/hal/thread/ns3/thread.cc
        model/libiec61850/hal/memory/lib_memory.cc
        model/libiec61850/hal/filesystem/ns3/filesystem.cc
        model/libiec61850/hal/ethernet/ns3/ethernet.cc
)

set(libiec61850_hal_header_files
        model/libiec61850/hal/inc/hal_base.h
        model/libiec61850/hal/inc/hal_time.h
        model/libiec61850/hal/inc/hal_thread.h
        model/libiec61850/hal/inc/lib_memory.h
        model/libiec61850/hal/inc/hal_filesystem.h
        model/libiec61850/hal/inc/hal_socket.h
        model/libiec61850/hal/inc/hal_ethernet.h
        model/libiec61850/hal/inc/platform_endian.h
)

# TODO: maybe we don't need all of them.
set(libiec61850_common_header_files
        model/libiec61850/src/common/inc/buffer_chain.h
        model/libiec61850/src/common/inc/byte_buffer.h
        model/libiec61850/src/common/inc/conversions.h
        model/libiec61850/src/common/inc/libiec61850_common_api.h
        model/libiec61850/src/common/inc/libiec61850_platform_includes.h
        model/libiec61850/src/common/inc/linked_list.h
        model/libiec61850/src/common/inc/map.h
        model/libiec61850/src/common/inc/mem_alloc_linked_list.h
        model/libiec61850/src/common/inc/simple_allocator.h
        model/libiec61850/src/common/inc/string_map.h
        model/libiec61850/src/common/inc/string_utilities.h
)

set(libiec61850_mms_header_files
        model/libiec61850/src/mms/inc/iso_connection_parameters.h
        model/libiec61850/src/mms/inc/mms_client_connection.h
        model/libiec61850/src/mms/inc/mms_common.h
        model/libiec61850/src/mms/inc/mms_server.h
        model/libiec61850/src/mms/inc/mms_type_spec.h
        model/libiec61850/src/mms/inc/mms_types.h
        model/libiec61850/src/mms/inc/mms_value.h
)

set(libiec61850_asn1_source_files
        model/libiec61850/src/mms/iso_mms/asn1c/AccessResult.cc
        model/libiec61850/src/mms/iso_mms/asn1c/Address.cc
        model/libiec61850/src/mms/iso_mms/asn1c/AlternateAccess.cc
        model/libiec61850/src/mms/iso_mms/asn1c/AlternateAccessSelection.cc
        model/libiec61850/src/mms/iso_mms/asn1c/BIT_STRING.cc
        model/libiec61850/src/mms/iso_mms/asn1c/BOOLEAN.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ConcludeRequestPDU.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ConcludeResponsePDU.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ConfirmedErrorPDU.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ConfirmedRequestPdu.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ConfirmedResponsePdu.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ConfirmedServiceRequest.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ConfirmedServiceResponse.cc
        model/libiec61850/src/mms/iso_mms/asn1c/Data.cc
        model/libiec61850/src/mms/iso_mms/asn1c/DataAccessError.cc
        model/libiec61850/src/mms/iso_mms/asn1c/DataSequence.cc
        model/libiec61850/src/mms/iso_mms/asn1c/DefineNamedVariableListRequest.cc
        model/libiec61850/src/mms/iso_mms/asn1c/DefineNamedVariableListResponse.cc
        model/libiec61850/src/mms/iso_mms/asn1c/DeleteNamedVariableListRequest.cc
        model/libiec61850/src/mms/iso_mms/asn1c/DeleteNamedVariableListResponse.cc
        model/libiec61850/src/mms/iso_mms/asn1c/FloatingPoint.cc
        model/libiec61850/src/mms/iso_mms/asn1c/GeneralizedTime.cc
        model/libiec61850/src/mms/iso_mms/asn1c/GetNameListRequest.cc
        model/libiec61850/src/mms/iso_mms/asn1c/GetNameListResponse.cc
        model/libiec61850/src/mms/iso_mms/asn1c/GetNamedVariableListAttributesRequest.cc
        model/libiec61850/src/mms/iso_mms/asn1c/GetNamedVariableListAttributesResponse.cc
        model/libiec61850/src/mms/iso_mms/asn1c/GetVariableAccessAttributesRequest.cc
        model/libiec61850/src/mms/iso_mms/asn1c/GetVariableAccessAttributesResponse.cc
        model/libiec61850/src/mms/iso_mms/asn1c/INTEGER.cc
        model/libiec61850/src/mms/iso_mms/asn1c/Identifier.cc
        model/libiec61850/src/mms/iso_mms/asn1c/IndexRangeSeq.cc
        model/libiec61850/src/mms/iso_mms/asn1c/InformationReport.cc
        model/libiec61850/src/mms/iso_mms/asn1c/InitRequestDetail.cc
        model/libiec61850/src/mms/iso_mms/asn1c/InitResponseDetail.cc
        model/libiec61850/src/mms/iso_mms/asn1c/InitiateErrorPdu.cc
        model/libiec61850/src/mms/iso_mms/asn1c/InitiateRequestPdu.cc
        model/libiec61850/src/mms/iso_mms/asn1c/InitiateResponsePdu.cc
        model/libiec61850/src/mms/iso_mms/asn1c/Integer16.cc
        model/libiec61850/src/mms/iso_mms/asn1c/Integer32.cc
        model/libiec61850/src/mms/iso_mms/asn1c/Integer8.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ListOfVariableSeq.cc
        model/libiec61850/src/mms/iso_mms/asn1c/MMSString.cc
        model/libiec61850/src/mms/iso_mms/asn1c/MmsPdu.cc
        model/libiec61850/src/mms/iso_mms/asn1c/NULL.cc
        model/libiec61850/src/mms/iso_mms/asn1c/NativeEnumerated.cc
        model/libiec61850/src/mms/iso_mms/asn1c/NativeInteger.cc
        model/libiec61850/src/mms/iso_mms/asn1c/OCTET_STRING.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ObjectClass.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ObjectName.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ParameterSupportOptions.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ReadRequest.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ReadResponse.cc
        model/libiec61850/src/mms/iso_mms/asn1c/RejectPDU.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ScatteredAccessDescription.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ServiceError.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ServiceSupportOptions.cc
        model/libiec61850/src/mms/iso_mms/asn1c/StructComponent.cc
        model/libiec61850/src/mms/iso_mms/asn1c/TimeOfDay.cc
        model/libiec61850/src/mms/iso_mms/asn1c/TypeSpecification.cc
        model/libiec61850/src/mms/iso_mms/asn1c/UTF8String.cc
        model/libiec61850/src/mms/iso_mms/asn1c/UnconfirmedPDU.cc
        model/libiec61850/src/mms/iso_mms/asn1c/UnconfirmedService.cc
        model/libiec61850/src/mms/iso_mms/asn1c/Unsigned16.cc
        model/libiec61850/src/mms/iso_mms/asn1c/Unsigned32.cc
        model/libiec61850/src/mms/iso_mms/asn1c/Unsigned8.cc
        model/libiec61850/src/mms/iso_mms/asn1c/UtcTime.cc
        model/libiec61850/src/mms/iso_mms/asn1c/VariableAccessSpecification.cc
        model/libiec61850/src/mms/iso_mms/asn1c/VariableSpecification.cc
        model/libiec61850/src/mms/iso_mms/asn1c/VisibleString.cc
        model/libiec61850/src/mms/iso_mms/asn1c/WriteRequest.cc
        model/libiec61850/src/mms/iso_mms/asn1c/WriteResponse.cc
        model/libiec61850/src/mms/iso_mms/asn1c/asn_SEQUENCE_OF.cc
        model/libiec61850/src/mms/iso_mms/asn1c/asn_SET_OF.cc
        model/libiec61850/src/mms/iso_mms/asn1c/asn_codecs_prim.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ber_decoder.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ber_tlv_length.cc
        model/libiec61850/src/mms/iso_mms/asn1c/ber_tlv_tag.cc
        model/libiec61850/src/mms/iso_mms/asn1c/constr_CHOICE.cc
        model/libiec61850/src/mms/iso_mms/asn1c/constr_SEQUENCE.cc
        model/libiec61850/src/mms/iso_mms/asn1c/constr_SEQUENCE_OF.cc
        model/libiec61850/src/mms/iso_mms/asn1c/constr_SET_OF.cc
        model/libiec61850/src/mms/iso_mms/asn1c/constr_TYPE.cc
        model/libiec61850/src/mms/iso_mms/asn1c/constraints.cc
        model/libiec61850/src/mms/iso_mms/asn1c/der_encoder.cc
        model/libiec61850/src/mms/iso_mms/asn1c/per_decoder.cc
        model/libiec61850/src/mms/iso_mms/asn1c/per_encoder.cc
        model/libiec61850/src/mms/iso_mms/asn1c/per_support.cc
        model/libiec61850/src/mms/iso_mms/asn1c/xer_decoder.cc
        model/libiec61850/src/mms/iso_mms/asn1c/xer_encoder.cc
        model/libiec61850/src/mms/iso_mms/asn1c/xer_support.cc
)

set(libiec61850_source_files
        ${libiec61850_hal_source_files}
        ${libiec61850_asn1_source_files}
)

set(libiec61850_header_files
        model/libiec61850/config/stack_config.h
        ${libiec61850_common_header_files}
        ${libiec61850_hal_header_files}
        ${libiec61850_mms_header_files}
)

set(libiec61850_test_sources
        test/model/libiec61850/hal/time/test-time.cc
        test/model/libiec61850/hal/thread/test-thread.cc
        test/model/libiec61850/hal/filesystem/test-filesystem.cc
)

################################# MODULE FILES #################################

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

############################# BUILD_LIB PARAMETERS #############################

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

set (CMAKE_REQUIRED_FLAGS_DEFAULT ${CMAKE_REQUIRED_FLAGS})
set (CMAKE_REQUIRED_FLAGS -Wno-write-strings)
set (CXXFLAGS_EXTRA -Wno-write-strings)

build_lib(
    LIBNAME libiec61850-ns3
    SOURCE_FILES ${source_files}
    HEADER_FILES ${header_files}
    PRIVATE_HEADER_FILES ${private_header_files}
    LIBRARIES_TO_LINK ${libcore}
    TEST_SOURCES ${test_sources}
                 ${examples_as_tests_sources}
)

set (CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS_DEFAULT})
