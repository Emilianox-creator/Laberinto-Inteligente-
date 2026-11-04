#include <stdexcept>
#include <vector>

#include "DFS.h"
#include "Maze.h"
#include "Verificar.h"

using pruebas::verificar;
using pruebas::verificarLanza;

namespace
{

// Laberinto de 2x2 hecho a mano, con forma de "U" acostada:
//
//   (0,0) - (0,1)        (0,1) es un callejón sin salida.
//     |
//   (1,0) - (1,1)        (1,1) es la meta.
Maze laberintoConCallejon()
{
    Maze laberinto(2, 2);
    laberinto.abrirPasaje({0, 0}, Direction::DERECHA);
    laberinto.abrirPasaje({0, 0}, Direction::ABAJO);
    laberinto.abrirPasaje({1, 0}, Direction::DERECHA);
    return laberinto;
}

void encuentraElCaminoEnUnPasillo()
{
    Maze laberinto(3, 1);
    laberinto.abrirPasaje({0, 0}, Direction::DERECHA);
    laberinto.abrirPasaje({0, 1}, Direction::DERECHA);

    DFS busqueda;
    const std::vector<Position> camino = busqueda.buscarCamino(laberinto, {0, 0}, {0, 2});
    const std::vector<Position> esperado = {{0, 0}, {0, 1}, {0, 2}};
    verificar(camino == esperado, "en un pasillo recto el camino pasa por todas las celdas");
    verificar(busqueda.haTerminado() && busqueda.encontroSalida(), "la búsqueda termina encontrando la meta");
}

void retrocedeDesdeUnCallejon()
{
    const Maze laberinto = laberintoConCallejon();
    // Probar DERECHA primero obliga a entrar al callejón (0,1) y regresar.
    DFS busqueda({Direction::DERECHA, Direction::ABAJO, Direction::IZQUIERDA, Direction::ARRIBA});
    busqueda.iniciar(laberinto, {0, 0}, {1, 1});

    std::vector<Position> pasos;
    while (!busqueda.haTerminado())
    {
        pasos.push_back(busqueda.siguientePaso(laberinto));
    }

    const std::vector<Position> pasosEsperados = {{0, 1}, {0, 0}, {1, 0}, {1, 1}};
    verificar(pasos == pasosEsperados, "entra al callejón, retrocede (pop) y sigue por abajo");

    const std::vector<Position> caminoEsperado = {{0, 0}, {1, 0}, {1, 1}};
    verificar(busqueda.getCamino() == caminoEsperado, "el callejón ya no forma parte del camino");
    verificar(busqueda.estaVisitada({0, 1}), "el callejón queda marcado como visitado");
}

void cadaPasoEsAUnaCeldaVecina()
{
    const Maze laberinto = laberintoConCallejon();
    DFS busqueda;
    busqueda.iniciar(laberinto, {0, 1}, {1, 1});

    bool todosVecinos = true;
    Position anterior = busqueda.posicionActual();
    while (!busqueda.haTerminado())
    {
        const Position siguiente = busqueda.siguientePaso(laberinto);
        todosVecinos = todosVecinos && direccionHacia(anterior, siguiente) != Direction::NINGUNA;
        anterior = siguiente;
    }
    verificar(todosVecinos, "siguientePaso siempre avanza o retrocede una sola celda");
}

void metaInalcanzableTerminaSinCamino()
{
    const Maze laberinto(2, 1); // sin pasajes: la meta está encerrada
    DFS busqueda;
    verificar(busqueda.buscarCamino(laberinto, {0, 0}, {0, 1}).empty(),
              "si la meta no es alcanzable el camino está vacío");
    verificar(busqueda.haTerminado() && !busqueda.encontroSalida(), "termina sin encontrar la salida");
    verificar(busqueda.posicionActual() == Position{0, 0}, "el agente se queda en el inicio");
}

void inicioIgualAMetaYaTermino()
{
    const Maze laberinto(2, 2);
    DFS busqueda;
    const std::vector<Position> camino = busqueda.buscarCamino(laberinto, {1, 1}, {1, 1});
    verificar(camino == std::vector<Position>{{1, 1}}, "si ya está en la meta el camino es solo esa celda");
}

void usoIncorrectoLanzaExcepcion()
{
    const Maze laberinto(2, 2);
    DFS busqueda;
    verificarLanza<std::logic_error>([&] { (void)busqueda.posicionActual(); },
                                     "posicionActual antes de iniciar lanza logic_error");
    verificarLanza<std::logic_error>([&] { (void)busqueda.siguientePaso(laberinto); },
                                     "siguientePaso antes de iniciar lanza logic_error");
    verificarLanza<std::out_of_range>([&] { busqueda.iniciar(laberinto, {0, 0}, {9, 9}); },
                                      "una meta fuera del laberinto lanza out_of_range");
    verificar(!busqueda.estaVisitada({0, 0}), "sin iniciar, ninguna celda está visitada");
}

} // namespace

void ejecutarPruebasDFS()
{
    encuentraElCaminoEnUnPasillo();
    retrocedeDesdeUnCallejon();
    cadaPasoEsAUnaCeldaVecina();
    metaInalcanzableTerminaSinCamino();
    inicioIgualAMetaYaTermino();
    usoIncorrectoLanzaExcepcion();
}
