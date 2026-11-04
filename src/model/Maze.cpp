#include "Maze.h"

#include <stdexcept>

Maze::Maze(int ancho, int alto)
    : ancho(ancho), alto(alto), celdas(), entrada{0, 0}, salida{alto - 1, ancho - 1}
{
    if (ancho <= 0 || alto <= 0)
    {
        throw std::invalid_argument("Maze: el ancho y el alto deben ser positivos");
    }
    celdas.resize(static_cast<std::size_t>(ancho) * static_cast<std::size_t>(alto));
}

int Maze::getAncho() const
{
    return ancho;
}

int Maze::getAlto() const
{
    return alto;
}

Position Maze::getEntrada() const
{
    return entrada;
}

Position Maze::getSalida() const
{
    return salida;
}

void Maze::setEntradaSalida(Position nuevaEntrada, Position nuevaSalida)
{
    if (!estaDentro(nuevaEntrada) || !estaDentro(nuevaSalida))
    {
        throw std::out_of_range("Maze: la entrada y la salida deben estar dentro del laberinto");
    }
    entrada = nuevaEntrada;
    salida = nuevaSalida;
}

bool Maze::estaDentro(Position posicion) const
{
    return posicion.fila >= 0 && posicion.fila < alto && posicion.columna >= 0 &&
           posicion.columna < ancho;
}

bool Maze::hayMuro(Position posicion, Direction direccion) const
{
    if (!estaDentro(posicion))
    {
        throw std::out_of_range("Maze::hayMuro: posición fuera del laberinto");
    }
    return muroEn(celdaEn(posicion), direccion);
}

bool Maze::puedeMover(Position posicion, Direction direccion) const
{
    if (!estaDentro(posicion) || direccion == Direction::NINGUNA)
    {
        return false;
    }
    // Los muros del borde nunca se abren, pero revisar también el destino
    // evita depender de eso.
    return !hayMuro(posicion, direccion) && estaDentro(desplazar(posicion, direccion));
}

void Maze::abrirPasaje(Position posicion, Direction direccion)
{
    if (direccion == Direction::NINGUNA)
    {
        throw std::invalid_argument("Maze::abrirPasaje: la dirección no puede ser NINGUNA");
    }
    const Position vecina = desplazar(posicion, direccion);
    if (!estaDentro(posicion) || !estaDentro(vecina))
    {
        throw std::out_of_range("Maze::abrirPasaje: no se puede abrir un pasaje hacia afuera");
    }
    // Cada celda guarda sus cuatro muros, así que el muro compartido existe
    // dos veces: hay que quitarlo en ambas para que sean consistentes.
    quitarMuro(celdaEn(posicion), direccion);
    quitarMuro(celdaEn(vecina), opuesta(direccion));
}

bool Maze::esSalida(Position posicion) const
{
    return posicion == salida;
}

int Maze::indice(Position posicion) const
{
    return posicion.fila * ancho + posicion.columna;
}

Cell& Maze::celdaEn(Position posicion)
{
    return celdas[static_cast<std::size_t>(indice(posicion))];
}

const Cell& Maze::celdaEn(Position posicion) const
{
    return celdas[static_cast<std::size_t>(indice(posicion))];
}

bool Maze::muroEn(const Cell& celda, Direction direccion)
{
    switch (direccion)
    {
    case Direction::ARRIBA:
        return celda.muroArriba;
    case Direction::ABAJO:
        return celda.muroAbajo;
    case Direction::IZQUIERDA:
        return celda.muroIzquierda;
    case Direction::DERECHA:
        return celda.muroDerecha;
    case Direction::NINGUNA:
        break;
    }
    throw std::invalid_argument("Maze: NINGUNA no corresponde a ningún muro");
}

void Maze::quitarMuro(Cell& celda, Direction direccion)
{
    switch (direccion)
    {
    case Direction::ARRIBA:
        celda.muroArriba = false;
        return;
    case Direction::ABAJO:
        celda.muroAbajo = false;
        return;
    case Direction::IZQUIERDA:
        celda.muroIzquierda = false;
        return;
    case Direction::DERECHA:
        celda.muroDerecha = false;
        return;
    case Direction::NINGUNA:
        break;
    }
    throw std::invalid_argument("Maze: NINGUNA no corresponde a ningún muro");
}
