#include <SPI.h>>
#include <Gamebuino.h>
Gamebuino gb;
//Sorry for this pointers but I wanted to play with them a bit 
{*/
84x48  resolution

divisors of 84	1, 2, 3, 4, 6, 7, 12, 14, 21, 28, 42, 84
								84 42	28 21 14 12 7		6		4		3		2		1 
divisors of 48	1, 2, 3, 4, 6, 8, 12, 16, 24, 48
								48 24 16 12 8  6  4   3   2   1

*/}

#define TEST 1

{ //BITMAPS
const byte ship[] PROGMEM = {7, 3,
0001000,
0011100,
0111110
}

const byte alien_1[] PROGMEM = {9, 9,
000000000,
011111110,
010010010,
010010010,
010010010,
010010010,
010010010,
011111110,
000000000
}
const byte alien_2[] PROGMEM = {9, 9,
000000000,
011111110,
010010010,
010101010,
010010010,
010100010,
010111010,
011111110,
000000000
}
const byte alien_3[] PROGMEM = {9, 9,
000000000,
011111110,
010111010,
010001010,
010111010,
010001010,
010111010,
011111110,
000000000
}
}//BITMAPS END

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

bool aliens[3][8];
int aliens_X[8] = {0, 9, 18, 27, 36, 45, 54, 63};
const int aliens_Y[3] = {1, 10, 19};

void setup(){
	gb.begin();
	gb.titleScreen(F("Space_Invaders"));
	
	for (int i = 0; i < 3; i++){ //setup 3 rows of aliens ready for invasion
		for (int j = 0; j < 8; j++){
			aliens[i][j] = true;
		}
	}
	
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
	
	for (int i = 0; i < 3; i++){ //Draw aliens
		for (int j = 0; j < 8; j++){
			if( *(*(aliens + i)+j)){
				switch (i){
					case 0:
						gb.display.drawBitmap(*(aliens_X + j), *(aliens_Y + i), alien_1);
						break;
					case 1:
						gb.display.drawBitmap(*(aliens_X + j), *(aliens_Y + i), alien_2);
						break;
					case 2:
						gb.display.drawBitmap(*(aliens_X + j), *(aliens_Y + i), alien_3);
						break;
				}
			}											
		}
	} //Draw aliens END
													
													
}
}
