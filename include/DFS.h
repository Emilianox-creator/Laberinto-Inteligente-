#pragma once

#include <array>
#include <stack>
#include <vector>

#include "Direction.h"
#include "Maze.h"
#include "Position.h"

// Búsqueda en profundidad (Depth-First Search) que resuelve el laberinto UN
// PASO A LA VEZ, para que AIPlayer avance una celda por turno y se vea cómo
// explora. La cima de la pila es la celda donde "está" el agente: avanzar a
// una vecina nueva es push y retroceder desde un callejón sin salida es pop.
//
// Cada celda entra a la pila una sola vez y sale a lo más una vez, así que la
// búsqueda completa toma O(celdas) pasos y O(celdas) memoria.
class DFS
{
public:
    using OrdenExploracion = std::array<Direction, 4>;

    // El orden en que se prueban las vecinas cambia el recorrido: dos IA con
    // órdenes distintos exploran el mismo laberinto de forma diferente.
    explicit DFS(OrdenExploracion orden = DIRECCIONES_MOVIMIENTO);

    // Prepara una búsqueda nueva. Lanza std::out_of_range si el inicio o la
    // meta están fuera del laberinto.
    void iniciar(const Maze& laberinto, Position inicio, Position meta);

    // Avanza un paso y devuelve la nueva posición del agente, que siempre es
    // vecina de la anterior (o la misma, si la búsqueda ya terminó). Hay que
    // pasarle el mismo laberinto que a iniciar. Lanza std::logic_error si no
    // se llamó a iniciar antes.
    Position siguientePaso(const Maze& laberinto);

    // Lanza std::logic_error si no se llamó a iniciar antes.
    Position posicionActual() const;
    bool haTerminado() const;
    bool encontroSalida() const;
    bool estaVisitada(Position posicion) const;

    // El camino desde el inicio hasta la posición actual (el contenido de la
    // pila, de la base a la cima).
    std::vector<Position> getCamino() const;

    // Corre la búsqueda completa de una vez. Devuelve el camino del inicio al
    // fin, o un vector vacío si el fin no es alcanzable.
    std::vector<Position> buscarCamino(const Maze& laberinto, Position inicio, Position fin);

private:
    int indice(Position posicion) const;
    void visitar(Position posicion);

    OrdenExploracion orden;
    std::stack<Position> pila;
    std::vector<bool> visitadas;
    Position meta;
    int anchoLaberinto = 0;
    int altoLaberinto = 0;
    bool terminado = false;
    bool encontrada = false;
};
