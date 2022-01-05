if( WIN32 )
    set(quickevent_BUILD_TYPE STATIC)
    add_definitions("-DUSE_STATIC")
endif()

project(quickevent VERSION "3.0.1")

configure_file(
    "${quickevent_SOURCE_DIR}/QuickEventConfig.h.in"
    "${quickevent_SOURCE_DIR}/quickconfig.h"
    )

add_library(quickevent STATIC
    quick.h
    quickcontroller.h
    quickapplication.h
    quickevent.h
    quickwork.h
    quickconfig.h
    quickcontroller.cpp
    quickapplication.cpp
    quickevent.cpp
    quickwork.cpp
)

set_target_properties(quickevent PROPERTIES AUTOMOC TRUE )
target_link_libraries(quickevent PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
