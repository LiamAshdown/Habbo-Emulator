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

# output generic information about the options
if( WITH_CORE_DEBUG )
  message("")
  message(" *** WITH_CORE_DEBUG - WARNING!")
  message(" *** additional core debug logs have been enabled!")
  message("* Use coreside debug     	  : Yes")
  add_definitions(-DSTEERSTONE_CORE_DEBUG)
else()
  message("* Use coreside debug           : No  (default)")
endif()

if( WITH_WARNINGS )
  message("* Show all warnings      	  : Yes")
else()
  message("* Show compile-warnings  	  : No  (default)")
endif()

if( WITH_HEADLESS_DEBUG )
  message("* Enable Headless Players       : Yes")
  add_definitions(-DHEADLESS_DEBUG)
else()
  message("* Enable Headless Players      : No  (default)")
endif()