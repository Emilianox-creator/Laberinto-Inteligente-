#pragma once

#include <string>

#include "Direction.h"
#include "Maze.h"
#include "Player.h"
#include "Position.h"

// Jugador controlado por una persona. No conoce el teclado ni SFML: Game
// traduce las teclas a una Direction y se la pasa con setDireccion. Así esta
// clase sigue siendo Model y se puede probar sin abrir una ventana.
class HumanPlayer : public Player
{
public:
    HumanPlayer(int id, std::string nombre, Position inicio);

    void setDireccion(Direction direccion);

    // Entrega la dirección pendiente y la "consume": cada tecla presionada
    // produce un solo paso.
    Direction decidirMovimiento(const Maze& laberinto) override;

private:
    Direction direccionPendiente = Direction::NINGUNA;
};
