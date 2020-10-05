cmake_minimum_required(VERSION 3.12.0)

project(cso2-starter LANGUAGES CXX)

option(STARTER_USE_LTO "Use 'Link Time Optimizations'" ON)

set(STARTER_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(STARTER_LIBS_DIR "${STARTER_ROOT_DIR}/external")
set(STARTER_RESOURCES_DIR "${STARTER_ROOT_DIR}/resources")
set(STARTER_GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(STARTER_VERSION_FILE "${STARTER_ROOT_DIR}/version.txt")
set(STARTER_VERSION_TEMPLATE
    "${CMAKE_SOURCE_DIR}/headers/starter_version.hpp.in")
set(STARTER_VERSION_OUT "${STARTER_GENERATED_DIR}/starter_version.hpp")
set(STARTER_GIT_DIR "${CMAKE_SOURCE_DIR}/.git")

include(program_version)
generate_program_version(
  VERSION_FILE
  ${STARTER_VERSION_FILE}
  HEADER_TEMPLATE
  ${STARTER_VERSION_TEMPLATE}
  HEADER_OUT
  ${STARTER_VERSION_OUT}
  GIT_DIR
  ${STARTER_GIT_DIR})

if(STARTER_USE_LTO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT STARTER_IPO_SUPPORTED OUTPUT STARTER_IPO_ERR)

  if(STARTER_IPO_SUPPORTED)
    message(STATUS "cso2-starter: Using Link Time Optimizations")
  else()
    message(
      FATAL_ERROR
        "Link time optimization is not supported: <${CMAKE_INTERPROCEDURAL_OPTIMIZATION}>"
    )
  endif()
endif()

#
# find dependencies
#
find_package(
  Qt5
  COMPONENTS Widgets
  REQUIRED)
if(WIN32)
  find_package(Qt5 COMPONENTS WinExtras REQUIRED)
endif()

# Auto generate Qt files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set the .ui files path
set(CMAKE_AUTOUIC_SEARCH_PATHS "${STARTER_ROOT_DIR}/resources/layouts")

#
# add source files to the project
#
set(STARTER_SOURCES_BASE "sources/csstarterapp.cpp"
                         "sources/launchersettings.cpp" "sources/main.cpp")

set(STARTER_SOURCES_LAYOUTS
    "sources/layouts/aboutdialog.cpp"
    "sources/layouts/launchersettingsdialog.cpp"
    "sources/layouts/mainwindow.cpp")

set(STARTER_HEADERS_BASE "headers/csstarterapp.hpp"
                         "headers/launchersettings.hpp" ${STARTER_VERSION_OUT})

set(STARTER_HEADERS_LAYOUTS
    "headers/layouts/aboutdialog.hpp"
    "headers/layouts/launchersettingsdialog.hpp"
    "headers/layouts/mainwindow.hpp")

set(STARTER_RESOURCES_BREEZE "resources/icons-breeze.qrc")

set(STARTER_RESOURCES "resources/icons-starter.qrc" ${STARTER_RESOURCES_BREEZE})

set(STARTER_RESOURCES_LAYOUTS
    "resources/layouts/aboutdialog.ui"
    "resources/layouts/launchersettingsdialog.ui"
    "resources/layouts/mainwindow.ui")

if(WIN32)
  set(STARTER_RESOURCES_WIN32 "resources/win32/starter.ico"
                              "resources/win32/starter.rc")
else()
  set(STARTER_RESOURCES_WIN32)
endif()

source_group("Source Files" FILES ${STARTER_SOURCES_BASE})
source_group("Source Files\\Layouts" FILES ${STARTER_SOURCES_LAYOUTS})
source_group("Header Files" FILES ${STARTER_HEADERS_BASE})
source_group("Header Files\\Layouts" FILES ${STARTER_HEADERS_LAYOUTS})
source_group("Resources Files" FILES ${STARTER_RESOURCES})
source_group("Resources Files\\Layouts" FILES ${STARTER_RESOURCES_LAYOUTS})

qt5_add_binary_resources(icons-breeze ${STARTER_RESOURCES_BREEZE})

file(
  GLOB
  STARTER_ALL_SOURCES
  ${STARTER_SOURCES_BASE}
  ${STARTER_SOURCES_LAYOUTS}
  ${STARTER_HEADERS_BASE}
  ${STARTER_HEADERS_LAYOUTS}
  ${STARTER_RESOURCES_LAYOUTS}
  ${STARTER_RESOURCES_WIN32})

qt5_add_resources(STARTER_ALL_SOURCES ${STARTER_RESOURCES})

# force c++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# Add executable to build.
#
if(WIN32)
  add_executable(cso2-starter WIN32 ${STARTER_ALL_SOURCES})
else()
  add_executable(cso2-starter ${STARTER_ALL_SOURCES})
endif()

# add fallback menu icons
add_dependencies(cso2-starter icons-breeze)

#
# Enable all warnings
#
if(MSVC)
  target_compile_options(cso2-starter PRIVATE /W4)
else()
  target_compile_options(cso2-starter PRIVATE -Wall -Wextra -Wconversion
                                              -pedantic)
endif()

if(STARTER_USE_LTO)
  set_property(TARGET cso2-starter PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

#
# Setup include directories
#
target_include_directories(cso2-starter PRIVATE "headers")
target_include_directories(cso2-starter PRIVATE "headers/layouts")

# the generated version header's directory
target_include_directories(cso2-starter PRIVATE ${STARTER_GENERATED_DIR})

#
# link libraries
#

# multithreading
if(UNIX)
  target_link_libraries(cso2-starter pthread)
endif()

# link Qt5
target_include_directories(cso2-starter PRIVATE ${Qt5Widgets_INCLUDE_DIRS})
target_link_libraries(cso2-starter Qt5::Widgets)
if(WIN32)
  target_link_libraries(cso2-starter Qt5::WinExtras)
endif()
