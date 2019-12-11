#* Liam Ashdown
#* Copyright (C) 2019
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
#*

# CMake policies (can not be handled elsewhere)
cmake_minimum_required (VERSION 3.1)
# C++ Standard
set (CMAKE_CXX_STANDARD 17)

# Add this options before PROJECT keyword
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Set projectname (must be done AFTER setting configurationtypes)
project(SteerStone)

# CMake policies (can not be handled elsewhere)
cmake_policy(SET CMP0005 OLD)
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 OLD) # Disable 'Ignore COMPILE_DEFINITIONS_<Config> properties'
endif()

if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW) # Only interpret if() arguments as variables or keywords when unquoted - prevents intepreting if (SOME_STRING_VARIABLE MATCHES "MSVC") as if (SOME_STRING_VARIABLE MATCHES "1")
endif()

# In our case use new way to treat BOOST_ROOT environement variable
if (POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

# Set macro-directory
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros")

# Boost macro-directory
set(Boost_USE_STATIC_LIBS ON)

list(APPEND CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/cmake/macros"
  "${CMAKE_SOURCE_DIR}/dep/cotire/CMake")

# Build in RelWithDebInfo-mode by default if not explicitly set
if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# Set default buildoptions and print them
include(cmake/options.cmake)

# Set up platform
include(CheckPlatform)

find_package(OpenSSL REQUIRED)
find_package(MySQL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system program_options thread regex)

# Print out the results before continuing
include(cmake/showoptions.cmake)

# Add dependencies
add_subdirectory(dep)

# Add core sources
add_subdirectory(src)