#include "../include/Tablero.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <cmath>

Tablero::Tablero(int filas, int columnas, int minas)
    : filas(filas), columnas(columnas), minas(minas), finDelJuego(false), victoria(false), primerClick(true), minasGeneradas(false), tieneAntidotoDisponible(true), usosDetectorMinas(5) {
    celdas.resize(filas, std::vector<Celda>(columnas));
    // Cargar textura del insecto
    texturaInsecto.loadFromFile("assets/images/insecto.png");
    texturaBandera.loadFromFile("assets/images/Bandera1.png");
}

void Tablero::descubrir(int fila, int columna) {
    if (finDelJuego || fila < 0 || columna < 0 || fila >= filas || columna >= columnas)
        return;
    
    // Si es el primer clic, generar minas evitando la celda clickeada
    if (primerClick) {
        primerClick = false;
        
        // Generar minas ahora, evitando la celda donde se hizo clic
        if (!minasGeneradas) {
            minasGeneradas = true;
            int minasColocadas = 0;
            while (minasColocadas < minas) {
                int i = rand() % filas;
                int j = rand() % columnas;
                // No poner mina donde el usuario clickeó, ni en celdas cercanas (radio de 2)
                if ((i == fila && j == columna) ||
                    (abs(i - fila) <= 2 && abs(j - columna) <= 2)) {
                    continue;
                }
                if (!celdas[i][j].esMina()) {
                    celdas[i][j].setMina(true);
                    minasColocadas++;
                }
            }
            calcularMinasAlrededor();
        }
        
        // Descubrir donde el usuario clickeó
        celdas[fila][columna].setEstado(Celda::Descubierta);
        
        // Expandir recursivamente si tiene 0 minas alrededor
        if (celdas[fila][columna].getMinasAlrededor() == 0) {
            descubrirRecursivo(fila, columna);
        }
        
        return;
    }
    
    Celda& celda = celdas[fila][columna];
    if (celda.getEstado() == Celda::Descubierta || celda.getEstado() == Celda::Marcada)
        return;
    
    celda.setEstado(Celda::Descubierta);
    
    // Verificar si hay una bomba temporal en esta celda
    activarBombaEnCelda(fila, columna);
    
    if (celda.esMina()) {
        // Si tiene antídoto disponible, usarlo en lugar de perder
        if (tieneAntidotoDisponible) {
            usarAntidoto();
            std::cout << "¡ANTÍDOTO ACTIVADO! Sobreviviste a la mina." << std::endl;
        } else {
            finDelJuego = true;
            victoria = false;
        }
    } else if (celda.getMinasAlrededor() == 0) {
        descubrirRecursivo(fila, columna);
    }
    
    // Verificar victoria
    int descubiertas = 0;
    for (int i = 0; i < filas; ++i)
        for (int j = 0; j < columnas; ++j)
            if (!celdas[i][j].esMina() && celdas[i][j].getEstado() == Celda::Descubierta)
                ++descubiertas;
    if (descubiertas == filas * columnas - minas) {
        finDelJuego = true;
        victoria = true;
    }
}

void Tablero::marcar(int fila, int columna) {
    if (finDelJuego || fila < 0 || columna < 0 || fila >= filas || columna >= columnas)
        return;
    Celda& celda = celdas[fila][columna];
    if (celda.getEstado() == Celda::Descubierta)
        return;
    if (celda.getEstado() == Celda::Marcada)
        celda.setEstado(Celda::Oculta);
    else
        celda.setEstado(Celda::Marcada);
}

void Tablero::marcarFilaDeMinas(int fila) {
    if (fila < 0 || fila >= filas)
        return;
    for (int j = 0; j < columnas; ++j) {
        if (celdas[fila][j].esMina() && celdas[fila][j].getEstado() == Celda::Oculta) {
            celdas[fila][j].setEstado(Celda::Marcada);
        }
    }
}

void Tablero::marcarColumnaDeMinas(int columna) {
    if (columna < 0 || columna >= columnas)
        return;
    for (int i = 0; i < filas; ++i) {
        if (celdas[i][columna].esMina() && celdas[i][columna].getEstado() == Celda::Oculta) {
            celdas[i][columna].setEstado(Celda::Marcada);
        }
    }
}

void Tablero::usarAntidoto() {
    tieneAntidotoDisponible = false;
}

bool Tablero::tieneAntidoto() const {
    return tieneAntidotoDisponible;
}

bool Tablero::esFinDelJuego() const { return finDelJuego; }
bool Tablero::esVictoria() const { return victoria; }

void Tablero::dibujar(sf::RenderWindow& window, float size) const {
    sf::Font font;
      font.loadFromFile("assets/fonts/Minecraft.ttf"); // Cambiada a fuente existente
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            sf::Vector2f pos(j * size, i * size);
            
            if (celdas[i][j].getEstado() == Celda::Marcada) {
                sf::Sprite spriteBandera(texturaBandera);
                spriteBandera.setPosition(pos);
                spriteBandera.setScale(size / spriteBandera.getLocalBounds().width, size / spriteBandera.getLocalBounds().height);
                window.draw(spriteBandera);
            } else {
                window.draw(celdas[i][j].getShape(size, pos));
            }

            if (celdas[i][j].getEstado() == Celda::Descubierta) {
                if (celdas[i][j].esMina()) {
                    sf::Sprite spriteInsecto(texturaInsecto);
                    spriteInsecto.setPosition(pos.x, pos.y);
                    // Escalar el sprite para que quepa en la celda
                    spriteInsecto.setScale(size / spriteInsecto.getLocalBounds().width, 
                                          size / spriteInsecto.getLocalBounds().height);
                    window.draw(spriteInsecto);
                } else if (celdas[i][j].getMinasAlrededor() > 0) {
                    sf::Text text;
                    text.setFont(font);
                    text.setString(std::to_string(celdas[i][j].getMinasAlrededor()));
                    text.setCharacterSize(static_cast<unsigned int>(size / 2));
                    text.setFillColor(sf::Color::Blue);
                    text.setPosition(pos.x + size / 4, pos.y + size / 8);
                    window.draw(text);
                }
            }
        }
    }
}

void Tablero::colocarMinas() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disFila(0, filas - 1);
    std::uniform_int_distribution<> disCol(0, columnas - 1);
    int colocadas = 0;
    while (colocadas < minas) {
        int f = disFila(gen);
        int c = disCol(gen);
        if (!celdas[f][c].esMina()) {
            celdas[f][c].setMina(true);
            ++colocadas;
        }
    }
}

void Tablero::calcularMinasAlrededor() {
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            if (celdas[i][j].esMina()) {
                celdas[i][j].setMinasAlrededor(-1);
                continue;
            }
            int count = 0;
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    int ni = i + di, nj = j + dj;
                    if (ni >= 0 && ni < filas && nj >= 0 && nj < columnas && celdas[ni][nj].esMina())
                        ++count;
                }
            }
            celdas[i][j].setMinasAlrededor(count);
        }
    }
}

void Tablero::descubrirRecursivo(int fila, int columna) {
    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            int ni = fila + di, nj = columna + dj;
            if (ni >= 0 && ni < filas && nj >= 0 && nj < columnas) {
                Celda& celda = celdas[ni][nj];
                if (celda.getEstado() == Celda::Oculta && !celda.esMina()) {
                    celda.setEstado(Celda::Descubierta);
                    if (celda.getMinasAlrededor() == 0)
                        descubrirRecursivo(ni, nj);
                }
            }
        }
    }
}

void Tablero::marcarMinaAlAzar() {
    if (usosDetectorMinas <= 0 || !minasGeneradas)
        return;
    
    // Encontrar todas las minas ocultas
    std::vector<std::pair<int, int>> minasOcultas;
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            if (celdas[i][j].esMina() && celdas[i][j].getEstado() == Celda::Oculta) {
                minasOcultas.push_back({i, j});
            }
        }
    }
    
    // Si hay minas ocultas, marcar una al azar
    if (!minasOcultas.empty()) {
        int idx = rand() % minasOcultas.size();
        celdas[minasOcultas[idx].first][minasOcultas[idx].second].setEstado(Celda::Marcada);
        usosDetectorMinas--;
        std::cout << "¡MINA MARCADA! Usos restantes: " << usosDetectorMinas << std::endl;
    }
}

int Tablero::getUsosDetectorMinas() const {
    return usosDetectorMinas;
}

void Tablero::crearBombaTemporalEnCelda(int fila, int columna) {
    // Verificar que la posición sea válida y no sea mina
    if (fila < 0 || fila >= filas || columna < 0 || columna >= columnas)
        return;
    
    if (celdas[fila][columna].esMina())
        return;
    
    // Verificar si ya existe una bomba en esa posición
    for (size_t i = 0; i < bombas.size(); ++i) {
        if (bombas[i].first == fila && bombas[i].second == columna) {
            // Resetear el tiempo si ya existe
            tiemposBomba[i] = 20.0f;
            return;
        }
    }
    
    // Crear nueva bomba
    bombas.push_back({fila, columna});
    tiemposBomba.push_back(20.0f);
}

void Tablero::actualizarBombas(float deltaTime) {
    // Actualizar tiempo de todas las bombas
    for (size_t i = 0; i < tiemposBomba.size(); ) {
        tiemposBomba[i] -= deltaTime;
        if (tiemposBomba[i] <= 0.0f) {
            // Remover bomba expirada
            bombas.erase(bombas.begin() + i);
            tiemposBomba.erase(tiemposBomba.begin() + i);
        } else {
            ++i;
        }
    }
}

bool Tablero::esBombaEnCelda(int fila, int columna) const {
    for (const auto& bomba : bombas) {
        if (bomba.first == fila && bomba.second == columna) {
            return true;
        }
    }
    return false;
}

std::vector<std::pair<int, int>> Tablero::obtenerPosicionesBombas() const {
    return bombas;
}

int Tablero::getMinas() const {
    return minas;
}

int Tablero::getMinasMarcadas() const {
    int cont = 0;
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            if (celdas[i][j].getEstado() == Celda::Marcada) {
                cont++;
            }
        }
    }
    return cont;
}

const Celda& Tablero::getCelda(int fila, int columna) const {
    return celdas[fila][columna];
}

bool Tablero::activarBombaEnCelda(int fila, int columna) {
    // Buscar si hay una bomba en esta celda
    for (size_t i = 0; i < bombas.size(); ++i) {
        if (bombas[i].first == fila && bombas[i].second == columna) {
            // Hay una bomba, causarle daño al jugador
            // Si tiene antídoto, usarlo
            if (tieneAntidotoDisponible) {
                usarAntidoto();
                std::cout << "¡BOMBA TEMPORAL DESACTIVADA POR EL ANTÍDOTO!" << std::endl;
            } else {
                finDelJuego = true;
                victoria = false;
                std::cout << "¡BOMBA TEMPORAL! GAME OVER" << std::endl;
            }
            
            // Remover la bomba después de ser activada
            bombas.erase(bombas.begin() + i);
            tiemposBomba.erase(tiemposBomba.begin() + i);
            return true;
        }
    }
    return false;
}

