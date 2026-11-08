#pragma once

#include "Direction.h"

// Coordenada de una celda: fila 0 es la de arriba y columna 0 la de la izquierda,
// igual que al indexar una matriz.
struct Position
{
    int fila = 0;
    int columna = 0;

    constexpr bool operator==(const Position& otra) const
    {
        return fila == otra.fila && columna == otra.columna;
    }

    // En C++17 el operador != no se genera solo a partir de == (en C++20 sí).
    constexpr bool operator!=(const Position& otra) const
    {
        return !(*this == otra);
    }
};

// Celda vecina en la dirección indicada. No revisa si queda dentro del
// laberinto: eso le toca a Maze, que es quien conoce el tamaño.
constexpr Position desplazar(Position posicion, Direction direccion)
{
    switch (direccion)
    {
    case Direction::ARRIBA:
        return {posicion.fila - 1, posicion.columna};
    case Direction::ABAJO:
        return {posicion.fila + 1, posicion.columna};
    case Direction::IZQUIERDA:
        return {posicion.fila, posicion.columna - 1};
    case Direction::DERECHA:
        return {posicion.fila, posicion.columna + 1};
    case Direction::NINGUNA:
        return posicion;
    }
    return posicion;
}

// Lo inverso de desplazar: qué dirección lleva de "desde" a "hasta".
// Devuelve NINGUNA si no son vecinas, porque solo se avanza una celda a la vez.
constexpr Direction direccionHacia(Position desde, Position hasta)
{
    for (Direction direccion : DIRECCIONES_MOVIMIENTO)
    {
        if (desplazar(desde, direccion) == hasta)
        {
            return direccion;
        }
    }
    return Direction::NINGUNA;
}
