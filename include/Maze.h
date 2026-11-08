#pragma once

#include <vector>

#include "Cell.h"
#include "Direction.h"
#include "Position.h"

// Laberinto rectangular de ancho x alto celdas. Es parte del Model: solo guarda
// muros y responde preguntas sobre ellos; no sabe nada de SFML ni de jugadores.
class Maze
{
public:
    // Crea el laberinto con todas las celdas cerradas. Lanza
    // std::invalid_argument si el ancho o el alto no son positivos.
    Maze(int ancho, int alto);

    int getAncho() const;
    int getAlto() const;
    Position getEntrada() const;
    Position getSalida() const;
    void setEntradaSalida(Position entrada, Position salida);

    bool estaDentro(Position posicion) const;

    // Lanza std::out_of_range si la posición está fuera del laberinto y
    // std::invalid_argument si la dirección es NINGUNA.
    bool hayMuro(Position posicion, Direction direccion) const;

    // Versión "segura" de hayMuro: responde false en vez de lanzar, porque
    // para los jugadores un movimiento inválido simplemente no ocurre.
    bool puedeMover(Position posicion, Direction direccion) const;

    // Quita el muro entre la celda y su vecina, en las dos celdas. Lanza
    // std::out_of_range si el pasaje saldría del laberinto.
    void abrirPasaje(Position posicion, Direction direccion);

    bool esSalida(Position posicion) const;

private:
    // La matriz se guarda en un vector plano, fila por fila (row-major):
    // la celda (fila, columna) vive en fila * ancho + columna.
    int indice(Position posicion) const;
    Cell& celdaEn(Position posicion);
    const Cell& celdaEn(Position posicion) const;

    // Traducen una Direction al campo bool de Cell que le corresponde.
    static bool muroEn(const Cell& celda, Direction direccion);
    static void quitarMuro(Cell& celda, Direction direccion);

    int ancho;
    int alto;
    std::vector<Cell> celdas;
    Position entrada;
    Position salida;
};
