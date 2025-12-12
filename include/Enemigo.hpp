#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

// ============================================
// CLASE ENEMIGO - Personaje perseguidor
// Características:
// - IA de persecución basada en distancia Manhattan
// - Crea bombas temporales cada 5 celdas
// - Sistema de parálisis (30 segundos)
// - Detecta obstáculos (celdas reveladas, bombas)
// ============================================
class Enemigo {
public:
    // ============================================
    // CONSTRUCTOR Y MÉTODOS PRINCIPALES
    // ============================================
    Enemigo(int filas, int columnas, const std::string& texturePath = "assets/images/plaga1.png");
    
    // Actualizar posición y estado cada frame
    void actualizar(float deltaTime, const std::vector<std::vector<int>>& estadoCeldas, bool juegoIniciado, int filaRaton, int colRaton);
    
    // Renderizar el enemigo en pantalla
    void dibujar(sf::RenderWindow& window, float cellSize, float offsetX = 0.0f, float offsetY = 0.0f) const;
    
    // Teletransportar si se sale del tablero
    void verificarYTeletransportar(const std::vector<std::vector<int>>& estadoCeldas);
    
    // ============================================
    // SISTEMA DE PARÁLISIS - Poder del jugador
    // ============================================
    void paralizarTemporalmente(float duracion);  // Paralizar por duracion segundos
    bool estaParalizado() const;                  // ¿Está paralizado?
    
    // ============================================
    // MÉTODOS DE INFORMACIÓN
    // ============================================
    int getFila() const;                          // Obtener fila actual
    int getColumna() const;                       // Obtener columna actual
    std::pair<int, int> obtenerPosicion() const;  // Obtener (fila, columna)
    int getCasillasRecorridas() const;            // Obtener casillas recorridas
    void resetearCasillasRecorridas();            // Resetear contador de casillas

private:
    // ============================================
    // ATRIBUTOS PRIVADOS - Estado del enemigo
    // ============================================
    
    // Dimensiones del tablero
    int filas, columnas;
    
    // Posición actual y destino
    int fila, columna;              // Posición actual (en celdas)
    int filaDestino, columnaDestino; // Hacia dónde se dirige
    
    // Movimiento y timing
    float tiempoMovimiento;         // Tiempo acumulado para movimiento suave
    float velocidadMovimiento;      // Velocidad de movimiento (0-1)
    int casillasRecorridas;         // Contador para generar bombas cada 5
    int pasosEnDireccion;           // Contador para cambiar dirección
    
    // Efectos especiales
    float tiempoParalizado;         // Tiempo de parálisis restante (segundos)
    
    // Dirección actual del movimiento
    enum class Direccion { ARRIBA, ABAJO, IZQUIERDA, DERECHA };
    Direccion direccionActual;
    
    // ============================================
    // MÉTODOS PRIVADOS - Lógica interna
    // ============================================
    
    // IA: Encontrar casilla libre cercana
    std::pair<int, int> encontrarCasillaLibre(const std::vector<std::vector<int>>& estadoCeldas);
    
    // IA: Calcular dirección hacia destino (Manhattan distance)
    Direccion calcularDireccionHacia(int filaDestino, int colDestino);
    
    // IA: Elegir dirección aleatoria
    void elegirDireccionAleatoria();
    
    // Generar posición inicial en uno de los bordes
    void generarPosicionInicial(int f, int c);
    
    // ============================================
    // RENDERIZADO
    // ============================================
    sf::Texture textura;            // Textura de la imagen del enemigo
    mutable sf::Sprite sprite;      // Sprite para renderizar (mutable para dibujar)
    bool texturasCargadas;          // ¿Se cargó la textura correctamente?
};
