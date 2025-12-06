
#include <SFML/Graphics.hpp>
#include "../include/Tablero.hpp"
#include "../include/Enemigo.hpp"
#include <iostream>
#include <algorithm>

int FILAS = 40;
int COLUMNAS = 40;
int MINAS = 320;
float CELDA_SIZE = 32.0f;
float SIDEBAR_WIDTH = 200.0f;
bool marcarFilaUsado = false;  // Control de uso del poder
bool detectorMinasUsado = false;  // Control de uso del detector
bool paralizanteUsado = false;  // Control de uso del paralizante
int ultimaCasillaContadaBomba = 0;  // Rastrear el último punto donde se creó bomba

int main()
{
    std::cout << "Creando tablero con: " << FILAS << " x " << COLUMNAS << " casillas, " << MINAS << " minas" << std::endl;
    sf::RenderWindow window(sf::VideoMode((COLUMNAS * CELDA_SIZE) + SIDEBAR_WIDTH, FILAS * CELDA_SIZE), "Buscaminas");
    std::cout << "Tamaño ventana: " << (int)((COLUMNAS * CELDA_SIZE) + SIDEBAR_WIDTH) << " x " << (int)(FILAS * CELDA_SIZE) << std::endl;
    Tablero tablero(FILAS, COLUMNAS, MINAS);
    
    // Crear enemigo en uno de los lados del tablero
    Enemigo enemigo(FILAS, COLUMNAS);
    
    // Reloj para deltaTime
    sf::Clock clock;
    
    // Control de si el juego ha iniciado (después del primer clic)
    bool juegoIniciado = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Verificar si se ha cerrado la ventana
            if (event.type == sf::Event::Closed)
                window.close();
            // Manejo de mouse para descubrir o marcar celdas
            if (event.type == sf::Event::MouseButtonPressed) {
                int x = event.mouseButton.x / CELDA_SIZE;
                int y = event.mouseButton.y / CELDA_SIZE;
                if (x >= 0 && x < COLUMNAS && y >= 0 && y < FILAS) {
                    juegoIniciado = true;  // Marcar que el juego ha comenzado
                    if (event.mouseButton.button == sf::Mouse::Left)
                        tablero.descubrir(y, x);
                    else if (event.mouseButton.button == sf::Mouse::Right)
                        tablero.marcar(y, x);
                }
            }
            // Poder: Marcar todas las minas de una fila con la tecla R
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) {
                    if (!marcarFilaUsado) {
                        int mouseY = sf::Mouse::getPosition(window).y / CELDA_SIZE;
                        if (mouseY >= 0 && mouseY < FILAS) {
                            tablero.marcarFilaDeMinas(mouseY);
                            marcarFilaUsado = true;
                            std::cout << "Poder activado: Se marcaron todas las minas de la fila " << mouseY << std::endl;
                        }
                    } else {
                        std::cout << "El poder 'Marcar Fila' ya fue usado." << std::endl;
                    }
                }
                // Poder: Marcar una mina al azar con la tecla D
                if (event.key.code == sf::Keyboard::D) {
                    if (tablero.getUsosDetectorMinas() > 0) {
                        tablero.marcarMinaAlAzar();
                    } else {
                        std::cout << "No tienes usos del detector de minas." << std::endl;
                    }
                }
                // Poder: Paralizar enemigo con la tecla P
                if (event.key.code == sf::Keyboard::P) {
                    if (!paralizanteUsado) {
                        enemigo.paralizarTemporalmente(30.0f);
                        paralizanteUsado = true;
                        std::cout << "¡ENEMIGO PARALIZADO POR 30 SEGUNDOS!" << std::endl;
                    } else {
                        std::cout << "El poder 'Paralizante' ya fue usado." << std::endl;
                    }
                }
            }
        }

        // Limpiar la ventana
        window.clear(sf::Color::Black);
        
        // Obtener deltaTime
        float deltaTime = clock.restart().asSeconds();
        
        // Actualizar enemigo
        // Primero, crear vector de estados de celdas para el enemigo
        std::vector<std::vector<int>> estadoCeldas(FILAS, std::vector<int>(COLUMNAS));
        for (int i = 0; i < FILAS; ++i) {
            for (int j = 0; j < COLUMNAS; ++j) {
                const Celda& celda = tablero.getCelda(i, j);
                if (celda.getEstado() == Celda::Oculta) {
                    estadoCeldas[i][j] = 0;  // Oculta
                } else if (celda.getEstado() == Celda::Descubierta) {
                    estadoCeldas[i][j] = 1;  // Descubierta
                } else {
                    estadoCeldas[i][j] = 2;  // Marcada
                }
            }
        }
        
        // Obtener posición del mouse en coordenadas de tablero
        sf::Vector2i posMousePixeles = sf::Mouse::getPosition(window);
        int filaRaton = posMousePixeles.y / CELDA_SIZE;
        int colRaton = posMousePixeles.x / CELDA_SIZE;
        
        // Limitar a los límites del tablero
        filaRaton = std::max(0, std::min(filaRaton, FILAS - 1));
        colRaton = std::max(0, std::min(colRaton, COLUMNAS - 1));
        
        // Actualizar enemigo (solo si el juego ha iniciado)
        enemigo.actualizar(deltaTime, estadoCeldas, juegoIniciado, filaRaton, colRaton);
        
        // Verificar si el enemigo está en una casilla descubierta y teletransportarlo
        enemigo.verificarYTeletransportar(estadoCeldas);
        
        // Obtener posición nueva y casillas actualizadas
        auto posNueva = enemigo.obtenerPosicion();
        int casillasAhora = enemigo.getCasillasRecorridas();
        
        // Debug: mostrar casillas recorridas cada segundo
        static float tiempoDebug = 0;
        tiempoDebug += deltaTime;
        if (tiempoDebug >= 1.0f) {
            std::cout << "Casillas recorridas: " << casillasAhora << ", Última bomba en: " << ultimaCasillaContadaBomba << std::endl;
            tiempoDebug = 0;
        }
        
        // Crear una bomba cada 5 casillas recorridas donde pasa el enemigo
        if (casillasAhora > ultimaCasillaContadaBomba && (casillasAhora % 5 == 0)) {
            ultimaCasillaContadaBomba = casillasAhora;
            
            // Poner bomba en la celda actual del enemigo si está oculta
            int filaEnemigo = enemigo.getFila();
            int colEnemigo = enemigo.getColumna();
            
            if (estadoCeldas[filaEnemigo][colEnemigo] == 0) {
                tablero.crearBombaTemporalEnCelda(filaEnemigo, colEnemigo);
                std::cout << "✓ Bomba creada en casilla " << casillasAhora << " en posición (" << filaEnemigo << ", " << colEnemigo << ")" << std::endl;
            } else {
                std::cout << "✗ Celda revelada en casilla " << casillasAhora << ", no se creó bomba" << std::endl;
            }
        }
        
        // Actualizar bombas
        tablero.actualizarBombas(deltaTime);
        
        // Dibujar el tablero
        tablero.dibujar(window, CELDA_SIZE);
        
        // Dibujar bombas temporales con efecto visual
        auto posBombas = tablero.obtenerPosicionesBombas();
        for (const auto& bomba : posBombas) {
            // Dibujar bomba como un cuadrado parpadeante amarillo/rojo
            sf::RectangleShape bombaDibujo(sf::Vector2f(CELDA_SIZE * 0.6f, CELDA_SIZE * 0.6f));
            bombaDibujo.setPosition(bomba.second * CELDA_SIZE + CELDA_SIZE * 0.2f, 
                                   bomba.first * CELDA_SIZE + CELDA_SIZE * 0.2f);
            bombaDibujo.setFillColor(sf::Color::Yellow);
            bombaDibujo.setOutlineColor(sf::Color::Red);
            bombaDibujo.setOutlineThickness(2.0f);
            window.draw(bombaDibujo);
        }
        
        // Dibujar enemigo
        enemigo.dibujar(window, CELDA_SIZE);
        
        // Dibujar barra lateral con poderes
        float boardWidth = COLUMNAS * CELDA_SIZE;
        
        // Fondo de la barra lateral
        sf::RectangleShape sidebar(sf::Vector2f(SIDEBAR_WIDTH, FILAS * CELDA_SIZE));
        sidebar.setPosition(boardWidth, 0);
        sidebar.setFillColor(sf::Color(50, 50, 50));
        window.draw(sidebar);
        
        // Cargar fuente para los poderes
        sf::Font font;
        font.loadFromFile("assets/fonts/Minecraft.ttf");
        
        // Título "PODERES"
        sf::Text titleText("PODERES", font, 20);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(boardWidth + 20, 20);
        window.draw(titleText);
        
        // Poder 1: Marcar fila
        sf::RectangleShape powerButton(sf::Vector2f(SIDEBAR_WIDTH - 20, 60));
        powerButton.setPosition(boardWidth + 10, 80);
        
        // Cambiar color según si fue usado
        if (marcarFilaUsado) {
            powerButton.setFillColor(sf::Color(200, 100, 100));  // Rojo oscuro
        } else {
            powerButton.setFillColor(sf::Color(100, 150, 255));  // Azul
        }
        
        powerButton.setOutlineColor(sf::Color::White);
        powerButton.setOutlineThickness(2);
        window.draw(powerButton);
        
        sf::Text powerText("Marcar Fila\n(Presiona R)", font, 14);
        powerText.setFillColor(sf::Color::White);
        powerText.setPosition(boardWidth + 15, 95);
        window.draw(powerText);
        
        // Poder 2: Antídoto
        sf::RectangleShape antidotoButton(sf::Vector2f(SIDEBAR_WIDTH - 20, 60));
        antidotoButton.setPosition(boardWidth + 10, 160);
        
        // Cambiar color según si fue usado
        if (!tablero.tieneAntidoto()) {
            antidotoButton.setFillColor(sf::Color(200, 100, 100));  // Rojo oscuro
        } else {
            antidotoButton.setFillColor(sf::Color(100, 255, 100));  // Verde
        }
        
        antidotoButton.setOutlineColor(sf::Color::White);
        antidotoButton.setOutlineThickness(2);
        window.draw(antidotoButton);
        
        sf::Text antidotoText("Antidoto\n(Automatico)", font, 14);
        antidotoText.setFillColor(sf::Color::White);
        antidotoText.setPosition(boardWidth + 20, 175);
        window.draw(antidotoText);
        
        // Poder 3: Detector de Minas
        sf::RectangleShape detectorButton(sf::Vector2f(SIDEBAR_WIDTH - 20, 60));
        detectorButton.setPosition(boardWidth + 10, 240);
        
        // Cambiar color según si tiene usos
        if (tablero.getUsosDetectorMinas() <= 0) {
            detectorButton.setFillColor(sf::Color(200, 100, 100));  // Rojo oscuro
        } else {
            detectorButton.setFillColor(sf::Color(255, 200, 100));  // Naranja
        }
        
        detectorButton.setOutlineColor(sf::Color::White);
        detectorButton.setOutlineThickness(2);
        window.draw(detectorButton);
        
        // Texto del detector con usos
        std::string detectorInfo = "Detector Mina\n(Presiona D)\n(" + std::to_string(tablero.getUsosDetectorMinas()) + " usos)";
        sf::Text detectorText(detectorInfo, font, 12);
        detectorText.setFillColor(sf::Color::White);
        detectorText.setPosition(boardWidth + 15, 250);
        window.draw(detectorText);
        
        // Poder 4: Paralizante
        sf::RectangleShape paralizanteButton(sf::Vector2f(SIDEBAR_WIDTH - 20, 60));
        paralizanteButton.setPosition(boardWidth + 10, 320);
        
        // Cambiar color según si fue usado
        if (paralizanteUsado) {
            paralizanteButton.setFillColor(sf::Color(200, 100, 100));  // Rojo oscuro
        } else {
            paralizanteButton.setFillColor(sf::Color(150, 100, 255));  // Púrpura
        }
        
        paralizanteButton.setOutlineColor(sf::Color::White);
        paralizanteButton.setOutlineThickness(2);
        window.draw(paralizanteButton);
        
        sf::Text paralizanteText("Paralizante\n(Presiona P)", font, 14);
        paralizanteText.setFillColor(sf::Color::White);
        paralizanteText.setPosition(boardWidth + 15, 335);
        window.draw(paralizanteText);

        // Mostrar la ventana
        window.display();
    }

    return 0;
}