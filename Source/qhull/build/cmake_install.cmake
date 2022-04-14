# Install script for directory: C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/qhull")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Debug/qhull.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Release/qhull.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/MinSizeRel/qhull.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/RelWithDebInfo/qhull.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Debug/rbox.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Release/rbox.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/MinSizeRel/rbox.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/RelWithDebInfo/rbox.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Debug/qconvex.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Release/qconvex.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/MinSizeRel/qconvex.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/RelWithDebInfo/qconvex.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Debug/qdelaunay.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Release/qdelaunay.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/MinSizeRel/qdelaunay.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/RelWithDebInfo/qdelaunay.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Debug/qvoronoi.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Release/qvoronoi.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/MinSizeRel/qvoronoi.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/RelWithDebInfo/qvoronoi.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Debug/qhalf.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Release/qhalf.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/MinSizeRel/qhalf.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/RelWithDebInfo/qhalf.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Debug/qhull_rd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Release/qhull_r.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/MinSizeRel/qhull_r.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/RelWithDebInfo/qhull_r.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Debug/qhull_rd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Release/qhull_r.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/MinSizeRel/qhull_r.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/RelWithDebInfo/qhull_r.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Debug/qhullcpp_d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Release/qhullcpp.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/MinSizeRel/qhullcpp.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/RelWithDebInfo/qhullcpp.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Debug/qhullstatic_d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Release/qhullstatic.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/MinSizeRel/qhullstatic.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/RelWithDebInfo/qhullstatic.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Debug/qhullstatic_rd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/Release/qhullstatic_r.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/MinSizeRel/qhullstatic_r.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/RelWithDebInfo/qhullstatic_r.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qhull/QhullTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qhull/QhullTargets.cmake"
         "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/CMakeFiles/Export/lib/cmake/Qhull/QhullTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qhull/QhullTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qhull/QhullTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Qhull" TYPE FILE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/CMakeFiles/Export/lib/cmake/Qhull/QhullTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Qhull" TYPE FILE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/CMakeFiles/Export/lib/cmake/Qhull/QhullTargets-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Qhull" TYPE FILE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/CMakeFiles/Export/lib/cmake/Qhull/QhullTargets-minsizerel.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Qhull" TYPE FILE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/CMakeFiles/Export/lib/cmake/Qhull/QhullTargets-relwithdebinfo.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Qhull" TYPE FILE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/CMakeFiles/Export/lib/cmake/Qhull/QhullTargets-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Qhull" TYPE FILE FILES
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/QhullExport/QhullConfig.cmake"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/QhullExport/QhullConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/qhull_r.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/qhullstatic.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/qhullstatic_r.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/qhullcpp.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libqhull" TYPE FILE FILES
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull/libqhull.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull/geom.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull/io.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull/mem.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull/merge.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull/poly.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull/qhull_a.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull/qset.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull/random.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull/stat.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull/user.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libqhull" TYPE FILE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull/DEPRECATED.txt")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libqhull_r" TYPE FILE FILES
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull_r/libqhull_r.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull_r/geom_r.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull_r/io_r.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull_r/mem_r.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull_r/merge_r.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull_r/poly_r.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull_r/qhull_ra.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull_r/qset_r.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull_r/random_r.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull_r/stat_r.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhull_r/user_r.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libqhullcpp" TYPE FILE FILES
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/Coordinates.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/functionObjects.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/PointCoordinates.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/Qhull.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullError.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullFacet.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullFacetList.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullFacetSet.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullHyperplane.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullIterator.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullLinkedList.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullPoint.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullPoints.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullPointSet.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullQh.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullRidge.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullSet.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullSets.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullStat.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullUser.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullVertex.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/QhullVertexSet.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/RboxPoints.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/RoadError.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/libqhullcpp/RoadLogEvent.h"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/qhulltest/RoadTest.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE RENAME "qhull.1" FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/html/qhull.man")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE RENAME "rbox.1" FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/html/rbox.man")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/qhull" TYPE FILE FILES
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/README.txt"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/REGISTER.txt"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/Announce.txt"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/COPYING.txt"
    "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/index.htm"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/qhull/html" TYPE DIRECTORY FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/html/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/qhull/src" TYPE FILE FILES "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/src/Changes.txt")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/zakla/Downloads/Insanity3D-main/Insanity3D-main/Source/qhull/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
