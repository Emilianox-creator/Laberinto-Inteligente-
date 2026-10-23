cmake_minimum_required(VERSION 3.10)
project(LaberintoInteligente)

# Fijar el estándar en C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Crear el ejecutable apuntando a src/main.cpp
add_executable(laberinto src/main.cpp)

# Agregar la carpeta include/ como directorio de headers
target_include_directories(laberinto PRIVATE include)