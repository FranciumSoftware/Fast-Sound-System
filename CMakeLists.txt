cmake_minimum_required(VERSION 3.16)

project(FastSoundSystem VERSION 0.1 LANGUAGES CXX)

# Enable Qt's automatic tools for UI, MOC, and RCC
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Enforce modern C++ standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Detect platform and set platform-specific properties
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(WINDOWS_PLATFORM TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(MACOS_PLATFORM TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LINUX_PLATFORM TRUE)
endif()

# Find Qt components
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Multimedia)

# Translation files
set(TS_FILES Fast-sound-system_fr_FR.ts)

# Project sources
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ${TS_FILES}
    ressources.qrc
    edit.h edit.cpp edit.ui
    comboboxdelegate.h comboboxdelegate.cpp
    audio_loader.h audio_loader.cpp
    filemanagerdialog.h filemanagerdialog.cpp filemanagerdialog.ui
    aubout.h aubout.cpp aubout.ui
)

# Configure executable for different platforms
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FastSoundSystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(FastSoundSystem SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(FastSoundSystem
            ${PROJECT_SOURCES}
        )
    endif()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# Link libraries
target_link_libraries(FastSoundSystem PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    target_link_libraries(FastSoundSystem PRIVATE Qt6::Multimedia)
endif()

# 
if(MACOS_PLATFORM)
    set_target_properties(FastSoundSystem PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE_GUI_IDENTIFIER com.franciumsoftware.FastSoundSystem
    )
endif()

# Install rules
install(TARGETS FastSoundSystem
    BUNDLE DESTINATION .  # Install the .app bundle in the current directory
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Exclude CPack DragNDrop generator (no .dmg file)
set(CPACK_GENERATOR "")  #