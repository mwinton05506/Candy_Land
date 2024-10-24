#include "Candy_Store.h"
#include <vector>
#include <time.h>
#include <cstdlib>

CandyStore::CandyStore()
{
    _candy_store_inventory[3];
}

void CandyStore::loadInventoryCandyStore(vector<Candy> candies)
{
    int num_of_candies = candies.size();
    srand(time(0));
    for(int i = 0; i < 3; i++)
    {
        if(candies[i].name == "Treasure Hunter's Truffle" || candies[i].name == "Jellybean of Vigor")
        {
            i--;
        }
        else
        {
            _candy_store_inventory[i] = candies[(rand() % num_of_candies) + i];
        }
    }
}

// CandyStore::CandyStore(vector<Candy> candies)
// {
//     int num_of_candies = candies.size();
//     srand(time(0));
//     for(int i = 0; i < 3; i++)
//     {
//         if(candies[i].name == "Treasure Hunter's Truffle" || candies[i].name == "Jellybean of Vigor")
//         {
//             i--;
//         }
//         else
//         {
//             _candy_store_inventory[i] = candies[(rand() % num_of_candies) + i];
//         }
//     }
// }

void CandyStore::displayStore()
{
    cout << "Welcome to the Candy Store! Look at out wide varity of candies:\n";
    for(int i = 0; i < 3; i++)
    {
        cout << "--------------------------------------------------" << endl;
        cout << "Name: " << _candy_store_inventory[i].name << endl;
        cout << "Decsription: " << _candy_store_inventory[i].description << endl;
        cout << "Effect: " << _candy_store_inventory[i].effect_type << endl;
        cout << "Effect Value: " << _candy_store_inventory[i].effect_vaule << endl;
        cout << "Candy Type: " << _candy_store_inventory[i].candy_type << endl;
        cout << "Price: " << _candy_store_inventory[i].price<< endl;
    }
}
bool CandyStore::sellCandy(Player player_list[], int player)
{
    string candy_select, candy_substitute;
    bool inventory_full = true;
    cout << "Which candy would you like to buy?\n";
    getline(cin, candy_select);
    for(int i = 0; i < 3; i++)
    {
        if(candy_select == _candy_store_inventory[i].name)
        {
            if(player_list[player].getGold() < _candy_store_inventory[i].price)
            {
                cout << "Sorry you cant afford that candy.\n";
                return false;
            }
            while (player_list[player].getCandyAmount() >= 9 && inventory_full)
            {
                cout << "What candy would you like to subsitute?\n";
                getline(cin, candy_substitute);
                for(int j = 0; j < 9; j++)
                {
                    if(player_list[player].removeCandy(candy_substitute))
                    {
                        inventory_full = false;
                        for (int k = 0; k < 3; k++)
                        {
                            if(_candy_store_inventory[k].name == candy_select)
                            {
                                if(player_list[player].getGold() < _candy_store_inventory[i].price)
                                {
                                    cout << "Sorry you cant afford that candy.\n";
                                    return false;
                                }
                                player_list[player].addCandy(_candy_store_inventory[k]);
                                player_list[player].setGold(player_list[player].getGold() - _candy_store_inventory[k].price);
                                cout << _candy_store_inventory[k].name << " has successfully been addded to your invetory\n";
                                cout << "Your updated candy invetory:\n";
                                player_list[player].printInventory();
                                return true;
                            }
                        }
                    }
                }
            }
            player_list[player].addCandy(_candy_store_inventory[i]);
            cout << _candy_store_inventory[i].name << " has successfully been addded to your invetory\n";
            cout << "Your updated candy invetory:\n";
            player_list[player].printInventory();
            return true;
        }
    }
    return false;
    
}