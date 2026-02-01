# Install script for directory: /home/sandy/Documents/v1.0/safecore-installer

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Safecore" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Safecore")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Safecore"
         RPATH "\$ORIGIN/../lib:/home/sandy/Qt/6.10.1/gcc_64/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/sandy/Documents/v1.0/safecore-installer/build/Safecore")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Safecore" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Safecore")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Safecore"
         OLD_RPATH "\$ORIGIN/../lib:/home/sandy/Qt/6.10.1/gcc_64/lib:"
         NEW_RPATH "\$ORIGIN/../lib:/home/sandy/Qt/6.10.1/gcc_64/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Safecore")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/share/applications/safespace-global.desktop")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/share/applications" TYPE FILE FILES "/home/sandy/Documents/v1.0/safecore-installer/build/safespace-global.desktop")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/share/icons/hicolor/16x16/apps/safespace-global.png")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/share/icons/hicolor/16x16/apps" TYPE FILE FILES "/home/sandy/Documents/v1.0/safecore-installer/images/launch/hicolor/16x16/apps/safespace-global.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/share/icons/hicolor/24x24/apps/safespace-global.png")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/share/icons/hicolor/24x24/apps" TYPE FILE FILES "/home/sandy/Documents/v1.0/safecore-installer/images/launch/hicolor/24x24/apps/safespace-global.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/share/icons/hicolor/32x32/apps/safespace-global.png")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/share/icons/hicolor/32x32/apps" TYPE FILE FILES "/home/sandy/Documents/v1.0/safecore-installer/images/launch/hicolor/32x32/apps/safespace-global.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/share/icons/hicolor/48x48/apps/safespace-global.png")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/share/icons/hicolor/48x48/apps" TYPE FILE FILES "/home/sandy/Documents/v1.0/safecore-installer/images/launch/hicolor/48x48/apps/safespace-global.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/share/icons/hicolor/64x64/apps/safespace-global.png")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/share/icons/hicolor/64x64/apps" TYPE FILE FILES "/home/sandy/Documents/v1.0/safecore-installer/images/launch/hicolor/64x64/apps/safespace-global.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/share/icons/hicolor/128x128/apps/safespace-global.png")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/share/icons/hicolor/128x128/apps" TYPE FILE FILES "/home/sandy/Documents/v1.0/safecore-installer/images/launch/hicolor/128x128/apps/safespace-global.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/share/icons/hicolor/256x256/apps/safespace-global.png")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/share/icons/hicolor/256x256/apps" TYPE FILE FILES "/home/sandy/Documents/v1.0/safecore-installer/images/launch/hicolor/256x256/apps/safespace-global.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/share/icons/hicolor/512x512/apps/safespace-global.png")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/share/icons/hicolor/512x512/apps" TYPE FILE FILES "/home/sandy/Documents/v1.0/safecore-installer/images/launch/hicolor/512x512/apps/safespace-global.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/sandy/Documents/v1.0/safecore-installer/build/.qt/deploy_ai_box_installer_db0de634b3.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/qml" TYPE DIRECTORY FILES "/home/sandy/Qt/6.10.1/gcc_64/qml/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2.so.6.10.1"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Basic.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Basic.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Basic.so.6.10.1"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2BasicStyleImpl.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2BasicStyleImpl.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2BasicStyleImpl.so.6.10.1"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2FluentWinUI3StyleImpl.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2FluentWinUI3StyleImpl.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2FluentWinUI3StyleImpl.so.6.10.1"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Fusion.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Fusion.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Fusion.so.6.10.1"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2FusionStyleImpl.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2FusionStyleImpl.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2FusionStyleImpl.so.6.10.1"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Imagine.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Imagine.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Imagine.so.6.10.1"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2ImagineStyleImpl.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2ImagineStyleImpl.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2ImagineStyleImpl.so.6.10.1"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Impl.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Impl.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Impl.so.6.10.1"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Material.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Material.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Material.so.6.10.1"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2MaterialStyleImpl.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2MaterialStyleImpl.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2MaterialStyleImpl.so.6.10.1"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Universal.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Universal.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2Universal.so.6.10.1"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2UniversalStyleImpl.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2UniversalStyleImpl.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickControls2UniversalStyleImpl.so.6.10.1"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickTemplates2.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickTemplates2.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickTemplates2.so.6.10.1"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickLayouts.so"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickLayouts.so.6"
    "/home/sandy/Qt/6.10.1/gcc_64/lib/libQt6QuickLayouts.so.6.10.1"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/usr/lib/x86_64-linux-gnu/libxcb-cursor.so.0"
    "/usr/lib/x86_64-linux-gnu/libxcb-cursor.so.0.0.0"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/sandy/Documents/v1.0/safecore-installer/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
