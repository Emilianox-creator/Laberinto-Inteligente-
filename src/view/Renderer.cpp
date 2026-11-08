#include "Renderer.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "AIPlayer.h"

namespace
{

constexpr int TAMANO_CELDA = 32;
constexpr int ALTURA_MINIMAPA = 130;
constexpr int MARGEN_LABERINTO = 8; // para que los muros del borde se vean completos
constexpr unsigned int LIMITE_FPS = 60;
constexpr float GROSOR_MURO = 3.0f;

constexpr sf::Color COLOR_FONDO(30, 30, 46);
constexpr sf::Color COLOR_PANEL(24, 24, 37);
constexpr sf::Color COLOR_MURO(205, 214, 244);
constexpr sf::Color COLOR_TEXTO(205, 214, 244);
constexpr sf::Color COLOR_TEXTO_SUAVE(147, 153, 178);
constexpr sf::Color COLOR_ENTRADA(166, 227, 161);
constexpr sf::Color COLOR_SALIDA(249, 226, 175);
constexpr sf::Color COLOR_CELDA_MINIMAPA(49, 50, 68);
constexpr sf::Color COLOR_SOMBRA(0, 0, 0, 170);
constexpr std::array<sf::Color, 2> COLORES_JUGADOR = {sf::Color(243, 139, 168), sf::Color(137, 180, 250)};

sf::Color colorDeJugador(int indice)
{
    return COLORES_JUGADOR[static_cast<std::size_t>(indice) % COLORES_JUGADOR.size()];
}

sf::Color conTransparencia(sf::Color color, std::uint8_t alfa)
{
    color.a = alfa;
    return color;
}

sf::Color mezclar(sf::Color a, sf::Color b)
{
    return sf::Color(static_cast<std::uint8_t>((a.r + b.r) / 2), static_cast<std::uint8_t>((a.g + b.g) / 2),
                    static_cast<std::uint8_t>((a.b + b.b) / 2));
}

std::string formatearSegundos(float segundos)
{
    std::ostringstream texto;
    texto << std::fixed << std::setprecision(1) << segundos << " s";
    return texto.str();
}

// Primero se busca junto al directorio desde donde se ejecuta el juego y
// luego en la carpeta assets del repositorio, cuya ruta pone CMake al compilar.
sf::Font cargarFuente()
{
    const std::filesystem::path rutaRelativa = std::filesystem::path("assets") / "fonts" / "tuffy.ttf";
    const std::vector<std::filesystem::path> candidatas = {
        rutaRelativa,
        std::filesystem::path(LABERINTO_DIR_ASSETS) / "fonts" / "tuffy.ttf",
    };
    for (const std::filesystem::path& ruta : candidatas)
    {
        sf::Font fuente;
        // exists evita que SFML imprima un error por cada ruta que no aplica.
        if (std::filesystem::exists(ruta) && fuente.openFromFile(ruta))
        {
            return fuente;
        }
    }
    throw std::runtime_error("No se pudo cargar la fuente " + rutaRelativa.string() +
                            ". Revisa que exista la carpeta assets del repositorio.");
}

int contarExploradas(const DFS& busqueda, const Maze& laberinto)
{
    int exploradas = 0;
    for (int fila = 0; fila < laberinto.getAlto(); ++fila)
    {
        for (int columna = 0; columna < laberinto.getAncho(); ++columna)
        {
            exploradas += busqueda.estaVisitada({fila, columna}) ? 1 : 0;
        }
    }
    return exploradas;
}

// Dos IA dan un paso en cada turno, así que sus pasos casi siempre son
// iguales; las celdas exploradas sí muestran cuál buscó mejor.
std::string resumenDeJugador(const Player& jugador, const Maze& laberinto)
{
    std::string resumen = jugador.getNombre() + ": " + std::to_string(jugador.getPasos()) + " pasos";
    if (const auto* ia = dynamic_cast<const AIPlayer*>(&jugador))
    {
        resumen += "  ·  " + std::to_string(contarExploradas(ia->getDFS(), laberinto)) + " celdas exploradas";
    }
    return resumen;
}

} // namespace

Renderer::Renderer(int ancho, int alto)
    : anchoCeldas(ancho), altoCeldas(alto), tamanoCelda(TAMANO_CELDA), alturaMinimapa(ALTURA_MINIMAPA),
        fuente(cargarFuente()),
      // Aquí ya se pueden usar anchoCeldas, tamanoCelda, etc., porque están
      // declarados antes que ventana en Renderer.h.
      ventana(sf::VideoMode({static_cast<unsigned int>(anchoCeldas * tamanoCelda + 2 * MARGEN_LABERINTO),
                            static_cast<unsigned int>(altoCeldas * tamanoCelda + 2 * MARGEN_LABERINTO +
                                                    alturaMinimapa)}),
            "Laberinto Inteligente", sf::Style::Titlebar | sf::Style::Close)
{
    ventana.setFramerateLimit(LIMITE_FPS);
}

bool Renderer::estaAbierta() const
{
    return ventana.isOpen();
}

sf::RenderWindow& Renderer::getVentana()
{
    return ventana;
}

void Renderer::dibujar(const GameState& estado)
{
    ventana.clear(COLOR_FONDO);
    switch (estado.getFase())
    {
    case GamePhase::MENU:
        dibujarMenu();
        break;
    case GamePhase::JUGANDO:
        dibujarLaberinto(estado);
        dibujarMinimapa(estado);
        break;
    case GamePhase::TERMINADO:
        // Se deja ver el laberinto final debajo de los resultados.
        dibujarLaberinto(estado);
        dibujarMinimapa(estado);
        dibujarResultados(estado);
        break;
    }
    ventana.display();
}

void Renderer::dibujarMenu()
{
    const sf::Vector2f tamanoVentana = static_cast<sf::Vector2f>(ventana.getSize());
    const float centro = tamanoVentana.x / 2.0f;
    const float arriba = tamanoVentana.y * 0.16f;

    dibujarTexto("Laberinto Inteligente", 46, {centro, arriba}, COLOR_TEXTO, true);
    dibujarTexto("Elige un modo de juego", 20, {centro, arriba + 60}, COLOR_TEXTO_SUAVE, true);

    dibujarTexto("1  ·  Máquina vs Máquina", 28, {centro, arriba + 140}, colorDeJugador(0), true);
    dibujarTexto("2  ·  Máquina vs Usuario", 28, {centro, arriba + 190}, COLOR_TEXTO, true);
    dibujarTexto("3  ·  Usuario vs Usuario", 28, {centro, arriba + 240}, colorDeJugador(1), true);

    dibujarTexto("Jugador 1: flechas      Jugador 2: W A S D", 18, {centro, arriba + 330}, COLOR_TEXTO_SUAVE,
                true);
    dibujarTexto("La IA resuelve el laberinto con búsqueda en profundidad (DFS)", 16, {centro, arriba + 365},
                COLOR_TEXTO_SUAVE, true);
    dibujarTexto("Esc: salir", 16, {centro, arriba + 420}, COLOR_TEXTO_SUAVE, true);
}

void Renderer::dibujarLaberinto(const GameState& estado)
{
    const Maze& laberinto = estado.getLaberinto();
    const float celda = static_cast<float>(tamanoCelda);

    rellenarCelda(laberinto.getEntrada(), conTransparencia(COLOR_ENTRADA, 90));
    rellenarCelda(laberinto.getSalida(), conTransparencia(COLOR_SALIDA, 150));

    // Rastro de cada IA: el contenido de la pila de su DFS, o sea, el camino
    // desde la entrada hasta donde va. Cuando retrocede, el rastro se acorta.
    for (int indice = 0; indice < estado.getNumJugadores(); ++indice)
    {
        const auto* ia = dynamic_cast<const AIPlayer*>(&estado.getJugador(indice));
        if (ia == nullptr)
        {
            continue;
        }
        for (Position paso : ia->getDFS().getCamino())
        {
            rellenarCelda(paso, conTransparencia(colorDeJugador(indice), 120), celda * 0.3f);
        }
    }

    // Se reutilizan dos rectángulos (uno horizontal y uno vertical) para
    // todos los muros, cambiando solo su posición.
    sf::RectangleShape muroHorizontal({celda + GROSOR_MURO, GROSOR_MURO});
    sf::RectangleShape muroVertical({GROSOR_MURO, celda + GROSOR_MURO});
    muroHorizontal.setFillColor(COLOR_MURO);
    muroVertical.setFillColor(COLOR_MURO);
    const float mitadGrosor = GROSOR_MURO / 2.0f;

    // Cada muro interior lo comparten dos celdas: basta con dibujar el de
    // arriba y el de la izquierda de cada una, más el borde de abajo y el de
    // la derecha al final.
    for (int fila = 0; fila < laberinto.getAlto(); ++fila)
    {
        for (int columna = 0; columna < laberinto.getAncho(); ++columna)
        {
            const Position posicion{fila, columna};
            const sf::Vector2f esquina = esquinaDeCelda(posicion);
            if (laberinto.hayMuro(posicion, Direction::ARRIBA))
            {
                muroHorizontal.setPosition({esquina.x - mitadGrosor, esquina.y - mitadGrosor});
                ventana.draw(muroHorizontal);
            }
            if (laberinto.hayMuro(posicion, Direction::IZQUIERDA))
            {
                muroVertical.setPosition({esquina.x - mitadGrosor, esquina.y - mitadGrosor});
                ventana.draw(muroVertical);
            }
            if (fila == laberinto.getAlto() - 1 && laberinto.hayMuro(posicion, Direction::ABAJO))
            {
                muroHorizontal.setPosition({esquina.x - mitadGrosor, esquina.y + celda - mitadGrosor});
                ventana.draw(muroHorizontal);
            }
            if (columna == laberinto.getAncho() - 1 && laberinto.hayMuro(posicion, Direction::DERECHA))
            {
                muroVertical.setPosition({esquina.x + celda - mitadGrosor, esquina.y - mitadGrosor});
                ventana.draw(muroVertical);
            }
        }
    }

    const float radio = celda * 0.28f;
    sf::CircleShape ficha(radio);
    ficha.setOrigin({radio, radio}); // así setPosition coloca el centro del círculo
    const bool compartenCelda =
        estado.getNumJugadores() == 2 && estado.getJugador(0).getPosicion() == estado.getJugador(1).getPosicion();
    for (int indice = 0; indice < estado.getNumJugadores(); ++indice)
    {
        sf::Vector2f centro = esquinaDeCelda(estado.getJugador(indice).getPosicion()) +
                            sf::Vector2f(celda / 2.0f, celda / 2.0f);
        if (compartenCelda)
        {
            // Separarlos un poco para que no se tapen al estar en la misma celda.
            centro.x += (indice == 0 ? -1.0f : 1.0f) * celda * 0.18f;
        }
        ficha.setFillColor(colorDeJugador(indice));
        ficha.setPosition(centro);
        ventana.draw(ficha);
    }
}

void Renderer::dibujarMinimapa(const GameState& estado)
{
    const Maze& laberinto = estado.getLaberinto();
    const sf::Vector2f tamanoVentana = static_cast<sf::Vector2f>(ventana.getSize());
    const float arriba = alturaDelLaberinto();
    const float margen = 10.0f;
    const float alturaTitulo = 20.0f;

    dibujarRectangulo({0.0f, arriba}, {tamanoVentana.x, static_cast<float>(alturaMinimapa)}, COLOR_PANEL);

    // --- Mapa pequeño: qué celdas ha explorado cada IA --------------------
    dibujarTexto("Exploración DFS", 14, {margen, arriba + 6.0f}, COLOR_TEXTO_SUAVE);
    const float espacio = static_cast<float>(alturaMinimapa) - 2.0f * margen - alturaTitulo;
    const float escala = std::max(1.0f, std::floor(std::min(espacio / static_cast<float>(laberinto.getAlto()),
                                                            220.0f / static_cast<float>(laberinto.getAncho()))));
    const sf::Vector2f origen(margen, arriba + margen + alturaTitulo);

    std::vector<std::pair<int, const DFS*>> busquedas;
    for (int indice = 0; indice < estado.getNumJugadores(); ++indice)
    {
        if (const auto* ia = dynamic_cast<const AIPlayer*>(&estado.getJugador(indice)))
        {
            busquedas.emplace_back(indice, &ia->getDFS());
        }
    }

    for (int fila = 0; fila < laberinto.getAlto(); ++fila)
    {
        for (int columna = 0; columna < laberinto.getAncho(); ++columna)
        {
            const Position posicion{fila, columna};
            sf::Color color = COLOR_CELDA_MINIMAPA;
            bool yaPintada = false;
            for (const auto& [indice, busqueda] : busquedas)
            {
                if (busqueda->estaVisitada(posicion))
                {
                    const sf::Color colorIA = colorDeJugador(indice);
                    color = yaPintada ? mezclar(color, colorIA) : colorIA;
                    yaPintada = true;
                }
            }
            if (posicion == laberinto.getEntrada())
            {
                color = COLOR_ENTRADA;
            }
            else if (laberinto.esSalida(posicion))
            {
                color = COLOR_SALIDA;
            }
            const sf::Vector2f esquina(origen.x + static_cast<float>(columna) * escala,
                                       origen.y + static_cast<float>(fila) * escala);
            dibujarRectangulo(esquina, {escala, escala}, color);
        }
    }
    // Los jugadores encima, en blanco con su color alrededor para distinguirlos.
    for (int indice = 0; indice < estado.getNumJugadores(); ++indice)
    {
        const Position posicion = estado.getJugador(indice).getPosicion();
        sf::RectangleShape marca({escala + 2.0f, escala + 2.0f});
        marca.setPosition({origen.x + static_cast<float>(posicion.columna) * escala - 1.0f,
                           origen.y + static_cast<float>(posicion.fila) * escala - 1.0f});
        marca.setFillColor(sf::Color::White);
        marca.setOutlineThickness(1.0f);
        marca.setOutlineColor(colorDeJugador(indice));
        ventana.draw(marca);
    }

    // --- Datos de la partida ----------------------------------------------
    const float columnaTexto = origen.x + static_cast<float>(laberinto.getAncho()) * escala + 30.0f;
    float renglon = arriba + 10.0f;
    dibujarTexto("Tiempo: " + formatearSegundos(estado.getTiempo()), 20, {columnaTexto, renglon}, COLOR_TEXTO);
    renglon += 32.0f;

    for (int indice = 0; indice < estado.getNumJugadores(); ++indice)
    {
        dibujarTexto(resumenDeJugador(estado.getJugador(indice), laberinto), 18, {columnaTexto, renglon},
                    colorDeJugador(indice));
        renglon += 26.0f;
    }

    dibujarTexto("Esc: volver al menú", 14, {columnaTexto, arriba + static_cast<float>(alturaMinimapa) - 26.0f},
                COLOR_TEXTO_SUAVE);
}

void Renderer::dibujarResultados(const GameState& estado)
{
    const sf::Vector2f tamanoVentana = static_cast<sf::Vector2f>(ventana.getSize());
    dibujarRectangulo({0.0f, 0.0f}, tamanoVentana, COLOR_SOMBRA);

    std::string titulo = "¡Empate!";
    sf::Color colorTitulo = COLOR_TEXTO;
    for (int indice = 0; indice < estado.getNumJugadores(); ++indice)
    {
        if (estado.getJugador(indice).getId() == estado.getGanador())
        {
            titulo = "¡Ganó " + estado.getJugador(indice).getNombre() + "!";
            colorTitulo = colorDeJugador(indice);
        }
    }

    const float centro = tamanoVentana.x / 2.0f;
    float renglon = tamanoVentana.y * 0.3f;
    dibujarTexto(titulo, 42, {centro, renglon}, colorTitulo, true);
    renglon += 60.0f;
    dibujarTexto("Tiempo: " + formatearSegundos(estado.getTiempo()), 22, {centro, renglon}, COLOR_TEXTO, true);
    renglon += 40.0f;

    for (int indice = 0; indice < estado.getNumJugadores(); ++indice)
    {
        dibujarTexto(resumenDeJugador(estado.getJugador(indice), estado.getLaberinto()), 20, {centro, renglon},
                    colorDeJugador(indice), true);
        renglon += 30.0f;
    }

    renglon += 30.0f;
    dibujarTexto("R: jugar otra vez     ·     Enter o Esc: volver al menú", 18, {centro, renglon},
                COLOR_TEXTO_SUAVE, true);
}

void Renderer::dibujarTexto(const std::string& texto, unsigned int tamano, sf::Vector2f posicion, sf::Color color,
                            bool centrado)
{
    // Los literales del código están en UTF-8; sin fromUtf8 los acentos y
    // la ñ se verían como símbolos raros.
    sf::Text dibujo(fuente, sf::String::fromUtf8(texto.begin(), texto.end()), tamano);
    dibujo.setFillColor(color);
    if (centrado)
    {
        const sf::FloatRect limites = dibujo.getLocalBounds();
        dibujo.setOrigin({limites.position.x + limites.size.x / 2.0f, limites.position.y + limites.size.y / 2.0f});
    }
    dibujo.setPosition(posicion);
    ventana.draw(dibujo);
}

void Renderer::dibujarRectangulo(sf::Vector2f posicion, sf::Vector2f tamano, sf::Color color)
{
    sf::RectangleShape rectangulo(tamano);
    rectangulo.setPosition(posicion);
    rectangulo.setFillColor(color);
    ventana.draw(rectangulo);
}

void Renderer::rellenarCelda(Position celda, sf::Color color, float margen)
{
    const float lado = static_cast<float>(tamanoCelda);
    dibujarRectangulo(esquinaDeCelda(celda) + sf::Vector2f(margen, margen),
                      {lado - 2.0f * margen, lado - 2.0f * margen}, color);
}

sf::Vector2f Renderer::esquinaDeCelda(Position celda) const
{
    return {static_cast<float>(MARGEN_LABERINTO + celda.columna * tamanoCelda),
            static_cast<float>(MARGEN_LABERINTO + celda.fila * tamanoCelda)};
}

float Renderer::alturaDelLaberinto() const
{
    return static_cast<float>(altoCeldas * tamanoCelda + 2 * MARGEN_LABERINTO);
}
