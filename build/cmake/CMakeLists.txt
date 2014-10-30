#
#  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of modis2scidb.
#
#  modis2scidb is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License version 3 as
#  published by the Free Software Foundation.
#
#  modis2scidb is distributed  "AS-IS" in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
#  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with modis2scidb. See COPYING. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
#
#
#  CMake scripts for modis2scidb
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#
#  Description: Main CMake script of modis2scidb build system.
#

cmake_minimum_required(VERSION 2.8.7)

project(modis2scidb)

#
# main variables for the build script
#

# variables that control the system versioning
set(MODIS2SCIDB_VERSION_MAJOR 0)
set(MODIS2SCIDB_VERSION_MINOR 1)
set(MODIS2SCIDB_VERSION_PATCH 0)
set(MODIS2SCIDB_VERSION_STATUS "alpha")
set(MODIS2SCIDB_VERSION_STRING "${MODIS2SCIDB_VERSION_MAJOR}.${MODIS2SCIDB_VERSION_MINOR}.${MODIS2SCIDB_VERSION_PATCH}-${MODIS2SCIDB_VERSION_STATUS}")
set(MODIS2SCIDB_VERSION "0x000100")

# variables storing relative paths
set(MODIS2SCIDB_ABSOLUTE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)


#
# global definitions, includes and configurations
#

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)

if(APPLE)
  add_definitions(-ftemplate-depth-1024)
endif()

include_directories(${CMAKE_BINARY_DIR})
include_directories(${MODIS2SCIDB_ABSOLUTE_ROOT_DIR}/src)


#
# Build will generate files into a single folder
#
if((CMAKE_GENERATOR STREQUAL Xcode) OR MSVC)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
else()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()


#
# including CMake's auxiliary files
#
include(CMakeDependentOption)


#
# first check of libraries
#

find_package(Boost 1.54.0 REQUIRED system date_time chrono timer filesystem thread log log_setup program_options)

find_package(GDAL REQUIRED)


#
# process MODIS2SCIDB configuration files
#

configure_file(${modis2scidb_SOURCE_DIR}/modis2scidb-config-pkg.cmake.in
               ${CMAKE_BINARY_DIR}/pkg/modis2scidb-config.cmake @ONLY)

configure_file(${modis2scidb_SOURCE_DIR}/modis2scidb-config-version.cmake.in
               ${CMAKE_BINARY_DIR}/modis2scidb-config-version.cmake @ONLY)

configure_file(${modis2scidb_SOURCE_DIR}/modis2scidb-config.cmake.in
               ${CMAKE_BINARY_DIR}/modis2scidb-config.cmake @ONLY)

configure_file(${MODIS2SCIDB_ABSOLUTE_ROOT_DIR}/src/modis2scidb/version.hpp.in
               ${CMAKE_BINARY_DIR}/modis2scidb_version.hpp @ONLY)

configure_file(${MODIS2SCIDB_ABSOLUTE_ROOT_DIR}/src/modis2scidb/config.hpp.in
               ${CMAKE_BINARY_DIR}/modis2scidb_config.hpp @ONLY)

#
# add executable
#

include_directories(${Boost_INCLUDE_DIR})
include_directories(${GDAL_INCLUDE_DIR})

file(GLOB MODIS2SCIDB_SRC_FILES ${MODIS2SCIDB_ABSOLUTE_ROOT_DIR}/src/modis2scidb/*.cpp)
file(GLOB MODIS2SCIDB_HDR_FILES ${MODIS2SCIDB_ABSOLUTE_ROOT_DIR}/src/modis2scidb/*.hpp)

add_executable(modis2scidb ${MODIS2SCIDB_HDR_FILES} ${MODIS2SCIDB_SRC_FILES})

target_link_libraries(modis2scidb ${Boost_SYSTEM_LIBRARY}
                                  ${Boost_DATE_TIME_LIBRARY}
                                  ${Boost_CHRONO_LIBRARY}
                                  ${Boost_TIMER_LIBRARY}
                                  ${Boost_FILESYSTEM_LIBRARY}
                                  ${Boost_THREAD_LIBRARY}
                                  ${Boost_LOG_LIBRARY}
                                  ${Boost_PROGRAM_OPTIONS_LIBRARY}
                                  ${GDAL_LIBRARY})
#
# install and targets export
#

install(TARGETS modis2scidb
        EXPORT modis2scidb-targets
        RUNTIME DESTINATION bin COMPONENT runtime
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT runtime
        BUNDLE  DESTINATION .   COMPONENT runtime)

