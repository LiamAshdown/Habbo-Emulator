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
#*/

set(LIBRARY_NAME shared)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Config)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Database)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Network)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Platform)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Platform/Thread)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/spdlog)
include_directories(${CMAKE_SOURCE_DIR}/dep/mysql/include)
include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})

file(GLOB_RECURSE source_list RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp" "*.h")

foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

add_library(${LIBRARY_NAME} STATIC ${source_list})

target_link_libraries(${LIBRARY_NAME}
  PUBLIC ${OPENSSL_LIBRARIES}
)

target_include_directories(${LIBRARY_NAME}
  PUBLIC ${OPENSSL_INCLUDE_DIR}
  PRIVATE ${Boost_INCLUDE_DIRS}
)

set_target_properties(${LIBRARY_NAME} PROPERTIES PROJECT_LABEL "Shared")
