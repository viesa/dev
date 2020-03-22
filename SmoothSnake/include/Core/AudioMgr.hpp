#pragma once

#include <array>
#include <SFML/Audio.hpp>

enum MusicMapping
{
    MusicTest,
    MusicCount
};

enum SoundMapping
{
    SoundTest,
    SoundCount
};

class AudioMgr
{
public:
    AudioMgr();
    ~AudioMgr();

    sf::Music *Get(MusicMapping music) { return m_allMusic[music]; }
    sf::SoundBuffer *&Get(SoundMapping sound) { return m_allSoundBuffers[sound]; }

private:
    std::array<sf::Music *, MusicMapping::MusicCount> m_allMusic;
    std::array<sf::SoundBuffer *, SoundMapping::SoundCount> m_allSoundBuffers;
};