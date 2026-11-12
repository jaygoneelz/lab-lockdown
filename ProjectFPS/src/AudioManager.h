#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <map>

class AudioManager {
public:
    // Get the single instance of AudioManager (Singleton)
    static AudioManager& getInstance();

    // Delete copy constructor and assignment operator (to enforce Singleton)
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // Initialization and Cleanup
    bool init();
    void cleanup();

    // Load audio
    bool loadMusic(const std::string& name, const std::string& filePath);
    bool loadSound(const std::string& name, const std::string& filePath);

    // Play audio
    void playMusic(const std::string& name, int loops = -1);
    void playSound(const std::string& name, int loops = 0);
    void playSoundAt(const std::string& name, int loops, int x, int y, int listenerX, int listenerY);

    // Spatial audio helpers
    void setListenerPosition(int x, int y);

private:
    // Private constructor/destructor for Singleton pattern
    AudioManager();
    ~AudioManager();

    // Maps to store music and sound effects
    std::map<std::string, Mix_Music*> musicMap;
    std::map<std::string, Mix_Chunk*> soundMap;

    // Listener position for spatial sound
    int listenerX;
    int listenerY;
};