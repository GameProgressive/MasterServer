#	Licensed to the Apache Software Foundation (ASF) under one
#	or more contributor license agreements.  See the NOTICE file
#	distributed with this work for additional information
#	regarding copyright ownership.  The ASF licenses this file
#	to you under the Apache License, Version 2.0 (the
#	"License"); you may not use this file except in compliance
#	with the License.  You may obtain a copy of the License at
#
#	http://www.apache.org/licenses/LICENSE-2.0
#
#	Unless required by applicable law or agreed to in writing,
#	software distributed under the License is distributed on an
#	"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#	KIND, either express or implied.  See the License for the
#	specific language governing permissions and limitations
#	under the License.
#
cmake_minimum_required(VERSION 2.8)
project("MasterServer")

# Add definitions for w32 build that dosen't support _WIN32 macro
if (WIN32)
	add_definitions("-DWIN32 -D__WIN32__")
endif()

# Set CMake output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(WITH_MDK "Build MDK from the local clone")

## Module Development Kit
if (WITH_MDK)
add_subdirectory("MDK")
include_directories("MDK/include")
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
target_link_libraries(MasterServer MDK)
#set_target_properties(MasterServer PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")
