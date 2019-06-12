#* Liam Ashdown
#* Copyright (C) 2018
#*
#* This program is free software: you can redistribute it and/or modify
#* it under the terms of the GNU General Public License as published by
#* the Free Software Foundation, either version 3 of the License, or
#* (at your option) any later version.
#*
#* This program is distributed in the hope that it will be useful,
#* but WITHOUT ANY WARRANTY; without even the implied warranty of
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#* GNU General Public License for more details.
#*
#* You should have received a copy of the GNU General Public License
#* along with this program.  If not, see <http://www.gnu.org/licenses/>.
#*/

cmake_minimum_required (VERSION 3.2)

project(QuadEmulator)

# Build in RelWithDebInfo-mode by default if not explicitly set
if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

if(WIN32)
  if(PLATFORM MATCHES X86) # 32-bit
    set(DEP_ARCH Win32)
  else() # 64-bit
    set(DEP_ARCH x64)
  endif()
endif()

# Add alternate lib dir for boost on windows
if(WIN32 AND NOT "$ENV{BOOST_ROOT}" STREQUAL "")
  set(BOOST_LIBRARYDIR "$ENV{BOOST_LIBRARYDIR}")
  set(BOOST_LIBRARYDIR "${BOOST_LIBRARYDIR};$ENV{BOOST_ROOT}\\lib\\${DEP_ARCH}")
  set(BOOST_LIBRARYDIR "${BOOST_LIBRARYDIR};$ENV{BOOST_ROOT}\\stage\\${DEP_ARCH}")
  set(BOOST_LIBRARYDIR "${BOOST_LIBRARYDIR};$ENV{BOOST_ROOT}\\stage")
endif()

# Be sure to search only static lib
add_definitions(-DBOOST_ALL_NO_LIB)
set(Boost_USE_STATIC_LIBS ON)
#set(Boost_DEBUG ON)

if(MSVC)
  # Use at least verion 3.8.1 of FindBoost to fix some issues. Especialy in MSVC with fixed Toolset issues.
  # This is a workaround while VisualStudio does not embed higher cmake version than 3.7.2
  if(${CMAKE_VERSION} VERSION_LESS "3.11.3")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/macros)
    message(STATUS "FindBoost: included version 3.11.3 of FindBoost is used.")
  endif()
endif()

# In our case use new way to treat BOOST_ROOT environement variable
if (POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

# Find needed packages and if necessery abort if something important is missing
find_package(Boost REQUIRED COMPONENTS system program_options thread regex)

if(NOT Boost_FOUND)
  message(STATUS "BOOST_ROOT = $ENV{BOOST_ROOT}")
  message(STATUS "BOOST_LIBRARYDIR = $ENV{BOOST_LIBRARYDIR}")
  if ("$ENV{BOOST_ROOT}" STREQUAL "")
    message(WARNING "BOOST_ROOT is not set, please set it correctly to your boost folder.")
  else()
    if ("$ENV{BOOST_LIBRARYDIR}" STREQUAL "")
        message(STATUS "Please put all ${DEP_ARCH} boost lib files in $ENV{BOOST_ROOT}\\lib\\${DEP_ARCH} folder")
        message(STATUS "Or set BOOST_LIBRARYDIR to your boost lib folder.")
    else()
        message(STATUS "BOOST_LIBRARYDIR is set, please verify if correct libs files are in that folder.")
    endif()
  endif()
  message(FATAL_ERROR "This project requires boost.  Please install from http://www.boost.org")
endif()

# Win32 OpenSSL
if(WIN32)
  set(OPENSSL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dep/")
  set(OPENSSL_LIBRARIES "${CMAKE_SOURCE_DIR}/dep/lib/${DEP_ARCH}_release/libcrypto.dll.lib")
  set(OPENSSL_DEBUG_LIBRARIES "${CMAKE_SOURCE_DIR}/dep/lib/${DEP_ARCH}_debug/libcrypto.dll.lib")
endif()

# Add sub directories
add_subdirectory(src)