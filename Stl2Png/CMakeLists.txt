GENERATE_PROGECT(Stl2Png)

add_executable(${PROJECT_NAME} WIN32 ${Stl2Png_SRCS})

target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})

