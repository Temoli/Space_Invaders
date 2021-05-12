#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
#define TEST 0 //if 1 game is in test mode - death is disabled
//I'm really sorry for this pointers, I know what I have created ... but I wanted to play with them a bit, at least it's faster :d
//84x48  resolution

//SHIP SETTIGNS
const int SHIP_W = 5;
const int SHIP_H = 3;
const int SHIP_V = 1;

int ship_x = 38;
const int SHIP_y = LCDHEIGHT - 3;
int lifes = 3;
int gm = 0;
int GM_DURATION = 2 * 20; //god mode counter, after 3s ship will be vulnerable; seconds * 20; seconds * (1000ms / 50ms) - 50 because main loop works every 50ms 

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

const int ALIENS_AMOUNT = 7;
int aliens_x[ALIENS_AMOUNT];
// int aliens_X[8] = {0, 9, 18, 27, 36, 45, 54, 63};
const int ALIENS_ROWS = 3;
const int ALIENS_Y[3] = {1, 10, 19};
bool aliens[ALIENS_ROWS][ALIENS_AMOUNT]; //it will be true if alien exist or fale if alien is dead 
int aliens_left = ALIENS_AMOUNT * ALIENS_ROWS;

const byte ALIEN_1[] PROGMEM = {
8, 8,
B00011000,
B10111101,
B11111111,
B11111111,
B10011001,
B00011000,
B00111100,
B00011000,
};
const byte ALIEN_2[] PROGMEM = {8, 8,
B00011000,
B11011011,
B10011001,
B10111101,
B11111111,
B00111100,
B00111100,
B00011000,
};
const byte ALIEN_3[] PROGMEM = {8, 8,
B00111100,
B01111110,
B11111111,
B01111110,
B00111100,
B00111100,
B00011000,
B00011000,
};

//BULLET SETTINGS
const int BULLET_V = 2;
const int BULLETS_AMOUNT = 15;
int bullets[BULLETS_AMOUNT][2]; //it was int bullets[BULLETS_AMOUNT][BULLETS_AMOUNT]; check if some loop doesn't make loop out of table (maybe loop will work on 10 elements but now Y is set to 2
const int RESET_BULLET = -7;

const int A_BULLET_V = 2;
const int A_BULLETS_AMOUNT = 5;
int a_bullets[A_BULLETS_AMOUNT][2];

void draw_bullets(const int &BULLETS_AMOUNT, int bullets_table[][2]);
void (*fun)(const int &BULLETS_AMOUNT, int bullets_table[][2]);

//correction in aliens move - while first or last column is destroyed aliens will move further - to the edge of the screen
int x_correction_l = 0;
int correction_l_count = 0;
int x_correction_r = 0;
int correction_r_count = 0;

int col = 0;
void setup(){
	gb.begin();
	gb.titleScreen(F("Space_Invaders"));
	gb.display.persistence = false;

	for(int i = 0; i < ALIENS_ROWS; i++) //set up 3 rows of aliens; it will be true if alien exist or fale if alien is dead
		for(int j = 0; j < ALIENS_AMOUNT; j++)
			*(*(aliens + i) + j) = true;
	
	for (int i = 0; i < ALIENS_AMOUNT; i++) //set up aliens starting x position if alien_w = 8 -> 0, 9, 18, 27
		*(aliens_x + i) = i * (alien_w + 1);

	for (int i = 0; i < BULLETS_AMOUNT; i++) // reset ship bulets
		*(*(bullets + i)) = RESET_BULLET;

	for (int i = 0; i < A_BULLETS_AMOUNT; i++) // reset alien bulets
		*(*(a_bullets + i)) = RESET_BULLET;

	fun = draw_bullets;
}

void loop(){
while (gb.update()){ //returns true every 50ms; 20fps
	//INPUT
	if(gb.buttons.repeat(BTN_C, 0)) gb.titleScreen(F("Space Invaders"));

	//move left/right
	if(gb.buttons.repeat(BTN_LEFT, 0) && ship_x > 0) //move ship
		ship_x -= SHIP_V;
	else if(gb.buttons.repeat(BTN_RIGHT, 0) && ship_x < LCDWIDTH - SHIP_W)
		ship_x += SHIP_V;
	

	//fire
	if(gb.buttons.pressed(BTN_A)){ // create bullet start coords
		for (int i = 0; i < BULLETS_AMOUNT; i++)
			if (*(*(bullets + i)) == RESET_BULLET){
				*(*(bullets + i)) = ship_x + 3;
				*(*(bullets + i) + 1) = LCDHEIGHT - 2;
				break;
			}
	}

	//LOGIC

	//WON / LOST check if win if so go to title screen and reset some values
	if (aliens_left <= 0 || lifes <= 0){
		delay(1000); //wait 1 seconds to give some time to see what happened and then reset game
		ship_x = 38;
		lifes = 3;
		direction = RIGHT;
		aliens_left = ALIENS_AMOUNT * ALIENS_ROWS;
		gm = 0;

		x_correction_l = 0;
		correction_l_count = 0;
		x_correction_r = 0;
		correction_r_count = 0;

		for(int i = 0; i < ALIENS_ROWS; i++) //set up 3 rows of aliens; it will be true if alien exist or fale if alien is dead
			for(int j = 0; j < ALIENS_AMOUNT; j++)
				*(*(aliens + i) + j) = true;
		
		for (int i = 0; i < ALIENS_AMOUNT; i++) //set up aliens starting x position if alien_w = 8 -> 0, 9, 18, 27
			*(aliens_x + i) = i * (alien_w + 1);

		for (int i = 0; i < BULLETS_AMOUNT; i++) //reset ship bullets
			*(*(bullets + i)) = RESET_BULLET;

		for (int i = 0; i < A_BULLETS_AMOUNT; i++) // reset alien bulets
			*(*(a_bullets + i)) = RESET_BULLET;

		gb.titleScreen(F("Space Invaders"));
	}

	//move aliens left/right
		//section 1 - if first or last coumn of aliens is destroyed this will add correction to aliens movement to move aliens formation to the edge of the screen	
	if ( *(*(aliens) + correction_l_count) == false && *(*(aliens + 1) + correction_l_count) == false && *(*(aliens + 2) + correction_l_count) == false){
		x_correction_l = (correction_l_count + 1) * (alien_w + 1);
		correction_l_count += 1;
	}

	if ( *(*(aliens) + (ALIENS_AMOUNT - 1) - correction_r_count) == false && *(*(aliens + 1) + (ALIENS_AMOUNT - 1) - correction_r_count) == false && *(*(aliens + 2) + (ALIENS_AMOUNT - 1) - correction_r_count) == false){
		x_correction_r = (correction_r_count + 1) * (alien_w + 1);
		correction_r_count += 1;
	}
		//section 2 - alien movement; just moves aliens left - right
	switch(direction){
		case RIGHT:
			for (int i = 0; i < ALIENS_AMOUNT; i++)
				*(aliens_x + i) += alien_v; //move aliens
			if (*aliens_x >= (LCDWIDTH - (ALIENS_AMOUNT * (alien_w + 1) - 1)) + x_correction_r)  //change aliens direction
					direction = LEFT;
			break;
		case LEFT:
			for (int i = 0; i < ALIENS_AMOUNT; i++)
				*(aliens_x + i) -= alien_v;
			if(*aliens_x < 1 - x_correction_l)
					direction = RIGHT;
			break;
	}
	//move aliens END

	//move ship bullets up, and/or delete bullet
	for (int i = 0; i < BULLETS_AMOUNT; i++){
		if ( *(*(bullets + i)) != RESET_BULLET) //move
			*(*(bullets + i) + 1) -= BULLET_V;

		if ( *(*(bullets + i) + 1) < -3){ //delete bullet (it's off the screen)
			*(*(bullets + i)) = RESET_BULLET;
			*(*(bullets + i) + 1) = RESET_BULLET;
		}
	}
	
	//alien shoot
	//if ( *(*(alien_bullets)) >= LCDHEIGHT) *(*(alien_bullets)) = RESET_BULLET
	for (int j = 2; j >= 0; j--){
		for (int i = 0; i < A_BULLETS_AMOUNT; i++){
			col = random(0, ALIENS_AMOUNT);
			if ( ( *(*(aliens + j) + col) == true ) && ( *(*(a_bullets + i)) == RESET_BULLET) ) {
				*(*(a_bullets + i)) = *(aliens_x + col) + 4;
				*(*(a_bullets + i) + 1) = (j + 1) * 8;
			}
		}
	}
	
	//move alien bullets and/or delete bullet
	for (int i = 0; i < A_BULLETS_AMOUNT; i++){
		if ( *(*(a_bullets + i)) != RESET_BULLET){
			*(*(a_bullets + i) + 1) += A_BULLET_V;
		}

		if ( *(*(a_bullets + i) + 1) > LCDHEIGHT + 3){ //delete bullet (it's off the screen)
			*(*(a_bullets + i)) = RESET_BULLET;
			*(*(a_bullets + i) + 1) = RESET_BULLET;
		}
	}
	
	//collision bullet - ship
	#if(!TEST)
	if (gm < GM_DURATION) gm++;
	else{
		for (int bx = 0; bx < A_BULLETS_AMOUNT; bx++){
			if(gb.collideRectRect( *(*(a_bullets + bx)), *(*(a_bullets + bx) + 1), 2, 3, ship_x, SHIP_y, SHIP_W, SHIP_H)){
				ship_x = LCDWIDTH + 3;
				lifes -= 1;
				
				for (int i = 0; i < A_BULLETS_AMOUNT; i++){
					*(*(a_bullets + i)) = RESET_BULLET;
					*(*(a_bullets + i) + 1) = RESET_BULLET;
				}
				
				delay(1000);
				
				ship_x = 38;					
			}
		}
	}
	#endif

	//collision bullet - alien
	for (int i = 0; i < ALIENS_ROWS; i++){
		for (int j = 0; j < ALIENS_AMOUNT; j++){
			for (int b = 0; b < BULLETS_AMOUNT; b++){
				if (*(*(aliens + i) + j) == true && gb.collideRectRect( *(aliens_x + j), *(ALIENS_Y + i), alien_w, alien_h, *(*(bullets + b)), *(*(bullets + b) + 1), 2, 3)){
					gb.sound.playTick();
					*(*(aliens + i ) + j) = false;
					aliens_left -= 1;
					*(*(bullets + b)) = RESET_BULLET;
					*(*(bullets + b) + 1) = RESET_BULLET;
				}
			}
		}
	}

	//DRAW
	gb.display.clear();
	gb.display.drawBitmap(ship_x, SHIP_y, ship);
	
	//draw ship bullets
	// draw_bullets(BULLETS_AMOUNT, bullets);
	(*fun) (BULLETS_AMOUNT, bullets);
	//draw aliens bullets
	// draw_bullets(A_BULLETS_AMOUNT, a_bullets);
	(*fun)(A_BULLETS_AMOUNT, a_bullets);

	//Draw aliens
	for (int i = 0; i < ALIENS_ROWS; i++){
		for (int j = 0; j < ALIENS_AMOUNT; j++){
			if( *(*(aliens + i)+j)){
				switch (i){
					case 0:
						gb.display.drawBitmap(*(aliens_x + j), *(ALIENS_Y + i), ALIEN_1);
						break;
					case 1:
						gb.display.drawBitmap(*(aliens_x + j), *(ALIENS_Y + i), ALIEN_2);
						break;
					case 2:
						gb.display.drawBitmap(*(aliens_x + j), *(ALIENS_Y + i), ALIEN_3);
						break;
				} //switch
			} //if					
		} //for_2
	} //for_1 //Draw aliens END
} //while (gb.update()) END
} //void loop() END

// FUNCTIONS
void draw_bullets(const int &BULLETS_AMOUNT, int bullets_table[][2]){
	for (int i = 0; i < BULLETS_AMOUNT; i++){ 
		if (*(*(bullets_table + i)) != RESET_BULLET) 
			gb.display.fillRect( *(*(bullets_table + i)), *(*(bullets_table + i) + 1), 2, 3);
	}
}