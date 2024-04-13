cmake_minimum_required(VERSION 3.10)

project(app)

# for dll import use case 
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl -lX11")
    set (CMAKE_CXX_FLAGS -pthread)
endif()

# SET(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_BUILD_TYPE "Debug")

include_directories(vender/glfw/include)
include_directories(purple/include)
include_directories(src)

# add_subdirectory(vender/glfw)
add_subdirectory(purple)

aux_source_directory(src SRCS)
aux_source_directory(src/platform SRCS_PLATFORM)
aux_source_directory(src/action SRCS_ACTION)
aux_source_directory(src/editor SRCS_EDITOR)
aux_source_directory(src/config SRCS_CONFIG)


if (WIN32)
    message(STATUS "Using Win32 library")
    link_directories(lib/win64)
elseif (APPLE)
    message(STATUS "Using APPLE library")
elseif (UNIX)
    message(STATUS "Using Linux library")
    link_directories(lib/linux)
endif()

link_libraries(glfw3)

add_executable(${PROJECT_NAME}  ${SRCS}
                                ${SRCS_PLATFORM}
                                ${SRCS_ACTION}
                                ${SRCS_EDITOR}
                                ${SRCS_CONFIG}
                                )

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wall")
# set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-mwindows")

target_link_libraries(${PROJECT_NAME} purple)
target_link_libraries(${PROJECT_NAME} glfw3)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(X11)
    link_directories( /usr/lib/X11 )
    target_link_libraries(${PROJECT_NAME} X11)
endif()







