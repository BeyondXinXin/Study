project(Example1)

if( WIN32 )
    add_definitions( -DUNICODE )
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

include_directories(${3rdparty_Path})

file(GLOB_RECURSE SOURCES "*.cpp" "*.cxx")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE RESOURCES "*.qrc")
file(GLOB_RECURSE FORMS *.ui)

message(--------------------------------)
message(${Resource_Path})
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
    ${FORMS}
    )

set_target_properties(${PROJECT_NAME}
    PROPERTIES AUTOMOC TRUE
    )
target_link_libraries(${PROJECT_NAME}
    Qt5::Core Qt5::Gui Qt5::Widgets
    ${3rdparty_Libaries}
    )
