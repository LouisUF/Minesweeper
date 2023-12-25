#include <string>
#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
using namespace std;
#include "TextureManager.h"
struct Sprite {
    sf::Sprite currentSprite;


    Sprite(sf::Texture& pTexture, float pXOrigin, float pYOrigin, float pXPosition, float pYPosition) {
        currentSprite.setTexture(pTexture);
        currentSprite.setOrigin(pXOrigin, pYOrigin);
        currentSprite.setPosition(pXPosition, pYPosition);
    }

};
#ifndef MINESWEEPER_SPRITE_H
#define MINESWEEPER_SPRITE_H

#endif //MINESWEEPER_SPRITE_H
