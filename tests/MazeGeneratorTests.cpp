#include "DFS.h"
#include "Maze.h"
#include "MazeGenerator.h"
#include "Verificar.h"

using pruebas::verificar;

namespace
{

constexpr int ANCHO = 12;
constexpr int ALTO = 9;

// Cuenta cada pasaje una sola vez, mirando solo hacia la derecha y hacia abajo.
int contarPasajes(const Maze& laberinto)
{
    int pasajes = 0;
    for (int fila = 0; fila < laberinto.getAlto(); ++fila)
    {
        for (int columna = 0; columna < laberinto.getAncho(); ++columna)
        {
            pasajes += laberinto.puedeMover({fila, columna}, Direction::DERECHA) ? 1 : 0;
            pasajes += laberinto.puedeMover({fila, columna}, Direction::ABAJO) ? 1 : 0;
        }
    }
    return pasajes;
}

bool mismosMuros(const Maze& a, const Maze& b)
{
    for (int fila = 0; fila < a.getAlto(); ++fila)
    {
        for (int columna = 0; columna < a.getAncho(); ++columna)
        {
            for (Direction direccion : DIRECCIONES_MOVIMIENTO)
            {
                if (a.hayMuro({fila, columna}, direccion) != b.hayMuro({fila, columna}, direccion))
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void mismaSemillaMismoLaberinto()
{
    MazeGenerator generador;
    const Maze primero = generador.generarLaberinto(ANCHO, ALTO, 42);
    const Maze segundo = generador.generarLaberinto(ANCHO, ALTO, 42);
    verificar(mismosMuros(primero, segundo), "la misma semilla genera el mismo laberinto");
    verificar(generador.getUltimaSemilla() == 42, "getUltimaSemilla devuelve la semilla usada");
}

void elLaberintoEsPerfecto()
{
    MazeGenerator generador;
    const Maze laberinto = generador.generarLaberinto(ANCHO, ALTO, 7);

    // Un grafo conexo con V nodos y exactamente V - 1 aristas es un árbol:
    // no tiene ciclos y hay un único camino entre cada par de celdas.
    verificar(contarPasajes(laberinto) == ANCHO * ALTO - 1, "tiene exactamente celdas - 1 pasajes");

    DFS busqueda;
    bool todasAlcanzables = true;
    for (int fila = 0; fila < ALTO; ++fila)
    {
        for (int columna = 0; columna < ANCHO; ++columna)
        {
            todasAlcanzables =
                todasAlcanzables && !busqueda.buscarCamino(laberinto, laberinto.getEntrada(), {fila, columna}).empty();
        }
    }
    verificar(todasAlcanzables, "todas las celdas son alcanzables desde la entrada");
}

void entradaYSalidaEnEsquinasOpuestas()
{
    MazeGenerator generador;
    const Maze laberinto = generador.generarLaberinto(ANCHO, ALTO, 3);
    verificar(laberinto.getEntrada() == Position{0, 0}, "la entrada es la esquina superior izquierda");
    verificar(laberinto.getSalida() == Position{ALTO - 1, ANCHO - 1}, "la salida es la esquina inferior derecha");
}

void losBordesSiguenCerrados()
{
    MazeGenerator generador;
    const Maze laberinto = generador.generarLaberinto(ANCHO, ALTO, 99);
    bool bordesCerrados = true;
    for (int columna = 0; columna < ANCHO; ++columna)
    {
        bordesCerrados = bordesCerrados && laberinto.hayMuro({0, columna}, Direction::ARRIBA) &&
                         laberinto.hayMuro({ALTO - 1, columna}, Direction::ABAJO);
    }
    for (int fila = 0; fila < ALTO; ++fila)
    {
        bordesCerrados = bordesCerrados && laberinto.hayMuro({fila, 0}, Direction::IZQUIERDA) &&
                         laberinto.hayMuro({fila, ANCHO - 1}, Direction::DERECHA);
    }
    verificar(bordesCerrados, "el generador nunca abre los muros del borde");
}

void laberintoDeUnaCelda()
{
    MazeGenerator generador;
    const Maze laberinto = generador.generarLaberinto(1, 1, 5);
    verificar(laberinto.esSalida(laberinto.getEntrada()), "en un laberinto de 1x1 la entrada es la salida");
}

} // namespace

void ejecutarPruebasMazeGenerator()
{
    mismaSemillaMismoLaberinto();
    elLaberintoEsPerfecto();
    entradaYSalidaEnEsquinasOpuestas();
    losBordesSiguenCerrados();
    laberintoDeUnaCelda();
}
