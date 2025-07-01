cmake_minimum_required(VERSION 3.16)

project(Fast-sound-system VERSION 0.1 LANGUAGES CXX)

set(CMAKE_SYSTEM_NAME MACOS)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)


set(TS_FILES Fast-sound-system_fr_FR.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Fast-sound-system
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ressources.qrc
        edit.h edit.cpp edit.ui
        comboboxdelegate.h comboboxdelegate.cpp
        vlcpp/common.hpp vlcpp/Dialog.hpp vlcpp/Equalizer.hpp vlcpp/EventManager.hpp vlcpp/Instance.hpp vlcpp/Internal.hpp vlcpp/Media.hpp vlcpp/MediaDiscoverer.hpp vlcpp/MediaLibrary.hpp vlcpp/MediaList.hpp vlcpp/MediaListPlayer.hpp vlcpp/MediaPlayer.hpp vlcpp/Picture.hpp vlcpp/RendererDiscoverer.hpp vlcpp/structures.hpp vlcpp/vlc.hpp
        audio_loader.h audio_loader.cpp
        filemanagerdialog.h filemanagerdialog.cpp filemanagerdialog.ui
        aubout.h aubout.cpp aubout.ui
    )


# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Fast-sound-system APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Fast-sound-system SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Fast-sound-system
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(Fast-sound-system PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Fast-sound-system PRIVATE Qt6::Multimedia)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Fast-sound-system)
endif()
set_target_properties(Fast-sound-system PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Fast-sound-system
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Fast-sound-system)
endif()
