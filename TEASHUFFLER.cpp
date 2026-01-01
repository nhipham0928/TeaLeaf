#include "TEASHUFFLER.H"
#include <iostream>
#include <algorithm> 

TeaShuffler::TeaShuffler(TeaReader* readerPtr) {
    this->oracle = readerPtr;
    this->isShuffling = false;

    if (!shuffleBuffer.loadFromFile("shuffle.wav")) {
        std::cout << "Loi: Khong tim thay file shuffle.wav" << std::endl;
    }
    shuffleSound.setBuffer(shuffleBuffer);
    shuffleSound.setLoop(true);
    shuffleSound.setVolume(40);
}

void TeaShuffler::start() {
    isShuffling = true;
    animationTimer.restart();
    shuffleSound.play();
}

void TeaShuffler::stop() {
    isShuffling = false;
    shuffleSound.stop();
}

void TeaShuffler::update(sf::Sprite& sprite) {
    if (!isShuffling) return;

    if (animationTimer.getElapsedTime().asSeconds() > 0.05f) {
        TeaSymbol* tempSym = oracle->getRandomSymbol();
        if (tempSym && tempSym->isImageLoaded()) {
            sprite.setTexture(*tempSym->getTexture());

            sf::FloatRect bounds = sprite.getLocalBounds();
            sprite.setOrigin(bounds.width / 2, bounds.height / 2);

            float targetSize = 280.0f;
            float scale = targetSize / std::max(bounds.width, bounds.height);
            sprite.setScale(scale, scale);

            float centerX = 640.0f;
            float centerY = 310.0f;

            // Rung lac
            float offsetX = (rand() % 20) - 10.0f;
            float offsetY = (rand() % 20) - 10.0f;
            sprite.setPosition(centerX + offsetX, centerY + offsetY);
        }
        animationTimer.restart();
    }
}

bool TeaShuffler::isRunning() const {
    return isShuffling;
}