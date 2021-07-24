GENERATE_PROGECT(PedicleScrewSimulation)

add_executable(${PROJECT_NAME} WIN32 ${PedicleScrewSimulation_SRCS})

target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})


vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
    )



