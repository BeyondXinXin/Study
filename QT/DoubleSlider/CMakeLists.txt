GENERATE_PROGECT(DoubleSlider)

add_executable(${PROJECT_NAME} WIN32 ${DoubleSlider_SRCS})

target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Widgets)
