#include <cstdlib>
#include <iostream>

#include "Verificar.h"

// Cada archivo *Tests.cpp define una de estas funciones.
void ejecutarPruebasMaze();
void ejecutarPruebasDFS();
void ejecutarPruebasMazeGenerator();
void ejecutarPruebasPlayer();
void ejecutarPruebasGameState();

int main()
{
    std::cout << "Probando Maze...\n";
    ejecutarPruebasMaze();
    std::cout << "Probando DFS...\n";
    ejecutarPruebasDFS();
    std::cout << "Probando MazeGenerator...\n";
    ejecutarPruebasMazeGenerator();
    std::cout << "Probando Player, HumanPlayer y AIPlayer...\n";
    ejecutarPruebasPlayer();
    std::cout << "Probando GameState...\n";
    ejecutarPruebasGameState();

    std::cout << pruebas::totalVerificaciones << " verificaciones, " << pruebas::totalFallas
              << " fallas\n";

    // CTest decide si la prueba pasó según el código de salida del programa.
    return pruebas::totalFallas == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
