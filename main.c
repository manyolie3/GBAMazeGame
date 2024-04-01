#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/openingscreen3.h"
#include "images/bitty2.h"
#include "images/playscreen2.h"
#include "images/brickwall.h"
#include "images/openingscreen.h"


int checkCollision(struct Villager player, struct Platform platform) {
    if (player.col < platform.col + platform.width &&
        player.col + player.width > platform.col &&
        player.row < platform.row + platform.height &&
        player.row + player.height > platform.row) {
        return 1;
    }
    return 0;
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
  platform0.width = 75;
  platform0.height = 3;

  struct Platform platform02;
  platform02.row = 135;
  platform02.col = 95;
  platform02.width = 145;
  platform02.height = 3;

  struct Platform platform1;
  platform1.row = 110;
  platform1.col = 0;
  platform1.width = 75;
  platform1.height = 3;

  struct Platform platform2;
  platform2.row = 110;
  platform2.col = 95;
  platform2.width = 145;
  platform2.height = 3;

  struct Platform platform3;
  platform3.row = 85;
  platform3.col = 0;
  platform3.width = 100;
  platform3.height = 3;

  struct Platform platform4;
  platform4.row = 85;
  platform4.col = 120;
  platform4.width = 120;
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
  platform7.width = 90;
  platform7.height = 3;

  struct Platform platform8;
  platform8.row = 35;
  platform8.col = 110;
  platform8.width = 130;
  platform8.height = 3;




  int verticalVelocity = 0;
  int horizontalVelocity = 0;
  
  unsigned int timer = 60;
  int gameStart = 0;

  // Load initial application state
  enum gba_state state = START;
  drawFullScreenImageDMA(openingscreen3);
  drawCenteredString(HEIGHT / 2, WIDTH / 2, 0, 0, "HELP BITTY COLLECT LEAVES AND GET HOME!", BLACK);
  drawCenteredString(HEIGHT / 2, WIDTH / 2, 15, 15, "PRESS ENTER TO PLAY!", BLACK);


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
      drawFullScreenImageDMA(openingscreen3);
      drawCenteredString(HEIGHT / 2, WIDTH / 2, 0, 0, "HELP BITTY COLLECT LEAVES AND GET HOME!", BLACK);
      drawCenteredString(HEIGHT / 2, WIDTH / 2, 15, 15, "PRESS ENTER TO PLAY!", BLACK); 
      villager.row = 145;
      villager.col = 5;
    }
    

    switch (state) {
      
      case START:
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
          state = PLAY;
          drawFullScreenImageDMA(playscreen2);
        }

        timer = 60;
        gameStart = 1;

        break;

      case PLAY:
        drawImageDMA(platform0.row, platform0.col, platform0.width, platform0.height, brickwall);
        drawImageDMA(platform02.row, platform02.col, platform02.width, platform02.height, brickwall);
        drawImageDMA(platform1.row, platform1.col, platform1.width, platform1.height, brickwall);
        drawImageDMA(platform2.row, platform2.col, platform2.width, platform2.height, brickwall);
        drawImageDMA(platform3.row, platform3.col, platform3.width, platform3.height, brickwall);
        drawImageDMA(platform4.row, platform4.col, platform4.width, platform4.height, brickwall);
        drawImageDMA(platform5.row, platform5.col, platform5.width, platform5.height, brickwall);
        drawImageDMA(platform6.row, platform6.col, platform6.width, platform6.height, brickwall);
        drawImageDMA(platform7.row, platform7.col, platform7.width, platform7.height, brickwall);
        drawImageDMA(platform8.row, platform8.col, platform8.width, platform8.height, brickwall);


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
          if (villager.row >= 15) {
            verticalVelocity = -1;
          }
        } else if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
          if (villager.row <= 160 - villager.height) {
            verticalVelocity = 1;
          }
        }
        
        villager.col += horizontalVelocity;
        villager.row += verticalVelocity;

       
        // if (checkCollision(villager, platform1)) {
        //   state = LOSE;
        //   break;
        // } else if (checkCollision(villager, platform2)) {
        //   state = LOSE;
        //   break;
        // } else if (checkCollision(villager, platform3)) {
        //   state = LOSE;
        //   break;
        // } else if (checkCollision(villager, platform4)) {
        //   state = LOSE;
        //   break;
        // } else if (checkCollision(villager, platform5)) {
        //   state = LOSE;
        //   break;
        // } else if (checkCollision(villager, platform6)) {
        //   state = LOSE;
        //   break;
        // }


        // Draw the sprite in its new position
        drawImageDMA(villager.row, villager.col, villager.width, villager.height, bitty2);


        break;
      case WIN:

        // state = ?
        break;
      case LOSE:
        drawFullScreenImageDMA(openingscreen);
        break;
    }

     if (state == PLAY) {
      drawImageDMA(5, 5, 60, 7, playscreen2);
      char timerStr[10];
      snprintf(timerStr, sizeof(timerStr), "T: %d", timer);
      drawString(5, 5 , timerStr, BLACK);
     }

    previousButtons = currentButtons;
  }

  UNUSED(previousButtons);

  return 0;
}
