#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
//84x48  resolution

#define TEST 1

const byte ship[] PROGMEM = {8, 3,
B00100000,
B01110000,
B11111000,
};
const byte alien_1[] PROGMEM = {
8, 8,
B11111111,
B10010001,
B10010001,
B10010001,
B10010001,
B10010001,
B10010001,
B11111111,
};
const byte alien_2[] PROGMEM = {8, 8,
B11111111,
B11111111,
B10000101,
B10001001,
B10010001,
B10100001,
B11111111,
B11111111,
};
const byte alien_3[] PROGMEM = {8, 8,
B11111111,
B11111111,
B10000011,
B10011111,
B10000011,
B10000011,
B11111111,
B11111111,
};

int ship_v = 1;
int ship_x = 38;
const int SHIP_Y = LCDHEIGHT - 3;

int alien_v = 1;

bool aliens[3][8];
int aliens_X[8] = {0, 9, 18, 27, 36, 45, 54, 63};
const int aliens_Y[3] = {1, 10, 19};

void setup(){
	gb.begin();
	gb.titleScreen(F("Space_Invaders"));
	gb.display.persistence = false;
	
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
	if (*aliens_X < 12){
		for (int i = 0; i < 8; i++){
			*(aliens_X + i) += alien_v;
		}
	} else if(*aliens_X > 0){
		for (int i = 0; i < 8; i++){
			*(aliens_X + i) -= alien_v;
		}
	}

	//DRAW
	gb.display.clear();
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
