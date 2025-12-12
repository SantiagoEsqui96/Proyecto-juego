#pragma once
#include "Celda.hpp" // Estructura de datos para cada celda
#include <vector>
#include <SFML/Graphics.hpp>
#include <random>
#include <utility>

// ============================================
// CLASE TABLERO - Gestión del juego
// Controla la lógica principal del Minesweeper:
// - Generación de minas
// - Revelación de celdas
// - Expansión automática (flood-fill)
// - Sistema de bombas temporales
// - Detección de victoria/derrota
// ============================================
class Tablero {
public:
    // ============================================
    // MÉTODOS PÚBLICOS - Interacción con el tablero
    // ============================================
    Tablero(int filas, int columnas, int minas);  // Constructor
    
    // Revelar celda (puede expandir si tiene 0 minas cercanas)
    void descubrir(int fila, int columna);
    
    // Marcar/desmarcar celda con bandera
    void marcar(int fila, int columna);
    
    // PODERES - Marcar automáticamente
    void marcarFilaDeMinas(int fila);           // Marca todas las minas de una fila
    void marcarColumnaDeMinas(int columna);     // Marca todas las minas de una columna
    void marcarMinaAlAzar();                    // Marca una mina aleatoria
    
    // PODERES - Sistema de antídoto
    int getUsosDetectorMinas() const;           // Obtener usos restantes del detector
    void usarAntidoto();                        // Usar antídoto (sobrevive a una mina)
    bool tieneAntidoto() const;                 // ¿Tiene antídoto disponible?
    
    // ESTADO DEL JUEGO
    bool esFinDelJuego() const;                 // ¿Está en fin del juego?
    bool esVictoria() const;                    // ¿Ganó?
    
    // RENDERIZADO
    void dibujar(sf::RenderWindow& window, float size = 32.0f, float offsetX = 0.0f, float offsetY = 0.0f) const;
    
    // INFORMACIÓN DEL TABLERO
    int getFilas() const;                       // Obtener número de filas
    int getColumnas() const;                    // Obtener número de columnas
    int getMinas() const;                       // Obtener número total de minas
    int getMinasMarcadas() const;               // Obtener número de minas marcadas
    const Celda& getCelda(int fila, int columna) const;  // Obtener referencia a celda
    
    // ============================================
    // SISTEMA DE BOMBAS TEMPORALES - Creadas por enemigos
    // ============================================
    void crearBombaTemporalEnCelda(int fila, int columna);      // Crear bomba en celda
    void actualizarBombas(float deltaTime);                      // Actualizar tiempo de bombas
    bool esBombaEnCelda(int fila, int columna) const;           // ¿Hay bomba en esta celda?
    std::vector<std::pair<int, int>> obtenerPosicionesBombas() const;  // Obtener posiciones de bombas
    bool activarBombaEnCelda(int fila, int columna);            // Activar bomba (retorna si hay)

private:
    // ============================================
    // ATRIBUTOS PRIVADOS - Estado interno del tablero
    // ============================================
    
    // Dimensiones y configuración
    int filas, columnas, minas;
    
    // Matriz de celdas (el tablero en sí)
    std::vector<std::vector<Celda>> celdas;
    
    // Estado del juego
    bool finDelJuego;                   // ¿Está terminado?
    bool victoria;                      // ¿Se ganó?
    bool primerClick;                   // ¿Es el primer clic? (para generar minas)
    bool minasGeneradas;                // ¿Ya se generaron las minas?
    
    // Sistema de poderes
    bool tieneAntidotoDisponible;       // ¿Tiene antídoto disponible?
    int usosDetectorMinas;              // Usos restantes del detector (máx 5)
    
    // Texturas para renderizado
    sf::Texture texturaInsecto;         // Textura de mina
    sf::Texture texturaBandera;         // Textura de bandera (marcar)
    
    // ============================================
    // SISTEMA DE BOMBAS TEMPORALES
    // ============================================
    std::vector<std::pair<int, int>> bombas;    // Posiciones (fila, columna) de bombas
    std::vector<float> tiemposBomba;            // Tiempo restante de cada bomba (máx 20s)
    
    void colocarMinas();
    void calcularMinasAlrededor();
    void descubrirRecursivo(int fila, int columna);
};