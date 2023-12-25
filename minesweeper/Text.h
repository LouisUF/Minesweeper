#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
using namespace std;
#include "TextureManager.h"

struct Text {
    string displayText;
    sf::Font font;
    int size;

};