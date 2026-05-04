cmake_minimum_required(VERSION 3.16)

project(TestSources)

set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../ZMachineCoreTests/testdata")

file(GLOB INFORM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.inf")

set(COMPILED_STORIES "")
foreach(INF_FILE ${INFORM_SOURCES})
    get_filename_component(FILENAME_NO_EXT ${INF_FILE} NAME_WE)
    SET(ZMACHINE_VERSION "5")
    set(OUT_FILE "${FILENAME_NO_EXT}.z${ZMACHINE_VERSION}")
    add_custom_command(
        OUTPUT ${OUT_FILE}
        COMMAND inform -v${ZMACHINE_VERSION} ${INF_FILE}
        DEPENDS ${INF_FILE}
        COMMENT "Compiling ${INF_FILE} -> ${OUT_FILE}"
        VERBATIM
    )
    make_directory(${OUTPUT_DIR})
    configure_file(${INF_FILE} "${OUTPUT_DIR}/${OUT_FILE}" COPYONLY)
    list(APPEND COMPILED_STORIES ${OUT_FILE})
endforeach()

add_custom_target(TestSources ALL DEPENDS ${COMPILED_STORIES})