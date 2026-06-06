cmake_minimum_required(VERSION 3.15)
project(AlienHarvest)

set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR "C:\Users\oliwi\Desktop\SFML-2.6.2-windows-vc17-64-bit")

# szukamy sfml w wersji 2.x
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

# tu recznie dopisujemy kazdy plik (jak dojdzie cos nowego, trzeba tu dodac!)
add_executable(${PROJECT_NAME} 
    "src/main.cpp"
    "src/Player.cpp"
    "src/Bullet.cpp"
    "src/Alien.cpp"
    "include/GameObject.h"
    "include/Player.h"
    "include/Bullet.h"
    "include/Alien.h"
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)