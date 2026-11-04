#include "DFS.h"

#include <algorithm>
#include <stdexcept>

DFS::DFS(OrdenExploracion orden) : orden(orden)
{
}

void DFS::iniciar(const Maze& laberinto, Position inicio, Position nuevaMeta)
{
    if (!laberinto.estaDentro(inicio) || !laberinto.estaDentro(nuevaMeta))
    {
        throw std::out_of_range("DFS::iniciar: el inicio y la meta deben estar dentro del laberinto");
    }

    anchoLaberinto = laberinto.getAncho();
    altoLaberinto = laberinto.getAlto();
    meta = nuevaMeta;
    visitadas.assign(static_cast<std::size_t>(anchoLaberinto) * static_cast<std::size_t>(altoLaberinto),
                     false);
    pila = std::stack<Position>();

    visitar(inicio);
    encontrada = (inicio == meta);
    terminado = encontrada;
}

Position DFS::siguientePaso(const Maze& laberinto)
{
    const Position actual = posicionActual();
    if (terminado)
    {
        return actual;
    }

    // Avanzar: la primera vecina alcanzable que todavía no se visitó.
    for (Direction direccion : orden)
    {
        if (!laberinto.puedeMover(actual, direccion))
        {
            continue;
        }
        const Position vecina = desplazar(actual, direccion);
        if (!estaVisitada(vecina))
        {
            visitar(vecina);
            if (vecina == meta)
            {
                encontrada = true;
                terminado = true;
            }
            return vecina;
        }
    }

    // Callejón sin salida. Si solo queda el inicio en la pila, ya se exploró
    // todo lo alcanzable sin encontrar la meta. El inicio se queda en la pila
    // para que posicionActual siga siendo válida.
    if (pila.size() == 1)
    {
        terminado = true;
        return actual;
    }

    // Backtracking: regresar a la celda anterior del camino.
    pila.pop();
    return pila.top();
}

Position DFS::posicionActual() const
{
    if (pila.empty())
    {
        throw std::logic_error("DFS: hay que llamar a iniciar antes de usar la búsqueda");
    }
    return pila.top();
}

bool DFS::haTerminado() const
{
    return terminado;
}

bool DFS::encontroSalida() const
{
    return encontrada;
}

bool DFS::estaVisitada(Position posicion) const
{
    const bool dentro = posicion.fila >= 0 && posicion.fila < altoLaberinto && posicion.columna >= 0 &&
                        posicion.columna < anchoLaberinto;
    return dentro && visitadas[static_cast<std::size_t>(indice(posicion))];
}

std::vector<Position> DFS::getCamino() const
{
    // std::stack no se puede recorrer, así que se vacía una copia (de la cima
    // a la base) y luego se invierte el resultado.
    std::stack<Position> copia = pila;
    std::vector<Position> camino;
    camino.reserve(copia.size());
    while (!copia.empty())
    {
        camino.push_back(copia.top());
        copia.pop();
    }
    std::reverse(camino.begin(), camino.end());
    return camino;
}

std::vector<Position> DFS::buscarCamino(const Maze& laberinto, Position inicio, Position fin)
{
    iniciar(laberinto, inicio, fin);
    while (!terminado)
    {
        siguientePaso(laberinto);
    }
    if (!encontrada)
    {
        return {};
    }
    return getCamino();
}

int DFS::indice(Position posicion) const
{
    return posicion.fila * anchoLaberinto + posicion.columna;
}

void DFS::visitar(Position posicion)
{
    visitadas[static_cast<std::size_t>(indice(posicion))] = true;
    pila.push(posicion);
}
