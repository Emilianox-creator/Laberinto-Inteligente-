#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "Position.h"

// View: dibuja el GameState con SFML. Solo lee el estado, nunca lo modifica;
// las decisiones del juego son de Game.
class Renderer
{
public:
    // ancho y alto son las celdas del laberinto: la ventana se ajusta a ese
    // tamaño más la franja del minimapa. Lanza std::runtime_error si no
    // encuentra la fuente.
    Renderer(int ancho, int alto);

    bool estaAbierta() const;
    // Game la necesita para leer los eventos del teclado.
    sf::RenderWindow& getVentana();
    void dibujar(const GameState& estado);

private:
    void dibujarMenu();
    void dibujarLaberinto(const GameState& estado);
    void dibujarMinimapa(const GameState& estado);
    void dibujarResultados(const GameState& estado);

    void dibujarTexto(const std::string& texto, unsigned int tamano, sf::Vector2f posicion, sf::Color color,
                      bool centrado = false);
    void dibujarRectangulo(sf::Vector2f posicion, sf::Vector2f tamano, sf::Color color);
    // Pinta el interior de una celda; el margen deja un borde sin pintar.
    void rellenarCelda(Position celda, sf::Color color, float margen = 0.0f);
    sf::Vector2f esquinaDeCelda(Position celda) const;
    float alturaDelLaberinto() const;

    // Los miembros se inicializan en el orden en que se declaran (no en el
    // del constructor). La ventana va al final porque su tamaño depende de los
    // anteriores y la fuente se carga antes para no abrir una ventana que se
    // cerraría enseguida si la fuente falta.
    int anchoCeldas;
    int altoCeldas;
    int tamanoCelda;
    int alturaMinimapa;
    sf::Font fuente;
    sf::RenderWindow ventana;
};
