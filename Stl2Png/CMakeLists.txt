macro(GENERATE_PROGECT PROGECT_NAME)
    project(${PROGECT_NAME})
    file(GLOB_RECURSE SOURCES "*.cpp" "*.cxx" "*.cc")
    file(GLOB_RECURSE HEADERS "*.h")
    file(GLOB_RECURSE FORMS *.ui)
    source_group("" FILES ${HEADERS} ${SOURCES})
    set(${PROGECT_NAME}_SRCS ${SOURCES} ${HEADERS} ${FORMS})
    set(${PROGECT_NAME}_SOURCE_DIR ${PROJECT_SOURCE_DIR}
        CACHE INTERNAL "${PROGECT_NAME}_SOURCE_DIR")
endmacro()

macro(ST_LINK PROGECT_NAME LIBRARIES_NAME)
    include_directories(${${LIBRARIES_NAME}_SOURCE_DIR})
    target_link_libraries(${PROGECT_NAME} ${LIBRARIES_NAME})
endmacro()

GENERATE_PROGECT(STL2PNG_APP)
add_executable(${PROJECT_NAME} WIN32 ${STL2PNG_APP_SRCS})

target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})






