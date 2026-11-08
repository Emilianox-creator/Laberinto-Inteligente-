#include <stdexcept>

#include "Maze.h"
#include "Verificar.h"

using pruebas::verificar;
using pruebas::verificarLanza;

namespace
{

void laberintoNuevoTieneTodosLosMuros()
{
    const Maze laberinto(3, 2);
    verificar(laberinto.getAncho() == 3 && laberinto.getAlto() == 2, "Maze guarda ancho y alto");

    bool todasCerradas = true;
    for (int fila = 0; fila < laberinto.getAlto(); ++fila)
    {
        for (int columna = 0; columna < laberinto.getAncho(); ++columna)
        {
            for (Direction direccion : DIRECCIONES_MOVIMIENTO)
            {
                todasCerradas = todasCerradas && laberinto.hayMuro({fila, columna}, direccion) &&
                                !laberinto.puedeMover({fila, columna}, direccion);
            }
        }
    }
    verificar(todasCerradas, "un Maze nuevo tiene los cuatro muros en todas sus celdas");
}

void entradaYSalidaPorDefectoSonEsquinas()
{
    const Maze laberinto(4, 3);
    verificar(laberinto.getEntrada() == Position{0, 0}, "la entrada por defecto es (0, 0)");
    verificar(laberinto.getSalida() == Position{2, 3}, "la salida por defecto es la esquina opuesta");
    verificar(laberinto.esSalida({2, 3}) && !laberinto.esSalida({0, 0}), "esSalida compara con la salida");
}

void abrirPasajeQuitaElMuroEnAmbasCeldas()
{
    Maze laberinto(2, 2);
    laberinto.abrirPasaje({0, 0}, Direction::DERECHA);

    verificar(!laberinto.hayMuro({0, 0}, Direction::DERECHA), "se quita el muro de la celda origen");
    verificar(!laberinto.hayMuro({0, 1}, Direction::IZQUIERDA), "se quita el muro de la vecina");
    verificar(laberinto.puedeMover({0, 0}, Direction::DERECHA), "se puede ir hacia la vecina");
    verificar(laberinto.puedeMover({0, 1}, Direction::IZQUIERDA), "y se puede regresar");
    verificar(laberinto.hayMuro({0, 0}, Direction::ABAJO), "los demás muros siguen en su lugar");
}

void noSePuedeSalirDelLaberinto()
{
    Maze laberinto(2, 2);
    verificar(!laberinto.puedeMover({0, 0}, Direction::ARRIBA), "no se puede mover fuera por arriba");
    verificar(!laberinto.puedeMover({5, 5}, Direction::ABAJO), "puedeMover responde false fuera del laberinto");
    verificar(!laberinto.puedeMover({0, 0}, Direction::NINGUNA), "NINGUNA nunca es un movimiento");

    verificarLanza<std::out_of_range>([&] { laberinto.abrirPasaje({0, 0}, Direction::IZQUIERDA); },
                                      "abrir un pasaje hacia afuera lanza out_of_range");
    verificarLanza<std::out_of_range>([&] { (void)laberinto.hayMuro({-1, 0}, Direction::ABAJO); },
                                      "hayMuro fuera del laberinto lanza out_of_range");
    verificarLanza<std::invalid_argument>([&] { laberinto.abrirPasaje({0, 0}, Direction::NINGUNA); },
                                          "abrir un pasaje con NINGUNA lanza invalid_argument");
    verificarLanza<std::out_of_range>([&] { laberinto.setEntradaSalida({0, 0}, {2, 0}); },
                                      "una salida fuera del laberinto lanza out_of_range");
}

void tamanoInvalidoLanzaExcepcion()
{
    verificarLanza<std::invalid_argument>([] { Maze laberinto(0, 5); }, "ancho 0 lanza invalid_argument");
    verificarLanza<std::invalid_argument>([] { Maze laberinto(5, -1); }, "alto negativo lanza invalid_argument");
}

void funcionesDePositionYDirection()
{
    verificar(desplazar({2, 2}, Direction::ARRIBA) == Position{1, 2}, "ARRIBA resta una fila");
    verificar(desplazar({2, 2}, Direction::DERECHA) == Position{2, 3}, "DERECHA suma una columna");
    verificar(desplazar({2, 2}, Direction::NINGUNA) == Position{2, 2}, "NINGUNA no mueve");
    verificar(opuesta(Direction::IZQUIERDA) == Direction::DERECHA, "la opuesta de IZQUIERDA es DERECHA");
    verificar(direccionHacia({1, 1}, {2, 1}) == Direction::ABAJO, "direccionHacia encuentra la vecina de abajo");
    verificar(direccionHacia({1, 1}, {3, 1}) == Direction::NINGUNA, "direccionHacia da NINGUNA si no son vecinas");
}

} // namespace

void ejecutarPruebasMaze()
{
    laberintoNuevoTieneTodosLosMuros();
    entradaYSalidaPorDefectoSonEsquinas();
    abrirPasajeQuitaElMuroEnAmbasCeldas();
    noSePuedeSalirDelLaberinto();
    tamanoInvalidoLanzaExcepcion();
    funcionesDePositionYDirection();
}
