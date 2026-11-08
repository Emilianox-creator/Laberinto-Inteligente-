#pragma once

// En qué pantalla está el juego. Game y Renderer deciden qué hacer y qué
// dibujar según la fase (una máquina de estados muy simple).
enum class GamePhase
{
    MENU,
    JUGANDO,
    TERMINADO
};
