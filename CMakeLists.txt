cmake_minimum_required(VERSION 3.21)
project(SortVisualizer)

set(CMAKE_CXX_STANDARD 20)

add_executable(SortVisualizer src/main.cpp src/SortHandler.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML-2.6.1/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio REQUIRED)

include_directories(C:/SFML-2.6.1/include)
target_link_libraries(SortVisualizer sfml-system sfml-window sfml-graphics sfml-audio)