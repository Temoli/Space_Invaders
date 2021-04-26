#include <SPI.h>>
#include <Gamebuino.h>
Gamebuino gb;
//84x48  resolution
#define TEST 1

{ //BITMAPS
const byte ship[] PROGMEM = {7, 3,
0001000
0011100
0111110
}

const byte alien_2[] PROGMEM = {9, 9,
000000000
000000000
000000000
000000000
000000000
000000000
000000000
000000000
}
const byte alien_3[] PROGMEM = {9, 9,
000000000
000000000
000000000
000000000
000000000
000000000
000000000
000000000
}
const byte alien_1[] PROGMEM = {9, 9,
000000000
000000000
000000000
000000000
000000000
000000000
000000000
000000000
}
}

int ship_v = 1;
int ship_x = 38;
const int SHIP_Y = LCDHEIGHT - 3;

int alien_v = 1;
int alien_1_x;
int alien_1_y;
int alien_2_x;
int alien_2_y;
int alien_3_x;
int alien_3_y;


void setup(){
	gb.begin();
	gb.titleScreen(F("Space_Invaders"));
}

void loop(){
while (gb.update()){ //returns true every 50ms; 20fps
	//INPUT
	if(gb.buttons.repeat(BTN_C, 0)) gb.titleScreen(F("Space Invaders"));

	if(gb.buttons.repeat(BTN_LEFT, 0)){ //move ship
		ship_x -= ship_v;
	}else if(gb.buttons.repeat(BTN_RIGHT, 0)){
		ship_x += ship_v;
	}

	//LOGIC


	//DRAW
	gb.display.drawBitmap(ship_x, SHIP_Y, ship);
}
}
