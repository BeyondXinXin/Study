GENERATE_PROGECT(InteractSeparately)

add_executable(${PROJECT_NAME} WIN32 ${InteractSeparately_SRCS})

target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})






