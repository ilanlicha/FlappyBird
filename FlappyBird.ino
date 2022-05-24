#include <Gamebuino-Meta.h>
#include "assets/rgb565.h"

#define DISPLAY_MODE DISPLAY_MODE_RGB565

//image
Image background(BACKGROUND_IMAGE_DATA);
Image piece(PIECE_IMAGE_DATA);
Image bird(IMAGE_DATA);
Image tube(TUBE_IMAGE_DATA);
Image tube2(TUBE2_IMAGE_DATA);

//Background
int background_positionX = 0;
int background_positionY = 0;

//Piece
int piece_positionX = 10;
int piece_positionY = 0;

//bird
bool dead = false;
int birdPosX = 10;
float birdPosY = gb.display.height()/2;
int birdHeight = 12;
int birdWidht = 19;
float velocityY = 0;
float gravity = .2;

void displayBird(){
  if (!dead){
    velocityY += gravity;
    birdPosY + velocityY + birdHeight >= gb.display.height() ? birdPosY = gb.display.height() - birdHeight : birdPosY += velocityY;    
  }
  gb.display.drawImage(background_positionX, background_positionY, background);
  gb.display.drawImage(piece_positionX, piece_positionY, piece);
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

