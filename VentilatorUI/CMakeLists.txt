################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Nicolo' Genesio <nicolo.genesio@iit.it>

cmake_minimum_required(VERSION 3.5)

project(VentilatorUI
        LANGUAGES C
        VERSION 0.0.1)

file(GLOB ${PROJECT_NAME}_SRC ${CMAKE_CURRENT_SOURCE_DIR}/WIN32/*.c)
file(GLOB ${PROJECT_NAME}_HDR ${CMAKE_CURRENT_SOURCE_DIR}/WIN32/*.h)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC} ${${PROJECT_NAME}_HDR})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
target_include_directories(${PROJECT_NAME} PRIVATE "C:\\Program Files (x86)\\Embedded Wizard 9.30\\Platforms\\Tara\\Win32\\RGBA8888\\GFX"
                                                   "C:\\Program Files (x86)\\Embedded Wizard 9.30\\Platforms\\Tara\\Win32\\RTE")

target_link_libraries(${PROJECT_NAME} PRIVATE "C:\\Program Files (x86)\\Embedded Wizard 9.30\\Platforms\\Tara\\Win32\\RGBA8888\\GFX\\ewgfx.lib"
                                              "C:\\Program Files (x86)\\Embedded Wizard 9.30\\Platforms\\Tara\\Win32\\RTE\\ewrte.lib")
