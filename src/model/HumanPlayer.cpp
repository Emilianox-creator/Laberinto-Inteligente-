#include "HumanPlayer.h"

#include <utility>

HumanPlayer::HumanPlayer(int id, std::string nombre, Position inicio)
    : Player(id, std::move(nombre), inicio)
{
}

void HumanPlayer::setDireccion(Direction direccion)
{
    direccionPendiente = direccion;
}

// El humano decide con el teclado, no mirando el laberinto; por eso el
// parámetro no tiene nombre (así el compilador no avisa que no se usa).
Direction HumanPlayer::decidirMovimiento(const Maze& /*laberinto*/)
{
    const Direction direccion = direccionPendiente;
    direccionPendiente = Direction::NINGUNA;
    return direccion;
}
