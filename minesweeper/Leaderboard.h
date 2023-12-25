#include <string>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Sprite.h"
#include <chrono>
#include <sstream>

using namespace std;
#include "TextureManager.h"
#include "Tile.h"

struct Leaderboard {
int height;
int width;
int cols;
int rows;
map<int, Tile> tileMap;
sf::Font font;
string playerName;
float playerTime;
string playerTimeString;

Leaderboard(int wHeight, int wWidth, sf::Font Wfont, int wCol, int wRows, map<int, Tile>& wTileMap, string wPlayerName) {
     height = wHeight;
     width = wWidth;
     font = Wfont;
     cols = wCol;
     rows = wRows;
     tileMap = wTileMap;
     playerName = wPlayerName;
     playerTime = 0.0f;
}

void setTime(string pTime) {
    playerTimeString = pTime;
    replace(pTime.begin(), pTime.end(), ':', '.');
    float timeFloat = stof(pTime);
    playerTime = timeFloat;
    cout << "Player Time is " << playerTime << endl;
    cout << "Time String is " << playerTimeString << endl;
}

void makeWindow(bool gameEnded) {
    string s1;
    string s2;
    string s3;
    string s4;
    string s5;
    string displayString;

    sf::Text LbText;
    LbText.setString("LEADERBOARD");
    LbText.setFont(font);
    LbText.setCharacterSize(20);
    LbText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    LbText.setFillColor(sf::Color::White);

    sf::FloatRect LbTextRect = LbText.getLocalBounds();

    LbText.setOrigin(LbTextRect.left + LbTextRect.width/2.0f, LbTextRect.top + LbTextRect.height/2.0f);
    LbText.setPosition(sf::Vector2f(cols * 16 / 2.0f, ((rows * 16) + 100) / 2.0f - 120));


    sf::RenderWindow LbWindow(sf::VideoMode( cols * 16, (rows * 16)+50l), "Minesweeper", sf::Style::Close);
    fstream lbstream("files/leaderboard.txt");
    string line;
    map<int, string> stringMap;

    int currentIndex = 0;
    while(getline(lbstream, line))
    {
        stringstream linestream(line);
        string value;


        while(getline(linestream, value, ','))
        {
            stringMap[currentIndex] = value;
            cout << stringMap[currentIndex] << endl;
            currentIndex++;
        }

    }

    string timeString1 = stringMap[0];
    string oTimeString1 = timeString1;
    replace(timeString1.begin(), timeString1.end(), ':', '.');
    float time1 = stof(timeString1);

    string timeString2 = stringMap[2];
    string oTimeString2 = timeString2;
    replace(timeString2.begin(), timeString2.end(), ':', '.');
    float time2 = stof(timeString2);
    
    string timeString3 = stringMap[4];
    string oTimeString3 = timeString3;
    replace(timeString3.begin(), timeString3.end(), ':', '.');
    float time3 = stof(timeString3);

    string timeString4 = stringMap[6];
    string oTimeString4 = timeString4;
    replace(timeString4.begin(), timeString4.end(), ':', '.');
    float time4 = stof(timeString4);

    string timeString5 = stringMap[8];
    string oTimeString5 = timeString5;
    replace(timeString5.begin(), timeString5.end(), ':', '.');
    float time5 = stof(timeString5);

    if(gameEnded) {
        ofstream toLb("files/leaderboard.txt");
        if (playerTime < time1) {
            toLb << playerTimeString + "," + playerName << endl;
            toLb << oTimeString1 + "," + stringMap[1] << endl;
            toLb << oTimeString2 + "," + stringMap[3] << endl;
            toLb << oTimeString3 + "," + stringMap[5] << endl;
            toLb << oTimeString4 + "," + stringMap[7] << endl;
            stringMap[9] = stringMap[7];
            stringMap[7] = stringMap[5];
            stringMap[5] = stringMap[3];
            stringMap[3] = stringMap[1];
            stringMap[1] = playerName + "*";
            stringMap[0] = playerTimeString;

            stringMap[2] = oTimeString1;
            stringMap[4] = oTimeString2;
            stringMap[6] = oTimeString3;
            stringMap[8] = oTimeString4;

        } else if (playerTime < time2) {
            toLb << oTimeString1 + "," + stringMap[1] << endl;
            toLb << playerTimeString + "," + playerName << endl;
            toLb << oTimeString2 + "," + stringMap[3] << endl;
            toLb << oTimeString3 + "," + stringMap[5] << endl;
            toLb << oTimeString4 + "," + stringMap[7] << endl;
            stringMap[9] = stringMap[7];
            stringMap[7] = stringMap[5];
            stringMap[5] = stringMap[3];
            stringMap[3] = playerName + "*";
            stringMap[2] = playerTimeString;

            stringMap[4] = oTimeString2;
            stringMap[6] = oTimeString3;
            stringMap[8] = oTimeString4;

        } else if (playerTime < time3) {
            toLb << oTimeString1 + "," + stringMap[1] << endl;
            toLb << oTimeString2 + "," + stringMap[3] << endl;
            toLb << playerTimeString + "," + playerName << endl;
            toLb << oTimeString3 + "," + stringMap[5] << endl;
            toLb << oTimeString4 + "," + stringMap[7] << endl;
            stringMap[9] = stringMap[7];
            stringMap[7] = stringMap[5];
            stringMap[5] = playerName + "*";
            stringMap[4] = playerTimeString;

            stringMap[6] = oTimeString3;
            stringMap[8] = oTimeString4;

        } else if (playerTime < time4) {
            toLb << oTimeString1 + "," + stringMap[1] << endl;
            toLb << oTimeString2 + "," + stringMap[3] << endl;
            toLb << oTimeString3 + "," + stringMap[5] << endl;
            toLb << playerTimeString + "," + playerName << endl;
            toLb << oTimeString4 + "," + stringMap[7] << endl;
            stringMap[9] = stringMap[7];
            stringMap[7] = playerName + "*";
            stringMap[6] = playerTimeString;

            stringMap[8] = oTimeString4;
        } else if (playerTime < time5) {
            toLb << oTimeString1 + "," + stringMap[1] << endl;
            toLb << oTimeString2 + "," + stringMap[3] << endl;
            toLb << oTimeString3 + "," + stringMap[5] << endl;
            toLb << oTimeString4 + "," + stringMap[7] << endl;
            toLb << playerTimeString + "," + playerName << endl;
            stringMap[9] = playerName + "*";
            stringMap[8] = playerTimeString;
        } else {
            toLb << timeString1 + "," + stringMap[1] << endl;
            toLb << timeString2 + "," + stringMap[3] << endl;
            toLb << timeString3 + "," + stringMap[5] << endl;
            toLb << timeString4 + "," + stringMap[7] << endl;
            toLb << timeString5 + "," + stringMap[9] << endl;
        }
    }

    s1 = "1. \t" + stringMap[0] + "\t" + stringMap[1] + "\n\n";
    s2 = "2. \t" + stringMap[2] + "\t" + stringMap[3] + "\n\n";
    s3 = "3. \t" + stringMap[4] + "\t" + stringMap[5] + "\n\n";
    s4 = "4. \t" + stringMap[6] + "\t" + stringMap[7] + "\n\n";
    s5 = "5. \t" + stringMap[8] + "\t" + stringMap[9] + "\n\n";
    displayString = s1 + s2 + s3 + s4 + s5;




    sf::Text LbDisplayText;
    LbDisplayText.setString(displayString);
    LbDisplayText.setFont(font);
    LbDisplayText.setCharacterSize(18);
    LbDisplayText.setStyle(sf::Text::Bold);
    LbDisplayText.setFillColor(sf::Color::White);

    sf::FloatRect LbDisplayTextRect = LbDisplayText.getLocalBounds();

    LbDisplayText.setOrigin(LbDisplayTextRect.left + LbDisplayTextRect.width/2.0f, LbDisplayTextRect.top + LbDisplayTextRect.height/2.0f);
    LbDisplayText.setPosition(sf::Vector2f(cols * 16 / 2.0f, ((rows * 16) + 100) / 2.0f + 20));

    while(LbWindow.isOpen()) {

            sf::Event event;
            while (LbWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    LbWindow.close();
                }
            }
        LbWindow.clear(sf::Color::Blue);
            LbWindow.draw(LbText);
            LbWindow.draw(LbDisplayText);
        LbWindow.display();
    }

}
};