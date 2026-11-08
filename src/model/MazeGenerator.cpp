#include "MazeGenerator.h"

#include <stack>
#include <stdexcept>
#include <vector>

#include "DFS.h"

Maze MazeGenerator::generarLaberinto(int ancho, int alto)
{
    std::random_device fuenteDeSemillas;
    return generarLaberinto(ancho, alto, fuenteDeSemillas());
}

Maze MazeGenerator::generarLaberinto(int ancho, int alto, unsigned int semilla)
{
    ultimaSemilla = semilla;
    rng.seed(semilla);

    Maze laberinto(ancho, alto);
    tallarPasillos(laberinto, {0, 0});
    elegirEntradaYSalida(laberinto);

    if (!validar(laberinto))
    {
        // Un laberinto perfecto siempre tiene solución; si no, es un bug aquí.
        throw std::logic_error("MazeGenerator: el laberinto generado no tiene solución");
    }
    return laberinto;
}

unsigned int MazeGenerator::getUltimaSemilla() const
{
    return ultimaSemilla;
}

void MazeGenerator::tallarPasillos(Maze& laberinto, Position inicio)
{
    const int ancho = laberinto.getAncho();
    std::vector<bool> visitadas(static_cast<std::size_t>(ancho) * static_cast<std::size_t>(laberinto.getAlto()),
                                false);
    auto indice = [ancho](Position p) { return static_cast<std::size_t>(p.fila * ancho + p.columna); };

    std::stack<Position> pila;
    pila.push(inicio);
    visitadas[indice(inicio)] = true;

    while (!pila.empty())
    {
        const Position actual = pila.top();

        std::vector<Direction> haciaNoVisitadas;
        for (Direction direccion : DIRECCIONES_MOVIMIENTO)
        {
            const Position vecina = desplazar(actual, direccion);
            if (laberinto.estaDentro(vecina) && !visitadas[indice(vecina)])
            {
                haciaNoVisitadas.push_back(direccion);
            }
        }

        if (haciaNoVisitadas.empty())
        {
            pila.pop(); // callejón sin salida: retroceder
            continue;
        }

        // Elegir al azar es lo que hace que cada laberinto sea distinto. Ojo:
        // uniform_int_distribution puede dar números distintos en cada
        // compilador, así que una semilla solo se repite en la misma plataforma.
        std::uniform_int_distribution<std::size_t> elegir(0, haciaNoVisitadas.size() - 1);
        const Direction direccion = haciaNoVisitadas[elegir(rng)];
        const Position vecina = desplazar(actual, direccion);

        laberinto.abrirPasaje(actual, direccion);
        visitadas[indice(vecina)] = true;
        pila.push(vecina);
    }
}

void MazeGenerator::elegirEntradaYSalida(Maze& laberinto)
{
    // Esquinas opuestas: así el camino de la entrada a la salida cruza todo
    // el laberinto y la carrera dura más.
    laberinto.setEntradaSalida({0, 0}, {laberinto.getAlto() - 1, laberinto.getAncho() - 1});
}

bool MazeGenerator::validar(const Maze& laberinto) const
{
    DFS busqueda;
    return !busqueda.buscarCamino(laberinto, laberinto.getEntrada(), laberinto.getSalida()).empty();
}
