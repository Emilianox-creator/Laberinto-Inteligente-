#include "AIPlayer.h"

#include <utility>

AIPlayer::AIPlayer(int id, std::string nombre, Position inicio, DFS::OrdenExploracion orden)
    : Player(id, std::move(nombre), inicio), dfs(orden)
{
}

Direction AIPlayer::decidirMovimiento(const Maze& laberinto)
{
    // La búsqueda empieza en el primer turno y no en el constructor, porque
    // al construir al jugador todavía no se conoce el laberinto.
    if (!busquedaIniciada)
    {
        dfs.iniciar(laberinto, posicion, laberinto.getSalida());
        busquedaIniciada = true;
    }
    if (dfs.haTerminado())
    {
        return Direction::NINGUNA;
    }
    const Position siguiente = dfs.siguientePaso(laberinto);
    return direccionHacia(posicion, siguiente);
}

void AIPlayer::reiniciar(Position inicio)
{
    Player::reiniciar(inicio);
    busquedaIniciada = false;
}

const DFS& AIPlayer::getDFS() const
{
    return dfs;
}
