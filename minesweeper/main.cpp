#include <string>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Sprite.h"
#include <chrono>
using namespace std;
#include "TextureManager.h"
#include "Tile.h"
#include "Leaderboard.h"

//Instead of tile map and tile num, pass in a pointer to a tile?
//Instead of a texturemap, store each texture inside the tile sprite?
void revealTiles(map<int, Tile> &tileMap, int tileNum, sf::Texture& revealedTexture, map<int, sf::Texture> &textureMap, int& numRevealed)
{
    if (tileMap[tileNum].isRevealed)
    {
        return;
    }
    numRevealed++;
    tileMap[tileNum].isRevealed = true;
    tileMap[tileNum].tileSprite.setTexture(revealedTexture);
    if(tileMap[tileNum].numAdjMines == 0)
    {
        for(int i = 0; i < tileMap[tileNum].adjacentTiles.size(); i++)
        {
            int newTileNum = tileMap[tileNum].adjacentTiles[i] -> numberedTile;
            revealTiles(tileMap, newTileNum, revealedTexture, textureMap, numRevealed);
        }
    } else {

        int currentAdjMines = tileMap[tileNum].numAdjMines;
        tileMap[tileNum].numSprite.setTexture(textureMap[currentAdjMines]);
       return;
    }

    }

    void revealMines(map<int, Tile>& tileMap, sf::Texture& mineTexture, bool minesRevealed, sf::Texture &hiddenTexture)
    {
        int iterNum = 0;
        for(auto i = tileMap.begin(); i != tileMap.end(); i++)
        {
            if (tileMap[iterNum].isMine)
            {
                if(minesRevealed)
                    tileMap[iterNum].mineSprite.setTexture(hiddenTexture);
                else if (!minesRevealed)
                    tileMap[iterNum].mineSprite.setTexture(mineTexture);
            }
            iterNum++;
        }
    }

int main()
{
    //TODO: Add negative to timer,
    // Lines 11-26: Reads in values from config and stores in variables
    ifstream configFile("files/board_config.cfg");
    string numColumns;
    string numRows;
    string numMines;
    int newColumns = 0;
    int newRows = 0;
    int newMines = 0;
    if(configFile.is_open())
    {
        getline(configFile, numColumns);
        getline(configFile, numRows);
        getline(configFile, numMines);
        newColumns = stoi(numColumns);
        newRows = stoi(numRows);
        newMines = stoi(numMines);
    }
    //Creates the name window
    int welcomeWidth = newColumns * 32;
    int welcomeHeight = (newRows * 32) + 100;

    sf::RenderWindow welcomeWindow(sf::VideoMode( welcomeWidth, welcomeHeight), "Minesweeper", sf::Style::Close);

    // Creating font object. SFML font objects will use .loadFromFile() function
    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        return 0;
    }
    // Creating text object called nameText
    //sf::Text nameText("Press name to begin", font, 16); // Only takes 3 arguments
    sf::Text welcomeText;
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setFont(font);
    welcomeText.setCharacterSize(24);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcomeText.setFillColor(sf::Color::White);

    sf::FloatRect welcomeTextRect = welcomeText.getLocalBounds();

    // Origin is default the top left corner, so we change this to be the middle of the x and y axis
    welcomeText.setOrigin(welcomeTextRect.left + welcomeTextRect.width/2.0f, welcomeTextRect.top + welcomeTextRect.height/2.0f);
    welcomeText.setPosition(sf::Vector2f(welcomeWidth / 2.0f, welcomeHeight / 2.0f - 150));


    sf::Text enterText;
    enterText.setString("Enter your name:");
    enterText.setFont(font);
    enterText.setCharacterSize(20);
    enterText.setStyle(sf::Text::Bold);
    enterText.setFillColor(sf::Color::White);

    sf::FloatRect enterTextRect = enterText.getLocalBounds();

    // Origin is default the top left corner, so we change this to be the middle of the x and y axis
    enterText.setOrigin(enterTextRect.left + enterTextRect.width/2.0f, enterTextRect.top + enterTextRect.height/2.0f);
    enterText.setPosition(sf::Vector2f(welcomeWidth / 2.0f, welcomeHeight / 2.0f - 75));

    sf::Text nameText;
    string currentString;
    nameText.setString("|");
    nameText.setFont(font);
    nameText.setCharacterSize(18);
    nameText.setStyle(sf::Text::Bold);
    nameText.setFillColor(sf::Color::Yellow);

    sf::FloatRect nameTextRect = nameText.getLocalBounds();

    // Origin is default the top left corner, so we change this to be the middle of the x and y axis
    nameText.setOrigin(nameTextRect.left + nameTextRect.width/2.0f, nameTextRect.top + nameTextRect.height/2.0f);
    nameText.setPosition(sf::Vector2f(welcomeWidth / 2.0f, welcomeHeight / 2.0f - 45));

    // Everything that is updated has to occur in the while loop
    while (welcomeWindow.isOpen()) { // while the name window is active, we do everything below here
        sf::Event event;
        while (welcomeWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            {
                welcomeWindow.close();
                return 0;
            }// Click X on the window


            if (event.type == sf::Event::TextEntered) {
                int textSize = currentString.size();
                if (isalpha(static_cast<char>(event.text.unicode)) && textSize < 10)
                {
                    char currentChar = static_cast<char>(event.text.unicode);
                    if(textSize == 0)
                    {
                       currentChar = toupper(currentChar);
                    } else {
                        currentChar = tolower(currentChar);
                    }
                    currentString += currentChar;
                    nameText.setString(currentString + "|");
                    nameTextRect = nameText.getLocalBounds();
                    nameText.setOrigin(nameTextRect.left + nameTextRect.width/2.0f, nameTextRect.top + nameTextRect.height/2.0f);
                } else if (event.text.unicode == 8 && textSize > 0)
                {
                    currentString.erase(textSize - 1, 1);
                    nameText.setString(currentString + "|");
                    nameTextRect = nameText.getLocalBounds();
                    nameText.setOrigin(nameTextRect.left + nameTextRect.width/2.0f, nameTextRect.top + nameTextRect.height/2.0f);
                }
            }
            if (event.type == sf::Event::KeyPressed) { // Getting user input
                int textSize = currentString.size();
                if (event.key.code == sf::Keyboard::Enter && textSize >= 1) {  // event.key.code == ASCII values
                    welcomeWindow.close();
                }
            }
        }

        // Most code will go above this line for this window
        welcomeWindow.clear(sf::Color::Blue); // Set background color of the window
        welcomeWindow.draw(welcomeText); // have to draw each object for this specific window
        welcomeWindow.draw(enterText);
        welcomeWindow.draw(nameText);
        welcomeWindow.display(); // Final
    }


    bool resetWindow = false;
    while(resetWindow == false) {
    sf::Texture& happyFaceTexture = TextureManager::getTexture("face_happy");
    sf::Sprite happyFaceSprite;
    happyFaceSprite.setTexture(happyFaceTexture);
    happyFaceSprite.setPosition((newColumns / 2.0f) * 32 - 32, 32 * (newRows + 0.5f));

    sf::Texture& loseFaceTexture = TextureManager::getTexture("face_lose");

    sf::Texture& winFaceTexture = TextureManager::getTexture("face_win");



    sf::Texture& debugTexture = TextureManager::getTexture("debug");
    sf::Sprite debugSprite;
    debugSprite.setTexture(debugTexture);
    debugSprite.setPosition(newColumns * 32 - 304, 32 * (newRows + 0.5f));

    sf::Texture& pauseTexture = TextureManager::getTexture("pause");
    sf::Sprite pauseSprite;
    pauseSprite.setTexture(pauseTexture);
    pauseSprite.setPosition(newColumns * 32 - 240, 32 * (newRows + 0.5f));

    sf::Texture& playTexture = TextureManager::getTexture("play");

    sf::Texture leaderboardTexture;
    leaderboardTexture.loadFromFile("files/images/leaderboard.png");
    sf::Sprite leaderboardSprite;
    leaderboardSprite.setTexture(leaderboardTexture);
    leaderboardSprite.setPosition(newColumns * 32 - 176, 32 * (newRows + 0.5f));

    sf::IntRect zeroRect(0, 0, 21, 32);
    sf::IntRect oneRect(21, 0, 21, 32);
    sf::IntRect twoRect(42, 0, 21, 32);
    sf::IntRect threeRect(63, 0, 21, 32);
    sf::IntRect fourRect(84, 0, 21, 32);
    sf::IntRect fiveRect(105, 0, 21, 32);
    sf::IntRect sixRect(126, 0, 21, 32);
    sf::IntRect sevenRect(147, 0, 21, 32);
    sf::IntRect eightRect(168, 0, 21, 32);
    sf::IntRect nineRect(189, 0, 21, 32);


    sf::Texture digitsTexture;
    digitsTexture.loadFromFile("files/images/digits.png");

    sf::Sprite counterSprite1(digitsTexture);
    counterSprite1.setTextureRect(zeroRect); //When updating mines, update the setTextureRect to different number rects
    counterSprite1.setPosition(33, 32 * (newRows + 0.5f) + 16);

    sf::Sprite counterSprite2(digitsTexture);
    counterSprite2.setTextureRect(zeroRect); //When updating time, update the setTextureRect to different number rects
    counterSprite2.setPosition(counterSprite1.getPosition().x + 21, 32 * (newRows + 0.5f) + 16);

    sf::Sprite counterSprite3(digitsTexture);
    counterSprite3.setTextureRect(zeroRect); //When updating time, update the setTextureRect to different number rects
    counterSprite3.setPosition(counterSprite2.getPosition().x + 21, 32 * (newRows + 0.5f) + 16);

    sf::Sprite minutesSprite1(digitsTexture);
    minutesSprite1.setTextureRect(zeroRect);
    minutesSprite1.setPosition((newColumns * 32) - 97, 32 * (newRows + 0.5f) + 16);

    sf::Sprite minutesSprite2(digitsTexture);
    minutesSprite2.setTextureRect(zeroRect);
    minutesSprite2.setPosition(minutesSprite1.getPosition().x + 21, 32 * (newRows + 0.5f) + 16);

    sf::Sprite secondsSprite1(digitsTexture);
    secondsSprite1.setTextureRect(zeroRect);
    secondsSprite1.setPosition((newColumns * 32) - 54, 32 * (newRows + 0.5f) + 16);

    sf::Sprite secondsSprite2(digitsTexture);
    secondsSprite2.setTextureRect(zeroRect);
    secondsSprite2.setPosition(secondsSprite1.getPosition().x + 21, 32 * (newRows + 0.5f) + 16);

    int onesDigit = 0;
    int tensDigit = 0;
    int hundDigit = 0;

    int tempMines = newMines;
    onesDigit = tempMines % 10;
    tempMines = tempMines / 10;
    tensDigit = tempMines % 10;
    tempMines = tempMines / 10;
    hundDigit = tempMines % 10;

    map<int, sf::IntRect> numsMap;
    numsMap[0] = zeroRect;
    numsMap[1] = oneRect;
    numsMap[2] = twoRect;
    numsMap[3] = threeRect;
    numsMap[4] = fourRect;
    numsMap[5] = fiveRect;
    numsMap[6] = sixRect;
    numsMap[7] = sevenRect;
    numsMap[8] = eightRect;
    numsMap[9] = nineRect;

    counterSprite1.setTextureRect(numsMap[hundDigit]);
    counterSprite2.setTextureRect(numsMap[tensDigit]);
    counterSprite3.setTextureRect(numsMap[onesDigit]);

    map<int, Tile> tileMap;
    int currentX = 0;
    int currentY = 0;

     sf::Texture hiddenTexture = TextureManager::getTexture("tile_hidden");
    //hiddenTexture.loadFromFile("files/images/tile_hidden.png");

     sf::Texture revealedTexture = TextureManager::getTexture("tile_revealed");
    //revealedTexture.loadFromFile("files/images/tile_revealed.png");

    sf::Texture mineTexture = TextureManager::getTexture("mine");
    sf::Texture flagTexture = TextureManager::getTexture("flag");
    int tileNumber = 0;
    for(int j = 0; j < newRows; j++)
    {
        for(int i = 0; i < newColumns; i++)
        {
            Tile newTile;
           newTile.tileSprite.setTexture(hiddenTexture);
            newTile.tileSprite.setPosition(currentX, currentY);
            newTile.allRevealedSprite.setTexture(revealedTexture);
            newTile.allRevealedSprite.setPosition(currentX, currentY);
            newTile.flagSprite.setPosition(currentX, currentY);
            newTile.numSprite.setPosition(currentX, currentY);
            newTile.numberedTile = tileNumber;
            currentX += 32;
            tileMap[tileNumber] = newTile;
            tileNumber++;
        }
        currentY += 32;
        currentX = 0;
    }

    for(int i = 0; i < newMines; i++)
    {
        int randNum = rand() % (newColumns * newRows);
        tileMap[randNum].isMine = true;
        tileMap[randNum].mineSprite.setPosition(tileMap[randNum].tileSprite.getPosition().x,tileMap[randNum].tileSprite.getPosition().y);

        if(randNum == 0)
        {
            tileMap[randNum + 1].numAdjMines++;
            tileMap[randNum + newColumns].numAdjMines++;
            tileMap[randNum + (newColumns + 1)].numAdjMines++;
        } else if (randNum == (newColumns * newRows - 1))
        {
            tileMap[randNum - 1].numAdjMines++;
            tileMap[randNum - newColumns].numAdjMines++;
            tileMap[randNum - (newColumns + 1)].numAdjMines++;
        } else if (randNum == newColumns - 1)
        {
            tileMap[randNum - 1].numAdjMines++;
            tileMap[randNum + newColumns].numAdjMines++;
            tileMap[randNum + (newColumns - 1)].numAdjMines++;
        } else if (randNum == newColumns * (newRows -1))
        {
            tileMap[randNum + 1].numAdjMines++;
            tileMap[randNum - newColumns].numAdjMines++;
            tileMap[randNum - (newColumns - 1)].numAdjMines++;
        } else if (randNum < newColumns)
        {
            tileMap[randNum + 1].numAdjMines++;
            tileMap[randNum - 1].numAdjMines++;
            tileMap[randNum + newColumns].numAdjMines++;
            tileMap[randNum + (newColumns + 1)].numAdjMines++;
            tileMap[randNum + (newColumns - 1)].numAdjMines++;
        } else if (randNum > (newColumns * (newRows - 1)))
        {
            tileMap[randNum + 1].numAdjMines++;
            tileMap[randNum - 1].numAdjMines++;
            tileMap[randNum - newColumns].numAdjMines++;
            tileMap[randNum - (newColumns + 1)].numAdjMines++;
            tileMap[randNum - (newColumns - 1)].numAdjMines++;
        } else if (randNum % newColumns == 0) {
            tileMap[randNum + newColumns].numAdjMines++;
            tileMap[randNum - newColumns].numAdjMines++;
            tileMap[randNum + 1].numAdjMines++;
            tileMap[randNum + (newColumns + 1)].numAdjMines++;
            tileMap[randNum - (newColumns - 1)].numAdjMines++;
        } else if (randNum % newColumns == (newColumns -1))
        {
            tileMap[randNum + newColumns].numAdjMines++;
            tileMap[randNum + (newColumns - 1)].numAdjMines++;
            tileMap[randNum - 1].numAdjMines++;
            tileMap[randNum - newColumns].numAdjMines++;
            tileMap[randNum - (newColumns + 1)].numAdjMines++;
        }
        else {
            tileMap[randNum + 1].numAdjMines++;
            tileMap[randNum - 1].numAdjMines++;
            tileMap[randNum + newColumns].numAdjMines++;
            tileMap[randNum - newColumns].numAdjMines++;
            tileMap[randNum + (newColumns + 1)].numAdjMines++;
            tileMap[randNum - (newColumns + 1)].numAdjMines++;
            tileMap[randNum + (newColumns - 1)].numAdjMines++;
            tileMap[randNum - (newColumns - 1)].numAdjMines++;
        }
    }

    int iterationTile = 0;
    for(auto i = tileMap.begin(); i != tileMap.end(); i++)
    {

      if(iterationTile == 0)
        {
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + 1]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + newColumns]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + (newColumns + 1)]);
        } else if (iterationTile == (newColumns * newRows - 1))
        {
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - 1]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - newColumns]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - (newColumns + 1)]);
        } else if (iterationTile == newColumns - 1)
        {
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - 1]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + newColumns]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + (newColumns - 1)]);
        } else if (iterationTile == newColumns * (newRows -1))
        {
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + 1]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - newColumns]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - (newColumns - 1)]);
        } else if (iterationTile < newColumns)
        {
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + 1]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - 1]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + newColumns]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + (newColumns + 1)]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + (newColumns - 1)]);
        } else if (iterationTile > (newColumns * (newRows - 1)))
        {
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + 1]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - 1]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - newColumns]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - (newColumns + 1)]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - (newColumns - 1)]);
        } else if (iterationTile % newColumns == 0) {
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + newColumns]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - newColumns]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + 1]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + (newColumns + 1)]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - (newColumns - 1)]);
        } else if (iterationTile % newColumns == (newColumns -1))
        {
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + newColumns]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + (newColumns - 1)]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - 1]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - newColumns]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - (newColumns + 1)]);
        }
        else {
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + 1]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - 1]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + newColumns]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - newColumns]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + (newColumns + 1)]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - (newColumns + 1)]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile + (newColumns - 1)]);
            i -> second.adjacentTiles.push_back(&tileMap[iterationTile - (newColumns - 1)]);
        }
        iterationTile++;
    }

int updatedMines = newMines;
map<int, sf::Texture> textureMap;
sf::Texture num1 = TextureManager::getTexture("number_1");
    sf::Texture num2 = TextureManager::getTexture("number_2");
    sf::Texture num3 = TextureManager::getTexture("number_3");
    sf::Texture num4 = TextureManager::getTexture("number_4");
    sf::Texture num5 = TextureManager::getTexture("number_5");
    sf::Texture num6 = TextureManager::getTexture("number_6");
    sf::Texture num7 = TextureManager::getTexture("number_7");
    sf::Texture num8 = TextureManager::getTexture("number_8");

    textureMap[1] = num1;
    textureMap[2] = num2;
    textureMap[3] = num3;
    textureMap[4] = num4;
    textureMap[5] = num5;
    textureMap[6] = num6;
    textureMap[7] = num7;
    textureMap[8] = num8;

    bool minesRevealed = false;
    int numRevealed = 0;
        int minutes = 0;
        int seconds = 0;

        auto start_time = chrono::high_resolution_clock::now();
        auto pauseTime = chrono::high_resolution_clock::now();
        auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count();

        bool paused = false;
        bool endGame = false;
        bool drawLb = false;
        Leaderboard lb(welcomeHeight, welcomeWidth, font, newColumns, newRows, tileMap, currentString);

        sf::RenderWindow gameWindow(sf::VideoMode(welcomeWidth, welcomeHeight), "Minesweeper", sf::Style::Close);
        while (gameWindow.isOpen() && resetWindow == false) {
            sf::Event event;
            while (gameWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed){
                    gameWindow.close();
                    return 0;
                }

                if (event.type == sf::Event::MouseButtonPressed) {

                    sf::Vector2i mouse;
                    mouse = sf::Mouse::getPosition(gameWindow);
                    int tileX = mouse.x / 32;
                    int tileY = mouse.y / 32;
                    int tileNum = tileX + (tileY * newColumns);
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        if (happyFaceSprite.getGlobalBounds().contains(gameWindow.mapPixelToCoords(mouse))) {
                            resetWindow = true;
                            gameWindow.close();
                        }
                        if (!endGame ) {
                            if(pauseSprite.getGlobalBounds().contains(gameWindow.mapPixelToCoords(mouse))) {
                                paused = !paused;
                                if (paused) {
                                    pauseTime = chrono::high_resolution_clock::now();
                                    pauseSprite.setTexture(playTexture);
                                }else{
                                    auto unPausedTime = chrono::high_resolution_clock::now();
                                    elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count();
                                    pauseSprite.setTexture(pauseTexture);
                                }
                            }
                            if(leaderboardSprite.getGlobalBounds().contains(gameWindow.mapPixelToCoords(mouse))) {
                                drawLb = true;
                                paused = !paused;
                               // paused = !paused;
                                // lb.makeWindow();
                               // paused = !paused;

                            }
                            if(!paused) {
                                if (debugSprite.getGlobalBounds().contains(gameWindow.mapPixelToCoords(mouse))) {
                                    revealMines(tileMap, mineTexture, minesRevealed, hiddenTexture);
                                    minesRevealed = !minesRevealed;
                                }


                                if (tileMap[tileNum].isMine) {
                                    happyFaceSprite.setTexture(loseFaceTexture);
                                    tileMap[tileNum].mineSprite.setTexture(mineTexture);

                                    for (auto i = tileMap.begin(); i != tileMap.end(); i++) {
                                        if (i->second.isMine) {
                                            i->second.isRevealed = true;
                                            i->second.tileSprite.setTexture(revealedTexture);
                                            i->second.mineSprite.setTexture(mineTexture);
                                        }

                                    }
                                    paused = true;
                                    endGame = true;
                                } else if (tileMap[tileNum].numAdjMines > 0 && tileMap[tileNum].isMine == false) {
                                    int currentAdjMines = tileMap[tileNum].numAdjMines;
                                    tileMap[tileNum].numSprite.setTexture(textureMap[currentAdjMines]);
                                }
                                revealTiles(tileMap, tileNum, revealedTexture, textureMap, numRevealed);
                            }

                        } else {
                            cout << "GAME OVER";
                        }

                    }
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !endGame && !paused) {
                        if (!tileMap[tileNum].isFlagged) {
                            tileMap[tileNum].flagSprite.setTexture(flagTexture);
                            tileMap[tileNum].isFlagged = true;
                            updatedMines--;

                        } else if (tileMap[tileNum].isFlagged) {
                            tileMap[tileNum].flagSprite.setTexture(hiddenTexture);
                            tileMap[tileNum].isFlagged = false;
                            updatedMines++;
                        }
                        int tMines = updatedMines;
                        onesDigit = tMines % 10;
                        tMines = tMines / 10;
                        tensDigit = tMines % 10;
                        tMines = tMines / 10;
                        hundDigit = tMines % 10;
                        counterSprite1.setTextureRect(numsMap[hundDigit]);
                        counterSprite2.setTextureRect(numsMap[tensDigit]);
                        counterSprite3.setTextureRect(numsMap[onesDigit]);
                    }
                    if(numRevealed >= (newRows * newColumns) - newMines)
                    {
                        happyFaceSprite.setTexture(winFaceTexture);
                        int iteratorCount = 0;
                        for(auto i = tileMap.begin(); i != tileMap.end(); i++)
                        {
                            if(tileMap[iteratorCount].isMine)
                            {
                                tileMap[iteratorCount].flagSprite.setTexture(flagTexture);
                                tileMap[iteratorCount].isFlagged = true;
                            }
                            iteratorCount++;
                        }

                        if(!endGame) {
                            int minutes0 = minutes / 10 % 10;
                            int minutes1 = minutes % 10;
                            int seconds0 = seconds / 10 % 10;
                            int seconds1 = seconds % 10;
                            string stringMin0 = to_string(minutes0);
                            string stringMin1 = to_string(minutes1);
                            string stringSec0 = to_string(seconds0);
                            string stringSec1 = to_string(seconds1);

                            string minutesSeconds = stringMin0 + stringMin1 + ":" + stringSec0 + stringSec1;
                            cout << "Minutes Seconds: " << minutesSeconds << endl;
                            lb.setTime(minutesSeconds);
                            drawLb = true;
                        }
                        endGame = true;
                        paused = true;
                    }
                }

            }

            gameWindow.clear(sf::Color::White);
            gameWindow.draw(happyFaceSprite);
            gameWindow.draw(debugSprite);
            gameWindow.draw(pauseSprite);
            gameWindow.draw(leaderboardSprite);
            gameWindow.draw(counterSprite1);
            gameWindow.draw(counterSprite2);
            gameWindow.draw(counterSprite3);

        auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
        int total_time = game_duration.count();


        if(!paused && !endGame) {
            total_time =  total_time - elapsed_paused_time;
            minutes = total_time / 60;
            seconds = total_time % 60;
        }

        int minutes0 = minutes / 10 % 10; //minutes index 0
        int minutes1 = minutes % 10; // minutes index 1
        int seconds0 = seconds / 10 % 10; // seconds index 0
        int seconds1 = seconds % 10; // seconds index 1

            minutesSprite1.setTextureRect(numsMap[minutes0]);
            minutesSprite2.setTextureRect(numsMap[minutes1]);
            secondsSprite1.setTextureRect(numsMap[seconds0]);
            secondsSprite2.setTextureRect(numsMap[seconds1]);


            gameWindow.draw(minutesSprite1);
            gameWindow.draw(minutesSprite2);
            gameWindow.draw(secondsSprite1);
            gameWindow.draw(secondsSprite2);

            if(!paused || (paused && endGame)) {
                for (auto i = tileMap.begin(); i != tileMap.end(); i++) {
                    gameWindow.draw(i->second.tileSprite);
                    if (i->second.isMine)
                        gameWindow.draw(i->second.mineSprite);
                    if (i->second.isFlagged)
                        gameWindow.draw(i->second.flagSprite);
                    if (i->second.numAdjMines > 0)
                        gameWindow.draw(i->second.numSprite);
                }
            } else if (paused && !endGame) {
                for (auto i = tileMap.begin(); i != tileMap.end(); i++) {
                    gameWindow.draw(i->second.allRevealedSprite);
                }
            }



            gameWindow.display();
            if(drawLb) {
                drawLb = false;
                lb.makeWindow(endGame);
                paused = !paused;

            }
        }
            resetWindow = false;
    }

    return 0;
}
