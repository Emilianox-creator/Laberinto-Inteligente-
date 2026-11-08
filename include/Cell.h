#pragma once

// Una casilla del laberinto. Toda celda nace con sus cuatro muros: el
// generador "talla" los pasillos quitándolos, nunca agregándolos.
struct Cell
{
    bool muroArriba = true;
    bool muroAbajo = true;
    bool muroIzquierda = true;
    bool muroDerecha = true;
};
