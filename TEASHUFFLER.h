#ifndef TEASHUFFLER_H
#define TEASHUFFLER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
#include "TEAREADER.H"

class TeaShuffler {
private:
    TeaReader* oracle;
    bool isShuffling;
    sf::Clock animationTimer;
    sf::SoundBuffer shuffleBuffer;
    sf::Sound shuffleSound;

public:
    TeaShuffler(TeaReader* readerPtr);
    void start();
    void stop();
    void update(sf::Sprite& sprite);
    bool isRunning() const;
};

#endif