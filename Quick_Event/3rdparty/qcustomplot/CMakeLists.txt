if( WIN32 )
    add_definitions( -DUNICODE )
endif()

add_library( qcustomplot STATIC
    src/qcustomplot.cpp
    src/qcustomplot.h)

set_target_properties( qcustomplot PROPERTIES AUTOMOC TRUE )

target_link_libraries( qcustomplot Qt5::Widgets Qt5::Network )
