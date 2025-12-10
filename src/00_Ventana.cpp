
#include <SFML/Graphics.hpp>
#include "../include/Tablero.hpp"
#include "../include/Enemigo.hpp"
#include "../include/GameState.hpp"
#include "../include/AudioManager.hpp"
#include <iostream>
#include <algorithm>

int FILAS = 40;
int COLUMNAS = 40;
int MINAS = 350;
int CELDA_SIZE = 32.0f;
float SIDEBAR_WIDTH = 200.0f;
bool marcarFilaUsado = false;
bool detectorMinasUsado = false;
bool paralizanteUsado = false;
bool escudoGanadorUsado = false;
float tiempoEscudoGanador = 0.0f;
bool musicaReproduciendo = false;
bool musicaMenuReproduciendo = false;
bool enemigosActivos = true;  // Variable para activar/desactivar enemigos
int ultimaCasillaContadaBomba = 0;  // Rastrear el último punto donde se creó bomba
int ultimaCasillaContadaBomba2 = 0;  // Rastrear bombas del segundo enemigo

// Función para reiniciar el juego
void reiniciarJuego(Tablero*& tablero, Enemigo*& enemigo, Enemigo*& enemigo2, bool& juegoIniciado, 
                    bool& marcarFilaUsado, bool& detectorMinasUsado, bool& paralizanteUsado, bool& escudoGanadorUsado,
                    int& ultimaCasillaContadaBomba, int& ultimaCasillaContadaBomba2, float& tiempoJuego, float& tiempoEscudoGanador,
                    AudioManager& audio, bool& musicaReproduciendo, bool& musicaMenuReproduciendo) {
    delete tablero;
    delete enemigo;
    delete enemigo2;
    tablero = new Tablero(FILAS, COLUMNAS, MINAS);
    enemigo = new Enemigo(FILAS, COLUMNAS);
    enemigo2 = new Enemigo(FILAS, COLUMNAS, "assets/images/plaga2.png");
    juegoIniciado = false;
    marcarFilaUsado = false;
    detectorMinasUsado = false;
    paralizanteUsado = false;
    escudoGanadorUsado = false;
    tiempoEscudoGanador = 0.0f;
    ultimaCasillaContadaBomba = 0;
    ultimaCasillaContadaBomba2 = 0;
    tiempoJuego = 0.0f;
    musicaReproduciendo = false;
    musicaMenuReproduciendo = false;
    audio.stopMusic();
    std::cout << "Juego reiniciado" << std::endl;
}

int main()
{
    std::cout << "Creando tablero con: " << FILAS << " x " << COLUMNAS << " casillas, " << MINAS << " minas" << std::endl;
    sf::RenderWindow window(sf::VideoMode((COLUMNAS * CELDA_SIZE) + SIDEBAR_WIDTH, FILAS * CELDA_SIZE), "Buscaplagas");
    std::cout << "Tamaño ventana: " << (int)((COLUMNAS * CELDA_SIZE) + SIDEBAR_WIDTH) << " x " << (int)(FILAS * CELDA_SIZE) << std::endl;
    
    // Usar punteros para poder reiniciar
    Tablero* tablero = new Tablero(FILAS, COLUMNAS, MINAS);
    Enemigo* enemigo = new Enemigo(FILAS, COLUMNAS);
    Enemigo* enemigo2 = new Enemigo(FILAS, COLUMNAS, "assets/images/plaga2.png");  // Segundo enemigo independiente
    
    // GameState y AudioManager
    GameState gameState;
    AudioManager& audio = AudioManager::getInstance();
    
    // Cargar fuente
    sf::Font font;
    font.loadFromFile("assets/fonts/Minecraft.ttf");
    
    // Intentar cargar audio (opcional, el juego funcionará sin audio)
    audio.loadSound("explosion", "assets/sounds/explosion.wav");
    audio.loadSound("victory", "assets/sounds/victory.wav");
    audio.loadSound("power", "assets/sounds/power_use.wav");
    audio.loadSound("enemy_hit", "assets/sounds/enemy_hit.wav");
    audio.loadSound("click_casilla", "assets/sounds/click_casilla_buena.wav");
    audio.loadMusic("background", "assets/music/musica.ogg");
    // Cargar música del menú
    audio.loadMusic("menu", "assets/music/enchanted_tiki_86.mp3");
    
    // Cargar texturas de iconos
    sf::Texture textureMarcarFila, textureAntidoto, textureDetector;
    textureMarcarFila.loadFromFile("assets/images/marcar_fila1.png");
    textureAntidoto.loadFromFile("assets/images/antidoto1.png");
    textureDetector.loadFromFile("assets/images/detector1.png");

    sf::Sprite spriteMarcarFila(textureMarcarFila);
    sf::Sprite spriteAntidoto(textureAntidoto);
    sf::Sprite spriteDetector(textureDetector);

    // Reloj para deltaTime y tiempo de juego
    sf::Clock clock;
    float tiempoJuego = 0.0f;
    
    // Control de si el juego ha iniciado
    bool juegoIniciado = false;
    std::string razonGameOver = "";

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Verificar si se ha cerrado la ventana
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Manejo de estados del juego
            if (gameState.getCurrentState() == GameState::MENU) {
                if (gameState.handleMenuInput(event, window)) {
                    if (gameState.getCurrentState() == GameState::PLAYING) {
                        reiniciarJuego(tablero, enemigo, enemigo2, juegoIniciado, marcarFilaUsado, 
                                      detectorMinasUsado, paralizanteUsado, escudoGanadorUsado, ultimaCasillaContadaBomba, ultimaCasillaContadaBomba2, tiempoJuego, tiempoEscudoGanador, audio, musicaReproduciendo, musicaMenuReproduciendo);
                    }
                }
                continue;
            }
            
            if (gameState.getCurrentState() == GameState::INSTRUCTIONS) {
                gameState.handleInstructionsInput(event);
                continue;
            }
            
            if (gameState.getCurrentState() == GameState::GAME_OVER) {
                if (gameState.handleGameOverInput(event)) {
                    if (gameState.getCurrentState() == GameState::PLAYING) {
                        reiniciarJuego(tablero, enemigo, enemigo2, juegoIniciado, marcarFilaUsado, 
                                      detectorMinasUsado, paralizanteUsado, escudoGanadorUsado, ultimaCasillaContadaBomba, ultimaCasillaContadaBomba2, tiempoJuego, tiempoEscudoGanador, audio, musicaReproduciendo, musicaMenuReproduciendo);
                    }
                }
                continue;
            }
            
            if (gameState.getCurrentState() == GameState::VICTORY) {
                if (gameState.handleVictoryInput(event)) {
                    if (gameState.getCurrentState() == GameState::PLAYING) {
                        reiniciarJuego(tablero, enemigo, enemigo2, juegoIniciado, marcarFilaUsado, 
                                      detectorMinasUsado, paralizanteUsado, escudoGanadorUsado, ultimaCasillaContadaBomba, ultimaCasillaContadaBomba2, tiempoJuego, tiempoEscudoGanador, audio, musicaReproduciendo, musicaMenuReproduciendo);
                    }
                }
                continue;
            }
            
            // Manejo de mouse para descubrir o marcar celdas (solo en estado PLAYING)
            if (event.type == sf::Event::MouseButtonPressed) {
                int x = event.mouseButton.x / CELDA_SIZE;
                int y = event.mouseButton.y / CELDA_SIZE;
                if (x >= 0 && x < COLUMNAS && y >= 0 && y < FILAS) {
                    if (!juegoIniciado) {
                        juegoIniciado = true;
                        // Reproducir música cuando comienza el juego
                        if (!musicaReproduciendo) {
                            audio.playMusic("background", true);
                            musicaReproduciendo = true;
                        }
                    }
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        tablero->descubrir(y, x);
                        audio.playSound("click_casilla");
                    } else if (event.mouseButton.button == sf::Mouse::Right)
                        tablero->marcar(y, x);
                }
            }
            
            // Poder: Marcar todas las minas de una fila con la tecla R
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) {
                    if (!marcarFilaUsado) {
                        int mouseY = sf::Mouse::getPosition(window).y / CELDA_SIZE;
                        if (mouseY >= 0 && mouseY < FILAS) {
                            tablero->marcarFilaDeMinas(mouseY);
                            marcarFilaUsado = true;
                            audio.playSound("power");
                            std::cout << "Poder activado: Se marcaron todas las minas de la fila " << mouseY << std::endl;
                        }
                    } else {
                        std::cout << "El poder 'Marcar Fila' ya fue usado." << std::endl;
                    }
                }
                // Poder: Marcar una mina al azar con la tecla D
                if (event.key.code == sf::Keyboard::D) {
                    if (tablero->getUsosDetectorMinas() > 0) {
                        tablero->marcarMinaAlAzar();
                        audio.playSound("power");
                    } else {
                        std::cout << "No tienes usos del detector de minas." << std::endl;
                    }
                }
                // Poder: Paralizar enemigo con la tecla P (solo si enemigos están activos)
                if (event.key.code == sf::Keyboard::P && enemigosActivos) {
                    if (!paralizanteUsado) {
                        enemigo->paralizarTemporalmente(30.0f);
                        enemigo2->paralizarTemporalmente(30.0f);
                        paralizanteUsado = true;
                        audio.playSound("power");
                        std::cout << "¡AMBOS ENEMIGOS PARALIZADOS POR 30 SEGUNDOS!" << std::endl;
                    } else {
                        std::cout << "El poder 'Paralizante' ya fue usado." << std::endl;
                    }
                }
                // Poder: Escudo Ganador con la tecla E (solo si enemigos están activos)
                if (event.key.code == sf::Keyboard::E && enemigosActivos) {
                    if (!escudoGanadorUsado) {
                        tiempoEscudoGanador = 30.0f;
                        escudoGanadorUsado = true;
                        audio.playSound("power");
                        std::cout << "¡ESCUDO GANADOR ACTIVADO! Eres invulnerable a los enemigos por 30 segundos!" << std::endl;
                    } else {
                        std::cout << "El poder 'Escudo Ganador' ya fue usado." << std::endl;
                    }
                }
            }
        }

        // Limpiar la ventana
        window.clear(sf::Color::Black);
        
        // Obtener deltaTime
        float deltaTime = clock.restart().asSeconds();
        
        // Reproducir música del menú cuando no está jugando
        if (gameState.getCurrentState() == GameState::MENU && !musicaMenuReproduciendo) {
            audio.stopMusic("background");  // Detener solo la música de juego
            audio.playMusic("menu", true);
            musicaMenuReproduciendo = true;
        }
        
        // Detener música del menú y cambiar a música de juego cuando comienza
        if (gameState.getCurrentState() == GameState::PLAYING && juegoIniciado && musicaMenuReproduciendo) {
            audio.stopMusic("menu");  // Detener solo la música del menú
            audio.playMusic("background", true);
            musicaMenuReproduciendo = false;
            musicaReproduciendo = true;
        }
        
        // Solo actualizar tiempo si está jugando
        if (gameState.getCurrentState() == GameState::PLAYING && juegoIniciado) {
            tiempoJuego += deltaTime;
        }
        
        // Actualizar enemigo
        // Primero, crear vector de estados de celdas para el enemigo
        std::vector<std::vector<int>> estadoCeldas(FILAS, std::vector<int>(COLUMNAS));
        for (int i = 0; i < FILAS; ++i) {
            for (int j = 0; j < COLUMNAS; ++j) {
                const Celda& celda = tablero->getCelda(i, j);
                if (celda.getEstado() == Celda::Oculta) {
                    estadoCeldas[i][j] = 0;
                } else if (celda.getEstado() == Celda::Descubierta) {
                    estadoCeldas[i][j] = 1;
                } else {
                    estadoCeldas[i][j] = 2;
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
        
        // Actualizar enemigo (solo si el juego ha iniciado y enemigos están activos)
        if (enemigosActivos) {
            enemigo->actualizar(deltaTime, estadoCeldas, juegoIniciado, filaRaton, colRaton);
            enemigo2->actualizar(deltaTime, estadoCeldas, juegoIniciado, filaRaton, colRaton);  // Segundo enemigo
            
            // Verificar si el enemigo está en una casilla descubierta y teletransportarlo
            enemigo->verificarYTeletransportar(estadoCeldas);
            enemigo2->verificarYTeletransportar(estadoCeldas);  // Segundo enemigo
        }
        
        // Actualizar tiempo del escudo ganador
        if (tiempoEscudoGanador > 0.0f) {
            tiempoEscudoGanador -= deltaTime;
        }
        
        // Detectar colisión enemigo-jugador
        if (gameState.getCurrentState() == GameState::PLAYING && juegoIniciado && enemigosActivos) {
            // Solo considerar colisión si el escudo NO está activo
            if (tiempoEscudoGanador <= 0.0f) {
                if (gameState.checkEnemyPlayerCollision(enemigo->getFila(), enemigo->getColumna(), filaRaton, colRaton)) {
                    razonGameOver = "¡El enemigo te alcanzó!";
                    gameState.setState(GameState::GAME_OVER);
                    audio.playSound("enemy_hit");
                }
                // Colisión con segundo enemigo
                if (gameState.checkEnemyPlayerCollision(enemigo2->getFila(), enemigo2->getColumna(), filaRaton, colRaton)) {
                    razonGameOver = "¡El enemigo 2 te alcanzó!";
                    gameState.setState(GameState::GAME_OVER);
                    audio.playSound("enemy_hit");
                }
            }
        }
        
        // Obtener posición nueva y casillas actualizadas
        auto posNueva = enemigo->obtenerPosicion();
        int casillasAhora = enemigo->getCasillasRecorridas();
        
        // Crear bombas solo si enemigos están activos
        if (enemigosActivos) {
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
                
                int filaEnemigo = enemigo->getFila();
                int colEnemigo = enemigo->getColumna();
                
                if (estadoCeldas[filaEnemigo][colEnemigo] == 0) {
                    tablero->crearBombaTemporalEnCelda(filaEnemigo, colEnemigo);
                    std::cout << "✓ Bomba creada en casilla " << casillasAhora << " en posición (" << filaEnemigo << ", " << colEnemigo << ")" << std::endl;
                } else {
                    std::cout << "✗ Celda revelada en casilla " << casillasAhora << ", no se creó bomba" << std::endl;
                }
            }
            
            // Crear bombas para el segundo enemigo
            int casillasAhora2 = enemigo2->getCasillasRecorridas();
            if (casillasAhora2 > ultimaCasillaContadaBomba2 && (casillasAhora2 % 5 == 0)) {
                ultimaCasillaContadaBomba2 = casillasAhora2;
                
                int filaEnemigo2 = enemigo2->getFila();
                int colEnemigo2 = enemigo2->getColumna();
                
                if (estadoCeldas[filaEnemigo2][colEnemigo2] == 0) {
                    tablero->crearBombaTemporalEnCelda(filaEnemigo2, colEnemigo2);
                    std::cout << "✓ Bomba 2 creada en casilla " << casillasAhora2 << " en posición (" << filaEnemigo2 << ", " << colEnemigo2 << ")" << std::endl;
                } else {
                    std::cout << "✗ Celda revelada en casilla " << casillasAhora2 << ", no se creó bomba 2" << std::endl;
                }
            }
        }
        
        // Actualizar bombas
        tablero->actualizarBombas(deltaTime);
        
        // Verificar fin del juego
        if (gameState.getCurrentState() == GameState::PLAYING && tablero->esFinDelJuego()) {
            if (tablero->esVictoria()) {
                gameState.setState(GameState::VICTORY);
                audio.playSound("victory");
            } else {
                razonGameOver = "¡Pisaste una plaga!";
                gameState.setState(GameState::GAME_OVER);
                audio.playSound("explosion");
            }
        }
        
        // Dibujar el tablero
        tablero->dibujar(window, CELDA_SIZE);
        
        // Dibujar bombas temporales con efecto visual
        auto posBombas = tablero->obtenerPosicionesBombas();
        for (const auto& bomba : posBombas) {
            sf::RectangleShape bombaDibujo(sf::Vector2f(CELDA_SIZE * 0.6f, CELDA_SIZE * 0.6f));
            bombaDibujo.setPosition(bomba.second * CELDA_SIZE + CELDA_SIZE * 0.2f, 
                                   bomba.first * CELDA_SIZE + CELDA_SIZE * 0.2f);
            bombaDibujo.setFillColor(sf::Color::Yellow);
            bombaDibujo.setOutlineColor(sf::Color::Red);
            bombaDibujo.setOutlineThickness(2.0f);
            window.draw(bombaDibujo);
        }
        
        // Dibujar enemigo (solo si están activos)
        if (enemigosActivos) {
            enemigo->dibujar(window, CELDA_SIZE);
            enemigo2->dibujar(window, CELDA_SIZE);  // Segundo enemigo
        }
        
        // Dibujar barra lateral con poderes
        float boardWidth = COLUMNAS * CELDA_SIZE;
        
        // Fondo de la barra lateral
        sf::RectangleShape sidebar(sf::Vector2f(SIDEBAR_WIDTH, FILAS * CELDA_SIZE));
        sidebar.setPosition(boardWidth, 0);
        sidebar.setFillColor(sf::Color(50, 50, 50));
        window.draw(sidebar);
        
        // Título "PODERES"
        sf::Text titleText("PODERES", font, 20);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(boardWidth + 20, 20);
        window.draw(titleText);
        
        // Poder 1: Marcar fila
        sf::RectangleShape powerButton(sf::Vector2f(SIDEBAR_WIDTH - 20, 60));
        powerButton.setPosition(boardWidth + 10, 80);
        
        if (marcarFilaUsado) {
            powerButton.setFillColor(sf::Color(200, 100, 100));
        } else {
            powerButton.setFillColor(sf::Color(100, 150, 255));
        }
        
        powerButton.setOutlineColor(sf::Color::White);
        powerButton.setOutlineThickness(2);
        window.draw(powerButton);
        
        sf::Text powerText("Marcar Fila\n(Presiona R)", font, 14);
        powerText.setFillColor(sf::Color::White);
        powerText.setPosition(boardWidth + 60, 95); // Movido a la derecha
        window.draw(powerText);

        // Icono Marcar Fila
        spriteMarcarFila.setPosition(boardWidth + 15, 85);
        spriteMarcarFila.setScale(40.0f / spriteMarcarFila.getLocalBounds().width, 40.0f / spriteMarcarFila.getLocalBounds().height);
        window.draw(spriteMarcarFila);
        
        // Poder 2: Antídoto
        sf::RectangleShape antidotoButton(sf::Vector2f(SIDEBAR_WIDTH - 20, 60));
        antidotoButton.setPosition(boardWidth + 10, 160);
        
        if (!tablero->tieneAntidoto()) {
            antidotoButton.setFillColor(sf::Color(200, 100, 100));
        } else {
            antidotoButton.setFillColor(sf::Color(100, 255, 100));
        }
        
        antidotoButton.setOutlineColor(sf::Color::White);
        antidotoButton.setOutlineThickness(2);
        window.draw(antidotoButton);
        
        sf::Text antidotoText("Antidoto\n(Automatico)", font, 14);
        antidotoText.setFillColor(sf::Color::White);
        antidotoText.setPosition(boardWidth + 60, 175); // Movido a la derecha
        window.draw(antidotoText);

        // Icono Antidoto
        spriteAntidoto.setPosition(boardWidth + 5, 162);
        spriteAntidoto.setScale(55.0f / spriteAntidoto.getLocalBounds().width, 55.0f / spriteAntidoto.getLocalBounds().height);
        window.draw(spriteAntidoto);
        
        // Poder 3: Detector de Minas
        sf::RectangleShape detectorButton(sf::Vector2f(SIDEBAR_WIDTH - 20, 60));
        detectorButton.setPosition(boardWidth + 10, 240);
        
        if (tablero->getUsosDetectorMinas() <= 0) {
            detectorButton.setFillColor(sf::Color(200, 100, 100));
        } else {
            detectorButton.setFillColor(sf::Color(255, 200, 100));
        }
        
        detectorButton.setOutlineColor(sf::Color::White);
        detectorButton.setOutlineThickness(2);
        window.draw(detectorButton);
        
        std::string detectorInfo = "Detector Mina\n(Presiona D)\n(" + std::to_string(tablero->getUsosDetectorMinas()) + " usos)";
        sf::Text detectorText(detectorInfo, font, 12);
        detectorText.setFillColor(sf::Color::White);
        detectorText.setPosition(boardWidth + 60, 250); // Movido a la derecha
        window.draw(detectorText);

        // Icono Detector
        spriteDetector.setPosition(boardWidth + 15, 245);
        spriteDetector.setScale(40.0f / spriteDetector.getLocalBounds().width, 40.0f / spriteDetector.getLocalBounds().height);
        window.draw(spriteDetector);
        
        // Poder 4: Paralizante (solo si enemigos están activos)
        if (enemigosActivos) {
            sf::RectangleShape paralizanteButton(sf::Vector2f(SIDEBAR_WIDTH - 20, 60));
            paralizanteButton.setPosition(boardWidth + 10, 320);
            
            if (paralizanteUsado) {
                paralizanteButton.setFillColor(sf::Color(200, 100, 100));
            } else {
                paralizanteButton.setFillColor(sf::Color(150, 100, 255));
            }
            
            paralizanteButton.setOutlineColor(sf::Color::White);
            paralizanteButton.setOutlineThickness(2);
            window.draw(paralizanteButton);
            
            sf::Text paralizanteText("Paralizante\n(Presiona P)", font, 14);
            paralizanteText.setFillColor(sf::Color::White);
            paralizanteText.setPosition(boardWidth + 15, 335);
            window.draw(paralizanteText);
        }
        
        // Poder 5: Escudo Ganador (solo si enemigos están activos)
        if (enemigosActivos) {
            sf::RectangleShape escudoButton(sf::Vector2f(SIDEBAR_WIDTH - 20, 60));
            escudoButton.setPosition(boardWidth + 10, 400);
            
            if (escudoGanadorUsado) {
                escudoButton.setFillColor(sf::Color(200, 100, 100));
            } else {
                if (tiempoEscudoGanador > 0.0f) {
                    escudoButton.setFillColor(sf::Color(255, 215, 0));  // Gold mientras está activo
                } else {
                    escudoButton.setFillColor(sf::Color(200, 200, 100));
                }
            }
            
            escudoButton.setOutlineColor(sf::Color::White);
            escudoButton.setOutlineThickness(2);
            window.draw(escudoButton);
            
            std::string escudoInfo = "Escudo Ganador\n(Presiona E)";
            if (tiempoEscudoGanador > 0.0f) {
                escudoInfo += "\n(" + std::to_string((int)tiempoEscudoGanador) + "s)";
            }
            sf::Text escudoText(escudoInfo, font, 12);
            escudoText.setFillColor(sf::Color::White);
            escudoText.setPosition(boardWidth + 15, 408);
            window.draw(escudoText);
        }

        // Contador de Plagas
        sf::RectangleShape counterBox(sf::Vector2f(SIDEBAR_WIDTH - 20, 40));
        counterBox.setPosition(boardWidth + 10, 580);
        counterBox.setFillColor(sf::Color(50, 50, 50));
        counterBox.setOutlineColor(sf::Color::White);
        counterBox.setOutlineThickness(2);
        window.draw(counterBox);

        std::string plagasInfo = "Plagas: " + std::to_string(tablero->getMinasMarcadas()) + " / " + std::to_string(tablero->getMinas());
        sf::Text plagasText(plagasInfo, font, 16);
        plagasText.setFillColor(sf::Color::White);
        plagasText.setPosition(boardWidth + 20, 590);
        window.draw(plagasText);
                
        // Dibujar indicador de proximidad si está jugando y enemigos están activos (mostrar el enemigo más cercano)
        if (gameState.getCurrentState() == GameState::PLAYING && juegoIniciado && enemigosActivos) {
            // Calcular distancia a cada enemigo
            int distancia1 = std::abs(enemigo->getFila() - filaRaton) + std::abs(enemigo->getColumna() - colRaton);
            int distancia2 = std::abs(enemigo2->getFila() - filaRaton) + std::abs(enemigo2->getColumna() - colRaton);
            
            // Determinar cuál está más cerca
            if (distancia1 <= distancia2) {
                gameState.drawProximityIndicator(window, enemigo->getFila(), enemigo->getColumna(), 
                                                filaRaton, colRaton, CELDA_SIZE, font);
            } else {
                gameState.drawProximityIndicator(window, enemigo2->getFila(), enemigo2->getColumna(), 
                                                filaRaton, colRaton, CELDA_SIZE, font);
            }
        }
        
        // Dibujar pantallas según estado
        if (gameState.getCurrentState() == GameState::MENU) {
            gameState.drawMenu(window, font);
        } else if (gameState.getCurrentState() == GameState::INSTRUCTIONS) {
            gameState.drawInstructions(window, font);
        } else if (gameState.getCurrentState() == GameState::GAME_OVER) {
            gameState.drawGameOver(window, font, razonGameOver);
        } else if (gameState.getCurrentState() == GameState::VICTORY) {
            gameState.drawVictory(window, font, tiempoJuego);
        }

        // Mostrar la ventana
        window.display();
    }
    
    // Limpiar memoria
    delete tablero;
    delete enemigo;

    return 0;
}
