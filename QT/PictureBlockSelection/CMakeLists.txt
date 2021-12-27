GENERATE_PROGECT(PictureBlockSelection)

add_executable(${PROJECT_NAME} WIN32 ${PictureBlockSelection_SRCS})

target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Widgets)
