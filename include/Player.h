#pragma once

#include <string>

#include "Direction.h"
#include "Maze.h"
#include "Position.h"

// Clase abstracta: define lo que todo jugador sabe hacer (moverse por el
// laberinto y contar sus pasos), pero deja que cada tipo de jugador decida
// CÓMO elige su siguiente movimiento. Así Game puede tratar igual a humanos
// e IA (polimorfismo).
class Player
{
public:
    Player(int id, std::string nombre, Position inicio);

    // Virtual para que, al destruir un HumanPlayer o un AIPlayer a través de
    // un puntero a Player (como hace unique_ptr<Player>), se ejecute el
    // destructor de la clase hija y no solo el de Player.
    virtual ~Player() = default;

    // "= 0" la hace virtual pura: Player no se puede instanciar y cada clase
    // hija está obligada a implementarla.
    virtual Direction decidirMovimiento(const Maze& laberinto) = 0;

    // Mueve una celda si no hay muro. Devuelve false (y no cuenta el paso)
    // si el movimiento no es posible.
    bool mover(Direction direccion, const Maze& laberinto);

    // Virtual porque AIPlayer también necesita olvidar su búsqueda.
    virtual void reiniciar(Position inicio);

    int getId() const;
    const std::string& getNombre() const;
    Position getPosicion() const;
    int getPasos() const;

protected:
    int id;
    std::string nombre;
    Position posicion;
    int pasos = 0;
};
