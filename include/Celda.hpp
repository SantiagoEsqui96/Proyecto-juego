#pragma once
#include <SFML/Graphics.hpp>

// ============================================
// ESTRUCTURA CELDA - Elemento básico del tablero
// Representa una celda individual del juego con:
// - Estado (oculta, descubierta, marcada)
// - Si contiene una mina
// - Número de minas adyacentes
// ============================================
class Celda {
public:
    // ============================================
    // ENUMERACIÓN - Estados de celda
    // ============================================
    enum Estado {
        Oculta,         // Sin revelar
        Descubierta,    // Revelada (muestra número o está vacía)
        Marcada         // Marcada como posible mina (bandera)
    };

    // ============================================
    // CONSTRUCTOR POR DEFECTO
    // ============================================
    Celda() : tieneMina(false), estado(Oculta), minasAlrededor(0) {}

    // ============================================
    // MÉTODOS DE MINA
    // ============================================
    void setMina(bool mina) { tieneMina = mina; }        // Establecer si tiene mina
    bool esMina() const { return tieneMina; }            // ¿Tiene mina?

    // ============================================
    // MÉTODOS DE ESTADO
    // ============================================
    void setEstado(Estado nuevoEstado) { estado = nuevoEstado; }  // Cambiar estado
    Estado getEstado() const { return estado; }                    // Obtener estado actual

    // ============================================
    // MÉTODOS DE PROXIMIDAD
    // ============================================
    void setMinasAlrededor(int cantidad) { minasAlrededor = cantidad; }  // Establecer contador
    int getMinasAlrededor() const { return minasAlrededor; }             // Obtener contador

    // ============================================
    // RENDERIZADO - Crear forma visual
    // Retorna un rectángulo con color según estado
    // ============================================
    sf::RectangleShape getShape(float size, sf::Vector2f position) const {
        // Crear cuadrado del tamaño especificado
        sf::RectangleShape shape(sf::Vector2f(size, size));
        shape.setPosition(position);
        
        // Seleccionar color según estado
        switch (estado) {
            case Oculta:
                shape.setFillColor(sf::Color(34, 139, 34));  // Verde oscuro (no revelada)
                break;
            case Descubierta:
                shape.setFillColor(sf::Color::White);        // Blanco (revelada)
                break;
            case Marcada:
                shape.setFillColor(sf::Color::Yellow);       // Amarillo (marcada)
                break;
        }
        
        // Borde negro para separar celdas
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(2);
        
        return shape;
    }

private:
    bool tieneMina;
    Estado estado;
    int minasAlrededor;
};