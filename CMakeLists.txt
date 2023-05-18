# 设置所需的最低CMake版本为3.5。
cmake_minimum_required(VERSION 3.5)

# 设置项目名称为"HeyPlot"，版本号为0.1，使用的编程语言为C++。
project(HeyPlot VERSION 0.1 LANGUAGES CXX)

# 启用自动UI编译（AUTOUIC）
set(CMAKE_AUTOUIC ON)
# 自动元对象编译（AUTOMOC）
set(CMAKE_AUTOMOC ON)
# 自动资源编译（AUTORCC）
set(CMAKE_AUTORCC ON)

# 设置C++标准为C++17，并要求编译器支持此标准。
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 首先，find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)尝试查找并加载Qt6或Qt5版本，
# 并要求同时加载Widgets和PrintSupport这两个模块。通过使用NAMES参数指定Qt6和Qt5作为可选名称，它会首先尝试找到Qt 6版本，
# 如果找不到再尝试Qt 5版本。而REQUIRED关键字确保这些模块是必需的，如果找不到任何一个模块，将会导致CMake错误并停止构建过程。
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
# 接下来，find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)根据之前找到的Qt版本，
# 使用Qt${QT_VERSION_MAJOR}作为变量来引用具体的Qt模块。${QT_VERSION_MAJOR}是一个变量，
# 它在之前的find_package命令中通过检查Qt的主要版本号进行了设置。这样做的目的是确保在之后的构建过程中使用与先前找到的Qt版本匹配的模块。
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

# 定义项目的源文件列表
set(PROJECT_SOURCES
        main.cpp
        widget.cpp
        widget.h
        widget.ui
        qcustomplot.cpp
        qcustomplot.h
        heyplot.cpp
        heyplot.h
        heycursor.cpp
        heycursor.h
        heyaxiscursor.cpp
        heyaxiscursor.h
)

# 包含源文件目录
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

# 如果Qt的主要版本号大于等于6，则使用"qt_add_executable"来创建可执行文件。否则，根据目标平台选择创建动态库（Android）或可执行文件。
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(HeyPlot
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(HeyPlot SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(HeyPlot
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 将Qt模块链接到目标可执行文件或动态库。
target_link_libraries(HeyPlot PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)

# 设置目标可执行文件的属性，如MacOSX的Bundle标识符、版本号和短版本字符串，以及在Windows下作为可执行文件运行。
set_target_properties(HeyPlot PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 安装目标可执行文件到指定的目录。
install(TARGETS HeyPlot
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 如果使用的是Qt 6，则使用"qt_finalize_executable"进行最后的可执行文件处理。
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(HeyPlot)
endif()
