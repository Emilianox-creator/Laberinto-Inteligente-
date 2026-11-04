#include "Player.h"

#include <utility>

Player::Player(int id, std::string nombre, Position inicio)
    : id(id), nombre(std::move(nombre)), posicion(inicio)
{
}

bool Player::mover(Direction direccion, const Maze& laberinto)
{
    if (!laberinto.puedeMover(posicion, direccion))
    {
        return false;
    }
    posicion = desplazar(posicion, direccion);
    ++pasos;
    return true;
}

void Player::reiniciar(Position inicio)
{
    posicion = inicio;
    pasos = 0;
}

int Player::getId() const
{
    return id;
}

const std::string& Player::getNombre() const
{
    return nombre;
}

Position Player::getPosicion() const
{
    return posicion;
}

int Player::getPasos() const
{
    return pasos;
}
