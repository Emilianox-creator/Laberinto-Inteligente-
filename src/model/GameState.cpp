#include "GameState.h"

#include <utility>

// Maze no tiene constructor sin parámetros. En el menú todavía no hay
// laberinto, así que se usa uno de 1x1 que nuevaPartida reemplaza.
GameState::GameState() : laberinto(1, 1)
{
}

void GameState::nuevaPartida(GameMode nuevoModo, Maze nuevoLaberinto)
{
    modo = nuevoModo;
    laberinto = std::move(nuevoLaberinto);
    jugadores.clear();
    ganador = SIN_GANADOR;
    tiempo = 0.0f;
    fase = GamePhase::JUGANDO;
}

void GameState::agregarJugador(std::unique_ptr<Player> jugador)
{
    jugadores.push_back(std::move(jugador));
}

const Maze& GameState::getLaberinto() const
{
    return laberinto;
}

Player& GameState::getJugador(int indice)
{
    return *jugadores.at(static_cast<std::size_t>(indice));
}

const Player& GameState::getJugador(int indice) const
{
    return *jugadores.at(static_cast<std::size_t>(indice));
}

int GameState::getNumJugadores() const
{
    return static_cast<int>(jugadores.size());
}

GamePhase GameState::getFase() const
{
    return fase;
}

void GameState::setFase(GamePhase nuevaFase)
{
    fase = nuevaFase;
}

void GameState::declararGanador(int id)
{
    ganador = id;
    fase = GamePhase::TERMINADO;
}

int GameState::getGanador() const
{
    return ganador;
}

GameMode GameState::getModo() const
{
    return modo;
}

float GameState::getTiempo() const
{
    return tiempo;
}

void GameState::sumarTiempo(float dt)
{
    tiempo += dt;
}
