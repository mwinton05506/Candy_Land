#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#define RED "\033[;41m"     /* Red */
#define GREEN "\033[;42m"   /* Green */
#define BLUE "\033[;44m"    /* Blue */
#define MAGENTA "\033[;45m" /* Magenta */
#define CYAN "\033[;46m"    /* Cyan */
#define ORANGE "\033[48;2;230;115;0m"  /* Orange (230,115,0)*/
#define RESET "\033[0m"

using namespace std;

struct Candy
{
    string name;
    string description;
    string effect_type;
    int effect_vaule;
    double price;
    string candy_type;
};

struct Riddle
{
    string riddle;
    string answer;
};


struct Tile
{
    string color;
    string tile_type;
};
 

class Board
{
private:
    const static int _BOARD_SIZE = 83;
    Tile _tiles[_BOARD_SIZE];
    const static int _MAX_CANDY_STORE = 3;
    int _candy_store_position[_MAX_CANDY_STORE];
    int _candy_store_count;
    int _player_position1;
    int _player_position2;

public:
    Board();

    void resetBoard();
    void displayTile(int);
    void displayBoard();

    bool setPlayerPosition1(int);
    bool setPlayerPosition2(int);

    int getBoardSize() const;
    int getCandyStoreCount() const;
    int getPlayerPosition1() const;
    int getPlayerPosition2() const;

    bool addCandyStore(int);
    bool isPositionCandyStore(int); 

    bool movePlayer1(int tile_to_move_forward1);
    bool movePlayer2(int tile_to_move_forward2);
};

#endif