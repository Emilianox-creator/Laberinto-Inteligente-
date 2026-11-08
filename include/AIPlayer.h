#pragma once

#include <string>

#include "DFS.h"
#include "Direction.h"
#include "Maze.h"
#include "Player.h"
#include "Position.h"

// Jugador controlado por la computadora. En cada turno le pide un paso a su
// DFS y lo convierte en una Direction. Como el DFS retrocede físicamente, se
// ve a la IA entrar en callejones y regresar.
class AIPlayer : public Player
{
public:
    // El orden de exploración es opcional (no está en el UML): permite que
    // dos IA en el mismo laberinto no hagan exactamente el mismo recorrido.
    AIPlayer(int id, std::string nombre, Position inicio,
             DFS::OrdenExploracion orden = DIRECCIONES_MOVIMIENTO);

    // Quien la llama debe aplicar la dirección con mover(), porque el DFS ya
    // cuenta con que el jugador se movió a esa celda.
    Direction decidirMovimiento(const Maze& laberinto) override;

    void reiniciar(Position inicio) override;

    // Solo lectura: el Renderer la usa para dibujar las celdas exploradas.
    const DFS& getDFS() const;

private:
    DFS dfs;
    bool busquedaIniciada = false;
};
