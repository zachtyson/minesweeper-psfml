#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include "Game.h"
#include "Tile.h"
using namespace std;
//Plan for the project
//GRID - a 2D array generated using the width and height integers in the config file, maybe try to place them on the map using a double for loop?
//Placing mines - Iterate through the entire board, for each spot on the grid let there be a small chance that a mine is placed there.
int main()
{
    ifstream configFile("boards/config.cfg");
    string configLine;
    getline(configFile, configLine);
    int width = stoi(configLine);
    getline(configFile, configLine);
    int height = stoi(configLine);
    getline(configFile, configLine);
    int mineCount = stoi(configLine);
    configFile.close();
 //   cout <<"Width: " << width << endl;
 //   cout <<"Height: " << height << endl;
 //   cout <<"Mine Count: " << mineCount << endl;
    sf::RenderWindow window(sf::VideoMode(width*32, height*32+88), "SFML works!");
    Game* sup = new Game(width,height, mineCount);
    sup->makeTiles();
    sup->refresh(window);
    window.clear(sf::Color(255,255,255));
    window.display();

    while (window.isOpen()){
        sf::Event event;
        window.clear(sf::Color(255,255,255));
        sup->refresh(window);
        window.display();
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            if(event.type == sf::Event::MouseButtonPressed){
                sup->tick(window);
            }

        }
    }
    return 0;
}