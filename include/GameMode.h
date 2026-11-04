#pragma once

// Quién juega la partida. Determina qué jugadores crea Game.
enum class GameMode
{
    MAQUINA_VS_MAQUINA,
    MAQUINA_VS_USUARIO,
    USUARIO_VS_USUARIO
};
