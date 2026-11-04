#pragma once

#include <SFML/Window/Keyboard.hpp>

#include "GameMode.h"
#include "GameState.h"
#include "HumanPlayer.h"
#include "MazeGenerator.h"
#include "Renderer.h"

// Controller: conecta todo. Lee el teclado (SFML), modifica el GameState
// (Model) y le pide al Renderer (View) que lo dibuje, una vez por frame.
class Game
{
public:
    Game();

    // El game loop: eventos -> actualizar -> dibujar, hasta cerrar la ventana.
    void ejecutar();

    void iniciarPartida(GameMode modo);
    void procesarEventos();
    void actualizar(float dt);
    void verificarVictoria();
    void volverAlMenu();

private:
    void crearJugadores(GameMode modo);
    void procesarTecla(sf::Keyboard::Key tecla);
    // El primer humano usa las flechas y el segundo W A S D. Devuelve
    // nullptr si en este modo no hay tantos humanos.
    HumanPlayer* buscarHumano(int numeroDeHumano);

    GameState estado;
    MazeGenerator generador;
    Renderer renderer;
    // Tiempo acumulado desde el último paso de las IA (ver actualizar).
    float acumuladorIA = 0.0f;
};
