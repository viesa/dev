#include "AudioMgr.hpp"
#include <iostream>

AudioMgr::AudioMgr()
{
    for (auto &music : m_allMusic)
        music = new sf::Music;

    for (auto &soundBuffer : m_allSoundBuffers)
        soundBuffer = new sf::SoundBuffer;

    if (!m_allMusic[MusicTest]->openFromFile("assets/audio/testMusic.wav"))
        std::cerr << "Error: Could not open testMusic.wave!" << std::endl;

    if (!m_allSoundBuffers[SoundTest]->loadFromFile("assets/audio/testSound.wav"))
        std::cerr << "Error: Could not open testSound.wave!" << std::endl;
}

AudioMgr::~AudioMgr()
{
    for (auto &music : m_allMusic)
        delete music;

    for (auto &soundBuffer : m_allSoundBuffers)
        delete soundBuffer;
}