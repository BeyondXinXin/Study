if( WIN32 )
    add_definitions( -DUNICODE )
endif()

project(3rdparty)

set(3rdpartyPath
    ${PROJECT_SOURCE_DIR}/qcustomplot/src
    ${PROJECT_SOURCE_DIR}/quickevent
    )
set(3rdparty_Path ${3rdpartyPath} CACHE INTERNAL "mailiu 3rdpartyPath")

set(3rdpartyLibaries
    qcustomplot
    quickevent
    )
set(3rdparty_Libaries ${3rdpartyLibaries} CACHE PATH "mailiu 3rdpartyLibaries")

add_subdirectory(quickevent)
add_subdirectory(qcustomplot)


