#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// ============================================
// CLASE GAMESTATE - Gestor de estados de UI
// Controla:
// - Pantalla actual (menú, juego, pausa, etc)
// - Renderizado de cada pantalla
// - Manejo de input en cada pantalla
// - Transiciones entre estados
// ============================================
class GameState {
public:
    // ============================================
    // ENUMERACIONES - Estados del juego
    // ============================================
    enum State {
        MENU,           // Pantalla principal
        PLAYING,        // Jugando
        PAUSED,         // Pausado
        INSTRUCTIONS,   // Instrucciones
        GAME_OVER,      // Derrota
        VICTORY         // Victoria
    };
    
    // ============================================
    // ENUMERACIONES - Niveles de dificultad
    // ============================================
    enum Difficulty {
        EASY,           // 20x20, 40 minas
        MEDIUM,         // 30x30, 135 minas
        HARD            // 40x40, 350 minas
    };

    // Constructor
    GameState();
    
    // ============================================
    // MÉTODOS DE ESTADO
    // ============================================
    State getCurrentState() const;              // Obtener estado actual
    void setState(State newState);              // Cambiar estado
    bool isMusicEnabled() const;                // ¿Música activada?

    // ============================================
    // MÉTODOS DE RENDERIZADO - Dibujar cada pantalla
    // ============================================
    void drawMenu(sf::RenderWindow& window, const sf::Font& font, Difficulty currentDifficulty, bool showDifficultyMenu, bool showSoundMenu);
    void drawSoundMenu(sf::RenderWindow& window, const sf::Font& font);
    void drawPause(sf::RenderWindow& window, const sf::Font& font);
    void drawInstructions(sf::RenderWindow& window, const sf::Font& font, const sf::Sprite& background);
    void drawGameOver(sf::RenderWindow& window, const sf::Font& font, const std::string& reason);
    void drawVictory(sf::RenderWindow& window, const sf::Font& font, float timeElapsed);
    
    // ============================================
    // MÉTODOS DE INPUT - Procesar eventos en cada pantalla
    // ============================================
    bool handleMenuInput(sf::Event& event, sf::RenderWindow& window, bool& showDifficultyMenu, bool& showSoundMenu);
    int handleDifficultyMenuInput(sf::Event& event, sf::RenderWindow& window);
    int handleSoundMenuInput(sf::Event& event, sf::RenderWindow& window);
    int handlePauseInput(sf::Event& event, sf::RenderWindow& window);
    bool handleInstructionsInput(sf::Event& event);
    bool handleGameOverInput(sf::Event& event);
    bool handleVictoryInput(sf::Event& event);
    
    // ============================================
    // DETECCIÓN Y CÁLCULOS
    // ============================================
    bool checkEnemyPlayerCollision(int enemyRow, int enemyCol, int playerRow, int playerCol);
    void drawProximityIndicator(sf::RenderWindow& window, int enemyRow, int enemyCol, 
                                int playerRow, int playerCol, float cellSize, const sf::Font& font,
                                float offsetX = 0.0f, float offsetY = 0.0f, int columnas = 40);

private:
    // ============================================
    // ATRIBUTOS PRIVADOS
    // ============================================
    State currentState;                         // Estado actual del juego
    sf::Texture backgroundTexture;              // Textura de fondo
    sf::Sprite backgroundSprite;                // Sprite del fondo
    float musicVolume;                          // Volumen de música (0-100)
    float soundVolume;                          // Volumen de efectos (0-100)
    bool musicEnabled;                          // ¿Música activada?
    
    // ============================================
    // MÉTODOS AUXILIARES PRIVADOS
    // ============================================
    int calculateDistance(int row1, int col1, int row2, int col2);  // Distancia Manhattan
    sf::Color getProximityColor(int distance);  // Color según distancia
};
