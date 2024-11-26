#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHRandom.h"
#include "FEHUtility.h"
#include "FEHImages.h"
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <fstream>

using namespace std;

int** place_bot_pieces();
int** place_player_pieces();
int* bot_move(int[6][6]);
int* player_move(int[6][6]);

int game_loop();

int* get_stats();
void put_stats(int[4]);

int menu_touch();
int back_touch(int);
int winner_touch();

int first_player();
int check_winner(int[6][6], int[6][6]);

int random_num(int);
bool position_in_array(int, int, int);

int boat_sizes[] = {4, 3, 2, 1};


class Draw //class that contains all draw functions
{
    public:
        Draw();
        void menu();
        void credits();
        void instructions();
        void statistics();
        void grid();
        void board(int [6][6], int [6][6]);
        void botShip(int,int,int);
        void playerShip(int,int,int);
        void turn(int);
        void winner(int);
    private:
};

int main(){
    Draw draw;
    int choice = 0;

    while(true){ // update loop
        switch(choice){
            case(0):
                draw.menu();
                choice = menu_touch(); // checks for buttons pressed on menu
                break;
            case(1):
                game_loop(); // going to have to change this later
                choice = back_touch(1);
                break;
            case(2):
                draw.statistics();
                choice = back_touch(2); // checks for back button press
                break;
            case(3):
                draw.instructions();
                choice = back_touch(3); // checks for back button press
                break;
            case(4):
                draw.credits();
                choice = back_touch(4); // checks for back button press
                break;
        }
        LCD.Update(); // draws new frame
    }
}

int game_loop(){
    Draw draw;

    int** bot_board_temp;
    int** player_board_temp;
    int bot_board[6][6]; // initalize bot board
    int player_board[6][6]; // initalize player board
    int bot_view[6][6]; // initalize bot view
    int player_view[6][6]; // initalize player view


    player_board_temp = place_player_pieces(); // let the player place pieces
    bot_board_temp = place_bot_pieces(); // place the bot pieces

    for(int i=0; i<6; i++){ // copy the bot + player boards from type int** to 2D arrays.
        for(int j=0; j<6; j++){
            bot_board[i][j] = bot_board_temp[i][j];
            player_board[i][j] = player_board_temp[i][j];
            bot_view[i][j] = 0;
            player_view[i][j] = 0;
        }
    }


    int player = first_player(); // random first player

    int turns[2] = {0, 0}; // keeps track of the number of turns for each person

    while(check_winner(player_view, player_board) == 0){ // run through the game
        draw.board(player_view, player_board);
        if(player == 1){ // bot turn
            turns[1] += 1;
            draw.turn(player);
            Sleep(1500);
            int* bot_m;
            bot_m = bot_move(bot_view); // get bot move

            if(player_board[bot_m[0]][bot_m[1]] == 0){ // place bot move on board
                bot_view[bot_m[0]][bot_m[1]] = -1;
                player_board[bot_m[0]][bot_m[1]] = -5;
            }
            else{
                bot_view[bot_m[0]][bot_m[1]] = 1;
                player_board[bot_m[0]][bot_m[1]] = 5;
            }

            draw.board(player_view, player_board); // update board

            player *= -1; // switch player
        }
        else if(player == -1){ // player turn
            turns[0] += 1;
            draw.turn(player);
            int* player_m;
            player_m = player_move(player_view); // get player move

            if(bot_board[player_m[0]][player_m[1]] == 0){ // place player move on board
                player_view[player_m[0]][player_m[1]] = -5;
            }
            else{
                player_view[player_m[0]][player_m[1]] = 5;
            }

            int temp_player[6][6];

            for(int i=0; i<6; i++){ // change the player board to incude the bots hits/misses
                for(int j=0; j<6; j++){
                    if(bot_view[i][j] != 0){
                        temp_player[i][j] = bot_view[i][j]*5;
                    }
                    else{
                        temp_player[i][j] = player_board[i][j];
                    }
                }
            }

            draw.board(player_view, temp_player); // update board
            player *= -1; // switch players
        }
    }

    int* stats;

    int winner = check_winner(player_view, player_board);

    stats = get_stats();

    if(winner == 2){
        if(turns[0] < stats[0]){
            stats[0] = turns[0];
        }
        if(turns[0] > stats[1]){
            stats[1] = turns[0];
        }
    }
    if(winner == 1){
        if(turns[0] < stats[2]){
            stats[2] = turns[0];
        }
        if(turns[0] > stats[3]){
            stats[3] = turns[0];
        }
    }

    //int t_stats[] = {stats[0], stats[1], stats[2], stats[3]};

    put_stats(stats);

    Sleep(1000);

    draw.winner(winner); // draw winner screen

    while(true){ // wait for press on winner menu
        int choice;
        choice = winner_touch();
        if(choice == 1){
            break;
        }
        else if(choice == 2){
            main();
            return 0;
        }
    }

}

int first_player(){ // choose a random first player
    int r = Random.RandInt();
    if (r > 16000){
        return 1; // player 2 goes first
    }
    return -1; // player 1 goes first
}

int** place_player_pieces(){
    Draw draw;
    //place 4 pieces
    //vertical or horizontal
    //make sure they don't hit each other and don't go out of bounds

    //place size 4
    //place size 3
    //place size 2
    //place size 1

    int current_player_board[6][6]; // blank player board
    int temp_bot_board[6][6]; // blank bot board (for drawing the screen)
    int dir[2];

    for(int i=0; i<6; i++){ // set boards to zeros
        for(int j=0; j<6; j++){
            current_player_board[i][j] = 0;
            temp_bot_board[i][j] = 0;
        }
    }

    draw.board(temp_bot_board, current_player_board);

    int current_ship = 1; // start with ship #1
    int x, y;
    
    
    while(true){
        draw.board(temp_bot_board, current_player_board);
        while(!LCD.Touch(&x, &y)){

        }

        while(LCD.Touch(&x, &y)){ // wait for touch

        }

        bool double_click = false;
        int start = time(NULL);
        while(time(NULL) - start < .75){ // check for double touch
            if(LCD.Touch(&x, &y)){
                double_click = true;
            }
        }

        if(double_click){ // set placement direction depending on if there 
            dir[0] = 0;
            dir[1] = 1;
        }
        else{
            dir[0] = 1;
            dir[1] = 0;
        }

        int board_left = 5;
        int board_right = 155;
        int board_top = 70;
        int board_bottom = 220;

        float cell_size[2] = {((board_right-board_left)/6), ((board_bottom-board_top)/6)}; 
        
        if(x > board_left && x < board_right && y > board_top && y < board_bottom){ //  if click on board
            int pos[2];
            pos[0] = (x - board_left) / cell_size[0]; // get which cell is clicked
            pos[1] = (y - board_top) / cell_size[1];


            bool valid = true;
            for(int i=0; i<boat_sizes[current_ship-1]; i++){ // check if the position is valid
                if(current_player_board[pos[0]+dir[0]*i][pos[1]+dir[1]*i] != 0){
                    valid = false;
                }
            }
            
            if(valid){
                for(int i=0; i<boat_sizes[current_ship-1]; i++){ // is the position is valid, place the ship
                    current_player_board[pos[0]+(dir[0]*i)][pos[1]+(dir[1]*i)] = current_ship;
                }
                current_ship += 1; // move to the next ship
            }

            draw.board(temp_bot_board, current_player_board);
        }

        if(current_ship > 4){ // end once all ships are placed
            break;
        }
    }
    
    int** player_b = 0;

    player_b = new int*[6]; // return player board as type int**
    for(int j=0; j<6; j++){
        player_b[j] = new int[6];
        for(int k=0; k<6; k++){
            player_b[j][k] = current_player_board[j][k];
        }
    }

    return player_b;

}

int** place_bot_pieces(){ // places the bot pieces randomly
    int** bot_b = 0;

    bot_b = new int*[6]; // 2D array of zeros
    for(int j=0; j<6; j++){
        bot_b[j] = new int[6];
        for(int k=0; k<6; k++){
            bot_b[j][k] = 0;
        }
    }

    int i=0;
    int dir;
    int pos[2];
    bool valid = true;
    while(true){
        valid = true;
        dir = random_num(4); // creates a random position and orientation
        pos[0] = random_num(6);
        pos[1] = random_num(6);

        for(int j=0; j<boat_sizes[i]; j++){ // checks of the position is valid
            switch(dir){
                case(0):
                    if(!position_in_array(pos[0], pos[1]+j, 6) || bot_b[pos[0]][pos[1]+j] != 0){
                        valid = false;
                    }
                    break;
                case(1):
                    if(!position_in_array(pos[0], pos[1]-j, 6) || bot_b[pos[0]][pos[1]-j] != 0){
                        valid = false;
                    }
                    break;
                case(2):
                    if(!position_in_array(pos[0]+j, pos[1], 6) || bot_b[pos[0]+j][pos[1]] != 0){
                        valid = false;
                    }
                    break;
                case(3):
                    if(!position_in_array(pos[0]-j, pos[1], 6) || bot_b[pos[0]-j][pos[1]] != 0){
                        valid = false;
                    }
                    break;
            }

            if(valid == false){
                break;
            }
        }

        if(valid == true){ // if valid, places boat in position
            for(int j=0; j<boat_sizes[i]; j++){
                switch(dir){
                    case(0):
                        bot_b[pos[0]][pos[1]+j] = i+1;
                        break;
                    case(1):
                        bot_b[pos[0]][pos[1]-j] = i+1;
                        break;
                    case(2):
                        bot_b[pos[0]+j][pos[1]] = i+1;
                        break;
                    case(3):
                        bot_b[pos[0]-j][pos[1]] = i+1;
                        break;
                }    
            }
            i+=1;
        }

        if(i == 4){ // end once all boats are placed
            return bot_b;
        }
    }
}

int check_winner(int bot_b[6][6], int player_b[6][6]){
    bool player_win = false;
    bool bot_win = true;

    int hits = 0;

    for(int i=0; i<6; i++){
        for(int j=0; j<6; j++){
            if(bot_b[i][j] == 5){ // add up the number of hits the player has
                hits += 1;
            }
            if(player_b[i][j] != 0 && player_b[i][j] != 5 && player_b[i][j] != -5){ // check to see if any boats exist on the player board
                bot_win = false;
            }
        }
    }

    if(hits == 10){ // if the player hits all ship spots
        player_win = true;
    }

    if(player_win && bot_win){
        return 3; // tie
    }
    if(player_win){
        return 2; // player wins
    }
    if(bot_win){
        return 1; // bot wins
    }
    return 0;
}

int* bot_move(int bot_vision[6][6]){
    int prob[6][6];
    int multiplier = 2;

    for(int i=0; i<6; i++){ // 2D array of zeros
        for(int j=0; j<6; j++){
            prob[i][j] = 0;
        }
    }

    for(int i=0; i<4; i++){
        for(int j=0; j<6; j++){
            for(int k=0; k<6; k++){ // for each boat, try each possible orientation and position on the board
                bool valid = true; 
                int ones = 0;
                for(int l=0; l<boat_sizes[i]; l++){ // try the horizontal oprientation
                    if(j+l > 5){ // if boat is out of bounds dont count it
                        valid = false;
                        break;
                    }
                    if(bot_vision[j+l][k] == -1){ // if boat hits a miss spot dont count it
                        valid = false;
                        break;
                    }
                    if(bot_vision[j+l][k] == 1){ // if boat touches a hit spot keep track for later
                        ones += 1;
                    }
                }
                if(valid){
                    for(int l=0; l<boat_sizes[i]; l++){
                        if(bot_vision[j+l][k] == 0){
                            prob[j+l][k] += 1 + (ones*multiplier); // if boat is in a valid spot add 1 to prob matrix. if touching a hit spot add a bonus
                        }
                    }
                }

                valid = true;
                ones = 0;
                for(int l=0; l<boat_sizes[i]; l++){ // try the vertical orientation
                    if(k+l > 5){
                        valid = false;
                        break;
                    }
                    if(bot_vision[j][k+l] == -1){
                        valid = false;
                        break;
                    }
                    if(bot_vision[j][k+l] == 1){
                        ones += 1;
                    }
                }
                if(valid){
                    for(int l=0; l<boat_sizes[i]; l++){
                        if(bot_vision[j][k+l] == 0){
                            prob[j][k+l] += 1 + (ones*multiplier);
                        }
                    }
                }
            }
        }
    }

    int max = 0;
    int* pos;

    pos = new int[2];

    for(int i=0; i<6; i++){ // get the position in prob matrix that has the highest value
        for(int j=0; j<6; j++){
            if(prob[i][j] > max){
                max = prob[i][j];
                pos[0] = i;
                pos[1] = j;
            }
        }
    }

    return pos; // return chosen spot
}

int* player_move(int player_vision[6][6]){
    int board_left = 165;
    int board_right = 315;
    int board_top = 70;
    int board_bottom = 220;
    int x, y;
    int* pos;

    pos = new int[2];

    float cell_size[2] = {((board_right-board_left)/6), ((board_bottom-board_top)/6)}; 

    while(!LCD.Touch(&x, &y)){ // wait for touch
        
    }

    if(x < board_left || x > board_right || y < board_top || y > board_bottom){ 
        return player_move(player_vision); // if touch outside of bounds, call again
    }

    pos[0] = (x - board_left) / cell_size[0]; // get cell pressed
    pos[1] = (y - board_top) / cell_size[1];

    if(player_vision[pos[0]][pos[1]] != 0){
        return player_move(player_vision); // if cell is not empty, call again
    }


    return pos; // return chose position
}

int* get_stats(){
    ifstream file;
    file.open("stats.txt"); // open stats file

    int* stats;
    stats = new int[4]; // [fastest win, slowest win, fastest loss, slowest loss]

    file >> stats[0]; // extract stats from file
    file >> stats[1];
    file >> stats[2];
    file >> stats[3];

    file.close();

    return stats; // return stats
}

void put_stats(int stats[4]){
    ofstream file;
    file.open("stats.txt"); // open file

    file << stats[0] << endl; // write to file
    file << stats[1] << endl;
    file << stats[2] << endl;
    file << stats[3] << endl;

    file.close();
}

bool position_in_array(int posx, int posy, int size){ // checks if a given position in inside an array of size by size
    if(posx < 0 || posx >= size){
        return false;
    }
    if(posy < 0 || posy >= size){
        return false;
    }
    return true;
}

int random_num(int max){ // create a random int from 0 to max (not incluiding max)
    int r = Random.RandInt();
    r /= (32767/max)+1;
    return r;
}

Draw::Draw(){}

//draws BLANK grid
void Draw::grid(){
    //draw two 6x6 grids
    //max x pixels -- 320
    //max y pixels -- 240
    //height and width of a char -- 17 x 12 -- 17y X 12x

    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Battleship", 98, 8);
    int x,x2,y,width,height;
    x = 5;
    y = 70;
    width = 150;
    height = 150;
    LCD.DrawRectangle(x,y,width,height);
    LCD.WriteAt("You", 59.5, y-25); //3 characters
    x2 = 165;
    LCD.DrawRectangle(x2,y,width,height);
    LCD.WriteAt("Computer",x2+75-48,y-25); //8 characters

    //Draw vertical grid lines
    for(int i = 1; i < 6; i++){
        LCD.DrawLine(x + (width/6)*(i), y, x + i*(width/6), y+height);
    }
    for(int i = 1; i < 6; i++){
        LCD.DrawLine(x2 + (width/6)*(i), y, x2 + i*(width/6), y+height);
    }
    //draw horizontal grid lines
    for(int i = 1; i < 6; i++){
        LCD.DrawLine(x, y+height/6*i, x+width, y+height/6*i);
    }
    for(int i = 1; i < 6; i++){
        LCD.DrawLine(x2, y+height/6*i, x2+width, y+height/6*i);
    }

}

//draw circle in corresponding grid slot
void Draw::botShip(int i, int j, int value){
    //variables
    int x,y,width,height,radius;
    x = 165 + 150/12;
    y = 70 + 150/12;
    width = 150/6;
    height = 150/6;
    radius = 10;
    //always red, because that means it's been hit (ONLY SHOWN HIT SHIPS)
    
    if(value == 5){
        LCD.SetFontColor(RED);
    }
    if(value == -5){
        LCD.SetFontColor(BLUE);
    }
    LCD.DrawCircle(x+height*i,y+width*j,radius);
    LCD.FillCircle(x+height*i, y+width*j, radius);
}


//draw circle in corresponding grid slot
void Draw::playerShip(int i, int j, int value){
    //variables
    int x,y,width,height,radius;
    x = 5 + 150/12;
    y = 70 + 150/12;
    width = 150/6;
    height = 150/6;
    radius = 10;
    LCD.SetFontColor(WHITE);
    //IF VALUE IS FIVE, DRAW CIRCLE AS RED
    if(value == 5){
        LCD.SetFontColor(RED);
    }
    if(value == -5){
        LCD.SetFontColor(BLUE);
    }
    LCD.DrawCircle(x+height*i,y+width*j,radius);
    LCD.FillCircle(x+height*i, y+width*j, radius);
}

//draw filled boards
void Draw::board(int botBoard[6][6], int playerBoard[6][6]){
    //max x pixels -- 320
    //max y pixels -- 240
    //white circles to represent ship placement
    //red circles to represent hit ships

    LCD.Clear(BLACK);

    //draw two grids
    grid();

    
    for(int i = 0; i < 6; i++){
        for(int j = 0; j<6; j++){
            if(botBoard[i][j] != 0){
                botShip(i,j, botBoard[i][j]);
            }
        }
    }

        for(int i = 0; i < 6; i++){
        for(int j = 0; j<6; j++){
            if(playerBoard[i][j] != 0){
                playerShip(i,j, playerBoard[i][j]);
            }
        }
    }

    LCD.Update();
}   

int menu_touch(){ // checks to see if the menu buttons are touched
    int x, y;
    if(LCD.Touch(&x, &y)){
        while(LCD.Touch(&x, &y)){
                
        }
        if(x >= 125 && x <= 195 && y >= 45 && y <= 85){
            return 1; // play button pressed
        }
        if(x >= 88 && x <= 232 && y >= 90 && y <= 130){
            return 2; // stats button pressed
        }
        if(x >= 76 && x <= 274 && y >= 135 && y <= 175){
            return 3; // instructions button pressed
        }
        if(x >= 107 && x <= 213 && y >= 180 && y <= 220){
            return 4; // credits button pressed
        }
    }
    
    return 0; // no button touched
}

int back_touch(int current){ // checks if back buttons are pressed
    int x, y;
    if(LCD.Touch(&x, &y)){
        while(LCD.Touch(&x, &y)){
                
        }
        if(x >= 2 && x <= 56 && y >= 2 && y <= 21){
            return 0; // of back buttons are pressed return 0
        }
    }

    return current; // if back buttons not pressed, return current
}

int winner_touch(){
    int x, y;
    if(LCD.Touch(&x, &y)){
        while(LCD.Touch(&x, &y)){ // wait for touch

        }
        if(x >= 88 && x <=232 && y >= 70 && y <= 110){
            return 1; // play again
        }
        if(x >= 76 && x <=244 && y >= 135 && y <= 175){
            return 2; // back to menu
        }
    }

    return 0; // nothing pressed
}

void Draw::menu(){ // draws the menu
    // draw title
    LCD.Clear(BLACK);

    FEHImage background;
    background.Open("background.png");
    background.Draw(0, 0);

    LCD.SetFontColor(WHITE);
    LCD.SetBackgroundColor(WHITE);
    LCD.WriteAt("Battleship", 98, 8);
    
    // draw play button
    LCD.DrawRectangle(125, 45, 70, 40);
    LCD.WriteAt("Play", 135, 58);

    // draw sttats button
    LCD.DrawRectangle(88, 90, 144, 40);
    LCD.WriteAt("Statistics", 98, 103);

    // draw instructio    
    LCD.DrawRectangle(76, 135, 168, 40);
    LCD.WriteAt("Instructions", 86, 148);

    // draw credits button
    LCD.DrawRectangle(107, 180, 106, 40);
    LCD.WriteAt("Credits", 117, 193);
}

void Draw::statistics(){ // draws the statistics page
    LCD.Clear(BLACK);

    int* stats; // get stats
    stats = get_stats();

    FEHImage background;
    background.Open("background.png");
    background.Draw(0, 0);

    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Statistics", 100, 5); // 10 characters
    LCD.WriteAt("Fastest game won:Turn " + to_string(stats[0]), 0, 45); // write stats
    LCD.WriteAt("Slowest game won:Turn " + to_string(stats[1]), 0, 85);
    LCD.WriteAt("Fastest game lost:Turn " + to_string(stats[2]), 0, 125);
    LCD.WriteAt("Slowest game lost:Turn " + to_string(stats[3]), 0, 165);
    
    LCD.SetFontColor(RED);
    LCD.WriteAt("BACK", 4, 5);
    LCD.DrawRectangle(2, 2, 54, 21);
}

void Draw::credits(){ // draws the credits page
    LCD.Clear(BLACK);

    FEHImage background;
    background.Open("background.png");
    background.Draw(0, 0);

    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Credits", 118, 5); // 7 characters
    LCD.WriteAt("Programmers", 94, 25); //11 characters
    LCD.WriteAt("Gavin Mclaughlan", 0, 48);
    LCD.WriteAt("Penelope Covey", 0, 73);
    LCD.WriteAt("Game Concept", 88, 100); //12 characters
    LCD.WriteAt("Milton Bradley (1967)", 0, 125);

    LCD.SetFontColor(RED);
    LCD.WriteAt("BACK", 4, 5);
    LCD.DrawRectangle(2, 2, 54, 21);
}

void Draw::instructions(){ //  draws the instructions page
    LCD.Clear(BLACK);

    FEHImage background;
    background.Open("background.png");
    background.Draw(0, 0);

    LCD.SetFontColor(WHITE);
	LCD.SetBackgroundColor(WHITE);
	LCD.WriteAt("Instructions", 88, 5); //12 characters
    LCD.WriteAt("Pick a spot for your ships", 4, 35); //26 characters
    LCD.WriteAt("Single tap = horizontal", 4, 55);
    LCD.WriteAt("Double tap = vertical", 4, 75);
    LCD.WriteAt("Sink your opponents ships", 4, 95); //25 characters
    LCD.WriteAt("Last one standing wins!", 4, 115); //23 characters
    
    LCD.SetFontColor(RED);
    LCD.WriteAt("BACK", 4, 5);
    LCD.DrawRectangle(2, 2, 54, 21);
}

void Draw::turn(int turn){
    LCD.SetFontColor(GREEN);
    if(turn == 1){
        LCD.WriteAt("Computer",192, 45);
    }
    else{
        LCD.WriteAt("You", 59.5, 45);
    }
    LCD.Update();
}

void Draw::winner(int winner){
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);
    if(winner == 1){
        LCD.WriteAt("Bot Wins!", 104, 15);
    }
    else if(winner == 2){
        LCD.WriteAt("Player Wins!", 86, 15);
    }

    LCD.DrawRectangle(88, 70, 144, 40);
    LCD.WriteAt("Play Again", 98, 83);

    LCD.DrawRectangle(76, 135, 168, 40);
    LCD.WriteAt("Back to Menu", 86, 148);

    LCD.Update();
}

