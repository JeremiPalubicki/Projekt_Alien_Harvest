cmake_minimum_required(VERSION 3.15)
project(AlienHarvest)

set(CMAKE_CXX_STANDARD 17)

# Prowadzacy: Tutaj prosimy podac wlasna sciezke do zainstalowanej biblioteki SFML
# set(SFML_DIR "C:/SFML-2.6.2/lib/cmake/SFML")

# Szukamy sfml w wersji 2.x
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

# Pliki zrodlowe projektu
add_executable(${PROJECT_NAME} 
    "src/main.cpp"
    "src/Player.cpp"
    "src/Bullet.cpp"
    "src/Alien.cpp"
    "include/GameObject.h"
    "include/Player.h"
    "include/Bullet.h"
    "include/Alien.h"
    "include/Runner.h" 
    "src/Runner.cpp" 
    "include/Boss.h" 
    "src/Boss.cpp"
    "include/AlienBullet.h" 
    "src/AlienBullet.cpp" 
    "include/Medkit.h" 
    "src/Medkit.cpp"
    "src/Obstacle.cpp"
    "include/Obstacle.h"
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# AUTOMATYCZNE KOPIOWANIE ZASOBÓW (Zapewnia działanie gry na każdym komputerze)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

