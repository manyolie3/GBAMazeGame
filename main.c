#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/playscreen2.h"
#include "images/house.h"
#include "images/bushes.h"
#include "images/bea.h"
#include "images/coin2.h"
#include "images/animalcrossingbeach.h"
#include "images/instructions.h"
#include "images/youlose.h"
#include "images/youwin.h"
#include "images/customopenscreen.h"
#include "images/bricks.h"


int checkCollision(struct Villager player, struct Platform platform) {
    if (player.col < platform.col + platform.width &&
        player.col + player.width > platform.col &&
        player.row < platform.row + platform.height &&
        player.row + player.height > platform.row) {
        return 1;
    }
    return 0;
}

int checkCollisionCoin(struct Villager player, struct Coin coin) {
    if (player.col < coin.col + coin.width &&
        player.col + player.width > coin.col &&
        player.row < coin.row + coin.height &&
        player.row + player.height > coin.row) {
        
        coin.collected = 1;
        return 1;
    }
    return 0;
}

int checkCollisionHouse(struct Villager player, struct House house) {
    if (player.col < house.col + house.width &&
        player.col + player.width > house.col &&
        player.row < house.row + house.height &&
        player.row + player.height > house.row) {
        house.collided = 1;
        return 1;
    }
    return 0;
}

void isCollected(struct Coin coin, const u16 *image) {
  if (!coin.collected) {
    drawImageDMA(coin.row, coin.col, coin.width, coin.height, image);
  }
}


enum gba_state {
  START,
  PLAY,
  WIN,
  LOSE,
};

int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;
  
  //declaring villager struct
  struct Villager villager;
  villager.row = 145;
  villager.col = 5;
  villager.width = 12;
  villager.height = 12;

  //declaring platform variables
  struct Platform platform0;
  platform0.row = 135;
  platform0.col = 0;
  platform0.width = 105;
  platform0.height = 3;

  struct Platform platform02;
  platform02.row = 135;
  platform02.col = 125;
  platform02.width = 115;
  platform02.height = 3;

  struct Platform platform1;
  platform1.row = 110;
  platform1.col = 0;
  platform1.width = 20;
  platform1.height = 3;

  struct Platform platform2;
  platform2.row = 110;
  platform2.col = 40;
  platform2.width = 200;
  platform2.height = 3;

  struct Platform platform3;
  platform3.row = 85;
  platform3.col = 0;
  platform3.width = 150;
  platform3.height = 3;

  struct Platform platform4;
  platform4.row = 85;
  platform4.col = 170;
  platform4.width = 70;
  platform4.height = 3;

  struct Platform platform5;
  platform5.row = 60;
  platform5.col = 0;
  platform5.width = 90;
  platform5.height = 3;

  struct Platform platform6;
  platform6.row = 60;
  platform6.col = 110;
  platform6.width = 130;
  platform6.height = 3;

  struct Platform platform7;
  platform7.row = 35;
  platform7.col = 0;
  platform7.width = 50;
  platform7.height = 3;

  struct Platform platform8;
  platform8.row = 35;
  platform8.col = 70;
  platform8.width = 170;
  platform8.height = 3;

  
  struct Coin coin3;
  coin3.row = 95;
  coin3.col = 180;
  coin3.width = 6;
  coin3.height = 6;
  coin3.collected = 0;

  struct Coin coin4;
  coin4.row = 120;
  coin4. col = 20;
  coin4.width = 6;
  coin4.height = 6;
  coin4.collected = 0;

  struct Coin coin5;
  coin5.row = 120;
  coin5.col = 40;
  coin5.width = 6;
  coin5.height = 6;
  coin5.collected = 0;

  struct Coin coin6;
  coin6.row = 120;
  coin6.col = 60;
  coin6.width = 6;
  coin6.height = 6;
  coin6.collected = 0;

  struct Coin coin7;
  coin7.row = 95;
  coin7.col = 200;
  coin7.width = 6;
  coin7.height = 6;
  coin7.collected = 0;

  struct Coin coin8;
  coin8.row = 95;
  coin8.col = 220;
  coin8.width = 6;
  coin8.height = 6;
  coin8.collected = 0;

  struct Coin coin11;
  coin11.row = 45;
  coin11.col = 20;
  coin11.width = 6;
  coin11.height = 6;
  coin11.collected = 0;

  struct Coin coin12;
  coin12.row = 45;
  coin12.col = 40;
  coin12.width = 6;
  coin12.height = 6;
  coin12.collected = 0;

  struct Coin coin13;
  coin13.row = 45;
  coin13.col = 60;
  coin13.width = 6;
  coin13.height = 6;
  coin13.collected = 0;

  struct House house1;
  house1.row = 20;
  house1.col = 220;
  house1.width = 15;
  house1.height = 15;
  house1.collided = 0;


  int verticalVelocity = 0;
  int horizontalVelocity = 0;
  

  unsigned int timer = 45;
  int gameStart = 0;
  int coinsCollected = 0;
  static int endScreen = 0;

  // Load initial application state
  enum gba_state state = START;
  drawFullScreenImageDMA(customopenscreen);
  drawCenteredString(10, WIDTH / 2, 0, 130, "Help Alice return home!", WHITE);
  drawCenteredString(10, WIDTH / 2, 0, 150, "Avoid colliding with the walls", WHITE);
  drawCenteredString(10, WIDTH / 2, 0, 170, "and help her collect coins!", WHITE);
  drawCenteredString(10, WIDTH / 2, 0, 190, "PRESS ENTER TO PLAY!", WHITE);
  

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    if (gameStart && vBlankCounter % 60 == 0) {
      if (timer > 0) {
        timer--;
      }
    }

    waitForVBlank();

    if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
      state = START;
      drawFullScreenImageDMA(customopenscreen);
      drawCenteredString(10, WIDTH / 2, 0, 130, "Help Alice return home!", WHITE);
      drawCenteredString(10, WIDTH / 2, 0, 150, "Avoid colliding with the walls", WHITE);
      drawCenteredString(10, WIDTH / 2, 0, 170, "and help her collect coins!", WHITE);
      drawCenteredString(10, WIDTH / 2, 0, 190, "PRESS ENTER TO PLAY!", WHITE);
      villager.row = 145;
      villager.col = 5;
      

      coin3.collected = 0;
      coin4.collected = 0;
      coin5.collected = 0;
      coin6.collected = 0;
      coin7.collected = 0;
      coin8.collected = 0;
      coin11.collected = 0;
      coin12.collected = 0;
      coin13.collected = 0;
      coinsCollected = 0;
      house1.collided = 0;
      endScreen = 0;

    }
    

    switch (state) {
      
      case START:
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
          state = PLAY;
          drawFullScreenImageDMA(playscreen2);
        }

        timer = 45;
        gameStart = 1;

        break;

      case PLAY:

        //draw brick walls
        drawImageDMA(platform0.row, platform0.col, platform0.width, platform0.height, bricks);
        drawImageDMA(platform02.row, platform02.col, platform02.width, platform02.height, bricks);
        drawImageDMA(platform1.row, platform1.col, platform1.width, platform1.height, bricks);
        drawImageDMA(platform2.row, platform2.col, platform2.width, platform2.height, bricks);
        drawImageDMA(platform3.row, platform3.col, platform3.width, platform3.height, bricks);
        drawImageDMA(platform4.row, platform4.col, platform4.width, platform4.height, bricks);
        drawImageDMA(platform5.row, platform5.col, platform5.width, platform5.height, bricks);
        drawImageDMA(platform6.row, platform6.col, platform6.width, platform6.height, bricks);
        drawImageDMA(platform7.row, platform7.col, platform7.width, platform7.height, bricks);
        drawImageDMA(platform8.row, platform8.col, platform8.width, platform8.height, bricks);

        //draw house
        drawImageDMA(house1.row, house1.col, house1.width, house1.height, house);
        house1.collided = 0;

        endScreen = 0;

        //draw coins
        isCollected(coin3, coin2);
        isCollected(coin4, coin2);
        isCollected(coin5, coin2);
        isCollected(coin6, coin2);
        isCollected(coin7, coin2);
        isCollected(coin8, coin2);
        isCollected(coin11, coin2);
        isCollected(coin12, coin2);
        isCollected(coin13, coin2);

        drawString(5, 75, "coins: ", BLACK);


        undrawImageDMA(villager.row, villager.col, villager.width, villager.height, playscreen2);
            
        horizontalVelocity = 0;
        verticalVelocity = 0;
        

        // horizontal movement (left/right)
        if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
          if (villager.col >= 1) {
            horizontalVelocity = -1;
          }
        } else if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
            if (villager.col <= 239 - villager.width) {
              horizontalVelocity = 1;
            }
        }
         
        // vertical movement (up/down)
        if (KEY_DOWN(BUTTON_UP, currentButtons)) {
          if (villager.row >= 20) {
            verticalVelocity = -1;
          }
        } else if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
          if (villager.row <= 160 - villager.height) {
            verticalVelocity = 1;
          }
        }
        
        villager.col += horizontalVelocity;
        villager.row += verticalVelocity;

       
        if (checkCollision(villager, platform1)) {
          state = LOSE;
          break;
        } else if (checkCollision(villager, platform2)) {
          state = LOSE;
          break;
        } else if (checkCollision(villager, platform3)) {
          state = LOSE;
          break;
        } else if (checkCollision(villager, platform4)) {
          state = LOSE;
          break;
        } else if (checkCollision(villager, platform5)) {
          state = LOSE;
          break;
        } else if (checkCollision(villager, platform6)) {
          state = LOSE;
          break;
        } else if (checkCollision(villager, platform0)) {
          state = LOSE;
          break;
        } else if (checkCollision(villager, platform02)) {
          state = LOSE;
          break;
        } else if (checkCollision(villager, platform7)) {
          state = LOSE;
          break;
        } else if (checkCollision(villager, platform8)) {
          state = LOSE;
          break;
        }

       
        if (checkCollisionCoin(villager, coin3) && !coin3.collected) {
          coin3.collected = 1;
          coinsCollected++;
          undrawImageDMA(coin3.row, coin3.col, coin3.width, coin3.height, playscreen2);
        } else if (checkCollisionCoin(villager, coin4) && !coin4.collected) {
          coin4.collected = 1;
          coinsCollected++;
          undrawImageDMA(coin4.row, coin4.col, coin4.width, coin4.height, playscreen2);
        } else if (checkCollisionCoin(villager, coin5) && !coin5.collected) {
          coin5.collected = 1;
          coinsCollected++;
          undrawImageDMA(coin5.row, coin5.col, coin5.width, coin5.height, playscreen2);
        } else if (checkCollisionCoin(villager, coin6) && !coin6.collected) {
          coin6.collected = 1;
          coinsCollected++;
          undrawImageDMA(coin6.row, coin6.col, coin6.width, coin6.height, playscreen2);
        } else if (checkCollisionCoin(villager, coin7) && !coin7.collected) {
          coin7.collected = 1;
          coinsCollected++;
          undrawImageDMA(coin7.row, coin7.col, coin7.width, coin7.height, playscreen2);
        } else if (checkCollisionCoin(villager, coin8) && !coin8.collected) {
          coin8.collected = 1;
          coinsCollected++;
          undrawImageDMA(coin8.row, coin8.col, coin8.width, coin8.height, playscreen2);
        } else if (checkCollisionCoin(villager, coin11) && !coin11.collected) {
          coin11.collected = 1;
          coinsCollected++;
          undrawImageDMA(coin11.row, coin11.col, coin11.width, coin11.height, playscreen2);
        } else if (checkCollisionCoin(villager, coin12) && !coin12.collected) {
          coin12.collected = 1;
          coinsCollected++;
          undrawImageDMA(coin12.row, coin12.col, coin12.width, coin12.height, playscreen2);
        } else if (checkCollisionCoin(villager, coin13) && !coin13.collected) {
          coin13.collected = 1;
          coinsCollected++;
          undrawImageDMA(coin13.row, coin13.col, coin13.width, coin13.height, playscreen2);
        }


        if (checkCollisionHouse(villager, house1)) {
          state = WIN;
          house1.collided = 0;
          break;
        }

        // Draw the sprite in its new position
        drawImageDMA(villager.row, villager.col, villager.width, villager.height, bea);

        break;
      case WIN:
        if (state == WIN && !endScreen) {
          drawFullScreenImageDMA(youwin);
          char coinsStr[30];
          snprintf(coinsStr, sizeof(coinsStr), "you collected %d coins", coinsCollected);
          drawString(20, 40, "YOU WON!", WHITE);
          drawString(35, 40, coinsStr, WHITE);
          endScreen = 1;
        }
        break;
      case LOSE:
        if (state == LOSE && !endScreen) {
          drawFullScreenImageDMA(youlose);
          drawString(HEIGHT / 2 - 30, 80, "you lost :(", WHITE);
          drawString(HEIGHT / 2, 30, "press backspace to try again!", WHITE);
          endScreen = 1;
        }
        break;
      }


    if (state == PLAY) {
      drawImageDMA(4, 8, 65, 8, playscreen2);
      char timerStr[10];
      snprintf(timerStr, sizeof(timerStr), "T: %d", timer);
      drawString(5, 5 , timerStr, BLACK);

      drawImageDMA(5, 115, 65, 7, playscreen2);
      char coinsStr[20];
      snprintf(coinsStr, sizeof(coinsStr), "%d", coinsCollected);
      drawString(5, 115, coinsStr, BLACK);
    }
  
    previousButtons = currentButtons;
  
  }

  UNUSED(previousButtons);

  return 0;
}
