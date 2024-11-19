#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHRandom.h"


void draw_menu();


int main(){
    while(true){
        draw_menu();
    }
    
}

void draw_menu(){
    // draw title
    LCD.Clear(BLACK);
    LCD.SetBackgroundColor(WHITE);
    LCD.WriteAt("Battleship", 100, 5);

    LCD.DrawRectangle(125, 45, 70, 40);
    LCD.WriteAt("Play", 135, 58);

    LCD.DrawRectangle(125, 90, 70, 40);
    LCD.WriteAt("Statistics", 116, 103);

    LCD.DrawRectangle(125, 135, 70, 40);
    LCD.WriteAt("Instructions", 110, 148);

    LCD.DrawRectangle(125, 180, 70, 40);
    LCD.WriteAt("Credits", 122, 193);
    LCD.Update();
}