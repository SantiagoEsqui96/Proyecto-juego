#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class GameState {
public:
    enum State {
        MENU,
        PLAYING,
        INSTRUCTIONS,
        GAME_OVER,
        VICTORY
    };

    GameState();
    
    // Getters y setters
    State getCurrentState() const;
    void setState(State newState);
    
    // Métodos de renderizado
    void drawMenu(sf::RenderWindow& window, const sf::Font& font);
    void drawInstructions(sf::RenderWindow& window, const sf::Font& font);
    void drawGameOver(sf::RenderWindow& window, const sf::Font& font, const std::string& reason);
    void drawVictory(sf::RenderWindow& window, const sf::Font& font, float timeElapsed);
    
    // Manejo de input en diferentes estados
    bool handleMenuInput(sf::Event& event, sf::RenderWindow& window);
    bool handleInstructionsInput(sf::Event& event);
    bool handleGameOverInput(sf::Event& event);
    bool handleVictoryInput(sf::Event& event);
    
    // Detección de colisión
    bool checkEnemyPlayerCollision(int enemyRow, int enemyCol, int playerRow, int playerCol);
    
    // Indicador de proximidad del enemigo
    void drawProximityIndicator(sf::RenderWindow& window, int enemyRow, int enemyCol, 
                                int playerRow, int playerCol, float cellSize, const sf::Font& font);

private:
    State currentState;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    // Métodos auxiliares
    int calculateDistance(int row1, int col1, int row2, int col2);
    sf::Color getProximityColor(int distance);
};
