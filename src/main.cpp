#include <optional> //ya viene incluído con la librería de abajo pero es buena práctica
#include <SFML/Graphics.hpp>

const unsigned int ANCHO_VENTANA = 800;
const unsigned int ALTO_VENTANA = 600;
const unsigned int LIMITE_FPS = 60;
const sf::Color COLOR_FONDO(30, 30, 46);

int main()
{
    // Crea la ventana, los parámetros del constructos son el tamaño, título, y elementos de la ventana.
    sf::RenderWindow window(
        sf::VideoMode({ANCHO_VENTANA, ALTO_VENTANA}), 
        "Laberinto Inteligente", 
        sf::Style::Titlebar | sf::Style::Close);
    
    //Limita los FPS, se declara solo una vez antes del bucle.
    window.setFramerateLimit(LIMITE_FPS);

    while (window.isOpen()){

        while (const std::optional evento = window.pollEvent()){

            if (evento->is<sf::Event::Closed>()){
                window.close();
            }

            else if (const auto* teclaPresionada = evento->getIf<sf::Event::KeyPressed>()){
                if (teclaPresionada->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }
        }
        
        window.clear(COLOR_FONDO);
        //Para el futuro: window.draw(...)
        window.display();
    }
}