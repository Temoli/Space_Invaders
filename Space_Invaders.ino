#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
//I'm really sorry for this pointers I know what I have done ... but I wanted to play with them a bit ... at least it's faster :d
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

const int LEFT = 1; //alien movement
const int RIGHT = 2;
int direction = RIGHT;

bool aliens[3][8];
int aliens_X[7] = {0, 9, 18, 27, 36, 45, 54};
// int aliens_X[8] = {0, 9, 18, 27, 36, 45, 54, 63};
const int aliens_Y[3] = {1, 10, 19};

int bullet_v = 1;
int bullets[20][20];

void setup(){
	gb.begin();
	gb.titleScreen(F("Space_Invaders"));
	gb.display.persistence = false;

	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 7; j++)
			*(*(aliens + i) + j) = true;
}

void loop(){
while (gb.update()){ //returns true every 50ms; 20fps
	//INPUT
	if(gb.buttons.repeat(BTN_C, 0)) gb.titleScreen(F("Space Invaders"));

	if(gb.buttons.repeat(BTN_LEFT, 0) && ship_x > 0){ //move ship
		ship_x -= ship_v;
	}else if(gb.buttons.repeat(BTN_RIGHT, 0) && ship_x < LCDWIDTH - 5){ //LCDWIDTH - ship width
		ship_x += ship_v;
	}

	if(gb.buttons.pressed(BTN_A)){ // create bullet start coords
		for (int i = 0; i < 20; i++){
			if (*(*(bullets + i)) == 0){
				*(*(bullets + i)) = ship_x + 3;
				*(*(bullets + i) + 1) = LCDHEIGHT - 2;
				break;
			}
		}
	}

	//LOGIC
	switch(direction){ //move alients left/right
		case LEFT:
			for (int i = 0; i < 7; i++)
				*(aliens_X + i) += alien_v;
			if (*aliens_X > 21) direction = RIGHT;
			break;
		case RIGHT:
			for (int i = 0; i < 7; i++)
				*(aliens_X + i) -= alien_v;
			if(*aliens_X < 1) direction = LEFT;
			break;
		}
	} //move aliens END

	for (int i = 0; i < 20; i++){ //move bullets up and/or delete bullet
		if ( *(*(bullets + i)) != 0) //move
			*(*(bullets + i) + 1) -= bullet_v;

		if ( *(*(bullets + i) + 1) < -2){ //delete
			*(*(bullets + i)) = 0;
			*(*(bullets + i) + 1) = 0;
		}
	}
			
	//DRAW
	gb.display.clear();
	gb.display.drawBitmap(ship_x, SHIP_Y, ship);
	for (int i = 0; i < 20; i++){
		if (*(*(bullets + i)) != 0) 
			gb.display.fillRect( *(*(bullets + i)), *(*(bullets + i) + 1), 2, 3);
			// gb.display.drawFastVLine( *(*(bullets + i)), *(*(bullets + i) + 1), 3);
	}

	for (int i = 0; i < 3; i++){ //Draw aliens
		for (int j = 0; j < 7; j++){
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