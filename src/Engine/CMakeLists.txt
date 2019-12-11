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

# Libraru Name
set(LIBRARY_NAME Engine)

# Include Directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Diagnostic)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Config)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Logger)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/PCH)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Singleton)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Threading)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Utility)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Network)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Database)
include_directories(${CMAKE_SOURCE_DIR}/dep/SFMT)

file(GLOB_RECURSE source_list RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp" "*.hpp")

foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

# Add Library
add_library(${LIBRARY_NAME} STATIC ${source_list})

# External Link Includes
target_include_directories(${LIBRARY_NAME}
  PRIVATE ${OPENSSL_INCLUDE_DIR}
  PRIVATE ${Boost_INCLUDE_DIRS}
  PRIVATE ${MYSQL_INCLUDE_DIR}
)

# Define OutDir to SOURCE/bin/(platform)_(configuaration) folder.
set_target_properties(${LIBRARY_NAME} PROPERTIES PROJECT_LABEL "Engine")