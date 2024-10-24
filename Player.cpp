#include "Player.h"
#include "Board.h"
#include <fstream>

Player::Player()
{
    _stamina = 0;
    _gold = 0.0;
    _candy_amount = 0;
    _name = "";
    _robbers_repel = false;
    _must_rest = false;
}

Player::Player(int stamina, double gold, string effect, Candy candy_array[], const int CANDY_ARR_SIZE)
{
    _stamina = stamina;
    _gold = gold;
    _effect = effect;
    int index=0;
    for (int i=0; i<CANDY_ARR_SIZE && index < 9; i++)
    {
        if(candy_array[i].name !=""){
            _inventory[index] = candy_array[i];
            index++;
        }
    }
    _candy_amount = index;
    _must_rest = false;
} 
int Player::getCandyAmount()
{
    return _candy_amount;
}
void Player::setStamina(int stamina)
{
    _stamina = stamina;
}
string Player::getName()
{
    return _name;
}
void Player::setName(string name)
{
    _name = name;
}
int Player::getStamina()
{
    return _stamina;
}
void Player::setGold(double gold)
{
    _gold = gold;
}
double Player::getGold()
{
    return _gold;
}
void Player::setEffect(string effect)
{
    _effect = effect;
}
string Player::getEffect()
{
    return _effect;
}
void Player::printInventory()
{
    string names[9];
    for(int i=0; i<9; i++)
    {
        if(i<_candy_amount){
            names[i] = _inventory[i].name;
        }
        else
        {
            names[i] = "Empty";
        }
    }
    for(int i=0; i<=6; i+=3)
    {
        cout << "|[" << names[i] << "]|[" << names[i+1] << "]|[" << names[i+2] << "]|\n";
    }
}

string Player::printInventoryToFile(int i)
{
    string output;
    string names[9];
    for(int i=0; i<9; i++)
    {
        if(i<_candy_amount){
            names[i] = _inventory[i].name;
        }
        else
        {
            names[i] = "Empty";
        }
    }
    
    output = "|[";
    output.append(names[i]);
    output.append("]|[");
    output.append(names[i+1]);
    output.append("]|[");
    output.append(names[i+2]);
    output.append("]|");
    return output;
     
}

Candy Player::findCandy(string candy_name)
{
    string temp_candy;
    Candy temp_arr[9];
    int candy_name_length = candy_name.length(), candy_inventory_length;
    for(int i=0; i<9; i++)
    {
        temp_arr[i] = _inventory[i];
    }
    for(int i=0; i<candy_name_length; i++)
    {
        candy_name[i] = tolower(candy_name[i]);
    }
    for(int i=0; i<9; i++)
    {
        candy_inventory_length = temp_arr[i].name.length();
        for(int j=0; j<candy_inventory_length; j++)
        {
            temp_arr[i].name[j] = tolower(temp_arr[i].name[j]);
        }
        if(temp_arr[i].name == candy_name){
            return _inventory[i];
        }
    }
    Candy c1;
    return c1;
}

string Player::findCandyEffect(string candy_effect)
{
    string temp_candy;
    Candy temp_arr[9];
    int candy_effect_length = candy_effect.length(), candy_inventory_length;
    for(int i=0; i<9; i++)
    {
        temp_arr[i] = _inventory[i];
    }
    for(int i=0; i<candy_effect_length; i++)
    {
        candy_effect[i] = tolower(candy_effect[i]);
    }
    for(int i=0; i<9; i++)
    {
        candy_inventory_length = temp_arr[i].name.length();
        for(int j=0; j<candy_inventory_length; j++)
        {
            temp_arr[i].effect_type[j] = tolower(temp_arr[i].name[j]);
        }
        if(temp_arr[i].effect_type == candy_effect){
            return _inventory[i].name;
        }
    }
    string candy_name = "";
    return candy_name;
}

string Player::findCandyType(string candy_effect)
{
    string temp_candy;
    Candy temp_arr[9];
    int candy_effect_length = candy_effect.length(), candy_inventory_length;
    for(int i=0; i<9; i++)
    {
        temp_arr[i] = _inventory[i];
    }
    for(int i=0; i<candy_effect_length; i++)
    {
        candy_effect[i] = tolower(candy_effect[i]);
    }
    for(int i=0; i<9; i++)
    {
        candy_inventory_length = temp_arr[i].name.length();
        for(int j=0; j<candy_inventory_length; j++)
        {
            temp_arr[i].candy_type[j] = tolower(temp_arr[i].name[j]);
        }
        if(temp_arr[i].candy_type == candy_effect){
            return _inventory[i].name;
        }
    }
    string candy_name = "";
    return candy_name;
}

bool Player::addCandy(Candy candy)
{
    if(_candy_amount >= 9)
    {
        return false;
    }
    else
    {
        _inventory[_candy_amount] = candy;
        _candy_amount++;
        return true;
    }
}
bool Player::removeCandy(string candy_name)
{
    if(_candy_amount <= 0){
        return false;
    }
    string temp_candy;
    Candy temp_arr[9];
    int candy_name_length = candy_name.length(), candy_inventory_length;
    for(int i=0; i<9; i++)
    {
        temp_arr[i] = _inventory[i];
    }
    for(int i=0; i<candy_name_length; i++)
    {
        candy_name[i] = tolower(candy_name[i]);
    }
    for(int i=0; i<_candy_amount; i++)
    {
        candy_inventory_length = temp_arr[i].name.length();
        for(int j=0; j<candy_inventory_length; j++)
        {
            temp_arr[i].name[j] = tolower(temp_arr[i].name[j]);
        }
        if(temp_arr[i].name == candy_name){
            for(int j=i+1; j<_candy_amount && j-1<10; j++){
                _inventory[j-1] = _inventory[j];
            }
            _candy_amount--;
            return true;
        }
    }
    return false;
}

bool Player::robbersRepel()
{
    if(_robbers_repel)
    {
        return true;
    }
    return false;
}

void Player::setRobbersRepel(bool robbers_repel)
{
    _robbers_repel = robbers_repel;
}

void Player::setMustRest(bool must_rest)
{
    _must_rest = must_rest;
}

bool Player::getMustRest()
{
    return _must_rest;
}

void Player::setCandyAmount(int candy_amount)
{
    _candy_amount = candy_amount;
}