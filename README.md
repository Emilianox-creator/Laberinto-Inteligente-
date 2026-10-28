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


## Instalación y compilación

### Requisitos

- **CMake 3.28 o mayor** (SFML 3.1.0 lo exige). Descarga la última versión para tu sistema operativo desde https://cmake.org/download/.
- Un compilador con soporte para **C++17**.
- **Git**, porque CMake lo usa para descargar SFML.

### SFML

- Versión usada: **SFML 3.1.0** (fijada con `GIT_TAG` en `CMakeLists.txt`).
- **No hay que instalarla a mano**: CMake la descarga y la compila automáticamente con `FetchContent` la primera vez que se configura el proyecto.
- Ojo: muchos tutoriales en internet son de SFML 2 y su código no compila en SFML 3. Guía de migración: https://www.sfml-dev.org/tutorials/3.0/getting-started/migrate/

### Compilar y ejecutar (todos los sistemas)

Desde la raíz del repositorio:

```bash
cmake -S . -B build      # 1. Configurar (solo la primera vez o si cambia CMakeLists.txt)
cmake --build build      # 2. Compilar (cada vez que cambies el código)
./build/laberinto        # 3. Ejecutar (macOS y Linux)
```

El ejecutable queda en `build/laberinto` (en Windows, `build/laberinto.exe`, o `build/Debug/laberinto.exe` si usas Visual Studio).

**La primera configuración tarda varios minutos**: SFML y sus dependencias (freetype, harfbuzz, etc.) pesan más de 400 MB. No está congelado, está descargando. Para ver el progreso:

```bash
cmake -S . -B build -DFETCHCONTENT_QUIET=OFF
```

**Usa siempre la misma carpeta de build.** Cada carpeta nueva (`-B otra-carpeta`) vuelve a descargar todo desde cero. El nombre `build` es solo una convención. Si en tu computadora ya configuraste otra carpeta (por ejemplo, la extensión CMake Tools de VS Code puede usar `build-mac` según tu `.vscode/settings.json`), usa esa en los tres comandos.

### Solo macOS: VS Code no encuentra CMake

Esto aplica a quien tenga Mac y vea en VS Code el error:

```
Ejecutable de CMake incorrecto: "". Compruebe que está instalado o que el valor de la configuración de "cmake.cmakePath" contiene la ruta de acceso correcta.
```

**Por qué pasa:** las apps abiertas desde el Dock o Spotlight no leen `~/.zshrc`. Si agregaste CMake al `PATH` solo ahí, la terminal lo encuentra, pero VS Code no.

**Solución (una de estas):**

- Agrega a tu `.vscode/settings.json` la ruta de tu CMake. Si lo instalaste desde el `.dmg` de cmake.org:
  ```json
  "cmake.cmakePath": "/Applications/CMake.app/Contents/bin/cmake"
  ```
  Para saber tu ruta, ejecuta `which cmake` en la terminal. La carpeta `.vscode/` está en `.gitignore`, así que este ajuste es personal y no se sube al repositorio.
- O abre VS Code desde la terminal con `code .` para que herede tu `PATH`.
