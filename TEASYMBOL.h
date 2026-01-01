#ifndef TEASYMBOL_H
#define TEASYMBOL_H

#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class TeaSymbol
{
private:
    sf::String name;
    sf::String meaning;
    sf::Texture texture;
    bool hasImage;      

public:
    TeaSymbol(sf::String n, sf::String m, string imgPath);

    sf::String getName() const;
    sf::String getMeaning() const;

    const sf::Texture* getTexture() const;
    bool isImageLoaded() const;
};

#endif
