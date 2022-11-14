cmake_minimum_required(VERSION 3.23)
project(proyecto_final)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        glew
        URL https://sourceforge.net/projects/glew/files/glew/2.2.0/glew-2.2.0.zip
)

FetchContent_Declare(
        glfw3
        GIT_REPOSITORY https://github.com/glfw/glfw.git
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
)

FetchContent_Declare(
        soil2-debug
        GIT_REPOSITORY https://github.com/SpartanJ/SOIL2.git
)

FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
)

FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb
)

FetchContent_MakeAvailable(glew glfw3 glm soil2-debug assimp stb)

find_package(OpenGL REQUIRED)

include_directories(
        ${glew_SOURCE_DIR}/include
        ${glfw3_SOURCE_DIR}/include
        ${glm_SOURCE_DIR}
        ${soil2-debug_SOURCE_DIR}/src
        ${assimp_SOURCE_DIR}/include
        ${stb_SOURCE_DIR}
)

add_executable(proyecto_final MainPrueba.cpp Shader.h Camera.h Mesh.h Model.h meshAnim.h modelAnim.h Texture.h)

target_link_libraries(proyecto_final glfw assimp soil2-debug OpenGL::GL GLEW glm::glm)
