#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>

// ============================================
// CLASE AUDIOMANAGER - Gestor de audio (Singleton)
// Características:
// - Instancia única en toda la aplicación
// - Gestiona música de fondo
// - Gestiona efectos de sonido
// - Control de volumen
// ============================================
class AudioManager {
public:
    // ============================================
    // PATRÓN SINGLETON - Acceso a instancia única
    // ============================================
    static AudioManager& getInstance();
    
    // Prevenir copias (propio del patrón Singleton)
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    
    // ============================================
    // CARGA DE RECURSOS - Cargar archivos
    // ============================================
    bool loadMusic(const std::string& name, const std::string& filepath);   // Cargar música (.ogg, .mp3)
    bool loadSound(const std::string& name, const std::string& filepath);   // Cargar efecto (.wav)
    
    // ============================================
    // CONTROL DE MÚSICA
    // ============================================
    void playMusic(const std::string& name, bool loop = true);   // Reproducir música (con loop)
    void stopMusic(const std::string& name = "");                // Detener (vacío = todas)
    void pauseMusic();                                           // Pausar música en reproducción
    void resumeMusic();                                          // Reanudar música pausada
    void setMusicVolume(float volume);                           // Volumen: 0.0 a 100.0
    
    // ============================================
    // CONTROL DE EFECTOS DE SONIDO
    // ============================================
    void playSound(const std::string& name);                     // Reproducir efecto
    void setSoundVolume(float volume);                           // Volumen: 0.0 a 100.0
    
    // ============================================
    // INFORMACIÓN DE ESTADO
    // ============================================
    bool isMusicPlaying() const;                                 // ¿Hay música en reproducción?
    
private:
    // ============================================
    // CONSTRUCTOR PRIVADO - Singleton
    // ============================================
    AudioManager();
    ~AudioManager() = default;
    
    // ============================================
    // ALMACENAMIENTO DE RECURSOS
    // ============================================
    std::map<std::string, sf::Music> musics;              // Música cargada (por nombre)
    std::map<std::string, sf::SoundBuffer> soundBuffers;  // Buffers de sonido
    std::map<std::string, sf::Sound> sounds;              // Efectos de sonido
    
    // ============================================
    // CONFIGURACIÓN
    // ============================================
    float musicVolume;      // Volumen actual de música (0-100)
    float soundVolume;      // Volumen actual de efectos (0-100)
};
