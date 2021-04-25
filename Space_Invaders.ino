#include <SPI.h>>
#include <Gamebuino.h>
Gamebuino gb;
//84x48  resolution
#define TEST 1

void setup(){
	gb.begin();
	gb.titleScreen(F("Space_Invaders"));
}

void loop(){
while (gb.update()){ //returns true every 50ms; 20fps
	//INPUT


	//LOGIC


	//DRAW
}
}