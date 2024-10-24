#include "Board.h"
#include "Candy_Store.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <limits>

using namespace std;

void displayCharacters(vector<Player> player_list);
vector<Candy> loadCandy(string file_name, vector<Candy> candies);
Candy findVectorCandy(string candy_name, vector<Candy> candies);
int split(string input_string, char seperator, string arr[], int arr_size);
vector<Player> loadCharacters(string file_name, vector<Player> character_list, vector<Candy> candies);
int drawCard(int player_position, Player player_list[], int player, int &turn_counter, vector<Riddle> riddles);
void sameTile(Player player_list[], int turn_counter, Board board);
void useCandy(string candy, Player player_list[], int player, vector<Candy> candies, int &gummy, vector<Riddle> riddles);
vector<Riddle> loadRiddles(string file_name);
bool gameRunning(Board board);
void hiddenTreasure(Player players_list[], int player, vector<Riddle> riddles);
bool playRockPaperScissors();
void calamities(Player player_list[], int player, int &gummy, bool &p1_candy_trap, bool &p2_candy_trap);

int main()
{ 
    Board board;
    vector<Candy> candies;
    vector<Player> character_list;
    vector<Riddle> riddles;
    vector<CandyStore> candy_stores_vec;
    string number_players = "2", player_name[2], character_selection, option, card_draw_select, candy_to_use, like_to_display_stats, candy_store, enter_candy_store;
    bool character_match, display_stats[2], p1_candy_trap = false, p2_candy_trap = false;
    int player1_position, player2_position, num_special_tiles = 12, turn_counter = 0, num_hidden_treasure = 3, gummy = 0, int_option, rand_calamity, candy_store_counter = 0;
    int special_tiles_pos[num_special_tiles] = {5, 9, 17, 24, 30, 38, 45, 53, 60, 69, 74, 80};
    int hidden_treasure_pos[num_hidden_treasure];
    int candy_store_pos[3];
    
    for (int i = 0; i < 3; i++)
    {
        srand(time(0));
        hidden_treasure_pos[i] = rand()%83;
    }
    srand(time(0));
    candy_store_pos[0] = (rand() % 9 + 1) * 3;
    //cout << "Candy Store 1 located at: " << candy_store_pos[0] << endl;
    candy_store_pos[1] = (rand() % 9 + 1) * 3 + 25;
    //cout << "Candy Store 2 located at: " << candy_store_pos[1] << endl;
    candy_store_pos[2] = (rand() % 9 + 1) * 3 + 53;
    //cout << "Candy Store 2 located at: " << candy_store_pos[2] << endl;

    candies = loadCandy("candies.txt", candies);
    character_list = loadCharacters("characters.txt", character_list, candies);
    riddles = loadRiddles("riddles.txt");

    for(int i = 0; i < 15; i++)
    {
        CandyStore c_store;
        c_store.loadInventoryCandyStore(candies);
        candy_stores_vec.push_back(c_store);
        
    }

    cout << "Welcome to candy land. Please enter the number of players:\n";
    // Here is code that can be used to make multiplayer, however this would require slight changes to the drawCard and a few other functions
    // getline(cin, number_players);
    // while (number_players != "2"){
    //     cout << "At this time we only support a game with two players. Please enter the number 2.\n";
    //     getline(cin, number_players);
    // }
    number_players = "2";
    const int party_size = stoi(number_players);
    Player player_list[party_size];
    for (int i = 0; i < 2; i++)
    {
        // Creates the players and also prevents them from selecting the same character
        cout << "Enter Player " << i+1 << " Name:\n";
        getline(cin, player_name[i]);
        cout << endl << "Welcome aboard " << player_name[i] << " and best of luck to you! Select a player from the list below. Simply type the name of the player you wish to play as.\n";
        displayCharacters(character_list);
        getline(cin, character_selection);
        character_match = false;
        while (!character_match)
        {
            for (int j = 0; j < character_list.size(); j++)
            {
                if(character_list[j].getName() == character_selection && (i == 0 || player_list[0].getName() != character_selection))
                {
                    cout << "Exellent choice " << character_selection << " will serve you well\n";
                    player_list[i] = character_list[j];
                    character_match = true;
                    break;
                }
            }
            if(!character_match)
            {
                cout << "Did not find that charater in the list provided or you selected the same character as Player 1. Please enter a new name for a charater.\n";
                getline(cin, character_selection);
            }
        }
        // Gives the option to choose whether or not the player wants the stats to be displayed at the beging of every turn
        cout << "Would you like to display your current stats before every turn? Enter Y for yes or anything else for no.\n";
        getline(cin, like_to_display_stats);
        if(like_to_display_stats == "Y")
        {
            display_stats[i] = true;
        }
        else
        {
            display_stats[i] = false;
        }
        cout << "Would you like to vist the Candy Store now? (Y for yes anything else for no)\n";
        enter_candy_store = "";
        getline(cin, enter_candy_store);
        if(enter_candy_store == "Y")
        {
            candy_stores_vec.at(candy_store_counter).displayStore();
            candy_stores_vec.at(candy_store_counter).sellCandy(player_list, i);
            candy_store_counter++;
        }
    }
    cout << "Let the games begin!!\n";
    // Ran into weird issues if i didnt set the players postion to 0 at the start of every game
    board.setPlayerPosition1(0);
    board.setPlayerPosition2(0);
    board.displayBoard();
    while(gameRunning(board))
    {
        // The while loop below controls players turns and exicutes the logic needed for each players turn
        // Turns are how the game progresses and by modding by 2 (would need to be changed based on the number of players)
        // It is very easy to not only skip peoples turns simply by adding two to the turn count but also make sure the game progresses well
        
        // This is the same for the other while loop

        while (turn_counter % 2 == 0)
        {
            if(p1_candy_trap)
            {
                cout << "It's " << player_name[0] << "'s turn. but " << player_name[0] << "is still stuck in taffy!\n";
                cout << player_name[0] << "'s turn is skipped\n";
                turn_counter++;
                p1_candy_trap = false;
                break;
            }
            cout << "It's " << player_name[0] << "'s turn\n";
            if(display_stats[0])
            {
                cout << "Here are your current stats Player 1:\n";
                cout << "Gold: " <<player_list[0].getGold() << endl;
                cout << "Stamina: " <<player_list[0].getStamina() << endl;
                cout << "Candies:\n";
                player_list[0].printInventory();
            }
            cout << "Please select a menu option (1, 2, or 3):\n1. Draw a card\n2. Use Candy\n3. Show player stats\n";
            getline(cin, option);
            while(option != "1" && option != "2" && option != "3")
            {
                cout << "Sorry that was not a valid option. Please enter again:\n";
                getline(cin, option);
            }
            int_option = stoi(option);
            switch(int_option)
            {
                case(1):
                    cout << "Press D to draw a card:\n";
                    getline(cin, card_draw_select);
                    if(card_draw_select == "D")
                    {
                        // The draw card function handles most of the game play logic, drawing a card, landing on special tiles, detecting hidden treasure etc.
                        player1_position = board.getPlayerPosition1();
                        board.setPlayerPosition1(drawCard(player1_position, player_list, 0, turn_counter, riddles) + player1_position);
                        if(board.getPlayerPosition1() == board.getPlayerPosition2())
                        {
                            sameTile(player_list, turn_counter, board);
                        }
                        if(gummy > 0)
                        {
                            turn_counter++;
                            gummy--;
                        }
                        srand(time(0));
                        rand_calamity = rand() % 10;
                        if(rand_calamity < 2)
                        {
                            cout << player_name[0] <<" hit a calamity!\n";
                            calamities(player_list, 0, gummy, p1_candy_trap, p2_candy_trap);
                        }
                        if(player1_position == candy_store_pos[0] || player1_position == candy_store_pos[1] || player1_position == candy_store_pos[2])
                        {
                            cout << "You hit a candy store tile!\n";
                            cout << "Would you like to enter the candy store? (Y for yes anything else for no)\n";
                            cout << "Would you like to vist the Candy Store now? (Y for yes anything else for no)\n";
                            getline(cin, enter_candy_store);
                            if(enter_candy_store == "Y")
                            {
                                candy_stores_vec.at(candy_store_counter).displayStore();
                                candy_stores_vec.at(candy_store_counter).sellCandy(player_list, 0);
                                candy_store_counter++;
                            }
                        }
                        board.displayBoard();
                        break;
                    }
                    else
                    {
                        break;
                    }
                case(2):
                    cout << "Here is a list of your candies\n";
                    player_list[0].printInventory();
                    cout << "Enter the name of the candy that you would like to use:\n";
                    getline(cin, candy_to_use);
                    useCandy(candy_to_use, player_list, 0, candies, gummy, riddles);
                    player_list[0].printInventory();
                    break;
                case(3):
                    cout << "Here are your current stats:\n";
                    cout << "Gold: " <<player_list[0].getGold() << endl;
                    cout << "Stamina: " <<player_list[0].getStamina() << endl;
                    cout << "Candies:\n";
                    player_list[0].printInventory();
                    break;
            }
             
            if(board.getPlayerPosition1() >= 82)
            {
                cout << "--------------------------------------------\n";
                cout << "----------PLAYER 1 WON THE GAME!!!----------\n";
                cout << "--------------------------------------------\n";
                cout << "Here are your ending stats:\n";
                cout << "Gold: " <<player_list[0].getGold() << endl;
                cout << "Stamina: " <<player_list[0].getStamina() << endl;
                cout << "Candies:\n";
                player_list[0].printInventory();
                ofstream output_stats_file("stats.txt");
                if(output_stats_file.is_open())
                {
                    for(int i = 0; i < 2; i++)
                    {
                        output_stats_file << "Player Name: " << player_name[i] << endl;
                        output_stats_file << "Character Name: " << player_list[i].getName() << endl;
                        output_stats_file << "Stamina: " << player_list[i].getStamina() << endl;
                        output_stats_file << "Gold: " << player_list[i].getGold() << endl;
                        output_stats_file << "Candies: " << endl;
                        output_stats_file << player_list[i].printInventoryToFile(0) << player_list[i].printInventoryToFile(3) << player_list[i].printInventoryToFile(6) << endl;

                    }
                }
                return 0;
            }
        }
        // See above
        while (turn_counter % 2 == 1)
        {
            if(p2_candy_trap)
            {
                cout << "It's " << player_name[1] << "'s turn. but " << player_name[1] << "is still stuck in taffy!\n";
                cout << player_name[1] << "'s turn is skipped\n";
                turn_counter++;
                p2_candy_trap = false;
                break;
            }
            cout << "It's " << player_name[1] << "'s turn\n";
            if(display_stats[1])
            {
                cout << "Here are your current stats Player 2:\n";
                cout << "Gold: " <<player_list[1].getGold() << endl;
                cout << "Stamina: " <<player_list[1].getStamina() << endl;
                cout << "Candies:\n";
                player_list[1].printInventory();
            }
            cout << "Please select a menu option (1, 2, or 3)::\n1. Draw a card\n2. Use Candy\n3. Show player stats\n";
            getline(cin, option);
            while(option != "1" && option != "2" && option != "3")
            {
                cout << "Sorry that was not a valid option. Please enter again:\n";
                getline(cin, option);
            }
            int_option = stoi(option);
            switch(int_option)
            {
                case(1):
                    cout << "Press D to draw a card:\n";
                    getline(cin, card_draw_select);
                    if(card_draw_select == "D")
                    {
                        player2_position = board.getPlayerPosition2();
                        board.setPlayerPosition2(drawCard(player2_position, player_list, 1, turn_counter, riddles) + player2_position);
                        if(board.getPlayerPosition1() == board.getPlayerPosition2())
                        {
                            sameTile(player_list, turn_counter, board);
                        }
                        if(gummy > 0)
                        {
                            turn_counter++;
                            gummy--;
                        }
                        
                        srand(time(0));
                        rand_calamity = rand() % 10;
                        if(rand_calamity < 2)
                        {
                            cout << player_name[1] <<" hit a calamity!\n";
                            calamities(player_list, 1, gummy, p1_candy_trap, p2_candy_trap);
                        }
                        if(player2_position == candy_store_pos[0] || player2_position == candy_store_pos[1] || player2_position == candy_store_pos[2])
                        {
                            cout << "You hit a candy store tile!\n";
                            cout << "Would you like to enter the candy store? (Y for yes anything else for no)\n";
                            cout << "Would you like to vist the Candy Store now? (Y for yes anything else for no)\n";
                            getline(cin, enter_candy_store);
                            if(enter_candy_store == "Y")
                            {
                                candy_stores_vec.at(candy_store_counter).displayStore();
                                candy_stores_vec.at(candy_store_counter).sellCandy(player_list, 1);
                                candy_store_counter++;
                            }
                        }
                        board.displayBoard();
                        break;
                    }
                    else
                    {
                        break;
                    }
                case(2):
                    cout << "Here is a list of your candies\n";
                    player_list[1].printInventory();
                    cout << "Enter the name of the candy that you would like to use:\n";
                    getline(cin, candy_to_use);
                    useCandy(candy_to_use, player_list, 1, candies, gummy, riddles);
                    break;
                case(3):
                    cout << "Here are your current stats:\n";
                    cout << "Gold: " <<player_list[1].getGold() << endl;
                    cout << "Stamina: " <<player_list[1].getStamina() << endl;
                    cout << "Candies:\n";
                    player_list[1].printInventory();
                    break;
            }
            if(board.getPlayerPosition2() >= 82)
            {
                cout << "--------------------------------------------\n";
                cout << "----------PLAYER 2 WON THE GAME!!!----------\n";
                cout << "--------------------------------------------\n";
                cout << "Here are your ending stats:\n";
                cout << "Gold: " <<player_list[0].getGold() << endl;
                cout << "Stamina: " <<player_list[0].getStamina() << endl;
                cout << "Candies:\n";
                player_list[0].printInventory();
                ofstream output_stats_file("stats.txt");
                if(output_stats_file.is_open())
                {
                    for(int i = 0; i < 2; i++)
                    {
                        output_stats_file << "Player Name: " << player_name[i] << endl;
                        output_stats_file << "Character Name: " << player_list[i].getName() << endl;
                        output_stats_file << "Stamina: " << player_list[i].getStamina() << endl;
                        output_stats_file << "Gold: " << player_list[i].getGold() << endl;
                        output_stats_file << "Candies: " << endl;
                        output_stats_file << player_list[i].printInventoryToFile(0) << player_list[i].printInventoryToFile(3) << player_list[i].printInventoryToFile(6) << endl;

                    }
                }
                return 0;
            }
            
        }
    }
    return 0;
}

bool gameRunning(Board board)
{
    // This controls the main while loop that runs all turns and makes sure that the game ends when it should
    if(board.getPlayerPosition1() >= 83 || board.getPlayerPosition2() >= 83)
    {
        return false;
    }
    return true;
}

void displayCharacters(vector<Player> player_list)
{
    for(int i=0; i < player_list.size(); i++)
    {
        cout << "Name: " << player_list[i].getName() << endl;
        cout << "Staminia: " << player_list[i].getStamina() << endl;
        cout << "Gold: " << player_list[i].getGold() << endl;
        cout << "Candies: " << endl;
        player_list[i].printInventory();
        cout << "--------------------------------------------------" << endl;
    }
}

Candy findVectorCandy(string candy_name, vector<Candy> candies)
{
    // Similar logic to the findCandy function from homework 7 but uses a vector instead of an array since it made things easier
    int candies_vector_size = candies.size(), candy_name_length = candy_name.length(), candy_name_vecotr_length;
    string temp_candy;
    Candy c1;
    for(int i=0; i<candy_name_length; i++)
    {
        candy_name[i] = tolower(candy_name[i]);
    }
    for (int i=0; i<candies_vector_size; i++)
    {
        candy_name_vecotr_length = candies[i].name.length();
        temp_candy = candies[i].name;
        for(int j=0; j<candy_name_vecotr_length; j++)
        {
            temp_candy[j] = tolower(temp_candy[j]);
        }
        if (temp_candy == candy_name){
            c1 = candies[i];
            return c1;
        }
    }
    return c1;
}
vector<Candy> loadCandy(string file_name, vector<Candy> candies)
{
    ifstream file(file_name);
    string line, candy_arr[6];
    if(file.is_open())
    {
        while(getline(file, line)){
            split(line, '|', candy_arr, 6);
            Candy c;
           
            c.name = candy_arr[0];
            c.description = candy_arr[1];
            c.effect_type = candy_arr[2];
            c.effect_vaule = stoi(candy_arr[3]);
            c.candy_type = candy_arr[4];
            c.price = stod(candy_arr[5]);
            candies.push_back(c);
        }
    }
    else
    {
        cout << "Failed to open file\n";
    }
    return candies;
}

vector<Player> loadCharacters(string file_name, vector<Player> players_list, vector<Candy> candies)
{
    // Creates a vector of Player objects this made acessing spesifc elements of each player object easier
    int i;
    double temp_gold;
    ifstream file(file_name);
    string line, player_arr[4], candy_names[9];
    if(file.is_open())
    {
        while(getline(file, line)){
            for (int i = 0; i < 4; i++)
            {
                player_arr[i] = "";
            }
            for (int i = 0; i < 9; i++)
            {
                candy_names[i] = "";
            }
            split(line, '|', player_arr, 4);
            // Here a player object is created and all the atrubites from the charaters.txt file is assigned to the player object before its added to the array
            Player p;
            p.setName(player_arr[0]);
            p.setStamina(stoi(player_arr[1]));
            temp_gold = stod(player_arr[2]);
            p.setGold(temp_gold);
            split(player_arr[3], ',', candy_names, 9);
            i = 0;
            while(i < 9)
            {
                if(candy_names[i] == "")
                {
                    break;
                }
                p.addCandy(findVectorCandy(candy_names[i], candies));
                i++;
            }
            p.setCandyAmount(5);
            players_list.push_back(p);
        }
    }
    else
    {
        cout << "Failed to open file\n";
    }
    return players_list;

}

vector<Riddle> loadRiddles(string file_name)
{
    string line, riddles_arr[2];
    vector<Riddle> riddles;
    ifstream file(file_name);
    if(file.is_open())
    {
        while(getline(file, line))
        {
            split(line, '|', riddles_arr, 2);
            // Similar to load players creates and vector of Riddle objects
            Riddle r;
            r.riddle = riddles_arr[0];
            r.answer = riddles_arr[1];
            riddles.push_back(r);
        }
    }
    else
    {
        cout << "Failed to open file\n";
    }
    return riddles;
}

bool answerRiddle(vector<Riddle> riddles)
{
    int rand_riddle, riddle_mod = (riddles.size() + 1);
    string player_answer;
    srand(time(0));
    rand_riddle = rand() % riddle_mod;
    cout << "You have to answer a riddle. You only get 1 try so make sure to type it correctly!\nHere is you ridddle:\n";
    cout << riddles[rand_riddle].riddle << endl;
    getline(cin, player_answer);
    if(player_answer == riddles[rand_riddle].answer)
    {
        cout << "You got the riddle right!\n";
        return true;
    }
    else
    {
        cout << "You failed the riddle!\n";
        return false;
    }
}

int split(string input_string, char seperator, string arr[], int arr_size)
{
    if(input_string.length() == 0)
    {
        return 0;
    }
    string temp_char;
    string temp_string;
    int fail_safe = 0, index = 0, input_length = input_string.length();
    for(int i = 0; i < input_length; i++)
    {
        if(input_string[i] != seperator)
        {
            temp_char = input_string[i];
            temp_string.append(temp_char);
            temp_char.clear();
        }
        else
        {
            arr[index] = temp_string;
            index++;
            fail_safe++;
            temp_string.clear();
        }
        if(index >= arr_size)
        {
            return -1;
        }
    }
    arr[index] = temp_string;
    return index + 1;
}

int drawCard(int player_position, Player player_list[], int player, int &turn_counter, vector<Riddle> riddles)
{
    /*
    This is where most of the game logic lies with a card being draw and then player is moved by the amount returned. 
    This is also how the win condtions are met since it has acess to the players postions and the moves
    */
    int draw, move, _double, mod_player_position, total_move = 0, special_tiles, special_rand;
    bool is_special = false, is_double = false;
    srand(time(0));
    _double = rand() % 100;
    srand(time(0));
    draw = rand()%3;
    mod_player_position = player_position % 3;
    // Since a player will need to rest if the stamina is too low this checks if the players stamina is too low and if it is
    // It changes the bool _must_rest to true meaning the player cant move.
    if(player_list[player].getStamina() <= 0)
    {
        cout << "Sorry you do not have enough stamina to contiue. Please rest and wait until you stamina has reach 20!\n";
        player_list[player].setMustRest(true);
        turn_counter++;
        return 0;
    }
    if(player_list[player].getMustRest())
    {
        if(player_list[player].getStamina() < 20)
        {
            cout << "Sorry you only have " << player_list[player].getStamina() << " but you need 20.\n";
            turn_counter++;
            // Player gets 5 stamina per turn they sit out AND try to draw a card since with only adding one it seemed too easy for the other 
            // player to get a massive lead and win the game almost every time I tried it
            player_list[player].setStamina(player_list[player].getStamina() + 5);
            return 0;
        }
    }
    player_list[player].setStamina(player_list[player].getStamina() - 1);
    // Decides randomly if the card was a double or not.
    if(_double > 80)
    {
        is_double = true;
        if(draw == 0)
        {
            move = 4;
        }
        else if(draw == 1)
        {
            move = 5;
        }
        else if(draw == 2)
        {
            move = 6;
        }
        total_move = move + player_position;
        if(total_move >= 83)
        {
            return 82 - player_position;
        }
        if(total_move % 3 == 0)
        {
            cout << "You drew a double pink card.\n";
        }
        else if(total_move% 3 == 1)
        {
            cout << "You drew a double green card.\n";
        }
        else if(total_move % 3 == 2)
        {
            cout << "You drew a double blue card.\n";
        }
    }
    // If it was not a double then it draws a card like normal
    else 
    {
        if(draw == 0)
        {
            move = 1;
        }
        else if(draw == 1)
        {
            move = 2;
        }
        else if(draw == 2)
        {
            move = 3;
        }
        total_move = move + player_position;
        if(total_move >= 83)
        {
            return 82 - player_position;
        }
        if(total_move % 3 == 0)
        {
            cout << "You drew a pink card.\n";
        }
        else if(total_move% 3 == 1)
        {
            cout << "You drew a green card.\n";
        }
        else if(total_move % 3 == 2)
        {
            cout << "You drew a blue card.\n";
        }
    }
    special_rand = rand() % 100;
    // Randomly decides whether or not a given tile is a special tile and then randomly detirmes what kind of specail tile it is
    if(special_rand <= 25)
    {
        special_tiles = rand() % 4;
        int gold_lost = (rand() % 6) + 5;
        if(special_tiles == 1)
        {
            move += 4;
            cout << "You landed on a Shortcut Tile! Move 4 extra tiles.\n";
        }
        else if(special_tiles == 2)
        {
            cout << "You landed on  an Ice Cream Stop Tile! Get an extra turn.\n";
            player_list[player].setStamina(player_list[player].getStamina() + 1);
            return move;
        }
        else if (special_tiles == 3)
        {
            move = -4;
            cout << "You landed on the Gumdrop Forest Tile! Move back 4 spaces and lose " << gold_lost << " gold.\n";
            player_list[player].setGold(player_list[player].getGold() - gold_lost);
        }
        else
        {
            move = 0;
            cout << "You landed on a Gingerbread House Tile! Move to your prevoius positon, and lose an immunity card.\n";
            player_list[player].removeCandy(player_list[player].findCandyEffect("immunity"));
        }
    }
    // Since their is above a 0.01% chance of less than 3 treasures per game ussaly there are about 5
    else if(special_rand > 26 && special_rand <= 31)
    {
        cout << "You landed on a hidden treasure!\n";
        hiddenTreasure(player_list, player, riddles);
    }
    turn_counter++;
    return move;
}

void sameTile(Player player_list[], int turn_counter, Board board)
{   
    int turn = turn_counter % 2; // 0 is player 1, 1 is player 2
    int rand_gold_lost; 
    double temp_gold;
    // If players are on the same tile it will find if they have a Robber's Repel Shield
    // If they do it removes the sheild and sends the other player back
    // If they dont it will remove some gold from the second player to land on the space
    if(player_list[turn].robbersRepel())
    {
        player_list[turn].setRobbersRepel(false);
        cout << "Player " << turn + 1 << " had a Robber's Repel. Player " << ((turn + 1) % 2) + 1 << " is moved back one space.\n";
        if(turn == 0)
        {
            board.movePlayer2(-1);
        }
        else
        {
            board.movePlayer1(-1);
        }
    }
    else
    {
        srand(time(0));
        rand_gold_lost = (rand() % 26) + 5;
        if (turn == 0)
        {
            player_list[1].setGold(player_list[1].getGold() + rand_gold_lost);
            player_list[0].setGold(player_list[0].getGold() - rand_gold_lost);
            cout << "Player 2 stole " << rand_gold_lost << " gold from Player 1!\n";
        }
        else
        {
            player_list[0].setGold(player_list[0].getGold() + rand_gold_lost);
            player_list[1].setGold(player_list[1].getGold() - rand_gold_lost);
            cout << "Player 1 stole " << rand_gold_lost << " gold from Player 2!\n";
        }
    }
    return;
}

void hiddenTreasure(Player players_list[], int player, vector<Riddle> riddles)
{
    // First the player must answer a riddle, if they do then the treasure is randomly assigned to the player
    // If they dont solve the riddle then the function simply returns with nothing happening
    if(!answerRiddle(riddles))
    {
        return;
    }  
    int chance, stam_gain, gold_gain;
    srand(time(0));
    chance = rand()%10;
    if(chance > 0 && chance < 3)
    {
        stam_gain = (rand() % 31) + 10;
        if(players_list[player].getStamina() + stam_gain >= 100)
        {
            players_list[player].setStamina(100);
            cout << "You landed on a hidden Stamina Refill! You staminia has been increased to 100.";

        }
        else
        {
            players_list[player].setStamina(players_list[player].getStamina() + stam_gain);
            cout << "You landed on a hidden Stamina Refill! Your stamnia has been increased to " << players_list[player].getGold() + stam_gain << ".\n";
        }
    }
    else if(chance > 3 && chance < 5)
    {
        gold_gain = (rand() % 31) + 10;
        if(players_list[player].getGold() + gold_gain >= 100)
        {
            players_list[player].setGold(100);
            cout << "You landed on a hidden Gold Windfall! You gold has been increased to 100.";

        }
        else
        {
            players_list[player].setGold(players_list[player].getGold() + gold_gain);
            cout << "You landed on a hidden Gold Windfall! Your gold has been increased to " << players_list[player].getGold() + stam_gain << ".\n";
        }
    }
    else if (chance > 4 && chance < 8)
    {
        players_list[player].setRobbersRepel(true);
        cout << "You landed on a hidden Robber's Reppel! You can now use Robber's Repel as a shield if when you land on the same space as the other player.\n";
    }
    else
    {
        // If they get one of the candies there is a 70% chance they get a Treasure Hunter and a 30% chance they get Jelly Bean of Vigor
        // Neither of those candies can be bought from the store
        int treasure_rand;
        srand(time(0));
        treasure_rand = rand() % 10;
        if(treasure_rand > 7)
        {
            Candy t_hunter;
            t_hunter.name = "Treasure Hunter's Truffle";
            t_hunter.description = "Allows you the chance to unlock hidden treasure";
            t_hunter.effect_type = "treasure";
            t_hunter.effect_vaule = 1;
            t_hunter.candy_type = "hidden treasure";
            if(players_list[player].addCandy(t_hunter))
            {
                cout << "You got a Treasure Hunter's Truffle\n";
                return;
            }
            else
            {
                cout << "HELP ME!!!" << endl;
            }
        }
        else
        {
            Candy j_vigor;
            j_vigor.name = "Jellybean of Vigor";
            j_vigor.description = "Offers some amount of stamina regeneration";
            j_vigor.effect_type = "stamina";
            j_vigor.effect_vaule = 50;
            j_vigor.candy_type = "hidden treasure";
            if(players_list[player].addCandy(j_vigor))
            {
                cout << "You got a Jellybean of Vigor!\n";
                return;
            }
            else
            {
                cout << "HELP ME!!!" << endl;
            }
        }
    }
}

void useCandy(string candy, Player player_list[], int player, vector<Candy> candies, int &gummy, vector<Riddle> riddles)
{
    // This allows the user to use a candy that they have in their inventory
    // If they use the candy the candy effects are then shown and the candy is removed from their inventory
    string effect_type;
    if(player_list[player].removeCandy(candy))
    {
        for (int i = 0; i < candies.size(); i++)
        {
            if(candies[i].name == candy)
            {
                if(candies[i].candy_type == "posion")
                {
                    if(candies[i].effect_vaule == -10)
                    {
                        if(player_list[(player + 1) % 2].removeCandy("Bubblegum Blast"))
                        {
                            cout << "Player " << (player + 1) % 2 + 1 << " blocked Player " << player + 1 << " posion candy!";
                            return;
                        }
                        else if(player_list[(player + 1) % 2].removeCandy("Sparkling Sapphire"))
                        {
                            cout << "Player " << (player + 1) % 2 + 1 << " blocked Player " << player + 1 << " posion candy!";
                            return;
                        }
                        else if(player_list[(player + 1) % 2].removeCandy("Caramel Comet"))
                        {
                            cout << "Player " << (player + 1) % 2 + 1 << " blocked Player " << player + 1 << " posion candy!";
                            return;
                        }
                        else if(player_list[(player + 1) % 2].removeCandy("Beets Beat Bears"))
                        {
                            cout << "Player " << (player + 1) % 2 + 1 << " blocked Player " << player + 1 << " posion candy!";
                            return;
                        }
                        else
                        {
                            player_list[(player + 1) % 2].setStamina(player_list[(player + 1) % 2].getStamina() - 10);
                            cout << "Player " << (player + 1) % 2 + 1 << " was poinsed and lost 10 stamina!\n";
                            return;
                        }
                    }
                    else if(candies[i].effect_vaule == -15)
                    {
                        if(player_list[(player + 1) % 2].removeCandy("Sparkling Sapphire"))
                        {
                            cout << "Player " << (player + 1) % 2 + 1 << " blocked Player " << player + 1 << " posion candy!";
                            return;
                        }
                        else if(player_list[(player + 1) % 2].removeCandy("Caramel Comet"))
                        {
                            cout << "Player " << (player + 1) % 2 + 1 << " blocked Player " << player + 1 << " posion candy!";
                            return;
                        }
                        else if(player_list[(player + 1) % 2].removeCandy("Beets Beat Bears"))
                        {
                            cout << "Player " << (player + 1) % 2 + 1 << " blocked Player " << player + 1 << " posion candy!";
                            return;
                        }
                        else
                        {
                            player_list[(player + 1) % 2].setStamina(player_list[(player + 1) % 2].getStamina() - 15);
                            cout << "Player " << (player + 1) % 2 + 1 << " was poinsed and lost 15 stamina!\n";
                            return;
                        }
                    }
                    else if(candies[i].effect_vaule == -20)
                    {
                        if(player_list[(player + 1) % 2].removeCandy("Caramel Comet"))
                        {
                            cout << "Player " << (player + 1) % 2 + 1 << " blocked Player " << player + 1 << " posion candy!";
                            return;
                        }
                        else if(player_list[(player + 1) % 2].removeCandy("Beets Beat Bears"))
                        {
                            cout << "Player " << (player + 1) % 2 + 1 << " blocked Player " << player + 1 << " posion candy!";
                            return;
                        }
                        else
                        {
                            player_list[(player + 1) % 2].setStamina(player_list[(player + 1) % 2].getStamina() - 20);
                            cout << "Player " << (player + 1) % 2 + 1 << " was poinsed and lost 20 stamina!\n";
                            return;
                        }
                    }
                    
                }
                else if(candies[i].candy_type == "magical")
                {
                    player_list[player].setStamina(player_list[player].getStamina() + candies[i].effect_vaule);
                    cout << "Player " << player + 1 << " used " << candies[i].name << " and gained " << candies[i].effect_vaule << " stamina!\n";

                }
                else if(candies[i].candy_type == "gummy")
                {
                    gummy = abs(candies[i].effect_vaule);
                    cout << "Player " << player + 1 << " used " << candies[i].name << " and Player "<< (player + 1) % 2 + 1 <<" lost " << abs(candies[i].effect_vaule) << " turns!\n"; 
                }
                else if(candies[i].candy_type == "treasure")
                {
                    if(answerRiddle(riddles))
                    {
                        cout << "Unlocked the treasure and gained 50 gold!\n";
                        player_list[player].setGold(player_list[player].getGold() + 50);
                    }
                }
            }
        }
    }
    else
    {
        cout << "That candy wasnt found in your inventory!\n";
        return;
    }
}

void calamities(Player player_list[], int player, int &gummy, bool &p1_candy_trap, bool &p2_candy_trap)
{
    srand(time(0));
    int calamity = rand() % 100, lost_gold = rand() % 10 + 5;
    string choice, use_magic, removed_candy;
    if(calamity < 30)
    {
        player_list[player].setGold(player_list[player].getGold() - lost_gold);
        cout << "Oh no! Candy Bandits have swiped you precoius gold coins. Player " << player + 1 << " lost " << lost_gold << " coins.\n";
    }
    else if(calamity >= 30 && calamity < 65)
    {
        cout << "Oh dear! You got lost in the lollipop labyrinth!\n";
        cout << "Player " << player + 1 << " choose to eiter play Rock Paper Scissors or lose a turn.\nIf you win you get your lost turn back.(Enter Y if you would like play or enter anything if you dont)\n";
        getline(cin, choice);
        // Reused the Rock Papper Scissors function from Homework 7.
        // Since gummy works for candies skipping turns it also works with the calamities
        if(choice == "Y")
        {
            if(playRockPaperScissors())
            {
                return;
            }
            else
            {
                gummy++;
            }
        }
        else
        {
            gummy++;
        }

    }
    else if(calamity >= 65 && calamity < 80)
    {
        int rand_stam = rand() % 6 + 5;

    }
    else
    {
        cout << "Ooops! Your are stuck in  sticky taffy trap!\n";
        cout << "Lose your next turn or sacrafise a magic candy to get unstuck. Enter 'S' if you would like to use magic candy.\n";
        getline(cin, use_magic);
        if(use_magic == "S")
        {
            removed_candy = player_list[player].findCandyType("magic");
            if(player_list[player].removeCandy(removed_candy))
            {
                cout << removed_candy << " was removed from your inventory and you are now free!\n";
                return;
            }
        }
        // This works to fully skip the turn not just prevent them from drawing a card. 
        if(player == 0)
        {
            p1_candy_trap = true;
        }
        else if(player == 1)
        {
            p2_candy_trap = true;
        }
    }
}

bool playRockPaperScissors()
{
    // Just the same function from Homework 7 but modified to allow for the use of the computer to randly select an option.
    int comp_select;
    char p1_select, p2_select;
    bool winner = false;
    while (!winner)
    {
        cout << "Player: Enter r, p, or s\n";
        cin >> p1_select;
        while (cin.fail() || (p1_select != 'r' && p1_select != 'p' && p1_select != 's'))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid selection!\n";
            cin >> p1_select;
        }
        srand(time(0));
        comp_select = rand() % 3;
        switch(comp_select)
        {
            case(0):
                p2_select = 'r';
                cout << "The game choose Rock!\n";
                break;
            case(1):
                p2_select = 's';
                cout << "The game choose Scissors!\n";
                break;
            case(2):
                p2_select = 'p';
                cout << "The game choose Paper!\n";
                break;
        }

        if (p1_select == p2_select)
        {
            cout << "Tie! Play again\n";
        }
        else if ((p1_select == 'r' && p2_select == 'p') ||
                 (p1_select == 's' && p2_select == 'r') ||
                 (p1_select == 'p' && p2_select == 's'))
        {
            cout << "Player has lost!\n";
            return false;
        }
        else if ((p1_select == 'r' && p2_select == 's') ||
                 (p1_select == 's' && p2_select == 'p') ||
                 (p1_select == 'p' && p2_select == 'r'))
        {
            cout << "Player has won!\n";
            return true;
        }
    }
}