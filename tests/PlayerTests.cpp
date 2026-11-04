#include "AIPlayer.h"
#include "HumanPlayer.h"
#include "Maze.h"
#include "MazeGenerator.h"
#include "Verificar.h"

using pruebas::verificar;

namespace
{

void humanoConsumeLaDireccionPendiente()
{
    const Maze laberinto(2, 2);
    HumanPlayer humano(1, "Ana", {0, 0});
    verificar(humano.decidirMovimiento(laberinto) == Direction::NINGUNA, "sin tecla no hay movimiento");

    humano.setDireccion(Direction::DERECHA);
    verificar(humano.decidirMovimiento(laberinto) == Direction::DERECHA, "entrega la dirección pendiente");
    verificar(humano.decidirMovimiento(laberinto) == Direction::NINGUNA, "cada tecla produce un solo paso");
}

void moverRespetaLosMuros()
{
    Maze laberinto(2, 1);
    HumanPlayer humano(1, "Ana", {0, 0});

    verificar(!humano.mover(Direction::DERECHA, laberinto), "no atraviesa un muro");
    verificar(humano.getPosicion() == Position{0, 0} && humano.getPasos() == 0,
              "un movimiento fallido no cambia la posición ni cuenta pasos");

    laberinto.abrirPasaje({0, 0}, Direction::DERECHA);
    verificar(humano.mover(Direction::DERECHA, laberinto), "se mueve si hay pasaje");
    verificar(humano.getPosicion() == Position{0, 1} && humano.getPasos() == 1, "avanza una celda y cuenta el paso");

    humano.reiniciar({0, 0});
    verificar(humano.getPosicion() == Position{0, 0} && humano.getPasos() == 0, "reiniciar regresa al inicio");
}

// Simula lo que hace Game: pedir una dirección y aplicarla, hasta llegar.
bool iaLlegaALaSalida(AIPlayer& ia, const Maze& laberinto, int limiteDeTurnos)
{
    for (int turno = 0; turno < limiteDeTurnos; ++turno)
    {
        if (laberinto.esSalida(ia.getPosicion()))
        {
            return true;
        }
        const Direction direccion = ia.decidirMovimiento(laberinto);
        if (direccion != Direction::NINGUNA && !ia.mover(direccion, laberinto))
        {
            return false; // la IA pidió atravesar un muro
        }
    }
    return laberinto.esSalida(ia.getPosicion());
}

void iaResuelveUnLaberintoGenerado()
{
    MazeGenerator generador;
    const Maze laberinto = generador.generarLaberinto(15, 10, 2024);
    const int celdas = laberinto.getAncho() * laberinto.getAlto();

    AIPlayer ia(1, "IA", laberinto.getEntrada());
    // Con DFS cada celda se pisa a lo más al entrar y al regresar.
    verificar(iaLlegaALaSalida(ia, laberinto, 2 * celdas), "la IA llega a la salida sin atravesar muros");
    verificar(ia.getPasos() <= 2 * (celdas - 1), "la IA no da más de 2 * (celdas - 1) pasos");
    verificar(ia.getDFS().encontroSalida(), "su DFS reporta que encontró la salida");
    verificar(ia.decidirMovimiento(laberinto) == Direction::NINGUNA, "al terminar ya no se mueve");
}

void iaReiniciadaVuelveABuscar()
{
    MazeGenerator generador;
    const Maze laberinto = generador.generarLaberinto(6, 6, 11);
    AIPlayer ia(2, "IA", laberinto.getEntrada());
    iaLlegaALaSalida(ia, laberinto, 100);

    ia.reiniciar(laberinto.getEntrada());
    verificar(ia.getPasos() == 0, "reiniciar borra los pasos");
    verificar(iaLlegaALaSalida(ia, laberinto, 100), "después de reiniciar vuelve a resolver el laberinto");
}

} // namespace

void ejecutarPruebasPlayer()
{
    humanoConsumeLaDireccionPendiente();
    moverRespetaLosMuros();
    iaResuelveUnLaberintoGenerado();
    iaReiniciadaVuelveABuscar();
}
