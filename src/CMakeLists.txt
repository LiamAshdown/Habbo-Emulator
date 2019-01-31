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

#Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
# Set the project name
project (QuadEmulator)

cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Common/Logging)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Entity)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Entity/Player)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Entity/Item)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Manager)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Network)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Network/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/PathFinding)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Quad)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Room)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Server/Handlers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Server/Packet)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Server/Protocol)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Server/Thread)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/World)

file(GLOB_RECURSE source_list RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp" "*.h")

add_executable(Game ${source_list})

foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
