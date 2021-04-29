#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
//I'm really sorry for this pointers, I know what I have created ... but I wanted to play with them a bit ... at least it's faster :d
//84x48  resolution

#define TEST 1
//SHIP SETTIGNS
int ship_w = 5;
int ship_h = 3;
int ship_v = 1;

int ship_x = 38;
const int SHIP_y = LCDHEIGHT - 3;

const byte ship[] PROGMEM = {8, 3,
B00100000,
B01110000,
B11111000,
};

//ALIENS SETTIGNS
int alien_w = 8;
int alien_h = 8;
int alien_1_h = 8;
int alien_2_h = 8;
int alien_3_h = 8;
int alien_v = 1;

const int LEFT = 1; //alien movement
const int RIGHT = 2;
int direction = RIGHT;

const int aliens_amount = 7;
int aliens_left = aliens_amount;
int aliens_x[aliens_amount];
//int aliens_x[7] = {0, 9, 18, 27, 36, 45, 54};
// int aliens_X[8] = {0, 9, 18, 27, 36, 45, 54, 63};
const int aliens_rows = 3;
const int aliens_y[3] = {1, 10, 19};
bool aliens[aliens_rows][aliens_amount]; //it will be true if alien exist or fale if alien is dead 

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

//BULLET SETTINGS
const int bullet_v = 1;
const int bullets_amount = 10;
int bullets[bullets_amount][2]; //it was int bullets[bullets_amount][bullets_amount]; check if some loop doesn't make loop out of table (maybe loop will work on 10 elements but now Y is set to 2

void setup(){
	gb.begin();
	gb.titleScreen(F("Space_Invaders"));
	gb.display.persistence = false;

	for(int i = 0; i < aliens_rows; i++) //set up 3 rows of aliens; it will be true if alien exist or fale if alien is dead
		for(int j = 0; j < aliens_amount; j++)
			*(*(aliens + i) + j) = true;
	
	for (int i = 0; i < aliens_amount; i++) //set up aliens starting x position if alien_w = 8 -> 0, 9, 18, 27
		*(aliens_x + i) = i * (alien_w + 1);
}

void loop(){
while (gb.update()){ //returns true every 50ms; 20fps
	//INPUT
	if(gb.buttons.repeat(BTN_C, 0)) gb.titleScreen(F("Space Invaders"));

	//move left/right
	if(gb.buttons.repeat(BTN_LEFT, 0) && ship_x > 0){ //move ship
		ship_x -= ship_v;
	}else if(gb.buttons.repeat(BTN_RIGHT, 0) && ship_x < LCDWIDTH - ship_w){
		ship_x += ship_v;
	}

	//fire
	if(gb.buttons.pressed(BTN_A)){ // create bullet start coords
		for (int i = 0; i < bullets_amount; i++){
			if (*(*(bullets + i)) == 0){
				*(*(bullets + i)) = ship_x + 3;
				*(*(bullets + i) + 1) = LCDHEIGHT - 2;
				break;
			}
		}
	}

	//LOGIC
	//move aliens left/right
	switch(direction){
		case LEFT:
			for (int i = 0; i < aliens_amount; i++)
				*(aliens_x + i) += alien_v; //move aliens
			if (*aliens_x >= (LCDWIDTH - (aliens_amount * (alien_w + 1) - 1)))  //change aliens direction //to do: make 21 a variable easy to change. it should depends on aliens_amount
					direction = RIGHT;
			break;
		case RIGHT:
			for (int i = 0; i < aliens_amount; i++)
				*(aliens_x + i) -= alien_v;
			if(*aliens_x < 1)
					direction = LEFT;
			break;
		}
	} //move aliens END

	//move bullets up and/or delete bullet
	for (int i = 0; i < bullets_amount; i++){
		if ( *(*(bullets + i)) != 0) //move			//to do: check if  !=0 isn't wrong
			*(*(bullets + i) + 1) -= bullet_v;

		if ( *(*(bullets + i) + 1) < -2){ //delete bullet (it's off the screen)
			*(*(bullets + i)) = 0;
			*(*(bullets + i) + 1) = 0;
		}
	}

			
	//collision bullet - alien
	for (int b = 0; b < bullets_amount; b++){
		for (int i = 0; i < aliens_rows; i++){
			for (int j = 0; i < aliens_amount; j++){
				if (aliens[i][j] == true && gb.collideRectRect( *(aliens_x + j), *(aliens_y + i), alien_w, alien_h, *(*(bullets + b)), *(*(bullets + b) + 1), 2, 3)){
					aliens[i][j] = false;
					aliens_left -= 1;
				}
			}
		}
	}
					
	//check if win if so go to title screen and reset some values
	if (aliens_left == 0){
		delay(3000); //wait 3 seconds to give some time to see what happened and then reset game
		gb.titleScreen(F("Space Invaders"));
		int ship_x = 38;
		int direction = RIGHT;
		int aliens_left = aliens_amount;
		
		for(int i = 0; i < aliens_rows; i++) //set up 3 rows of aliens
			for(int j = 0; j < aliens_amount; j++)
				*(*(aliens + i) + j) = true;
	
		for (int i = 0; i < aliens_amount; i++) //set up aliens starting x position if alien_w = 8 -> 0, 9, 18, 27
			*(aliens_x + i) = i * (alien_w + 1); //alien 1 aka 0 =0*(8+1) = 0;  alien 3 aka 4 = 3*(8+1) = 27
		
		for (int i = 0; i < bullets_amount; i++) //reset bullets coords
			for (int j = 0; j < 2; j++)
				*(*(bullets + i) + j) = 0;
	}

	//DRAW
	gb.display.clear();
	gb.display.drawBitmap(ship_x, SHIP_y, ship);
	for (int i = 0; i < bullets_amount; i++){ //draw bullets
		if (*(*(bullets + i)) != 0) 
			gb.display.fillRect( *(*(bullets + i)), *(*(bullets + i) + 1), 2, 3);
	}

	for (int i = 0; i < aliens_rows; i++){ //Draw aliens
		for (int j = 0; j < aliens_amount; j++){
			if( *(*(aliens + i)+j)){
				switch (i){
					case 0:
						gb.display.drawBitmap(*(aliens_x + j), *(aliens_y + i), alien_1);
						break;
					case 1:
						gb.display.drawBitmap(*(aliens_x + j), *(aliens_y + i), alien_2);
						break;
					case 2:
						gb.display.drawBitmap(*(aliens_x + j), *(aliens_y + i), alien_3);
						break;
				}
			}											
		}
	} //Draw aliens END
} //void loop() END
