#include "../include/GameState.hpp"
#include <cmath>
#include <sstream>
#include <iomanip>

GameState::GameState() : currentState(MENU) {}

GameState::State GameState::getCurrentState() const {
    return currentState;
}

void GameState::setState(State newState) {
    currentState = newState;
}

void GameState::drawMenu(sf::RenderWindow& window, const sf::Font& font) {
    // Fondo semi-transparente
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(0, 0, 0, 200));
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
    playButton.setPosition((window.getSize().x - 300) / 2, 350);
    playButton.setFillColor(sf::Color(0, 150, 0));
    playButton.setOutlineColor(sf::Color::White);
    playButton.setOutlineThickness(3);
    window.draw(playButton);
    
    sf::Text playText("JUGAR", font, 30);
    playText.setFillColor(sf::Color::White);
    sf::FloatRect playBounds = playText.getLocalBounds();
    playText.setPosition((window.getSize().x - playBounds.width) / 2, 360);
    window.draw(playText);
    
    // Botón Instrucciones
    sf::RectangleShape instructionsButton(sf::Vector2f(300, 60));
    instructionsButton.setPosition((window.getSize().x - 300) / 2, 450);
    instructionsButton.setFillColor(sf::Color(0, 100, 200));
    instructionsButton.setOutlineColor(sf::Color::White);
    instructionsButton.setOutlineThickness(3);
    window.draw(instructionsButton);
    
    sf::Text instructionsText("INSTRUCCIONES", font, 26);
    instructionsText.setFillColor(sf::Color::White);
    sf::FloatRect instrBounds = instructionsText.getLocalBounds();
    instructionsText.setPosition((window.getSize().x - instrBounds.width) / 2, 460);
    window.draw(instructionsText);
    
    // Botón Salir
    sf::RectangleShape exitButton(sf::Vector2f(300, 60));
    exitButton.setPosition((window.getSize().x - 300) / 2, 550);
    exitButton.setFillColor(sf::Color(150, 0, 0));
    exitButton.setOutlineColor(sf::Color::White);
    exitButton.setOutlineThickness(3);
    window.draw(exitButton);
    
    sf::Text exitText("SALIR", font, 30);
    exitText.setFillColor(sf::Color::White);
    sf::FloatRect exitBounds = exitText.getLocalBounds();
    exitText.setPosition((window.getSize().x - exitBounds.width) / 2, 560);
    window.draw(exitText);
}

void GameState::drawInstructions(sf::RenderWindow& window, const sf::Font& font) {
    // Fondo semi-transparente
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(0, 0, 0, 220));
    window.draw(background);
    
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
        "D - Detector: Marca una plaga al azar (5 usos)",
        "P - Paralizante: Paraliza al enemigo por 30 segundos (1 uso)",
        "Antidoto: Te salva automaticamente de 1 plaga (1 uso)",
        "",
        "PELIGROS:",
        "- El enemigo rojo te persigue y deja bombas cada 5 casillas",
        "- Las bombas duran 20 segundos (cuadrados amarillos)",
        "- Si el enemigo te alcanza, pierdes",
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
    sf::Text title("¡VICTORIA!", font, 80);
    title.setFillColor(sf::Color::Green);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition((window.getSize().x - titleBounds.width) / 2, 200);
    window.draw(title);
    
    // Mensaje de felicitación
    sf::Text congrats("¡Encontraste todas las plagas!", font, 30);
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

bool GameState::handleMenuInput(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        int mouseX = event.mouseButton.x;
        int mouseY = event.mouseButton.y;
        
        // Botón Jugar
        if (mouseX >= (window.getSize().x - 300) / 2 && mouseX <= (window.getSize().x + 300) / 2 &&
            mouseY >= 350 && mouseY <= 410) {
            setState(PLAYING);
            return true;
        }
        
        // Botón Instrucciones
        if (mouseX >= (window.getSize().x - 300) / 2 && mouseX <= (window.getSize().x + 300) / 2 &&
            mouseY >= 450 && mouseY <= 510) {
            setState(INSTRUCTIONS);
            return true;
        }
        
        // Botón Salir
        if (mouseX >= (window.getSize().x - 300) / 2 && mouseX <= (window.getSize().x + 300) / 2 &&
            mouseY >= 550 && mouseY <= 610) {
            window.close();
            return true;
        }
    }
    return false;
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
                                      int playerRow, int playerCol, float cellSize, const sf::Font& font) {
    int distance = calculateDistance(enemyRow, enemyCol, playerRow, playerCol);
    sf::Color color = getProximityColor(distance);
    
    // Calcular posición en la esquina superior derecha del tablero
    float boardWidth = cellSize * 40;  // Asumiendo 40 columnas
    
    // Indicador de proximidad
    sf::RectangleShape indicator(sf::Vector2f(180, 80));
    indicator.setPosition(boardWidth + 10, 420);
    indicator.setFillColor(sf::Color(50, 50, 50));
    indicator.setOutlineColor(color);
    indicator.setOutlineThickness(3);
    window.draw(indicator);
    
    sf::Text title("ENEMIGO", font, 16);
    title.setFillColor(sf::Color::White);
    title.setPosition(boardWidth + 50, 430);
    window.draw(title);
    
    std::string distText = "Distancia: " + std::to_string(distance);
    sf::Text distanceText(distText, font, 14);
    distanceText.setFillColor(color);
    distanceText.setPosition(boardWidth + 25, 460);
    window.draw(distanceText);
}
