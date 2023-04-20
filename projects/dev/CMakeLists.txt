cmake_minimum_required( VERSION 3.5 )
project(
    dev
    DESCRIPTION "Development project sandbox"
    VERSION 0.0.1
    LANGUAGES C ASM
    )

option(USE_GPIO "Enable GPIO Library" ON)

set( SOURCE
    ${SOURCE}
    src/project.c
    )

set( HEADERS
    ${HEADERS}
    include/project/project.h
    )

add_library(
    ${PROJECT_NAME}
    STATIC
        ${SOURCE}
        ${HEADERS}
    )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        core
    )

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# install(TARGETS ${PROJECT_NAME}
#     LIBRARY DESTINATION ${CONTROL_LIB_DIR}
#     ARCHIVE DESTINATION ${CONTROL_LIB_DIR}
#     )
