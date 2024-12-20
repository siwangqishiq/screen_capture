cmake_minimum_required(VERSION 3.10)

project(purple)

include_directories(include)

aux_source_directory(src SRCS)
aux_source_directory(src/audio      SRC_AUDIO)
aux_source_directory(src/input      SRC_INPUT)
aux_source_directory(src/resource   SRC_RESOURCE)
aux_source_directory(src/widget     SRC_WIDGET)
aux_source_directory(src/render     SRC_RENDER)
aux_source_directory(src/render/cmd     SRC_RENDER_CMD)
aux_source_directory(src/ui     SRC_RENDER_UI)

set(src_list "")
list(APPEND src_list ${SRCS})
list(APPEND src_list ${SRC_AUDIO})
list(APPEND src_list ${SRC_INPUT})
list(APPEND src_list ${SRC_RESOURCE})
list(APPEND src_list ${SRC_WIDGET})
list(APPEND src_list ${SRC_RENDER})
list(APPEND src_list ${SRC_RENDER_CMD})
list(APPEND src_list ${SRC_RENDER_UI})

set(src_list_count "")
list(LENGTH src_list src_list_count)
message("purple src file count : ${src_list_count}")

# 动态库
# add_library(${PROJECT_NAME} SHARED ${src_list})

# 静态库
add_library(${PROJECT_NAME} STATIC ${src_list})

# target_link_libraries(${PROJECT_NAME} glfw)



