#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
cmake_minimum_required(VERSION 2.8)
project("MasterServer")

include(CheckIncludeFiles)

# Add definitions for w32 build that dosen't support _WIN32 macro
if (WIN32)
	add_definitions("-DWIN32 -D__WIN32__")
endif()

# Set CMake output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (MSVC)
check_include_files(vld.h HAVE_VLD_H)

if (HAVE_VLD_H)
add_definitions(-DHAVE_VLD_H)
endif()

endif()

set(SOURCES
${CMAKE_CURRENT_LIST_DIR}/MSConfig.cpp
${CMAKE_CURRENT_LIST_DIR}/MSConfig.h
${CMAKE_CURRENT_LIST_DIR}/ini.c
${CMAKE_CURRENT_LIST_DIR}/ini.h
${CMAKE_CURRENT_LIST_DIR}/INIReader.cpp
${CMAKE_CURRENT_LIST_DIR}/INIReader.h
${CMAKE_CURRENT_LIST_DIR}/Main.cpp
${CMAKE_CURRENT_LIST_DIR}/Module.cpp
${CMAKE_CURRENT_LIST_DIR}/Module.h
${CMAKE_CURRENT_LIST_DIR}/ModuleManager.cpp
${CMAKE_CURRENT_LIST_DIR}/ModuleManager.h
${CMAKE_CURRENT_LIST_DIR}/Thread.cpp
${CMAKE_CURRENT_LIST_DIR}/Thread.h
)

if (WIN32)
	set(SOURCES ${SOURCES} ${CMAKE_CURRENT_LIST_DIR}/resource.h)
	set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/MasterServer.rc PROPERTIES LANGUAGE RC)
endif()

add_executable(MasterServer ${SOURCES})
target_link_libraries(MasterServer MDK ${CMAKE_DL_LIBS})
#set_target_properties(MasterServer PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")
