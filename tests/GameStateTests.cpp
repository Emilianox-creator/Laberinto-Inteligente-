#include <memory>
#include <stdexcept>

#include "GameState.h"
#include "HumanPlayer.h"
#include "Maze.h"
#include "Verificar.h"

using pruebas::verificar;
using pruebas::verificarLanza;

namespace
{

void empiezaEnElMenu()
{
    const GameState estado;
    verificar(estado.getFase() == GamePhase::MENU, "un GameState nuevo está en el menú");
    verificar(estado.getGanador() == GameState::SIN_GANADOR, "al inicio no hay ganador");
    verificar(estado.getNumJugadores() == 0, "al inicio no hay jugadores");
}

void nuevaPartidaReiniciaTodo()
{
    GameState estado;
    estado.nuevaPartida(GameMode::USUARIO_VS_USUARIO, Maze(5, 4));
    estado.agregarJugador(std::make_unique<HumanPlayer>(1, "Ana", Position{0, 0}));
    estado.sumarTiempo(3.5f);
    estado.declararGanador(1);

    estado.nuevaPartida(GameMode::MAQUINA_VS_USUARIO, Maze(7, 3));
    verificar(estado.getFase() == GamePhase::JUGANDO, "nuevaPartida pasa a JUGANDO");
    verificar(estado.getModo() == GameMode::MAQUINA_VS_USUARIO, "nuevaPartida guarda el modo");
    verificar(estado.getLaberinto().getAncho() == 7, "nuevaPartida reemplaza el laberinto");
    verificar(estado.getNumJugadores() == 0, "nuevaPartida borra a los jugadores anteriores");
    verificar(estado.getTiempo() == 0.0f, "nuevaPartida reinicia el tiempo");
    verificar(estado.getGanador() == GameState::SIN_GANADOR, "nuevaPartida borra al ganador");
}

void guardaJugadoresYGanador()
{
    GameState estado;
    estado.nuevaPartida(GameMode::USUARIO_VS_USUARIO, Maze(3, 3));
    estado.agregarJugador(std::make_unique<HumanPlayer>(1, "Ana", Position{0, 0}));
    estado.agregarJugador(std::make_unique<HumanPlayer>(2, "Luis", Position{0, 0}));

    verificar(estado.getNumJugadores() == 2, "cuenta a los jugadores agregados");
    verificar(estado.getJugador(1).getNombre() == "Luis", "getJugador devuelve al jugador por índice");
    verificarLanza<std::out_of_range>([&] { (void)estado.getJugador(2); },
                                      "un índice que no existe lanza out_of_range");

    estado.declararGanador(GameState::EMPATE);
    verificar(estado.getGanador() == GameState::EMPATE, "puede declararse un empate");
    verificar(estado.getFase() == GamePhase::TERMINADO, "declarar ganador termina la partida");
}

} // namespace

void ejecutarPruebasGameState()
{
    empiezaEnElMenu();
    nuevaPartidaReiniciaTodo();
    guardaJugadoresYGanador();
}
