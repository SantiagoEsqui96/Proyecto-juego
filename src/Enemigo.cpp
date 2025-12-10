#include "../include/Enemigo.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <climits>
#include <iostream>

Enemigo::Enemigo(int filas, int columnas, const std::string& texturePath)
    : filas(filas), columnas(columnas), 
      fila(0), columna(0),
      filaDestino(0), columnaDestino(0),
      tiempoMovimiento(0.0f), velocidadMovimiento(0.5f),
      casillasRecorridas(0),
      pasosEnDireccion(0),
      tiempoParalizado(0.0f),
      direccionActual(Direccion::DERECHA),
      texturasCargadas(false) {
    // Generar posición inicial en uno de los lados del tablero
    generarPosicionInicial(filas, columnas);
    
    // Cargar textura de la plaga
    if (textura.loadFromFile(texturePath)) {
        sprite.setTexture(textura);
        texturasCargadas = true;
        std::cout << "Textura de plaga cargada correctamente: " << texturePath << std::endl;
    } else {
        std::cerr << "Error: No se pudo cargar " << texturePath << std::endl;
    }
}

void Enemigo::generarPosicionInicial(int f, int c) {
    // Elegir aleatoriamente un lado del tablero
    int lado = rand() % 4;
    
    switch (lado) {
        case 0:  // Arriba (fila 0)
            fila = 0;
            columna = rand() % c;
            direccionActual = Direccion::ABAJO;
            break;
        case 1:  // Abajo (última fila)
            fila = f - 1;
            columna = rand() % c;
            direccionActual = Direccion::ARRIBA;
            break;
        case 2:  // Izquierda (columna 0)
            fila = rand() % f;
            columna = 0;
            direccionActual = Direccion::DERECHA;
            break;
        case 3:  // Derecha (última columna)
            fila = rand() % f;
            columna = c - 1;
            direccionActual = Direccion::IZQUIERDA;
            break;
    }
    
    filaDestino = fila;
    columnaDestino = columna;
}

void Enemigo::actualizar(float deltaTime, const std::vector<std::vector<int>>& estadoCeldas, bool juegoIniciado, int filaRaton, int colRaton) {
    // Actualizar parálisis
    if (tiempoParalizado > 0.0f) {
        tiempoParalizado -= deltaTime;
        return;  // No moverse si está paralizado
    }
    
    // No moverse si el juego no ha comenzado
    if (!juegoIniciado)
        return;
    
    tiempoMovimiento += deltaTime;
    
    // Moverse cada 1.5 segundos (velocidad media)
    if (tiempoMovimiento >= 1.5f) {
        tiempoMovimiento = 0.0f;
        
        // Probar todas las 4 direcciones y elegir la que más acerca al jugador
        int mejorDistancia = INT_MAX;
        Direccion mejorDireccion = Direccion::DERECHA;
        std::vector<Direccion> direccionesValidas;
        
        // Probar todas las direcciones
        std::vector<Direccion> todasDirecciones = {
            Direccion::ARRIBA, Direccion::ABAJO, 
            Direccion::IZQUIERDA, Direccion::DERECHA
        };
        
        for (Direccion dir : todasDirecciones) {
            int proximaFila = fila;
            int proximaColumna = columna;
            
            switch (dir) {
                case Direccion::ARRIBA:
                    proximaFila = fila - 1;
                    break;
                case Direccion::ABAJO:
                    proximaFila = fila + 1;
                    break;
                case Direccion::IZQUIERDA:
                    proximaColumna = columna - 1;
                    break;
                case Direccion::DERECHA:
                    proximaColumna = columna + 1;
                    break;
            }
            
            // Teletransportación en bordes
            if (proximaFila < 0) {
                proximaFila = filas - 1;
            } else if (proximaFila >= filas) {
                proximaFila = 0;
            }
            
            if (proximaColumna < 0) {
                proximaColumna = columnas - 1;
            } else if (proximaColumna >= columnas) {
                proximaColumna = 0;
            }
            
            // Solo evitar celdas descubierta (1), no marcadas ni ocultas
            int estadoCelda = estadoCeldas[proximaFila][proximaColumna];
            
            if (estadoCelda != 1) {  // Si no está descubierta, es válida
                direccionesValidas.push_back(dir);
                
                // Calcular distancia a el jugador desde esta nueva posición
                int difFila = filaRaton - proximaFila;
                int difCol = colRaton - proximaColumna;
                int distancia = abs(difFila) + abs(difCol);  // Distancia Manhattan
                
                if (distancia < mejorDistancia) {
                    mejorDistancia = distancia;
                    mejorDireccion = dir;
                }
            }
        }
        
        // Moverse en la mejor dirección encontrada
        if (!direccionesValidas.empty()) {
            int proximaFila = fila;
            int proximaColumna = columna;
            
            switch (mejorDireccion) {
                case Direccion::ARRIBA:
                    proximaFila = fila - 1;
                    break;
                case Direccion::ABAJO:
                    proximaFila = fila + 1;
                    break;
                case Direccion::IZQUIERDA:
                    proximaColumna = columna - 1;
                    break;
                case Direccion::DERECHA:
                    proximaColumna = columna + 1;
                    break;
            }
            
            // Teletransportación en bordes
            if (proximaFila < 0) {
                proximaFila = filas - 1;
            } else if (proximaFila >= filas) {
                proximaFila = 0;
            }
            
            if (proximaColumna < 0) {
                proximaColumna = columnas - 1;
            } else if (proximaColumna >= columnas) {
                proximaColumna = 0;
            }
            
            fila = proximaFila;
            columna = proximaColumna;
            direccionActual = mejorDireccion;
            casillasRecorridas++;
        }
    }
}

std::pair<int, int> Enemigo::encontrarCasillaLibre(const std::vector<std::vector<int>>& estadoCeldas) {
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            if (estadoCeldas[i][j] != 1) {  // No descubierta
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

Enemigo::Direccion Enemigo::calcularDireccionHacia(int filaDestino, int colDestino) {
    int difFila = filaDestino - fila;
    int difCol = colDestino - columna;
    
    // Priorizar movimiento horizontal vs vertical de forma aleatoria
    if (rand() % 2 == 0) {
        if (abs(difFila) > abs(difCol)) {
            return (difFila > 0) ? Direccion::ABAJO : Direccion::ARRIBA;
        } else {
            return (difCol > 0) ? Direccion::DERECHA : Direccion::IZQUIERDA;
        }
    } else {
        if (abs(difCol) > abs(difFila)) {
            return (difCol > 0) ? Direccion::DERECHA : Direccion::IZQUIERDA;
        } else {
            return (difFila > 0) ? Direccion::ABAJO : Direccion::ARRIBA;
        }
    }
}

void Enemigo::verificarYTeletransportar(const std::vector<std::vector<int>>& estadoCeldas) {
    // Si la celda actual está descubierta, teletransportarse
    if (estadoCeldas[fila][columna] == 1) {
        auto nuevaPosicion = encontrarCasillaLibre(estadoCeldas);
        if (nuevaPosicion != std::make_pair(-1, -1)) {
            fila = nuevaPosicion.first;
            columna = nuevaPosicion.second;
        }
    }
}

void Enemigo::dibujar(sf::RenderWindow& window, float cellSize) const {
    if (texturasCargadas) {
        // Dibujar sprite de la plaga
        // Calcular escala para que la imagen se ajuste a la celda
        sf::Vector2u textureSize = textura.getSize();
        float scaleX = cellSize / textureSize.x;
        float scaleY = cellSize / textureSize.y;
        
        // Usar la escala más pequeña para mantener proporciones
        float scale = std::min(scaleX, scaleY) * 1.6f; // 1.6 para hacer la plaga más grande
        
        sprite.setScale(scale, scale);
        sprite.setPosition(columna * cellSize, fila * cellSize);
        
        // Centrar el sprite en la celda
        sf::FloatRect bounds = sprite.getGlobalBounds();
        float offsetX = (cellSize - bounds.width) / 2;
        float offsetY = (cellSize - bounds.height) / 2;
        sprite.setPosition(columna * cellSize + offsetX, fila * cellSize + offsetY);
        
        window.draw(sprite);
    } else {
        // Fallback: Dibujar enemigo como un círculo rojo si no se cargó la textura
        sf::CircleShape enemigo(cellSize / 3);
        enemigo.setFillColor(sf::Color::Red);
        enemigo.setPosition(columna * cellSize + cellSize / 3, fila * cellSize + cellSize / 3);
        window.draw(enemigo);
        
        // Agregar ojos para más detalle
        sf::CircleShape ojo(cellSize / 8);
        ojo.setFillColor(sf::Color::White);
        ojo.setPosition(columna * cellSize + cellSize / 4, fila * cellSize + cellSize / 4);
        window.draw(ojo);
        
        ojo.setPosition(columna * cellSize + cellSize * 0.55f, fila * cellSize + cellSize / 4);
        window.draw(ojo);
    }
}

int Enemigo::getFila() const {
    return fila;
}

int Enemigo::getColumna() const {
    return columna;
}

std::pair<int, int> Enemigo::obtenerPosicion() const {
    return {fila, columna};
}

int Enemigo::getCasillasRecorridas() const {
    return casillasRecorridas;
}

void Enemigo::resetearCasillasRecorridas() {
    casillasRecorridas = 0;
}

void Enemigo::elegirDireccionAleatoria() {
    int direccion = rand() % 4;
    direccionActual = static_cast<Direccion>(direccion);
}

void Enemigo::paralizarTemporalmente(float duracion) {
    tiempoParalizado = duracion;
}

bool Enemigo::estaParalizado() const {
    return tiempoParalizado > 0.0f;
}
