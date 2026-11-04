#include "Game.h"

#include <algorithm>
#include <array>
#include <memory>
#include <optional>
#include <random>
#include <vector>

#include <SFML/Graphics.hpp>

#include "AIPlayer.h"

namespace
{

constexpr int ANCHO_LABERINTO = 25;
constexpr int ALTO_LABERINTO = 17;

// Las IA dan un paso cada este tiempo, sin importar los FPS.
constexpr float SEGUNDOS_POR_PASO_IA = 0.1f;

// Si la ventana se congela (por ejemplo, al arrastrarla), el siguiente dt
// sería enorme y las IA darían muchos pasos de golpe. Se limita a esto.
constexpr float DT_MAXIMO = 0.25f;

// Cada IA explora las vecinas en un orden aleatorio y distinto al de la otra.
// Se probó con 1000 laberintos: si las dos preferían siempre ir hacia la
// salida (derecha y abajo), el 36% de las partidas Máquina vs Máquina
// terminaban en empate; con órdenes aleatorios distintos baja a cerca del 11%
// y ninguna IA tiene ventaja. Se usa la semilla del laberinto para que una
// partida completa se pueda repetir con un solo número.
std::array<DFS::OrdenExploracion, 2> ordenesAleatorios(unsigned int semilla)
{
    std::mt19937 rng(semilla);
    std::array<DFS::OrdenExploracion, 2> ordenes = {DIRECCIONES_MOVIMIENTO, DIRECCIONES_MOVIMIENTO};
    std::shuffle(ordenes[0].begin(), ordenes[0].end(), rng);
    do
    {
        std::shuffle(ordenes[1].begin(), ordenes[1].end(), rng);
    } while (ordenes[1] == ordenes[0]);
    return ordenes;
}

Direction direccionDeFlechas(sf::Keyboard::Key tecla)
{
    switch (tecla)
    {
    case sf::Keyboard::Key::Up:
        return Direction::ARRIBA;
    case sf::Keyboard::Key::Down:
        return Direction::ABAJO;
    case sf::Keyboard::Key::Left:
        return Direction::IZQUIERDA;
    case sf::Keyboard::Key::Right:
        return Direction::DERECHA;
    default:
        return Direction::NINGUNA;
    }
}

Direction direccionDeWASD(sf::Keyboard::Key tecla)
{
    switch (tecla)
    {
    case sf::Keyboard::Key::W:
        return Direction::ARRIBA;
    case sf::Keyboard::Key::S:
        return Direction::ABAJO;
    case sf::Keyboard::Key::A:
        return Direction::IZQUIERDA;
    case sf::Keyboard::Key::D:
        return Direction::DERECHA;
    default:
        return Direction::NINGUNA;
    }
}

bool esIA(const Player& jugador)
{
    // dynamic_cast revisa en tiempo de ejecución el tipo real del objeto:
    // devuelve nullptr si el Player no es un AIPlayer.
    return dynamic_cast<const AIPlayer*>(&jugador) != nullptr;
}

} // namespace

Game::Game() : renderer(ANCHO_LABERINTO, ALTO_LABERINTO)
{
}

void Game::ejecutar()
{
    sf::Clock reloj;
    while (renderer.estaAbierta())
    {
        // dt (delta time): segundos que pasaron desde el frame anterior.
        const float dt = reloj.restart().asSeconds();
        procesarEventos();
        if (!renderer.estaAbierta())
        {
            break;
        }
        actualizar(dt);
        renderer.dibujar(estado);
    }
}

void Game::iniciarPartida(GameMode modo)
{
    estado.nuevaPartida(modo, generador.generarLaberinto(ANCHO_LABERINTO, ALTO_LABERINTO));
    crearJugadores(modo);
    acumuladorIA = 0.0f;
}

void Game::procesarEventos()
{
    sf::RenderWindow& ventana = renderer.getVentana();
    while (const std::optional evento = ventana.pollEvent())
    {
        if (evento->is<sf::Event::Closed>())
        {
            ventana.close();
        }
        else if (const auto* teclaPresionada = evento->getIf<sf::Event::KeyPressed>())
        {
            procesarTecla(teclaPresionada->code);
        }
    }
}

void Game::actualizar(float dt)
{
    if (estado.getFase() != GamePhase::JUGANDO)
    {
        return;
    }
    dt = std::min(dt, DT_MAXIMO);
    estado.sumarTiempo(dt);
    const Maze& laberinto = estado.getLaberinto();

    // Los humanos se mueven en cuanto presionan una tecla.
    for (int indice = 0; indice < estado.getNumJugadores(); ++indice)
    {
        Player& jugador = estado.getJugador(indice);
        if (!esIA(jugador))
        {
            jugador.mover(jugador.decidirMovimiento(laberinto), laberinto);
        }
    }
    verificarVictoria();

    // Las IA avanzan a ritmo fijo: se acumula el tiempo y se da un paso por
    // cada intervalo completo. Así van igual de rápido a 30 que a 60 FPS.
    acumuladorIA += dt;
    while (estado.getFase() == GamePhase::JUGANDO && acumuladorIA >= SEGUNDOS_POR_PASO_IA)
    {
        acumuladorIA -= SEGUNDOS_POR_PASO_IA;
        for (int indice = 0; indice < estado.getNumJugadores(); ++indice)
        {
            Player& jugador = estado.getJugador(indice);
            if (esIA(jugador))
            {
                jugador.mover(jugador.decidirMovimiento(laberinto), laberinto);
            }
        }
        // Se revisa después de cada paso para que, si dos IA llegan en el
        // mismo paso, cuente como empate y no gane la primera de la lista.
        verificarVictoria();
    }
}

void Game::verificarVictoria()
{
    if (estado.getFase() != GamePhase::JUGANDO)
    {
        return;
    }
    std::vector<int> enLaSalida;
    for (int indice = 0; indice < estado.getNumJugadores(); ++indice)
    {
        const Player& jugador = estado.getJugador(indice);
        if (estado.getLaberinto().esSalida(jugador.getPosicion()))
        {
            enLaSalida.push_back(jugador.getId());
        }
    }
    if (enLaSalida.size() == 1)
    {
        estado.declararGanador(enLaSalida.front());
    }
    else if (enLaSalida.size() > 1)
    {
        estado.declararGanador(GameState::EMPATE);
    }
}

void Game::volverAlMenu()
{
    estado.setFase(GamePhase::MENU);
}

void Game::crearJugadores(GameMode modo)
{
    const Position inicio = estado.getLaberinto().getEntrada();
    const std::array<DFS::OrdenExploracion, 2> ordenes = ordenesAleatorios(generador.getUltimaSemilla());
    switch (modo)
    {
    case GameMode::MAQUINA_VS_MAQUINA:
        estado.agregarJugador(std::make_unique<AIPlayer>(1, "IA Roja", inicio, ordenes[0]));
        estado.agregarJugador(std::make_unique<AIPlayer>(2, "IA Azul", inicio, ordenes[1]));
        break;
    case GameMode::MAQUINA_VS_USUARIO:
        estado.agregarJugador(std::make_unique<HumanPlayer>(1, "Jugador (flechas)", inicio));
        estado.agregarJugador(std::make_unique<AIPlayer>(2, "IA", inicio, ordenes[1]));
        break;
    case GameMode::USUARIO_VS_USUARIO:
        estado.agregarJugador(std::make_unique<HumanPlayer>(1, "Jugador 1 (flechas)", inicio));
        estado.agregarJugador(std::make_unique<HumanPlayer>(2, "Jugador 2 (WASD)", inicio));
        break;
    }
}

void Game::procesarTecla(sf::Keyboard::Key tecla)
{
    switch (estado.getFase())
    {
    case GamePhase::MENU:
        if (tecla == sf::Keyboard::Key::Escape)
        {
            renderer.getVentana().close();
        }
        else if (tecla == sf::Keyboard::Key::Num1 || tecla == sf::Keyboard::Key::Numpad1)
        {
            iniciarPartida(GameMode::MAQUINA_VS_MAQUINA);
        }
        else if (tecla == sf::Keyboard::Key::Num2 || tecla == sf::Keyboard::Key::Numpad2)
        {
            iniciarPartida(GameMode::MAQUINA_VS_USUARIO);
        }
        else if (tecla == sf::Keyboard::Key::Num3 || tecla == sf::Keyboard::Key::Numpad3)
        {
            iniciarPartida(GameMode::USUARIO_VS_USUARIO);
        }
        break;

    case GamePhase::JUGANDO:
        if (tecla == sf::Keyboard::Key::Escape)
        {
            volverAlMenu();
            break;
        }
        // Game es quien conoce el teclado: traduce la tecla a una Direction
        // y se la pasa al HumanPlayer, que así nunca toca SFML.
        if (const Direction direccion = direccionDeFlechas(tecla); direccion != Direction::NINGUNA)
        {
            if (HumanPlayer* humano = buscarHumano(1))
            {
                humano->setDireccion(direccion);
            }
        }
        else if (const Direction direccionWASD = direccionDeWASD(tecla); direccionWASD != Direction::NINGUNA)
        {
            if (HumanPlayer* humano = buscarHumano(2))
            {
                humano->setDireccion(direccionWASD);
            }
        }
        break;

    case GamePhase::TERMINADO:
        if (tecla == sf::Keyboard::Key::R)
        {
            iniciarPartida(estado.getModo());
        }
        else if (tecla == sf::Keyboard::Key::Enter || tecla == sf::Keyboard::Key::Escape)
        {
            volverAlMenu();
        }
        break;
    }
}

HumanPlayer* Game::buscarHumano(int numeroDeHumano)
{
    int humanosVistos = 0;
    for (int indice = 0; indice < estado.getNumJugadores(); ++indice)
    {
        if (auto* humano = dynamic_cast<HumanPlayer*>(&estado.getJugador(indice)))
        {
            ++humanosVistos;
            if (humanosVistos == numeroDeHumano)
            {
                return humano;
            }
        }
    }
    return nullptr;
}
