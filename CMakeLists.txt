cmake_minimum_required(VERSION 3.16)

project(ai_box_installer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Network)
get_filename_component(QT_PREFIX "${Qt6_DIR}/../../.." ABSOLUTE)

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ai_box_installer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${APP_ICON_RC}

        registrationservice.h registrationservice.cpp
        downloadtask.h downloadtask.cpp
        appcontroller.h appcontroller.cpp
        appconstants.h
        resources.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ai_box_installer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ai_box_installer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ai_box_installer
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ai_box_installer
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Network)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ai_box_installer)
endif()
set_target_properties(ai_box_installer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    OUTPUT_NAME Safecore
    BUILD_RPATH "\$ORIGIN/../lib"
    INSTALL_RPATH "\$ORIGIN/../lib"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

include(GNUInstallDirs)
install(TARGETS ai_box_installer
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

set(SAFECORE_DESKTOP "${CMAKE_CURRENT_BINARY_DIR}/safespace-global.desktop")
file(WRITE "${SAFECORE_DESKTOP}" "[Desktop Entry]\nType=Application\nName=SafeCore\nComment=SafeCore Installer and Docker Ops\nExec=/opt/safecore-installer/bin/Safecore\nIcon=safespace-global\nStartupWMClass=SafeCore\nTerminal=false\nCategories=Utility;\n")
install(FILES "${SAFECORE_DESKTOP}" DESTINATION "/usr/share/applications")

foreach(size 16 24 32 48 64 128 256 512)
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/images/launch/hicolor/${size}x${size}/apps/safespace-global.png"
          DESTINATION "/usr/share/icons/hicolor/${size}x${size}/apps")
endforeach()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_generate_deploy_app_script(
        TARGET ai_box_installer
        OUTPUT_SCRIPT deploy_script
        EXCLUDE_PLUGINS qtiff
        NO_UNSUPPORTED_PLATFORM_ERROR
    )
    install(SCRIPT ${deploy_script})
    install(DIRECTORY "${QT_PREFIX}/qml/" DESTINATION qml)
    qt_import_qml_plugins(ai_box_installer)
    qt_finalize_executable(ai_box_installer)
endif()

file(GLOB QT_QUICK_CONTROLS_LIBS "${QT_PREFIX}/lib/libQt6QuickControls2*.so*")
if(QT_QUICK_CONTROLS_LIBS)
  install(FILES ${QT_QUICK_CONTROLS_LIBS} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

file(GLOB QT_QUICK_TEMPLATES_LIBS "${QT_PREFIX}/lib/libQt6QuickTemplates2*.so*")
if(QT_QUICK_TEMPLATES_LIBS)
  install(FILES ${QT_QUICK_TEMPLATES_LIBS} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

file(GLOB QT_QUICK_LAYOUTS_LIBS "${QT_PREFIX}/lib/libQt6QuickLayouts*.so*")
if(QT_QUICK_LAYOUTS_LIBS)
  install(FILES ${QT_QUICK_LAYOUTS_LIBS} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

# Bundle libxcb-cursor to avoid external dependency at install time
file(GLOB XCB_CURSOR_LIBS "/usr/lib/x86_64-linux-gnu/libxcb-cursor.so.*")
if(XCB_CURSOR_LIBS)
  install(FILES ${XCB_CURSOR_LIBS} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

include(InstallRequiredSystemLibraries)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "safecore-installer")
set(CPACK_PACKAGE_VENDOR "SafeCore")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SafeCore Installer")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/safecore-installer")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "SafeCore")
set(CPACK_DEBIAN_PACKAGE_DEPENDS
    "libc6, libstdc++6, libgcc-s1, libx11-6, libxcb1, libxkbcommon0, libgl1, libegl1, libfontconfig1, libfreetype6, libglib2.0-0, ca-certificates")
set(CPACK_VERBATIM_VARIABLES ON)
include(CPack)
