#pragma once

#include <random>

#include "Maze.h"
#include "Position.h"

// Crea laberintos "perfectos": entre dos celdas cualesquiera hay exactamente un
// camino, es decir, los pasillos forman un árbol que cubre todas las celdas.
// Usa DFS aleatorio (recursive backtracker) con una pila explícita en lugar de
// recursión, para no desbordar la pila de llamadas en laberintos grandes.
class MazeGenerator
{
public:
    MazeGenerator() = default;

    // Usa una semilla nueva cada vez, así que cada partida es distinta.
    Maze generarLaberinto(int ancho, int alto);

    // Con la misma semilla sale el mismo laberinto, en la misma plataforma.
    // Sirve para pruebas y para repetir una partida.
    Maze generarLaberinto(int ancho, int alto, unsigned int semilla);

    unsigned int getUltimaSemilla() const;

private:
    void tallarPasillos(Maze& laberinto, Position inicio);
    void elegirEntradaYSalida(Maze& laberinto);
    bool validar(const Maze& laberinto) const;

    std::mt19937 rng;
    unsigned int ultimaSemilla = 0;
};
