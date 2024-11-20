#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHRandom.h"
#include "FEHUtility.h"
#include <string.h>

void draw_menu();
void draw_credits();
void draw_instructions();
void draw_statistics();

void game_loop();

int menu_touch();
int back_touch(int);


int main(){
    int choice = 0;

    while(true){
        switch(choice){
            case(0):
                draw_menu();
                Sleep(0.1);
                choice = menu_touch();
                break;
            case(1):
                game_loop(); // going to have to change this later
                break;
            case(2):
                draw_statistics();
                Sleep(0.1);
                choice = back_touch(2);
                break;
            case(3):
                draw_instructions();
                Sleep(0.1);
                choice = back_touch(3);
                break;
            case(4):
                draw_credits();
                Sleep(0.1);
                choice = back_touch(4);
                break;
        }
        LCD.Update();
    }
    
}


void game_loop(){
    // game here
}

int menu_touch(){ // checks to see if the menu buttons are touched
    int x, y;
    if(LCD.Touch(&x, &y)){
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

int back_touch(int current){
    int x, y;
    if(LCD.Touch(&x, &y)){
        if(x >= 125 && x <= 195 && y >= 202 && y <= 232){
            return 0;
        }
    }

    return current;
}

void draw_menu(){
    // draw title
    LCD.Clear(BLACK);
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

void draw_statistics(){
    LCD.Clear(BLACK);
    LCD.WriteAt("Statistics", 98, 5);

    LCD.DrawRectangle(125, 202, 70, 30);
    LCD.WriteAt("Back", 135, 210);
}

void draw_credits(){
    LCD.Clear(BLACK);
    LCD.WriteAt("Credits", 117, 5);

    LCD.DrawRectangle(125, 202, 70, 30);
    LCD.WriteAt("Back", 135, 210);
}

void draw_instructions(){
    LCD.Clear(BLACK);
    LCD.WriteAt("Instructions", 86, 5);

    LCD.DrawRectangle(125, 202, 70, 30);
    LCD.WriteAt("Back", 135, 210);

    /*
    //Let's start! Pick a spot for your ships while your opponent does the same
    //Pick a spot on the map to try and hit their ship into the sea!
    //Last ship standing wins!hit!
    LCD.Clear(BLACK);
	LCD.SetBackgroundColor(WHITE);
	LCD.WriteAt("Instructions", 100, 5);
	LCD //if you pick a location that is NOT occupied on your opponent's grid, it's a miss!
    */
}       
