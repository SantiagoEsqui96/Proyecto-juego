#include "../include/AudioManager.hpp"
#include <iostream>

AudioManager::AudioManager() : musicVolume(50.0f), soundVolume(70.0f) {}

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

bool AudioManager::loadMusic(const std::string& name, const std::string& filepath) {
    if (!musics[name].openFromFile(filepath)) {
        std::cerr << "Error: No se pudo cargar la música: " << filepath << std::endl;
        return false;
    }
    std::cout << "Música cargada: " << name << std::endl;
    return true;
}

bool AudioManager::loadSound(const std::string& name, const std::string& filepath) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filepath)) {
        std::cerr << "Error: No se pudo cargar el sonido: " << filepath << std::endl;
        return false;
    }
    
    soundBuffers[name] = buffer;
    sounds[name].setBuffer(soundBuffers[name]);
    sounds[name].setVolume(soundVolume);
    
    std::cout << "Sonido cargado: " << name << std::endl;
    return true;
}

void AudioManager::playMusic(const std::string& name, bool loop) {
    auto it = musics.find(name);
    if (it != musics.end()) {
        it->second.setLoop(loop);
        it->second.setVolume(musicVolume);
        it->second.play();
        std::cout << "Reproduciendo música: " << name << std::endl;
    } else {
        std::cerr << "Error: Música no encontrada: " << name << std::endl;
    }
}

void AudioManager::stopMusic(const std::string& name) {
    if (name.empty()) {
        // Detener todas las músicas
        for (auto& pair : musics) {
            pair.second.stop();
        }
    } else {
        auto it = musics.find(name);
        if (it != musics.end()) {
            it->second.stop();
        }
    }
}

void AudioManager::pauseMusic() {
    for (auto& pair : musics) {
        if (pair.second.getStatus() == sf::Music::Playing) {
            pair.second.pause();
        }
    }
}

void AudioManager::resumeMusic() {
    for (auto& pair : musics) {
        if (pair.second.getStatus() == sf::Music::Paused) {
            pair.second.play();
        }
    }
}

void AudioManager::setMusicVolume(float volume) {
    musicVolume = volume;
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
