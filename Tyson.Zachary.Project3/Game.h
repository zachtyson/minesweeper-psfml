//
// Created by Zach on 12/6/2021.
//
#include "Tile.h"
#include <map>
#include <unordered_map>
#ifndef PROJECT3_1_GAME_H
#define PROJECT3_1_GAME_H
using namespace std;
class Game {
public:
    int mines;
    int flags; //Flags remaining
    int bombsTotal; //Used for generating the map
    int tilesRevealed;
    int totalTiles;
    int dimH;
    int dimW;
    bool debugMode;
    bool gameRunning;
    bool gameWon; //Starts false, turns win checks happen every tile click, if a mine is clicked then this remains false and gameRunning turns false, if both false then lost (obviously)
    Game(int width, int height, int mineCount);
    void makeTiles();
    void refresh(sf::RenderWindow &window);
    void tick(sf::RenderWindow &window);
    void GetNeighbors();
    void toggleDebug();
    sf::Sprite debugIcon;
    sf::Sprite face;
    void revealNearby(Tile* tile);
    void loadBoard(const string& input);
    void newBoard();
    sf::Sprite testOne;
    sf::Sprite testTwo;
    sf::Sprite testThree;
    sf::Sprite negative;
    sf::Sprite hundreds;
    sf::Sprite tens;
    sf::Sprite ones;
    void updateBombCounter();
private:
    vector<vector<Tile*>> tiles;
    unordered_map<string, sf::Texture> gameTextures;

};

Game::Game(int width, int height, int mineCount)
{
flags = mineCount;
mines=mineCount;
bombsTotal = 0;
totalTiles = width*height;
tilesRevealed = 0;
dimW = width;
dimH = height;
tiles.resize(dimW, vector<Tile*>(dimH,nullptr));
sf::Texture textureToMap;
textureToMap.loadFromFile("images/mine.png");
gameTextures.emplace("mine", textureToMap);
textureToMap.loadFromFile("images/tile_hidden.png");
gameTextures.emplace("hidden", textureToMap);
textureToMap.loadFromFile("images/tile_revealed.png");
gameTextures.emplace("revealed", textureToMap);
textureToMap.loadFromFile("images/flag.png");
gameTextures.emplace("flag", textureToMap);
textureToMap.loadFromFile("images/number_1.png");
gameTextures.emplace("one", textureToMap);
textureToMap.loadFromFile("images/number_2.png");
gameTextures.emplace("two", textureToMap);
textureToMap.loadFromFile("images/number_3.png");
gameTextures.emplace("three", textureToMap);
textureToMap.loadFromFile("images/number_4.png");
gameTextures.emplace("four", textureToMap);
textureToMap.loadFromFile("images/number_5.png");
gameTextures.emplace("five", textureToMap);
textureToMap.loadFromFile("images/number_6.png");
gameTextures.emplace("six", textureToMap);
textureToMap.loadFromFile("images/number_7.png");
gameTextures.emplace("seven", textureToMap);
textureToMap.loadFromFile("images/number_8.png");
gameTextures.emplace("eight", textureToMap);
debugMode = true;

textureToMap.loadFromFile("images/test_1.png");
gameTextures.emplace("t1", textureToMap);
textureToMap.loadFromFile("images/test_2.png");
gameTextures.emplace("t2", textureToMap);
textureToMap.loadFromFile("images/test_3.png");
gameTextures.emplace("t3", textureToMap);
textureToMap.loadFromFile("images/digits.png");
gameTextures.emplace("digits", textureToMap);


textureToMap.loadFromFile("images/debug.png");
gameTextures.emplace("debug", textureToMap);
debugIcon.setTexture(gameTextures["debug"]);
debugIcon.setPosition((dimW*16)+96, dimH*32);
textureToMap.loadFromFile("images/face_happy.png");
gameTextures.emplace("smile", textureToMap);
textureToMap.loadFromFile("images/face_win.png");
gameTextures.emplace("win", textureToMap);
textureToMap.loadFromFile("images/face_lose.png");
gameTextures.emplace("lose", textureToMap);
face.setTexture(gameTextures["smile"]);
face.setPosition((dimW*16)-32, dimH*32);
testOne.setTexture(gameTextures["t1"]);
testOne.setPosition((dimW*16)+160, dimH*32);
testTwo.setTexture(gameTextures["t2"]);
testTwo.setPosition((dimW*16)+224, dimH*32);
testThree.setTexture(gameTextures["t3"]);
testThree.setPosition((dimW*16)+288, dimH*32);
gameRunning = true;
gameWon = false;

negative.setPosition(0,dimH*32);
hundreds.setPosition(21,dimH*32);
tens.setPosition(42,dimH*32);
ones.setPosition(63,dimH*32);
negative.setTexture(gameTextures["digits"]);
negative.setTextureRect(sf::Rect<int>(210, 0, 21, 32));
hundreds.setTexture(gameTextures["digits"]);
hundreds.setTextureRect(sf::Rect<int>(21*(mines/100), 0, 21, 32));
tens.setTexture(gameTextures["digits"]);
tens.setTextureRect(sf::Rect<int>(21*(mines/10%10), 0, 21, 32));
ones.setTexture(gameTextures["digits"]);
ones.setTextureRect(sf::Rect<int>(21*(mines%10), 0, 21, 32));

}

void Game::makeTiles() {
    face.setTexture(gameTextures["smile"]);
    vector<vector<bool>> randSpots;
    randSpots.resize(dimW, vector<bool>(dimH,false));
    int bombsNeeded = flags;
    while(bombsNeeded > 0) {
        short i = rand()%dimW;
        short j = rand()%dimH;
        if(randSpots[i][j]){
            continue;
        } else {
            randSpots[i][j] = true;
            bombsNeeded--;
        }
    }

    for(int i = 0; i < dimW; i++) {
      for(int j = 0; j < dimH; j++) {
           tiles[i][j] = new Tile(gameTextures,i,j);
           if(randSpots[i][j]) {
               tiles[i][j]->setStats(true,0,gameTextures);
               bombsTotal++;

           } else {
               tiles[i][j]->setStats(false,0,gameTextures);
           }
       }
    }
    GetNeighbors();
  //  for(int i = 0; i < dimW; i++) {
  //      for(int j = 0; j < dimH; j++) {
  //          cout<<tiles[i][j]->x<<" "<<tiles[i][j]->y<<endl;
  //      }
  //  }
    flags = mines;
    updateBombCounter();
}

void Game::refresh(sf::RenderWindow &window) {
    for(int i = 0; i < dimW; i++) {
        for(int j = 0; j < dimH; j++) {
            window.draw(tiles[i][j]->background);
            window.draw(tiles[i][j]->foreground);
            window.draw(tiles[i][j]->icon);
            window.draw(tiles[i][j]->flag);
            window.draw(tiles[i][j]->debug);
        }
    }
    window.draw(debugIcon);
    window.draw(face);
    window.draw(testOne);
    window.draw(testTwo);
    window.draw(testThree);
    window.draw(negative);
    window.draw(hundreds);
    window.draw(tens);
    window.draw(ones);
}

void Game::tick(sf::RenderWindow &window) { //lots of if branches
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);//returns vector2i, not idea what it is, but I hope It's what I think it could be
        //vector2i is a 2d vector with int as a parameter
        //if you don't pass Window as a parameter, it returns the mouse position
        //relative to the monitor and not the window
        if((mousePosition.x > 0 && mousePosition.x < dimW*32) && (mousePosition.y > 0 && mousePosition.y< dimH*32)&&gameRunning) {
            //Mouse position is inside the tiles
            if(tiles[mousePosition.x/32][mousePosition.y/32]->isFlagged) {
                return; //left-clicking a flagged tile should do nothing
            }
            if( !tiles[mousePosition.x/32][mousePosition.y/32]->isRevealed && tiles[mousePosition.x/32][mousePosition.y/32]->leftClick() && !tiles[mousePosition.x/32][mousePosition.y/32]->isFlagged){
                    tilesRevealed++;
            }
            if(tiles[mousePosition.x/32][mousePosition.y/32]->isBomb) {
                for(int i = 0; i < dimW; i++) {
                    for(int j = 0; j < dimH; j++) {
                        tiles[i][j]->Debug(true);
                        if(tiles[i][j]->isBomb){
                            tiles[i][j]->background.setColor(sf::Color(255,255,255,0));
                            tiles[i][j]->foreground.setColor(sf::Color(255,255,255,255));
                        }
                    }
                }
                gameRunning = false;
                gameWon = false;
                face.setTexture(gameTextures["lose"]);
            }
            revealNearby(tiles[mousePosition.x/32][mousePosition.y/32]);
            if(tilesRevealed==totalTiles-bombsTotal && gameRunning) {
                //WIN CONDITION
                gameRunning = false;
                gameWon = true;
                for(int i = 0; i < dimW; i++){
                    for(int j = 0; j < dimH;j++){
                        if(tiles[i][j]->isBomb){
                            tiles[i][j]->flag.setColor(sf::Color(255,255,255,255));
                            tiles[i][j]->icon.setColor(sf::Color(255,255,255,0));
                            flags = 0;
                            updateBombCounter();
                        }
                    }
                }

                face.setTexture(gameTextures["win"]);
            }
        } //Below is an else statement, for if the mouse is clicked on the white area below the map
        //Shouldn't do anything if outside the window
        else if(mousePosition.y>dimH*32&&mousePosition.y<dimH*32+65){ //Outside the tiles
            if(mousePosition.x>((dimW*16)+96) && mousePosition.x<((dimW*16)+160)){ //debug coordinates
                toggleDebug();
            }
            if(mousePosition.x>((dimW*16)-32) && mousePosition.x<((dimW*16)+32)){ //face coordinates
                newBoard();
            }
            if(mousePosition.x>((dimW*16)+160) && mousePosition.x<((dimW*16)+224)){ //debug coordinates
                loadBoard("boards/testboard1.brd");
            }
            if(mousePosition.x>((dimW*16)+224) && mousePosition.x<((dimW*16)+288)){ //debug coordinates
                loadBoard("boards/testboard2.brd");
            }
            if(mousePosition.x>((dimW*16)+288) && mousePosition.x<((dimW*16)+352)){ //debug coordinates
                loadBoard("boards/testboard3.brd");
            }
       }
    }
    if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        if((mousePosition.x > 0 && mousePosition.x < dimW*32) && (mousePosition.y > 0 && mousePosition.y< dimH*32)&&gameRunning) {
            tiles[mousePosition.x/32][mousePosition.y/32]->setFlag(gameTextures);
            if(tiles[mousePosition.x/32][mousePosition.y/32]->isFlagged) {
                flags--;
            } else if(!tiles[mousePosition.x/32][mousePosition.y/32]->isRevealed){
                flags++;
            }
        }
        updateBombCounter();
    }

}

void Game::GetNeighbors() {
    for(int i = 0; i < dimW;i++) {
        for(int j = 0; j < dimH; j++) {
            if(i != 0) {
                if(j > 0 ){
                    if(tiles[i-1][j-1] != nullptr) {
                        tiles[i][j]->surroundingTiles.push_back(tiles[i-1][j-1]);
                    }
                }
                if(tiles[i-1][j] != nullptr){
                    tiles[i][j]->surroundingTiles.push_back(tiles[i-1][j]);
                }
                if( j < dimH-1){
                    if(tiles[i-1][j+1]!= nullptr){
                        tiles[i][j]->surroundingTiles.push_back(tiles[i-1][j+1]);
                    }
                }

            }
            if(i != dimW-1) {

                if( j != 0) {
                    if(tiles[i+1][j-1]!=nullptr){
                        tiles[i][j]->surroundingTiles.push_back(tiles[i+1][j-1]);
                    }
                }
                if(tiles[i+1][j]!= nullptr){
                    tiles[i][j]->surroundingTiles.push_back(tiles[i+1][j]);
                }
                if( j != dimH-1) {
                    if(tiles[i+1][j+1]!= nullptr){
                        tiles[i][j]->surroundingTiles.push_back(tiles[i+1][j+1]);
                    }
                }
            }
            if(j > 0) {
                if(tiles[i][j-1] != nullptr){
                    tiles[i][j]->surroundingTiles.push_back(tiles[i][j-1]);
                }
            }
            if(j < dimH-1) {
                if(tiles[i][j+1]!= nullptr){
                    tiles[i][j]->surroundingTiles.push_back(tiles[i][j+1]);
                }
            }
           tiles[i][j]->surroundingTotal();
           tiles[i][j]->setIcon(gameTextures);
        }
    }
}

void Game::toggleDebug() {
    if(!gameRunning) {
        return;
    }
    for(int i = 0; i < dimW; i++) {
        for(int j = 0; j < dimH; j++) {
            tiles[i][j]->Debug(debugMode);
        }
    }
    debugMode = !debugMode;
}

void Game::revealNearby(Tile *tile) {
    if(tile->isBomb){
        return;
    }
    if(tile->surroundingBombs != 0) {
        return;
    }
    for(int i = 0; i < tile->surroundingTiles.size(); i++){
        if(tile->surroundingTiles.at(i)->isRevealed){
            continue;
        }
        if(tile->surroundingTiles.at(i)->leftClick() && !tile->surroundingTiles.at(i)->isFlagged){
            tilesRevealed++;
        }
        if(tile->surroundingTiles.at(i)->surroundingBombs == 0){
            revealNearby(tile->surroundingTiles.at(i));
        }
    }
}

void Game::loadBoard(const string& input) {
    face.setTexture(gameTextures["smile"]);
    for(int i = 0; i < dimW; i++) {
        for(int j = 0; j < dimH; j++) {
            delete tiles[i][j];
        }
    }
    bombsTotal = 0;
    totalTiles = dimW*dimH;
    gameRunning = true;
    gameWon = false;
    debugMode = true;
    tilesRevealed = 0;
    ifstream boardFile(input);
    string currentLine;
    for(int i = 0; i < dimH; i++) {
        getline(boardFile,currentLine);
        for(int j = 0; j < dimW; j++) {
            tiles[j][i] = new Tile(gameTextures,j,i);
            if(currentLine.at(j) == '1') {
                tiles[j][i]->setStats(true,0,gameTextures);
                bombsTotal++;

            } else {
                tiles[j][i]->setStats(false,0,gameTextures);
            }
        }
    }
    flags = bombsTotal;
    GetNeighbors();
    updateBombCounter();
}

void Game::newBoard() {
    face.setTexture(gameTextures["smile"]);
    for(int i = 0; i < dimW; i++) {
        for(int j = 0; j < dimH; j++) {
            delete tiles[i][j];
        }
    }
    flags = mines;
    bombsTotal = 0;
    totalTiles = dimW*dimH;
    gameRunning = true;
    gameWon = false;
    debugMode = true;
    tilesRevealed = 0;
    vector<vector<bool>> randSpots;
    randSpots.resize(dimW, vector<bool>(dimH,false));
    int bombsNeeded = flags;
    while(bombsNeeded > 0) {
        short i = rand()%dimW;
        short j = rand()%dimH;
        if(randSpots[i][j]){
            continue;
        } else {
            randSpots[i][j] = true;
            bombsNeeded--;
        }
    }
    for(int i = 0; i < dimW; i++) {
        for(int j = 0; j < dimH; j++) {
            tiles[i][j] = new Tile(gameTextures,i,j);
            if(randSpots[i][j]) {
                tiles[i][j]->setStats(true,0,gameTextures);
                bombsTotal++;

            } else {
                tiles[i][j]->setStats(false,0,gameTextures);
            }
        }
    }
    GetNeighbors();
    updateBombCounter();
}

void Game::updateBombCounter() {
    if(flags < 0){
        negative.setColor(sf::Color(255,255,255,255));
    } else {
        negative.setColor(sf::Color(255,255,255,0));
    }
    hundreds.setTexture(gameTextures["digits"]);
    hundreds.setTextureRect(sf::Rect<int>(abs(21*(flags/100)), 0, 21, 32));
    tens.setTexture(gameTextures["digits"]);
    tens.setTextureRect(sf::Rect<int>(abs(21*(flags/10%10)), 0, 21, 32));
    ones.setTexture(gameTextures["digits"]);
    ones.setTextureRect(sf::Rect<int>(abs(21*(flags%10)), 0, 21, 32));

}


#endif //PROJECT3_1_GAME_H
