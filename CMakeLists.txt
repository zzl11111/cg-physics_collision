####################
# A simple CMake Project File for your reference
# Modify whatever you want
# You can also use other project frameworks
####################

cmake_minimum_required(VERSION 3.18)

project(rigid-body-simulation)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# ######### External liberaries #############
# glfw
option(GLFW_BUILD_TESTS off)
option(GLFW_BUILD_EXAMPLES off)
option(GLFW_BUILD_DOCS off)
add_subdirectory(extern/glfw)
include_directories(extern/glfw/include)
list(APPEND DEPENDENCIES glfw ${glfw_LIBRARIES})

# glad
set(GLAD_INCLUDE extern/glad/include)
set(GLAD_SRC extern/glad/src/glad.c)
include_directories(${GLAD_INCLUDE})

# glm
add_subdirectory(extern/glm)
list(APPEND DEPENDENCIES glm::glm)
# ############################################

# Add the main library
add_library(libmain
    src/camera.cpp
src/blin.fs
src/blin.vs
    ${GLAD_SRC}
)
target_include_directories(libmain
    PUBLIC include
)
target_link_libraries(libmain PUBLIC ${DEPENDENCIES})

# The main opengl framework for simulation and rendering
add_executable(main src/main.cpp)
target_link_libraries(main PUBLIC libmain)