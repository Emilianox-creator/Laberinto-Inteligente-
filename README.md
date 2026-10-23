# Laberinto Inteligente - IA
# Convenciones del repositorio
Ramas: usan tipo/descripcion en minúsculas y con guiones. 
Ejemplo: `feature/agregar-jugador`.
Clases: usan PascalCase, sin espacios. 
Ejemplo: `Maze`, `HumanPlayer` y `GameState`.
Métodos: usan camelCase, sin espacios. 
Ejemplos: `generarLaberinto` y `buscarCamino`.
Archivos: usan el nombre de la clase principal y la extensión correspondiente. Ejemplo: `Maze.cpp`.
Revisión: nadie hace merge a `main` sin que otra persona revise el cambio.
Archivos ignorados por Git:
El archivo `.gitignore` excluye archivos generados al compilar y configuraciones locales del editor. Así evitamos subir archivos que no forman parte del código fuente.
# Posibles responsabilidades de las clases

Maze: representa el laberinto y sus celdas.
MazeGenerator: crea el laberinto.
DFS: busca una ruta por el laberinto mediante búsqueda en profundidad.
Player: representa a un jugador.
HumanPlayer representa al jugador controlado por una persona.
AIPlayer: representa al jugador controlado por la computadora.
Game: coordina el desarrollo de la partida.
GameState: guarda el estado actual de la partida.
Renderer: muestra el laberinto y los elementos del juego en pantalla.


## Integrantes

* **Ivan Emiliano Urzua Loera**
* **Eduardo Alejandro Vazquez Lopez**
* **Jose Emiliano Gonzalez Iñiguez**
* **Carlo Ramses Morales Martinez**

---
