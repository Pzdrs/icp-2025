cmake_minimum_required(VERSION 3.10)
project(icp-fuckfest VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(icp-fuckfest
    src/main.cpp
    src/app.cpp
)

# Link OpenCV libraries
target_link_libraries(icp-fuckfest ${OpenCV_LIBS})