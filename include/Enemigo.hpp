#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

class Enemigo {
public:
    Enemigo(int filas, int columnas, const std::string& texturePath = "assets/images/plaga1.png");
    
    void actualizar(float deltaTime, const std::vector<std::vector<int>>& estadoCeldas, bool juegoIniciado, int filaRaton, int colRaton);
    void dibujar(sf::RenderWindow& window, float cellSize, float offsetX = 0.0f, float offsetY = 0.0f) const;
    void verificarYTeletransportar(const std::vector<std::vector<int>>& estadoCeldas);
    void paralizarTemporalmente(float duracion);
    bool estaParalizado() const;
    
    int getFila() const;
    int getColumna() const;
    std::pair<int, int> obtenerPosicion() const;
    int getCasillasRecorridas() const;
    void resetearCasillasRecorridas();
    
private:
    int filas, columnas;
    int fila, columna;
    int filaDestino, columnaDestino;
    float tiempoMovimiento;
    float velocidadMovimiento;
    int casillasRecorridas;  // Contador de casillas recorridas
    int pasosEnDireccion;    // Contador para cambiar de dirección más frecuentemente
    float tiempoParalizado;  // Tiempo de parálisis restante
    
    enum class Direccion { ARRIBA, ABAJO, IZQUIERDA, DERECHA };
    Direccion direccionActual;
    
    std::pair<int, int> encontrarCasillaLibre(const std::vector<std::vector<int>>& estadoCeldas);
    Direccion calcularDireccionHacia(int filaDestino, int colDestino);
    void elegirDireccionAleatoria();
    void generarPosicionInicial(int f, int c);
    
    // Textura y sprite para la imagen del enemigo
    sf::Texture textura;
    mutable sf::Sprite sprite;  // mutable para poder modificarlo en dibujar()
    bool texturasCargadas;
};
