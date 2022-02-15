cmake_minimum_required(VERSION 3.5)

project(LearnOpenGL VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED
    COMPONENTS Widgets Concurrent Network Sql AxContainer PrintSupport OpenGL )
add_definitions(-DQT_MESSAGELOGCONTEXT)

file(GLOB_RECURSE SOURCES "*.cpp" "*.cxx" "*.cc")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE RESOURCES "*.qrc")
file(GLOB_RECURSE FORMS *.ui)

source_group("" FILES ${SOURCES} ${HEADERS} ${RESOURCES} ${FORMS})

set(SRCS ${SOURCES} ${HEADERS} ${RESOURCES} ${FORMS})

add_executable(${PROJECT_NAME} WIN32 ${SRCS})

target_link_libraries(
    ${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Concurrent
    Qt5::Network
    Qt5::OpenGL
    Qt5::Sql
    Qt5::AxContainer
    Qt5::PrintSupport)
