#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>

class AudioManager {
public:
    // Singleton pattern
    static AudioManager& getInstance();
    
    // Prevenir copia
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    
    // Cargar recursos de audio
    bool loadMusic(const std::string& name, const std::string& filepath);
    bool loadSound(const std::string& name, const std::string& filepath);
    
    // Control de música
    void playMusic(const std::string& name, bool loop = true);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    void setMusicVolume(float volume);  // 0.0 a 100.0
    
    // Control de efectos de sonido
    void playSound(const std::string& name);
    void setSoundVolume(float volume);  // 0.0 a 100.0
    
    // Estado
    bool isMusicPlaying() const;
    
private:
    AudioManager();
    ~AudioManager() = default;
    
    sf::Music music;
    std::map<std::string, sf::SoundBuffer> soundBuffers;
    std::map<std::string, sf::Sound> sounds;
    
    float musicVolume;
    float soundVolume;
};
