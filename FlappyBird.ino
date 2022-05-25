#include <Gamebuino-Meta.h>
#include <Vector.h>
#include "assets/imagesCodes.h"

using namespace std;

#define DISPLAY_MODE DISPLAY_MODE_RGB565

struct obstacle {
  int posX;
  int posY;
  bool coinSpawn;

  bool move(int, int);
  void draw();
  void score();
};

// --- Variables ---
bool game = true;
bool firstStart = false;
int points = 0;

// Définition des images
Image background(BACKGROUND);
Image coin(COIN);
Image bird(BIRD);
Image tube(TUBE_TOP);
Image tube2(TUBE_BOTTOM);

// Background
int background_positionX = 0;
int background_positionY = 0;

//bird
int birdPosX = 10;
int birdHeight = 12;
int birdWidht = 19;
float birdPosY = gb.display.height() / 2;
float velocityY = 0;
float gravity = .2;
bool dead = false;

//tubes
const int ELEMENT_COUNT_MAX = 50;
int sizeHole = 13;
int distanceObstacles = 40;
int tubeHeight = 40;
int tubeWidht = 14;
obstacle storage_array[ELEMENT_COUNT_MAX];
Vector<obstacle> obstacles(storage_array);

// --- Fonctions ---
bool obstacle::move(int x, int y) {
  posY += y;
  if ( posX + x < -tubeWidht ) {
    return true;
  }
  posX += x;
  return false;
}

void obstacle::draw() {
  // tube up
  gb.display.drawImage( posX, -tubeHeight + posY - sizeHole, tube);
  // tube down
  gb.display.drawImage(posX, posY + sizeHole , tube2);
  // coins
  if (coinSpawn) {
    gb.display.drawImage(posX + 2, posY - 4, coin);
  }
}

void displayBird() {
  if (!dead && firstStart) {
    velocityY += gravity;
    birdPosY + velocityY + birdHeight >= gb.display.height() ? birdPosY = gb.display.height() - birdHeight : birdPosY += velocityY;
  }
  gb.display.drawImage(birdPosX, birdPosY, bird, birdWidht, birdHeight);
}

void obstacle::score(){
  int tmpPoints = 0;
  char color = 'G';
  if(posX + 7 == birdPosX + birdWidht){
    tmpPoints++;
    if (coinSpawn)
      if (gb.collide.rectRect(posX + 2 , posY - 4 , 9 , 8 , birdPosX, birdPosY, birdWidht, birdHeight)) {
        tmpPoints++;
        coinSpawn = false;
        color = 'Y';
      }
    points += tmpPoints;
    color == 'G' ? gb.lights.fill(GREEN) : gb.lights.fill(YELLOW);
    gb.sound.play("assets/sfx_point.WAV");
  }
}

int randomTubeHeight(){
  return random(18, 46);
}

bool randomCoinSpawn(){
  return random(1, 11) == 1;
}

void death(){
  game = false;
  gb.sound.play("assets/sfx_die.WAV");
}

void tubeSpawner(){
  obstacles.push_back({ gb.display.width(), randomTubeHeight(), randomCoinSpawn()});
  gb.lights.fill(WHITE);
}

// --- Jeu ---
void setup() {
  gb.begin();
  gb.sound.setVolume(8);
  tubeSpawner();
}

void loop() {
  while (!gb.update());
  gb.display.clear();
  gb.display.drawImage(background_positionX, background_positionY, background);

  if (game && firstStart) {
    if (gb.buttons.pressed(BUTTON_A) && birdPosY - 2 > 0) {
      gb.sound.play("assets/sfx_wing.WAV");
      velocityY = -2;
    }
    displayBird();

    for (int i = 0; i < obstacles.size(); i++) {
      if (gb.collide.rectRect( obstacles[i].posX + 2, -tubeHeight + obstacles[i].posY - sizeHole, tubeWidht , tubeHeight , birdPosX+4, birdPosY+2, birdWidht - 8, birdHeight - 4)
        || gb.collide.rectRect( obstacles[i].posX + 2, obstacles[i].posY + 10, tubeWidht , tubeHeight , birdPosX+4, birdPosY+2, birdWidht - 8, birdHeight - 4))
          death();

      obstacles[i].draw();
      obstacles[i].score();

      if (obstacles[i].move(-1, 0)) {
        obstacles.remove(i);
        i -= 1;
      }
    }

    if (birdPosY + birdHeight == gb.display.height())
      death();

    if (obstacles[obstacles.size() - 1].posX < gb.display.width() - distanceObstacles - 3)
      tubeSpawner();

    gb.display.setCursor(gb.display.width()/2-5, 15);
    gb.display.print(points);
  } else if (!firstStart){
    gb.display.setCursor(gb.display.width() / 2 - 21, 15);
    gb.display.print("FLAPPY BIRD");
    gb.display.setCursor(gb.display.width() / 2 - 31, 25);
    gb.display.print("PRESS A TO START");
    displayBird();
    if (gb.buttons.pressed(BUTTON_A) && birdPosY - 2 > 0) {
      gb.sound.play("assets/sfx_wing.WAV");
      velocityY = -2;
      firstStart = true;
    }
  } else {
    gb.lights.fill(RED);
    gb.display.setCursor(gb.display.width() / 2 - 19, 15);
    gb.display.print("GAME OVER");
    gb.display.setCursor(gb.display.width() / 2 - 35, 25);
    gb.display.print("PRESS B TO RESTART");
    if (gb.buttons.pressed(BUTTON_B)) {
      gb.lights.fill(WHITE);
      gb.sound.play("assets/sfx_wing.WAV");
      birdPosY = gb.display.height() / 2;
      velocityY = -2;
      points = 0;
      game = true;
      obstacles.clear();
      obstacles.push_back((obstacle) {gb.display.width(), randomTubeHeight(), randomCoinSpawn()});
    }
  }

}

