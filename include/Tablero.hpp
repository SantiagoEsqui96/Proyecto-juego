#pragma once
#include "Celda.hpp" // Celda
#include <vector>
#include <SFML/Graphics.hpp>
#include <random>
#include <utility>

class Tablero {
public:
    Tablero(int filas, int columnas, int minas);
    void descubrir(int fila, int columna);
    void marcar(int fila, int columna);
    void marcarFilaDeMinas(int fila);
    void marcarColumnaDeMinas(int columna);
    void marcarMinaAlAzar();
    int getUsosDetectorMinas() const;
    void usarAntidoto();
    bool tieneAntidoto() const;
    bool esFinDelJuego() const;
    bool esVictoria() const;
    void dibujar(sf::RenderWindow& window, float size = 32.0f, float offsetX = 0.0f, float offsetY = 0.0f) const;
    int getFilas() const;
    int getColumnas() const;
    int getMinas() const;
    int getMinasMarcadas() const;
    const Celda& getCelda(int fila, int columna) const;
    
    // Métodos para bombas temporales
    void crearBombaTemporalEnCelda(int fila, int columna);
    void actualizarBombas(float deltaTime);
    bool esBombaEnCelda(int fila, int columna) const;
    std::vector<std::pair<int, int>> obtenerPosicionesBombas() const;
    bool activarBombaEnCelda(int fila, int columna);  // Retorna true si hay bomba y causa daño

private:
    int filas, columnas, minas;
    std::vector<std::vector<Celda>> celdas;
    bool finDelJuego;
    bool victoria;
    bool primerClick;
    bool minasGeneradas;
    bool tieneAntidotoDisponible;
    int usosDetectorMinas;
    sf::Texture texturaInsecto;
    sf::Texture texturaBandera;
    
    // Sistema de bombas temporales
    std::vector<std::pair<int, int>> bombas;  // Posiciones de bombas
    std::vector<float> tiemposBomba;  // Tiempo restante de cada bomba (máx 20 segundos)
    
    void colocarMinas();
    void calcularMinasAlrededor();
    void descubrirRecursivo(int fila, int columna);
};