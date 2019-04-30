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

set(EXECUTABLE_NAME Server)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Entity)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Entity/Habbo)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Entity/Item)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Catalogue)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Managers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/PathFinder)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Room)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Trigger)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Socket)
include_directories(${CMAKE_SOURCE_DIR}/src/Shared)
include_directories(${CMAKE_SOURCE_DIR}/dep/mysql/include)

file(GLOB_RECURSE source_list RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp" "*.h")

foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

add_executable(${EXECUTABLE_NAME} ${source_list})

target_include_directories(${EXECUTABLE_NAME}
  PRIVATE ${Boost_INCLUDE_DIRS}
  PRIVATE ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(${EXECUTABLE_NAME}
  shared
)

target_link_libraries(${EXECUTABLE_NAME}
  debug ${OPENSSL_DEBUG_LIBRARIES}
  ${CMAKE_SOURCE_DIR}/dep/mysql/lib/mysqlcppconn.lib
  ${Boost_LIBRARIES}
  )

# Define OutDir to source/bin/(platform)_(configuaration) folder.
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${DEV_BIN_DIR}")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${DEV_BIN_DIR}")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES PROJECT_LABEL "Server")

# Add conf file to dev folder
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different \"${CMAKE_CURRENT_SOURCE_DIR}/server.conf.dist.in\" \"$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/server.conf.dist\"
COMMAND ${CMAKE_COMMAND} -E copy_if_different \"${CMAKE_SOURCE_DIR}/dep/mysql/lib/mysqlcppconn.dll\" \"$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/mysqlcppconn.dll\")