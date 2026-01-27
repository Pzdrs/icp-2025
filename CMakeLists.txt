cmake_minimum_required(VERSION 3.10)
project(icp-fuckfest VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

find_package(OpenCV REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(icp-fuckfest ${SOURCES})

target_include_directories(icp-fuckfest PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(icp-fuckfest PRIVATE
    GLEW::GLEW
    glfw              
    glm::glm          
)
