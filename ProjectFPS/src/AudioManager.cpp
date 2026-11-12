#include <pch.h>
#include "AudioManager.h"

// Singleton instance getter
AudioManager& AudioManager::getInstance() {
    static AudioManager instance; 
    return instance;
}

AudioManager::AudioManager() : listenerX(0), listenerY(0) {}

AudioManager::~AudioManager() {
    cleanup();
}

// Initialize SDL audio and SDL_mixer
bool AudioManager::init() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

// Cleanup loaded audio and close SDL_mixer
void AudioManager::cleanup() {
    
    for (auto& musicPair : musicMap) {
        Mix_FreeMusic(musicPair.second);
    }
    musicMap.clear();

    for (auto& soundPair : soundMap) {
        Mix_FreeChunk(soundPair.second);
    }
    soundMap.clear();

    Mix_CloseAudio();
    SDL_Quit();
}

// Load a music file
bool AudioManager::loadMusic(const std::string& name, const std::string& filePath) {
    Mix_Music* music = Mix_LoadMUS(filePath.c_str());
    if (!music) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return false;
    }

    musicMap[name] = music;
    return true;
}

// Load a sound effect file
bool AudioManager::loadSound(const std::string& name, const std::string& filePath) {
    Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());
    if (!sound) {
        std::cerr << "Failed to load sound: " << Mix_GetError() << std::endl;
        return false;
    }

    soundMap[name] = sound;
    return true;
}

// Play a loaded music file
void AudioManager::playMusic(const std::string& name, int loops) {
    auto it = musicMap.find(name);
    if (it != musicMap.end()) {
        Mix_PlayMusic(it->second, loops);
    }
    else {
        std::cerr << "Music not found: " << name << std::endl;
    }
}

// Play a loaded sound effect file
void AudioManager::playSound(const std::string& name, int loops) {
    auto it = soundMap.find(name);
    if (it != soundMap.end()) {
        Mix_PlayChannel(-1, it->second, loops);
    }
    else {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

// Play a sound effect at a specific position relative to the listener
void AudioManager::playSoundAt(const std::string& name, int loops, int x, int y, int listenerX, int listenerY) {
    auto it = soundMap.find(name);
    if (it == soundMap.end()) {
        std::cerr << "Sound not found: " << name << std::endl;
        return;
    }

    // Calculate distance and angle
    int dx = x - listenerX;
    int dy = y - listenerY;
    double distance = sqrt(dx * dx + dy * dy);
    double angle = atan2(dy, dx) * 180.0 / M_PI; // Convert radians to degrees

    // Normalize angle for SDL (0-360 degrees, clockwise from top)
    if (angle < 0) angle += 360.0;

    // Clamp distance to a range SDL_mixer supports (0-255)
    int distanceClamped = std::min(static_cast<int>(distance), 255);

    // Play sound on a free channel
    int channel = Mix_PlayChannel(-1, it->second, loops);

    // Set position for spatial sound
    if (channel != -1) {
        Mix_SetPosition(channel, static_cast<Uint16>(angle), static_cast<Uint8>(distanceClamped));
    }
    else {
        std::cerr << "Failed to play sound: No free channel available" << std::endl;
    }
}

// Set the listener's position
void AudioManager::setListenerPosition(int x, int y) {
    listenerX = x;
    listenerY = y;
}