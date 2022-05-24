#include <Gamebuino-Meta.h>
#include "assets/rgb565.h"

#define DISPLAY_MODE DISPLAY_MODE_RGB565

Image bird(IMAGE_DATA);
Image tube(TUBE_IMAGE_DATA);
Image tube2(TUBE2_IMAGE_DATA);

int bird_positionX = 11;
int bird_positionY = 11;
int bird_speedX = 2;

int tube_positionX = 11;
int tube_positionY = 0;

int tube2_positionX = 11;
int tube2_positionY = 60;

void setup() {
    gb.begin();
}

void loop() {
    while (!gb.update());
        gb.display.clear();
        if (gb.buttons.repeat(BUTTON_RIGHT, 0)) {
            if (bird_positionX < gb.display.height()) {
                bird_positionX = bird_positionX + bird_speedX;
            }
        }

        if (gb.buttons.repeat(BUTTON_LEFT, 0)) {
            if (bird_positionX > 0) {
                bird_positionX = bird_positionX - bird_speedX;
            }
        }

        if (gb.buttons.repeat(BUTTON_DOWN, 0)) {
            if (bird_positionY + 30 < gb.display.width()) {
                bird_positionY = bird_positionY + bird_speedX;
            }
        }

        if (gb.buttons.repeat(BUTTON_UP, 0)) {
            if (bird_positionY > 0) {
                bird_positionY = bird_positionY - bird_speedX;
            }
        }

        gb.display.drawImage(bird_positionX, bird_positionY, bird, 19,11);
        gb.display.drawImage(tube_positionX, tube_positionY, tube);
        gb.display.drawImage(tube2_positionX, tube2_positionY, tube2);
}
