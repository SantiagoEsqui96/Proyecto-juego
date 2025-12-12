#include "../include/GameState.hpp"
#include <cmath>
#include <sstream>
#include <iomanip>

GameState::GameState() : currentState(MENU) {
    backgroundTexture.loadFromFile("assets/images/Portada.png");
}

GameState::State GameState::getCurrentState() const {
    return currentState;
}

void GameState::setState(State newState) {
    currentState = newState;
}

void GameState::drawMenu(sf::RenderWindow& window, const sf::Font& font, Difficulty currentDifficulty, bool showDifficultyMenu) {
    // Dibujar imagen de fondo
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / backgroundSprite.getLocalBounds().width,
        static_cast<float>(window.getSize().y) / backgroundSprite.getLocalBounds().height
    );
    window.draw(backgroundSprite);

    // Fondo semi-transparente
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(0, 0, 0, 150)); // Reducir opacidad para ver mejor la imagen
    window.draw(background);
    
    // Título del juego
    sf::Text title("BUSCAPLAGAS", font, 80);
    title.setFillColor(sf::Color::Green);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition((window.getSize().x - titleBounds.width) / 2, 100);
    window.draw(title);
    
    // Subtítulo
    sf::Text subtitle("Encuentra las plagas antes de que te alcancen", font, 24);
    subtitle.setFillColor(sf::Color::White);
    sf::FloatRect subtitleBounds = subtitle.getLocalBounds();
    subtitle.setPosition((window.getSize().x - subtitleBounds.width) / 2, 200);
    window.draw(subtitle);
    
    // Botón Jugar
    sf::RectangleShape playButton(sf::Vector2f(300, 60));
    playButton.setPosition((window.getSize().x - 300) / 2, 320);
    playButton.setFillColor(sf::Color(0, 150, 0));
    playButton.setOutlineColor(sf::Color::White);
    playButton.setOutlineThickness(3);
    window.draw(playButton);
    
    sf::Text playText("JUGAR", font, 30);
    playText.setFillColor(sf::Color::White);
    sf::FloatRect playBounds = playText.getLocalBounds();
    playText.setPosition((window.getSize().x - playBounds.width) / 2, 330);
    window.draw(playText);
    
    // Botón Dificultad
    sf::RectangleShape difficultyButton(sf::Vector2f(300, 60));
    difficultyButton.setPosition((window.getSize().x - 300) / 2, 400);
    difficultyButton.setFillColor(sf::Color(150, 100, 0));
    difficultyButton.setOutlineColor(sf::Color::White);
    difficultyButton.setOutlineThickness(3);
    window.draw(difficultyButton);
    
    std::string diffText;
    if (currentDifficulty == EASY) diffText = "FACIL";
    else if (currentDifficulty == MEDIUM) diffText = "MEDIO";
    else diffText = "DIFICIL";
    
    sf::Text diffLabel("DIFICULTAD: " + diffText, font, 18);
    diffLabel.setFillColor(sf::Color::White);
    sf::FloatRect diffBounds = diffLabel.getLocalBounds();
    diffLabel.setPosition((window.getSize().x - diffBounds.width) / 2, 413);
    window.draw(diffLabel);
    
    // Botón Enemigos ON/OFF
    sf::RectangleShape enemiesButton(sf::Vector2f(300, 60));
    enemiesButton.setPosition((window.getSize().x - 300) / 2, 480);
    enemiesButton.setFillColor(sf::Color(100, 100, 100));
    enemiesButton.setOutlineColor(sf::Color::White);
    enemiesButton.setOutlineThickness(3);
    window.draw(enemiesButton);
    
    extern bool enemigosActivos;
    std::string enemiesText = enemigosActivos ? "ENEMIGOS: ON" : "ENEMIGOS: OFF";
    sf::Text enemiesLabel(enemiesText, font, 20);
    enemiesLabel.setFillColor(sf::Color::White);
    sf::FloatRect enemiesBounds = enemiesLabel.getLocalBounds();
    enemiesLabel.setPosition((window.getSize().x - enemiesBounds.width) / 2, 493);
    window.draw(enemiesLabel);
    
    // Botón Instrucciones
    sf::RectangleShape instructionsButton(sf::Vector2f(300, 60));
    instructionsButton.setPosition((window.getSize().x - 300) / 2, 560);
    instructionsButton.setFillColor(sf::Color(0, 100, 200));
    instructionsButton.setOutlineColor(sf::Color::White);
    instructionsButton.setOutlineThickness(3);
    window.draw(instructionsButton);
    
    sf::Text instructionsText("INSTRUCCIONES", font, 22);
    instructionsText.setFillColor(sf::Color::White);
    sf::FloatRect instrBounds = instructionsText.getLocalBounds();
    instructionsText.setPosition((window.getSize().x - instrBounds.width) / 2, 570);
    window.draw(instructionsText);
    
    // Botón Salir (solo cuando submenú NO está abierto)
    if (!showDifficultyMenu) {
        sf::RectangleShape exitButton(sf::Vector2f(300, 60));
        exitButton.setPosition((window.getSize().x - 300) / 2, 640);
        exitButton.setFillColor(sf::Color(150, 0, 0));
        exitButton.setOutlineColor(sf::Color::White);
        exitButton.setOutlineThickness(3);
        window.draw(exitButton);
        
        sf::Text exitText("SALIR", font, 30);
        exitText.setFillColor(sf::Color::White);
        sf::FloatRect exitBounds = exitText.getLocalBounds();
        exitText.setPosition((window.getSize().x - exitBounds.width) / 2, 650);
        window.draw(exitText);
    }
    
    // Mostrar submenú de dificultad si está abierto
    if (showDifficultyMenu) {
        // Fondo oscuro para el submenú
        sf::RectangleShape submenuBackground(sf::Vector2f(window.getSize().x, window.getSize().y));
        submenuBackground.setFillColor(sf::Color(0, 0, 0, 100));
        window.draw(submenuBackground);
        
        // Título del submenú
        sf::Text submenuTitle("SELECCIONA DIFICULTAD", font, 40);
        submenuTitle.setFillColor(sf::Color::Yellow);
        submenuTitle.setStyle(sf::Text::Bold);
        sf::FloatRect submenuTitleBounds = submenuTitle.getLocalBounds();
        submenuTitle.setPosition((window.getSize().x - submenuTitleBounds.width) / 2, 150);
        window.draw(submenuTitle);
        
        // Botón FACIL
        sf::RectangleShape easyButton(sf::Vector2f(250, 70));
        easyButton.setPosition((window.getSize().x - 250) / 2, 260);
        easyButton.setFillColor(currentDifficulty == EASY ? sf::Color(0, 200, 0) : sf::Color(0, 100, 0));
        easyButton.setOutlineColor(sf::Color::White);
        easyButton.setOutlineThickness(3);
        window.draw(easyButton);
        
        sf::Text easyLabel("FACIL\n20x20 - 40 minas", font, 16);
        easyLabel.setFillColor(sf::Color::White);
        sf::FloatRect easyBounds = easyLabel.getLocalBounds();
        easyLabel.setPosition((window.getSize().x - easyBounds.width) / 2, 273);
        window.draw(easyLabel);
        
        // Botón MEDIO (predeterminado)
        sf::RectangleShape mediumButton(sf::Vector2f(250, 70));
        mediumButton.setPosition((window.getSize().x - 250) / 2, 360);
        mediumButton.setFillColor(currentDifficulty == MEDIUM ? sf::Color(200, 200, 0) : sf::Color(150, 100, 0));
        mediumButton.setOutlineColor(sf::Color::White);
        mediumButton.setOutlineThickness(3);
        window.draw(mediumButton);
        
        sf::Text mediumLabel("MEDIO\n30x30 - 135 minas", font, 16);
        mediumLabel.setFillColor(sf::Color::White);
        sf::FloatRect mediumBounds = mediumLabel.getLocalBounds();
        mediumLabel.setPosition((window.getSize().x - mediumBounds.width) / 2, 373);
        window.draw(mediumLabel);
        
        // Botón DIFÍCIL
        sf::RectangleShape hardButton(sf::Vector2f(250, 70));
        hardButton.setPosition((window.getSize().x - 250) / 2, 460);
        hardButton.setFillColor(currentDifficulty == HARD ? sf::Color(200, 0, 0) : sf::Color(100, 0, 0));
        hardButton.setOutlineColor(sf::Color::White);
        hardButton.setOutlineThickness(3);
        window.draw(hardButton);
        
        sf::Text hardLabel("DIFICIL\n40x40 - 350 minas", font, 16);
        hardLabel.setFillColor(sf::Color::White);
        sf::FloatRect hardBounds = hardLabel.getLocalBounds();
        hardLabel.setPosition((window.getSize().x - hardBounds.width) / 2, 473);
        window.draw(hardLabel);
        
        // Instrucción para cerrar submenú
        sf::Text closeHint("(Presiona ESC para cerrar)", font, 14);
        closeHint.setFillColor(sf::Color::Cyan);
        sf::FloatRect closeHintBounds = closeHint.getLocalBounds();
        closeHint.setPosition((window.getSize().x - closeHintBounds.width) / 2, 560);
        window.draw(closeHint);
    }
}


void GameState::drawPause(sf::RenderWindow& window, const sf::Font& font) {
    // Fondo semi-transparente oscuro
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(background);
    
    // Título PAUSA
    sf::Text title("PAUSA", font, 80);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition((window.getSize().x - titleBounds.width) / 2, 150);
    window.draw(title);
    
    // Botón Continuar
    sf::RectangleShape resumeButton(sf::Vector2f(300, 60));
    resumeButton.setPosition((window.getSize().x - 300) / 2, 300);
    resumeButton.setFillColor(sf::Color(0, 100, 200));
    resumeButton.setOutlineColor(sf::Color::White);
    resumeButton.setOutlineThickness(3);
    window.draw(resumeButton);
    
    sf::Text resumeText("CONTINUAR", font, 30);
    resumeText.setFillColor(sf::Color::White);
    sf::FloatRect resumeBounds = resumeText.getLocalBounds();
    resumeText.setPosition((window.getSize().x - resumeBounds.width) / 2, 310);
    window.draw(resumeText);
    
    // Botón Reiniciar
    sf::RectangleShape restartButton(sf::Vector2f(300, 60));
    restartButton.setPosition((window.getSize().x - 300) / 2, 380);
    restartButton.setFillColor(sf::Color(200, 100, 0));
    restartButton.setOutlineColor(sf::Color::White);
    restartButton.setOutlineThickness(3);
    window.draw(restartButton);
    
    sf::Text restartText("REINICIAR", font, 30);
    restartText.setFillColor(sf::Color::White);
    sf::FloatRect restartBounds = restartText.getLocalBounds();
    restartText.setPosition((window.getSize().x - restartBounds.width) / 2, 390);
    window.draw(restartText);
    
    // Botón Menú
    sf::RectangleShape menuButton(sf::Vector2f(300, 60));
    menuButton.setPosition((window.getSize().x - 300) / 2, 460);
    menuButton.setFillColor(sf::Color(150, 0, 0));
    menuButton.setOutlineColor(sf::Color::White);
    menuButton.setOutlineThickness(3);
    window.draw(menuButton);
    
    sf::Text menuText("MENU PRINCIPAL", font, 26);
    menuText.setFillColor(sf::Color::White);
    sf::FloatRect menuBounds = menuText.getLocalBounds();
    menuText.setPosition((window.getSize().x - menuBounds.width) / 2, 473);
    window.draw(menuText);
}

void GameState::drawInstructions(sf::RenderWindow& window, const sf::Font& font, const sf::Sprite& background) {
    // Dibujar fondo escalado a la ventana completa
    sf::Sprite scaledBackground = background;
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = scaledBackground.getTexture()->getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    scaledBackground.setScale(scaleX, scaleY);
    window.draw(scaledBackground);
    
    // Fondo semi-transparente oscuro sobre el fondo
    sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    // Título
    sf::Text title("INSTRUCCIONES", font, 50);
    title.setFillColor(sf::Color::Yellow);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition((window.getSize().x - titleBounds.width) / 2, 50);
    window.draw(title);
    
    // Instrucciones
    std::vector<std::string> instructions = {
        "OBJETIVO:",
        "Descubre todas las celdas sin plagas antes de que el enemigo te alcance",
        "",
        "CONTROLES:",
        "Click Izquierdo - Descubrir celda",
        "Click Derecho - Marcar celda",
        "",
        "PODERES:",
        "R - Marcar Fila: Marca todas las plagas de una fila (1 uso)",
        "C - Marcar Columna: Marca todas las plagas de una columna (1 uso)",
        "D - Detector: Marca una plaga al azar (5 usos)",
        "P - Paralizante: Paraliza al enemigo por 30 segundos (1 uso)",
        "E - Escudo Ganador: Invulnerable a enemigos por 30s (1 uso SOLO PARA GANAR)",
        "Antidoto: Te salva automaticamente de 1 plaga (1 uso)",
        "",
        "NOTA IMPORTANTE:",
        "Usa el Escudo Ganador solo para la victoria final. Los enemigos bloquean",
        "las ultimas celdas, por eso necesitas el escudo para acabar de revelar todo",
        "",
        "PELIGROS:",
        "- Los enemigos rojo y morado te persiguen y dejan bombas cada 5 casillas",
        "- Las bombas duran 20 segundos (cuadrados amarillos)",
        "- Si el enemigo te alcanza, pierdes (a menos que tengas escudo)",
        "",
        "Presiona ESC para volver al menu"
    };
    
    float yPos = 150;
    for (const auto& line : instructions) {
        sf::Text text(line, font, 18);
        if (line.find("OBJETIVO:") != std::string::npos || 
            line.find("CONTROLES:") != std::string::npos || 
            line.find("PODERES:") != std::string::npos ||
            line.find("PELIGROS:") != std::string::npos) {
            text.setFillColor(sf::Color::Cyan);
            text.setStyle(sf::Text::Bold);
        } else {
            text.setFillColor(sf::Color::White);
        }
        text.setPosition(100, yPos);
        window.draw(text);
        yPos += 30;
    }
}

void GameState::drawGameOver(sf::RenderWindow& window, const sf::Font& font, const std::string& reason) {
    // Fondo semi-transparente rojo
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(100, 0, 0, 200));
    window.draw(background);
    
    // Título GAME OVER
    sf::Text title("GAME OVER", font, 80);
    title.setFillColor(sf::Color::Red);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition((window.getSize().x - titleBounds.width) / 2, 200);
    window.draw(title);
    
    // Razón de la derrota
    sf::Text reasonText(reason, font, 30);
    reasonText.setFillColor(sf::Color::White);
    sf::FloatRect reasonBounds = reasonText.getLocalBounds();
    reasonText.setPosition((window.getSize().x - reasonBounds.width) / 2, 320);
    window.draw(reasonText);
    
    // Instrucciones
    sf::Text instructions("Presiona R para reiniciar o ESC para volver al menu", font, 24);
    instructions.setFillColor(sf::Color::Yellow);
    sf::FloatRect instrBounds = instructions.getLocalBounds();
    instructions.setPosition((window.getSize().x - instrBounds.width) / 2, 450);
    window.draw(instructions);
}

void GameState::drawVictory(sf::RenderWindow& window, const sf::Font& font, float timeElapsed) {
    // Fondo semi-transparente verde
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(0, 100, 0, 200));
    window.draw(background);
    
    // Título VICTORIA
    sf::Text title("VICTORIA!", font, 80);
    title.setFillColor(sf::Color::Green);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition((window.getSize().x - titleBounds.width) / 2, 200);
    window.draw(title);
    
    // Mensaje de felicitación
    sf::Text congrats("Encontraste todas las plagas!", font, 30);
    congrats.setFillColor(sf::Color::White);
    sf::FloatRect congratsBounds = congrats.getLocalBounds();
    congrats.setPosition((window.getSize().x - congratsBounds.width) / 2, 320);
    window.draw(congrats);
    
    // Tiempo transcurrido
    std::stringstream ss;
    ss << "Tiempo: " << std::fixed << std::setprecision(1) << timeElapsed << " segundos";
    sf::Text timeText(ss.str(), font, 26);
    timeText.setFillColor(sf::Color::Yellow);
    sf::FloatRect timeBounds = timeText.getLocalBounds();
    timeText.setPosition((window.getSize().x - timeBounds.width) / 2, 380);
    window.draw(timeText);
    
    // Instrucciones
    sf::Text instructions("Presiona R para reiniciar o ESC para volver al menu", font, 24);
    instructions.setFillColor(sf::Color::Cyan);
    sf::FloatRect instrBounds = instructions.getLocalBounds();
    instructions.setPosition((window.getSize().x - instrBounds.width) / 2, 450);
    window.draw(instructions);
}

bool GameState::handleMenuInput(sf::Event& event, sf::RenderWindow& window, bool& showDifficultyMenu) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        int mouseX = event.mouseButton.x;
        int mouseY = event.mouseButton.y;
        
        // Si el submenú está abierto, no procesar otros botones del menú
        if (showDifficultyMenu) {
            return false; // Dejar que handleDifficultyMenuInput maneje los clicks
        }
        
        // Botón Jugar
        if (mouseX >= (window.getSize().x - 300) / 2 && mouseX <= (window.getSize().x + 300) / 2 &&
            mouseY >= 320 && mouseY <= 380) {
            setState(PLAYING);
            return true;
        }
        
        // Botón Dificultad (abre/cierra submenú)
        if (mouseX >= (window.getSize().x - 300) / 2 && mouseX <= (window.getSize().x + 300) / 2 &&
            mouseY >= 400 && mouseY <= 460) {
            showDifficultyMenu = !showDifficultyMenu;
            return true;
        }
        
        // Botón Enemigos ON/OFF
        if (mouseX >= (window.getSize().x - 300) / 2 && mouseX <= (window.getSize().x + 300) / 2 &&
            mouseY >= 480 && mouseY <= 540) {
            extern bool enemigosActivos;
            enemigosActivos = !enemigosActivos;
            return true;
        }
        
        // Botón Instrucciones
        if (mouseX >= (window.getSize().x - 300) / 2 && mouseX <= (window.getSize().x + 300) / 2 &&
            mouseY >= 560 && mouseY <= 620) {
            setState(INSTRUCTIONS);
            return true;
        }
        
        // Botón Salir
        if (mouseX >= (window.getSize().x - 300) / 2 && mouseX <= (window.getSize().x + 300) / 2 &&
            mouseY >= 640 && mouseY <= 700) {
            window.close();
            return true;
        }
    }
    
    // ESC para cerrar submenú de dificultad
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        if (showDifficultyMenu) {
            showDifficultyMenu = false;
            return true;
        }
    }
    
    return false;
}

int GameState::handleDifficultyMenuInput(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        int mouseX = event.mouseButton.x;
        int mouseY = event.mouseButton.y;
        
        // Botón FACIL
        if (mouseX >= (window.getSize().x - 250) / 2 && mouseX <= (window.getSize().x + 250) / 2 &&
            mouseY >= 260 && mouseY <= 330) {
            return 0; // EASY
        }
        
        // Botón MEDIO
        if (mouseX >= (window.getSize().x - 250) / 2 && mouseX <= (window.getSize().x + 250) / 2 &&
            mouseY >= 360 && mouseY <= 430) {
            return 1; // MEDIUM
        }
        
        // Botón DIFÍCIL
        if (mouseX >= (window.getSize().x - 250) / 2 && mouseX <= (window.getSize().x + 250) / 2 &&
            mouseY >= 460 && mouseY <= 530) {
            return 2; // HARD
        }
    }
    
    return -1; // No selection
}


int GameState::handlePauseInput(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        setState(PLAYING);
        return 1; // Resume
    }
    
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        int mouseX = event.mouseButton.x;
        int mouseY = event.mouseButton.y;
        
        // Botón Continuar
        if (mouseX >= (window.getSize().x - 300) / 2 && mouseX <= (window.getSize().x + 300) / 2 &&
            mouseY >= 300 && mouseY <= 360) {
            setState(PLAYING);
            return 1; // Resume
        }
        
        // Botón Reiniciar
        if (mouseX >= (window.getSize().x - 300) / 2 && mouseX <= (window.getSize().x + 300) / 2 &&
            mouseY >= 380 && mouseY <= 440) {
            setState(PLAYING);
            return 2; // Restart
        }
        
        // Botón Menú
        if (mouseX >= (window.getSize().x - 300) / 2 && mouseX <= (window.getSize().x + 300) / 2 &&
            mouseY >= 460 && mouseY <= 520) {
            setState(MENU);
            return 3; // Menu
        }
    }
    return 0;
}

bool GameState::handleInstructionsInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        setState(MENU);
        return true;
    }
    return false;
}

bool GameState::handleGameOverInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::R) {
            setState(PLAYING);
            return true;  // Señal para reiniciar
        } else if (event.key.code == sf::Keyboard::Escape) {
            setState(MENU);
            return true;
        }
    }
    return false;
}

bool GameState::handleVictoryInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::R) {
            setState(PLAYING);
            return true;  // Señal para reiniciar
        } else if (event.key.code == sf::Keyboard::Escape) {
            setState(MENU);
            return true;
        }
    }
    return false;
}

bool GameState::checkEnemyPlayerCollision(int enemyRow, int enemyCol, int playerRow, int playerCol) {
    return (enemyRow == playerRow && enemyCol == playerCol);
}

int GameState::calculateDistance(int row1, int col1, int row2, int col2) {
    return std::abs(row1 - row2) + std::abs(col1 - col2);  // Distancia Manhattan
}

sf::Color GameState::getProximityColor(int distance) {
    if (distance <= 3) {
        return sf::Color::Red;
    } else if (distance <= 6) {
        return sf::Color(255, 165, 0);  // Naranja
    } else if (distance <= 10) {
        return sf::Color::Yellow;
    } else {
        return sf::Color::Green;
    }
}

void GameState::drawProximityIndicator(sf::RenderWindow& window, int enemyRow, int enemyCol, 
                                      int playerRow, int playerCol, float cellSize, const sf::Font& font,
                                      float offsetX, float offsetY, int columnas) {
    int distance = calculateDistance(enemyRow, enemyCol, playerRow, playerCol);
    sf::Color color = getProximityColor(distance);
    
    // Posicionar en la barra lateral, entre el último poder y el contador de plagas
    float boardWidth = columnas * cellSize;
    float indicatorX = boardWidth + offsetX + 10;
    float indicatorY = offsetY + 520;  // Entre los poderes y el contador
    
    // Indicador de proximidad
    sf::RectangleShape indicator(sf::Vector2f(180, 50));  // Más pequeño para caber mejor
    indicator.setPosition(indicatorX, indicatorY);
    indicator.setFillColor(sf::Color(50, 50, 50));
    indicator.setOutlineColor(color);
    indicator.setOutlineThickness(3);
    window.draw(indicator);
    
    sf::Text title("ENEMIGO", font, 14);
    title.setFillColor(sf::Color::White);
    title.setPosition(indicatorX + 35, indicatorY + 5);
    window.draw(title);
    
    std::string distText = "Dist: " + std::to_string(distance);
    sf::Text distanceText(distText, font, 12);
    distanceText.setFillColor(color);
    distanceText.setPosition(indicatorX + 25, indicatorY + 25);
    window.draw(distanceText);
}
