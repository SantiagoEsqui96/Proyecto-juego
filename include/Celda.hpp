#pragma once
#include <SFML/Graphics.hpp>

class Celda {
public:
    enum Estado {
        Oculta,
        Descubierta,
        Marcada
    };

    Celda() : tieneMina(false), estado(Oculta), minasAlrededor(0) {}

    void setMina(bool mina) { tieneMina = mina; }
    bool esMina() const { return tieneMina; }

    void setEstado(Estado nuevoEstado) { estado = nuevoEstado; }
    Estado getEstado() const { return estado; }

    void setMinasAlrededor(int cantidad) { minasAlrededor = cantidad; }
    int getMinasAlrededor() const { return minasAlrededor; }

    sf::RectangleShape getShape(float size, sf::Vector2f position) const {
        sf::RectangleShape shape(sf::Vector2f(size, size));
        shape.setPosition(position);
        switch (estado) {
            case Oculta:
                shape.setFillColor(sf::Color(34, 139, 34));  // Verde oscuro
                break;
            case Descubierta:
                shape.setFillColor(sf::Color::White);
                break;
            case Marcada:
                shape.setFillColor(sf::Color::Yellow);
                break;
        }
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(2);
        return shape;
    }

private:
    bool tieneMina;
    Estado estado;
    int minasAlrededor;
};