# CMakeLists.txt
cmake_minimum_required(VERSION 3.21)

project(QtQuickHotReloadDemo
    VERSION 1.0.0
    DESCRIPTION "Qt Quick QML Hot Reload Demo"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ===== 关键：设置QML输出目录 =====
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)

find_package(Qt6 6.5 COMPONENTS
    Core
    Gui
    Qml
    Quick
    REQUIRED
)

# ===== C++源文件 =====
set(PROJECT_SOURCES
    src/main.cpp
    src/application.h
    src/application.cpp
    src/data_model.h
    src/data_model.cpp
    src/qml_file_watcher.h
    src/qml_file_watcher.cpp
    src/qml_dynamic_loader.h
    src/qml_dynamic_loader.cpp
    src/qml_loader_registry.h
    src/qml_loader_registry.cpp
    src/qml_hot_reload_system.h
    src/qml_hot_reload_system.cpp
)

# ===== QML文件列表 =====
set(PROJECT_QML_FILES
    qml/main.qml
    qml/ContentView.qml
)

# ===== 创建可执行程序 =====
qt_add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

# ===== 添加QML模块（关键！） =====
qt_add_qml_module(${PROJECT_NAME}
    URI com.example.models              # QML模块URI
    VERSION 1.0                         # 版本
    # QML_FILES ${PROJECT_QML_FILES}      # QML文件
    SOURCES
        ${PROJECT_SOURCES}              # C++源文件
)

# ===== 复制QML文件到输出目录（运行时使用）=====
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/qml"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/qml"
    COMMENT "Copying QML files to output directory"
)

# ===== 链接库 =====
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ===== QML源目录定义 =====
set(QML_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/qml")

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        "QML_SOURCE_DIR=\"${QML_SOURCE_DIR}\""
        "PROJECT_VERSION=\"${PROJECT_VERSION}\""
)

# ===== 编译器设置 =====
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# ===== 打印配置信息 =====
message(STATUS "Qt Quick Hot Reload Demo Configuration:")
message(STATUS "  Project Version: ${PROJECT_VERSION}")
message(STATUS "  QML Source Dir: ${QML_SOURCE_DIR}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  QML Module URI: com.example.models")