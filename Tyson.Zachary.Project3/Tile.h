//
// Created by Zach on 12/6/2021.
//

#ifndef PROJECT3_1_TILE_H
#define PROJECT3_1_TILE_H
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>

#include <iostream>
class Tile{
public:
    sf::Sprite background;
    sf::Sprite foreground;
    sf::Sprite icon;
    sf::Sprite flag;
    sf::Sprite debug;
    Tile(std::unordered_map<std::string,sf::Texture> &textures, int x, int y);
    sf::Sprite* getIcon();
    int x;
    int y;
    bool isBomb;
    short surroundingBombs;
    bool isRevealed;
    bool isFlagged;
    void setStats(bool bomb, short bombsAround, std::unordered_map<std::string,sf::Texture> &textures);
    bool leftClick();
    std::vector<Tile*>surroundingTiles;
    void surroundingTotal();
    void setIcon(std::unordered_map<std::string,sf::Texture> &textures);
    void setFlag(std::unordered_map<std::string,sf::Texture> &textures);
    void Debug(bool debugMode);
};

Tile::Tile(std::unordered_map<std::string,sf::Texture> &textures,int x,int y) {
    background.setTexture(textures["hidden"]);
    background.setPosition(x*32,y*32);
    foreground.setTexture(textures["revealed"]);
    foreground.setPosition(x*32,y*32);
    flag.setTexture(textures["flag"]);
    flag.setPosition(x*32,y*32);
    icon.setTexture(textures["revealed"]);
    icon.setPosition(x*32,y*32);
    debug.setTexture(textures["mine"]);
    debug.setPosition(x*32,y*32);
    surroundingBombs = 0;
    this->x = x;
    this->y = y;
    isBomb=false;
    isFlagged = false;
    isRevealed=false;
}


sf::Sprite* Tile::getIcon() {
    return &background;
}

void Tile::setStats(bool bomb, short bombsAround, std::unordered_map<std::string,sf::Texture> &textures) {
    if(bomb) {
        isBomb=true;
    }
    icon.setColor(sf::Color(255,255,255,0));
    foreground.setColor(sf::Color(255,255,255,0));
    flag.setColor(sf::Color(255,255,255,0));
    debug.setColor(sf::Color(255,255,255,0));
}

bool Tile::leftClick() { //Returns false if the left click results in a game over
    if(isFlagged || isRevealed) {
        return true; //Left-clicking a flagged tile does nothing in Minesweeper
        //and if it's revealed, then obviously you can't do anything do it
    }
    background.setColor(sf::Color(255,255,255,0));
    foreground.setColor(sf::Color(255,255,255,255));
    icon.setColor(sf::Color(255,255,255,255));
    isRevealed= true;
    if(isBomb) {
        return false; //Game over
    }
    return true;


}

void Tile::surroundingTotal() {
    for(int i = 0; i < surroundingTiles.size(); i++){
        if(surroundingTiles.at(i)->isBomb){
            surroundingBombs++;
        }
    }
}

void Tile::setIcon(std::unordered_map<std::string,sf::Texture> &textures) {
    if(isBomb) {
        icon.setTexture(textures["mine"]);
        return;
    }
    switch (surroundingBombs) {
        case 0: icon.setTexture(textures["revealed"]); break;
        case 1: icon.setTexture(textures["one"]); break;
        case 2: icon.setTexture(textures["two"]); break;
        case 3: icon.setTexture(textures["three"]); break;
        case 4: icon.setTexture(textures["four"]); break;
        case 5: icon.setTexture(textures["five"]); break;
        case 6: icon.setTexture(textures["six"]); break;
        case 7: icon.setTexture(textures["seven"]); break;
        case 8: icon.setTexture(textures["eight"]); break;
        default: icon.setTexture(textures["revealed"]); break;
    }
}

void Tile::setFlag(std::unordered_map<std::string, sf::Texture> &textures) {
    if(isRevealed){
        return;
    }
    if(isFlagged){
        isFlagged = false;
        flag.setColor(sf::Color(255,255,255,0));
    } else {
        isFlagged = true;
        flag.setColor(sf::Color(255,255,255,255));
    }
}

void Tile::Debug(bool debugMode) {
    if(isBomb) {
        if(debugMode) {
            debug.setColor(sf::Color(255,255,255,255));
        } else {
            debug.setColor(sf::Color(255,255,255,0));
        }
    }
}


#endif //PROJECT3_1_TILE_H
