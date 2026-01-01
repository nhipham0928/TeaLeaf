#include "TEASYMBOL.H"
#include <iostream>
#include <fstream> 

TeaSymbol::TeaSymbol(sf::String n, sf::String m, string imgPath)
{
    this->name = n;
    this->meaning = m;

    if (this->texture.loadFromFile(imgPath))
    {
        this->hasImage = true;
    }
    else {
        this->hasImage = false;
    }
}

sf::String TeaSymbol::getName() const { return name; }
sf::String TeaSymbol::getMeaning() const { return meaning; }
const sf::Texture* TeaSymbol::getTexture() const { return &texture; }
bool TeaSymbol::isImageLoaded() const { return hasImage; }

