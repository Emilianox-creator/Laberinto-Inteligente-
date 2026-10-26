cmake_minimum_required(VERSION 3.10)
project(LaberintoInteligente)

# Fijar el estándar en C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG        3.1.0 # Use a specific stable tag
  GIT_SHALLOW    ON
)
FetchContent_MakeAvailable(SFML)


# Crear el ejecutable apuntando a src/main.cpp
add_executable(laberinto src/main.cpp)

# Agregar la carpeta include/ como directorio de headers
target_include_directories(laberinto PRIVATE include)
target_link_libraries(laberinto PRIVATE SFML::Graphics SFML::Window SFML::System)


