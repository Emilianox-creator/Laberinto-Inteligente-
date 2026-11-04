#pragma once

#include <memory>
#include <vector>

#include "GameMode.h"
#include "GamePhase.h"
#include "Maze.h"
#include "Player.h"

// Todo lo que describe la partida en este momento: el laberinto, los
// jugadores, la fase, el tiempo y el ganador. Game lo modifica y Renderer
// solo lo lee.
class GameState
{
public:
    // Los id de los jugadores empiezan en 1, así que estos valores no chocan.
    static constexpr int SIN_GANADOR = -1;
    static constexpr int EMPATE = -2;

    GameState();

    // Reemplaza el laberinto, borra a los jugadores anteriores, reinicia el
    // tiempo y pasa a la fase JUGANDO.
    void nuevaPartida(GameMode modo, Maze laberinto);

    // GameState se vuelve dueño del jugador (por eso recibe un unique_ptr).
    void agregarJugador(std::unique_ptr<Player> jugador);

    const Maze& getLaberinto() const;

    // Lanzan std::out_of_range si el índice no existe.
    Player& getJugador(int indice);
    const Player& getJugador(int indice) const;
    int getNumJugadores() const;

    GamePhase getFase() const;
    void setFase(GamePhase fase);

    // Guarda el id del ganador (o EMPATE) y termina la partida.
    void declararGanador(int id);
    int getGanador() const;

    GameMode getModo() const;
    float getTiempo() const;
    void sumarTiempo(float dt);

private:
    GameMode modo = GameMode::MAQUINA_VS_MAQUINA;
    GamePhase fase = GamePhase::MENU;
    Maze laberinto;
    // Se guardan punteros porque Player es abstracta: un vector<Player> no
    // podría contener HumanPlayer ni AIPlayer.
    std::vector<std::unique_ptr<Player>> jugadores;
    int ganador = SIN_GANADOR;
    float tiempo = 0.0f;
};
