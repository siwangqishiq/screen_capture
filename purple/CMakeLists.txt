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

# 动态库
# add_library(${PROJECT_NAME} 
#                 SHARED 
#                 ${SRCS}
#                 ${SRC_AUDIO}
#                 ${SRC_INPUT}
#                 ${SRC_RESOURCE}
#                 ${SRC_WIDGET}
#                 ${SRC_RENDER}
#                 ${SRC_RENDER_CMD})

# 静态库
add_library(${PROJECT_NAME} 
                STATIC 
                ${SRCS}
                ${SRC_AUDIO}
                ${SRC_INPUT}
                ${SRC_RESOURCE}
                ${SRC_WIDGET}
                ${SRC_RENDER}
                ${SRC_RENDER_CMD})

# target_link_libraries(${PROJECT_NAME} glfw)



