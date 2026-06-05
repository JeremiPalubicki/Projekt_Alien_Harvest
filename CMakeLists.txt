cmake_minimum_required(VERSION 3.15)
project(AlienHarvest)

set(CMAKE_CXX_STANDARD 17)

# szukamy sfml w wersji 2.x
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

# lapie wszystkie cpp i h, zeby nie wpisywac kazdego z osobna
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} "include/Bullet.h" "src/Bullet.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)