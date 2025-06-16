cmake_minimum_required(VERSION 3.16)

project(FlatVec-Viewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        interface/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FlatVec-Viewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        styles/qrc.qrc
        geometry/rectangle/rectangle.h geometry/rectangle/rectangle.cpp
        geometry/rectangle/invisiblerectangle.h geometry/rectangle/invisiblerectangle.cpp
        geometry/rectangle/curvedrectangle.h geometry/rectangle/curvedrectangle.cpp
        geometry/rectangle/ordinaryrectangle.h geometry/rectangle/ordinaryrectangle.cpp
        geometry/rectangle/slantedrectangle.h geometry/rectangle/slantedrectangle.cpp
        datamanagement/filemanager.h datamanagement/filemanager.cpp
        datamanagement/localdatamanager.h datamanagement/localdatamanager.cpp
        layers/areabox.h layers/areabox.cpp
        camera/camera.h camera/camera.cpp
        projectwindow.h projectwindow.cpp interface/projectwindow.ui
        camerawindow.h camerawindow.cpp interface/camerawindow.ui
        geometry/line/boxline.h geometry/line/boxline.cpp
        widgets/maingraphicsview.h widgets/maingraphicsview.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FlatVec-Viewer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(FlatVec-Viewer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FlatVec-Viewer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(FlatVec-Viewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FlatVec-Viewer)
endif()
set_target_properties(FlatVec-Viewer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS FlatVec-Viewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FlatVec-Viewer)
endif()
