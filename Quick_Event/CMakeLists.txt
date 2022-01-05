cmake_minimum_required(VERSION 3.1.0)
project(QuickEvent)

# Compile the project
set(Example1  ON)# 完整功能
set(Example2  ON)# 自动实例化 排序反射展示
set(Example3  ON)# 自动实例化 线程归属展示
set(Example4  ON)# 发布订阅 跨线程发布订阅展示
set(Example5  ON)# 发布订阅 多种发布展示（同步、异步、异步等待、自动）

# Set self path
set(yaoxin ON)
if(yaoxin)
    if(WIN32)
        set(VTK_DIR  "D:/lib/VTK-8.2.0/bulid")
        set(ITK_DIR  "D:/lib/InsightToolkit-4.13.2/bulid")
        set(OpenCV_DIR  "D:/lib/opencv-3.4.9/bulid")
        set(CGAL_DIR  "D:/lib/CGAL/bulid")
    endif()
endif()

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG
    ${PROJECT_SOURCE_DIR}/bin/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG
    ${PROJECT_SOURCE_DIR}/bin/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG
    ${PROJECT_SOURCE_DIR}/bin/Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE
    ${PROJECT_SOURCE_DIR}/bin/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE
    ${PROJECT_SOURCE_DIR}/bin/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE
    ${PROJECT_SOURCE_DIR}/bin/Release)


# Link to external third-party libraries
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(BUILD_SHARED_LIBS "" ON)
find_package(Qt5
    REQUIRED
    COMPONENTS
    Core
    Gui
    Widgets
    Sql
    Test
    Xml
    Network
    Concurrent
    )
find_package(Qt5LinguistTools)
add_definitions(-DQT_MESSAGELOGCONTEXT)

# Link to External third-party libraries
set(Torch  OFF)
set(OpenCV  OFF)
set(VTK812  OFF)
set(VTK820  OFF)
set(VTK900  OFF)
set(ITK  OFF)
set(Armadillo  OFF)

if(Torch)
    find_package(Torch REQUIRED)
    include_directories(${TORCH_INCLUDE_DIRS})
endif()
if(OpenCV)
    find_package(OpenCV REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()
if(VTK820)
    find_package(VTK 8.2.0 REQUIRED)
    include(${VTK_USE_FILE})
endif()
if(VTK812)
    find_package(VTK 8.1.2 REQUIRED)
    include(${VTK_USE_FILE})
endif()
if(VTK900)
    find_package(VTK 9.0.0 REQUIRED)
    include(${VTK_USE_FILE})
endif()
if(ITK)
    find_package(ITK REQUIRED)
    include(${ITK_USE_FILE})
endif()
if(Armadillo)
    find_package(Armadillo REQUIRED)
    include_directories(${ARMADILLO_INCLUDE_DIRS})
endif()

# Link to Internal third-party libraries
add_subdirectory(3rdparty)

# Set up the compiled project
set(Resource
    ${PROJECT_SOURCE_DIR}/Examples/resource/resource.qrc
    ${PROJECT_SOURCE_DIR}/Examples/resource/style.qrc)
set(Resource_Path ${Resource} CACHE PATH "mailiu Resource")

# Set up the compiled project
if(Example1)
    add_subdirectory(Examples/Example1)
endif()
if(Example2)
    add_subdirectory(Examples/Example2)
endif()
if(Example3)
    add_subdirectory(Examples/Example3)
endif()
if(Example4)
    add_subdirectory(Examples/Example4)
endif()
if(Example5)
    add_subdirectory(Examples/Example5)
endif()




# Clear
add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    )

