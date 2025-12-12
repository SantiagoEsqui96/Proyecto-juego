#include "../include/AudioManager.hpp"
#include <iostream>

// ============================================
// CONSTRUCTOR - Inicializa volúmenes
// ============================================
AudioManager::AudioManager() : musicVolume(50.0f), soundVolume(70.0f) {}

// ============================================
// PATRÓN SINGLETON - Obtener instancia única
// Garantiza que existe solo una instancia de AudioManager en toda la aplicación
// ============================================
AudioManager& AudioManager::getInstance() {
    static AudioManager instance;  // Se crea solo una vez
    return instance;
}

// ============================================
// MÉTODO: Cargar música de fondo
// Lee un archivo .ogg o .mp3 y lo almacena en caché
// ============================================
bool AudioManager::loadMusic(const std::string& name, const std::string& filepath) {
    // Intentar cargar archivo de música
    if (!musics[name].openFromFile(filepath)) {
        std::cerr << "Error: No se pudo cargar la música: " << filepath << std::endl;
        return false;
    }
    std::cout << "Música cargada: " << name << std::endl;
    return true;
}

// ============================================
// MÉTODO: Cargar efecto de sonido
// Lee un archivo .wav y lo almacena con su buffer
// ============================================
bool AudioManager::loadSound(const std::string& name, const std::string& filepath) {
    // Crear buffer de sonido (necesario para reproducción)
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filepath)) {
        std::cerr << "Error: No se pudo cargar el sonido: " << filepath << std::endl;
        return false;
    }
    
    // Almacenar buffer y crear sonido asociado
    soundBuffers[name] = buffer;
    sounds[name].setBuffer(soundBuffers[name]);
    sounds[name].setVolume(soundVolume);
    
    std::cout << "Sonido cargado: " << name << std::endl;
    return true;
}

// ============================================
// MÉTODO: Reproducir música
// Inicia la reproducción de una música cargada
// Puede establecer loop (repetición) automática
// ============================================
void AudioManager::playMusic(const std::string& name, bool loop) {
    auto it = musics.find(name);
    if (it != musics.end()) {
        // Configurar loop y volumen
        it->second.setLoop(loop);
        it->second.setVolume(musicVolume);
        // Iniciar reproducción
        it->second.play();
        std::cout << "Reproduciendo música: " << name << std::endl;
    } else {
        std::cerr << "Error: Música no encontrada: " << name << std::endl;
    }
}

// ============================================
// MÉTODO: Detener música
// Si no se especifica nombre, detiene TODAS las músicas
// ============================================
void AudioManager::stopMusic(const std::string& name) {
    if (name.empty()) {
        // Detener todas las músicas en reproducción
        for (auto& pair : musics) {
            pair.second.stop();
        }
    } else {
        // Detener música específica
        auto it = musics.find(name);
        if (it != musics.end()) {
            it->second.stop();
        }
    }
}

// ============================================
// MÉTODO: Pausar música
// Pausa todas las músicas que están en reproducción
// ============================================
void AudioManager::pauseMusic() {
    for (auto& pair : musics) {
        if (pair.second.getStatus() == sf::Music::Playing) {
            pair.second.pause();
        }
    }
}

// ============================================
// MÉTODO: Reanudar música
// Continúa la reproducción de músicas pausadas
// ============================================
void AudioManager::resumeMusic() {
    for (auto& pair : musics) {
        if (pair.second.getStatus() == sf::Music::Paused) {
            pair.second.play();
        }
    }
}

// ============================================
// MÉTODO: Establecer volumen de música
// Cambia el volumen de todas las músicas (0-100)
// ============================================
void AudioManager::setMusicVolume(float volume) {
    musicVolume = volume;
    // Aplicar nuevo volumen a todas las músicas cargadas
    for (auto& pair : musics) {
        pair.second.setVolume(musicVolume);
    }
}

void AudioManager::playSound(const std::string& name) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        it->second.play();
    } else {
        std::cerr << "Error: Sonido no encontrado: " << name << std::endl;
    }
}

void AudioManager::setSoundVolume(float volume) {
    soundVolume = volume;
    for (auto& pair : sounds) {
        pair.second.setVolume(soundVolume);
    }
}

bool AudioManager::isMusicPlaying() const {
    for (const auto& pair : musics) {
        if (pair.second.getStatus() == sf::Music::Playing) {
            return true;
        }
    }
    return false;
}
