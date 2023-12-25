#include <string>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Sprite.h"
using namespace std;
#include "TextureManager.h"
#pragma once

struct Tile : public sf::RectangleShape{
bool isMine;
bool isRevealed;
bool isFlagged;
vector<Tile*> adjacentTiles;
sf::Sprite tileSprite;
sf::Sprite mineSprite;
sf::Sprite flagSprite;
sf::Sprite numSprite;
sf::Sprite allRevealedSprite;
int numAdjMines;
int numberedTile;


Tile() {
    numAdjMines = 0;
    isFlagged = false;
    isRevealed = false;
    isMine = false;
}

void clicked() {

    isRevealed = true;
}

};
