#pragma once

#include <array>

// Las cuatro direcciones en las que se puede mover un jugador, más NINGUNA
// para "no moverse este turno". Es enum class (y no enum simple) para que los
// nombres no choquen con otros identificadores y no se conviertan solos a int.
enum class Direction
{
    ARRIBA,
    ABAJO,
    IZQUIERDA,
    DERECHA,
    NINGUNA
};

// Solo las direcciones de movimiento real, para poder recorrerlas con un for.
inline constexpr std::array<Direction, 4> DIRECCIONES_MOVIMIENTO = {
    Direction::ARRIBA,
    Direction::ABAJO,
    Direction::IZQUIERDA,
    Direction::DERECHA,
};

// Se usa al abrir un pasaje: si una celda pierde su muro DERECHA, su vecina
// tiene que perder su muro IZQUIERDA para que el pasillo sea de ida y vuelta.
constexpr Direction opuesta(Direction direccion)
{
    switch (direccion)
    {
    case Direction::ARRIBA:
        return Direction::ABAJO;
    case Direction::ABAJO:
        return Direction::ARRIBA;
    case Direction::IZQUIERDA:
        return Direction::DERECHA;
    case Direction::DERECHA:
        return Direction::IZQUIERDA;
    case Direction::NINGUNA:
        return Direction::NINGUNA;
    }
    return Direction::NINGUNA;
}
