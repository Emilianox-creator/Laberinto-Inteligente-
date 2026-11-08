#include <cstdlib>
#include <exception>
#include <iostream>

#include "Game.h"

// La ventana, el límite de FPS y el bucle de eventos que estaban aquí ahora
// viven en Renderer (crear y dibujar la ventana) y en Game (el game loop).
int main()
{
    try
    {
        Game juego;
        juego.ejecutar();
    }
    catch (const std::exception& error)
    {
        // Por ejemplo, si falta la fuente: mejor un mensaje claro que un
        // cierre sin explicación.
        std::cerr << "Error: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
