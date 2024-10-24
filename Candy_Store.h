#ifndef CANDY_STORE_H
#define CANDY_STORE_H
#include "Board.h"
#include "Player.h"


class CandyStore
{
private:
    const static int _MAX_CANDY_INVENTORY = 3;
    Candy _candy_store_inventory[_MAX_CANDY_INVENTORY];
public:
    //bool sellCandy(Player player_list[], int player);
    void displayStore();
    //CandyStore(vector<Candy> candies);
    bool sellCandy(Player player_list[], int player);
    CandyStore();
    void loadInventoryCandyStore(vector<Candy> candies);

};
#endif 