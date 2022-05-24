#include <Gamebuino-Meta.h>
#include <Vector.h>
#include "assets/rgb565.h"

using namespace std;

#define DISPLAY_MODE DISPLAY_MODE_RGB565

struct obstacle{
  int posX;
  int posY;
  int size;

  bool move(int, int);
  void draw();
};



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

//tubes

const int ELEMENT_COUNT_MAX = 50;
obstacle storage_array[ELEMENT_COUNT_MAX];

Vector<obstacle> obstacles(storage_array);

int size_hole = 10;
int distance_obstacles = 30;

bool obstacle::move(int x, int y){
  posY += y;
  if( posX + x < (0 - 3) ){
    return true;
  }
  posX += x;
  return false;
}

void obstacle::draw(){
  //tube up
  //gb.display.fillRect(posX, 0, 3, posY - (size_hole/2));
  gb.display.drawImage(posX, (40 - posY - (10/2)) * -1, tube);
  //tube down
  //gb.display.fillRect(posX, gb.display.height(), 3, ( (gb.display.height() - posY) - (size_hole/2)) * -1);
  gb.display.drawImage(posX, posY + 10, tube2);
}

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

  int holeY = random(10, 51);
  obstacles.push_back((obstacle){ gb.display.width(), holeY, size_hole});
}

void loop() {
  while(!gb.update());
  gb.display.clear();

  displayBird();
  
  if(obstacles[obstacles.size() - 1].posX < gb.display.width() - distance_obstacles - 3){
    int holeY = random(10, 51);
    obstacles.push_back({ gb.display.width(), holeY, size_hole});
    
  }

  for (int i = 0; i < obstacles.size(); i++)
  {
    gb.display.print(obstacles[i].posX);
    obstacles[i].draw();
  }

  

  for (int i = 0; i < obstacles.size(); i++)
  {

    if(obstacles[i].move(-1, 0)){
      obstacles.remove(i);
      i-=1;
    }
    
  }

  
  if (gb.buttons.pressed(BUTTON_A) && birdPosY-2 > 0){
    
    velocityY = -2;
  }
}

