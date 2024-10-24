#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "Board.h"

using namespace std;

struct Card
{
    string color;
    bool _is_double;
};


class Player
{
    private:
        const static int _MAX_CANDY_AMOUNT;
        int _stamina;
        double _gold;
        string _effect, _name;
        Candy _inventory[9];
        int _candy_amount;
        bool _robbers_repel;
        bool _must_rest;
    public: 
        Player();
        Player(int stamina, double gold, string effect, Candy candy_array[], const int CANDY_ARR_SIZE);
        int getCandyAmount();
        void setStamina(int stamina);
        int getStamina();
        void setGold(double gold);
        double getGold();
        void setEffect(string effect);
        string getEffect();
        void printInventory();
        Candy findCandy(string candy_name);
        string findCandyEffect(string candy_name);
        bool addCandy(Candy candy);
        bool removeCandy(string candy_name);
        void setName(string name);
        string getName();
        bool robbersRepel();
        void setRobbersRepel(bool robbers_repel);
        void setMustRest(bool must_rest);
        bool getMustRest();
        string findCandyType(string candy_type);
        void setCandyAmount(int);
        string printInventoryToFile(int row);

        
};

#endif