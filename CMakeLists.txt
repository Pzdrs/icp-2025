cmake_minimum_required(VERSION 3.10)
project(icp-fuckfest VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(icp-fuckfest
    src/main.cpp
)

# Link libraries using vcpkg targets
target_link_libraries(icp-fuckfest PRIVATE ${OpenCV_LIBS} GLEW::GLEW glfw glm::glm)