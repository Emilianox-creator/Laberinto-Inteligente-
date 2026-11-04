cmake_minimum_required(VERSION 3.28) # SFML 3.1.0 exige CMake 3.28 o mayor
project(LaberintoInteligente LANGUAGES CXX)

# Fijar el estándar en C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Sin extensiones propias del compilador, para que el código sea C++ estándar
# y compile igual en clang (Mac), gcc (Linux) y MSVC (Windows).
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG        3.1.0 # Use a specific stable tag
  GIT_SHALLOW    ON
)
FetchContent_MakeAvailable(SFML)

# Advertencias del compilador para nuestro código (no para el de SFML).
if(MSVC)
  set(OPCIONES_ADVERTENCIAS /W4)
else()
  set(OPCIONES_ADVERTENCIAS -Wall -Wextra -Wpedantic)
endif()

# Model: la lógica del juego como biblioteca aparte que NO se enlaza con SFML.
# Así los headers de SFML no están disponibles en el modelo y CMake ayuda a
# cumplir la regla del UML. También permite probar el modelo sin abrir ventanas.
add_library(laberinto_modelo STATIC
  src/model/Maze.cpp
)
target_include_directories(laberinto_modelo PUBLIC include)
target_compile_options(laberinto_modelo PRIVATE ${OPCIONES_ADVERTENCIAS})

# El juego: View + Controller, que sí usan SFML.
add_executable(laberinto src/main.cpp)
target_link_libraries(laberinto PRIVATE laberinto_modelo SFML::Graphics SFML::Window SFML::System)
target_compile_options(laberinto PRIVATE ${OPCIONES_ADVERTENCIAS})

# Pruebas del modelo. Se corren con: ctest --test-dir build --output-on-failure
enable_testing()
add_executable(pruebas_modelo
  tests/main.cpp
  tests/MazeTests.cpp
)
target_link_libraries(pruebas_modelo PRIVATE laberinto_modelo)
target_compile_options(pruebas_modelo PRIVATE ${OPCIONES_ADVERTENCIAS})
add_test(NAME pruebas_modelo COMMAND pruebas_modelo)
