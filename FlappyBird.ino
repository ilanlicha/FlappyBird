#include <Gamebuino-Meta.h>
#include "assets/rgb565.h"

#define DISPLAY_MODE DISPLAY_MODE_RGB565

//image
Image bird(IMAGE_DATA);
Image tube(TUBE_IMAGE_DATA);
Image tube2(TUBE2_IMAGE_DATA);

//bird
bool dead = false;
int birdPosX = 10;
float birdPosY = gb.display.height()/2;
int birdHeight = 11;
int birdWidht = 19;
float velocityY = 0;
float gravity = .2;

void displayBird(){
  if (!dead){
    velocityY += gravity;
    birdPosY + velocityY + birdHeight >= gb.display.height() ? birdPosY = gb.display.height() - birdHeight : birdPosY += velocityY;    
  }
  gb.display.drawImage(birdPosX, birdPosY, bird, birdWidht, birdHeight);
}

void setup() {
  gb.begin();
}

void loop() {
  while(!gb.update());
  gb.display.clear();
  displayBird();
  if (gb.buttons.pressed(BUTTON_A) && birdPosY-2 > 0){
    velocityY = -2;
  }
}

