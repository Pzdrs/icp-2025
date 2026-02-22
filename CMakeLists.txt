cmake_minimum_required(VERSION 3.10)
project(scuffcraft VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

find_package(OpenCV REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(nfd CONFIG REQUIRED)
find_package(OpenAL REQUIRED)

add_executable(scuffcraft ${SOURCES})

target_precompile_headers(scuffcraft PRIVATE ${PROJECT_SOURCE_DIR}/include/pch.hpp)
target_include_directories(scuffcraft PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(scuffcraft PRIVATE
    glad::glad
    glfw              
    glm::glm          
    imgui::imgui
    imguizmo::imguizmo
    nlohmann_json::nlohmann_json
    nfd::nfd
    OpenAL::OpenAL
)
