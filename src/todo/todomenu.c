#include "../calendar/calendarBackend.h"
#include "../color.h"
#include "../inputandstuff.h"
#include <graphx.h>
#include <stdio.h>
#include <ti/getcsc.h>
#include <ti/getkey.h>
extern const char *chars;
extern uint8_t lastKeyCode;
extern uint8_t cursorPos;
extern bool canBackspace;
extern Time *currentTime;
const char progressionTypes[3][11] = {"Completion", "Steps", "Numerical"};
char tempNameBuffer[20];
char stepBuffer[5][20];
/*
This tracks the current progress,
whether it be steps or number of items
24 bit
*/
/*
This tracks the current target steps or amount,
or if it's been completed or not
24 bit
*/
uint16_t temporaryGoalAmount;
char convertKeypadNumToNumberCharacter(uint8_t key) {
  /*
  Making a lookup table cuz lazy
  Either this or i use some sort of ascii table,
  ew, working smarter and not harder, couldn't be me
  */
  switch (key) {
  case sk_0:
    return '0';
    break;
  case sk_1:
    return '1';
    break;
  case sk_2:
    return '2';
    break;
  case sk_3:
    return '3';
    break;
  case sk_4:
    return '4';
    break;
  case sk_5:
    return '5';
    break;
  case sk_6:
    return '6';
    break;
  case sk_7:
    return '7';
    break;
  case sk_8:
    return '8';
    break;
  case sk_9:
    return '9';
    break;
  }
  // at this point just label this program as bloatware
}
void drawAddStepMenu(const char stepChar) {
  // Gets steps
  // using a lookup table cuz for some reason math isnt an option...
  uint8_t step = 0;
  switch (stepChar) {
  case '1':
    step = 1;

  case '2':
    step = 2;

  case '3':
    step = 3;

  case '4':
    step = 4;

  case '5':
    step = 5;
  }
  cursorPos = 0;
  uint8_t index = 0;
  char name[32] = "Enter name of step 1"; // this feels risky and too small
LOOPSTART:
  while (true) {
    gfx_FillScreen(WHITE);
    lastKeyCode = os_GetCSC();
    // Process kbd input
    switch (lastKeyCode) {
    case sk_Clear:
      gfx_End();
      exit(0);
      break;
    case sk_Enter:
      if (stepBuffer[index][0] != 0) {
        // TYPE SOMETHING IN DUMMY
        if (++index > step - 1) {
          // I LOVE GOTO HOLY CRAP
          goto FINISHED;
          // MASSIVE W
        }
        sprintf(name, "Name of step: %u/%u", index + 1, step);
        cursorPos = 0;
        goto LOOPSTART;
        break;
      }
    case sk_Del:
      for (uint8_t i = 0; i < 20; i++) {
        stepBuffer[index][i] = 0;
      }
      break;
    }
    // draw stuff
    for (uint8_t n = 0; n < 3; n++) {
      // text box
      gfx_Rectangle(2 + n, (GFX_LCD_HEIGHT / 2) + n, 314, 30);
    }
    gfx_SetTextScale(2, 2);
    if (chars[lastKeyCode] && cursorPos < 19) {
      stepBuffer[index][cursorPos] = chars[lastKeyCode];
      cursorPos++;
    }
    gfx_PrintStringXY(name, 2, (GFX_LCD_HEIGHT / 2) - 20);
    gfx_PrintStringXY(stepBuffer[index], 9, (GFX_LCD_HEIGHT / 2) + 10);
    gfx_SwapDraw();
  }
FINISHED:
  return;
}
char drawGetStepsMenu() {
  // please its 00:48 im tired asf mr daab just give me the A
  register char iHateCStrings = '1'; // REGISTER KEYWORD OOH YEAH BABY
  gfx_SetDrawBuffer();
  while (true) {
    gfx_FillScreen(WHITE);
    lastKeyCode = os_GetCSC();
    switch (lastKeyCode) {
    case sk_Clear:
      gfx_End();
      exit(0);
      break;
    case sk_Enter:
      return iHateCStrings;
      break;
    case sk_1:
      iHateCStrings = '1';
      break;
    case sk_2:
      iHateCStrings = '2';
      break;
    case sk_3:
      iHateCStrings = '3';
      break;
    case sk_4:
      iHateCStrings = '4';
      break;
    case sk_5:
      iHateCStrings = '5';
      break;
    }
    gfx_PrintStringXY("Steps: ", (GFX_LCD_WIDTH / 2) - 120, GFX_LCD_HEIGHT / 2);
    gfx_SetTextXY(170, GFX_LCD_HEIGHT / 2);
    gfx_SetTextScale(3, 3);
    gfx_PrintChar(iHateCStrings);
    gfx_SwapDraw();
  }
}
void drawGetNumericalValue() {
  char temp[7];
  gfx_SetDrawBuffer();
  cursorPos = 0;
  while (true) {
    lastKeyCode = os_GetCSC();
    // handle input
    switch (lastKeyCode) {
    case sk_Clear:
      gfx_End();
      exit(0);
      break;
    case sk_Del:
      for (uint8_t i = 0; i < 7; i++) {
        temp[i] = 0;
      }
      cursorPos = 0;
      break;
    case sk_Enter:
      cursorPos = 0;
      temporaryGoalAmount = 0;
      for (uint8_t i = 0; i < 6; i++) {
        // convert string to int
        temporaryGoalAmount += (57 - temp[i]) * (10 * i);
      }
      return;
      break;
    }
    // add num
    if (lastKeyCode <= sk_0 && lastKeyCode >= sk_9 && cursorPos < 6) {
      temp[cursorPos] = convertKeypadNumToNumberCharacter(lastKeyCode);
      cursorPos++;
    }
    // draw graphics
    gfx_FillScreen(WHITE);
    for (uint8_t i = 0; i < 5; i++) {
      // draw big border
      gfx_Rectangle(20 + i, 100 + i, 275, 80);
    }
    gfx_SetTextScale(2, 2);
    // draw cursro location
    char cursorLocation[7];
    sprintf(cursorLocation, "%u/%u", cursorPos, 6);
    gfx_PrintStringXY(cursorLocation, 20, 200);
    gfx_PrintStringXY("Enter the goal amount", 20, 80);
    gfx_SetTextScale(5, 5);
    // draw input
    gfx_PrintStringXY(temp, 35, 120);
    gfx_SwapDraw();
  }
}

void drawTodoMenu() {
  // draws main menu
  gfx_SetColor(MAGENTA);
  gfx_FillRectangle(3, (40 * cursorPos) + 20, 5, 13);
  gfx_SetColor(BLACK);
  if (cursorPos == 0)
    // indent when selected
    gfx_SetTextXY(15, 20);
  else
    gfx_SetTextXY(5, 20);
  gfx_SetTextScale(2, 2);
  gfx_FillRectangle(160, 0, 10, 240);
  // first menu item
  gfx_PrintString("view tasks");
  if (cursorPos == 1)
    // index while selected
    gfx_SetTextXY(15, 60);
  else
    gfx_SetTextXY(5, 60);
  gfx_PrintString("add task");
  if (cursorPos == 2)
    gfx_SetTextXY(15, 100);
  else
    gfx_SetTextXY(5, 100);
  // Now, print, wait, what am i doing again?
  gfx_SetTextXY(180, 150);
  gfx_SetTextScale(1, 1);
  gfx_PrintString("tasks completed: ");
}
void enterPromptMenu() {
  cursorPos = 0;
  canBackspace = false;
  for (uint8_t i = 0; i < 15; i++) {
    // clear buffer
    tempNameBuffer[i] = 0;
  }
}
void exitPromptMenu() {
  cursorPos = 0;
  canBackspace = true;
  for (uint8_t i = 0; i < 15; i++) {
    // clear buffer
    tempNameBuffer[i] = 0;
  }
}
/*
Not really "add progress", but im too lazy to refactor plus this is due in
2 days, but basically lets the user choose what progresstion type they use
*/
void drawAddProgressMenu() {
  gfx_SwapDraw();
  while (true) {
    gfx_FillScreen(WHITE);
    lastKeyCode = os_GetCSC();
    // input handling
    switch (lastKeyCode) {
    case sk_Enter:
      return;
      break;
    case sk_Clear:
      gfx_End();
      exit(0);
      break;
    case sk_Down:
      if (cursorPos < 2) {
        cursorPos++;
      } else {
        cursorPos = 0;
      }
      break;
    case sk_Up:
      if (cursorPos < 1) {
        cursorPos = 2;
      } else {
        cursorPos--;
      }
      break;
    }
    gfx_SetDrawBuffer();
    gfx_PrintStringXY("Select Completion Type", 5, 70);
    // draw selector
    gfx_SetColor(MAGENTA);
    gfx_FillRectangle(20, (40 * cursorPos) + 110, 5, 20);
    gfx_SetColor(BLACK);
    // draw boxes
    for (uint8_t y = 0; y < 3; y++) {
      uint8_t indentSize = 20;
      if (y == cursorPos) {
        indentSize = 40;
      }
      gfx_SetTextXY(indentSize, (40 * y) + 110);
      gfx_PrintString(progressionTypes[y]);
      for (uint8_t i = 0; i < 5; i++) {
        // border
        gfx_Rectangle(10 + i, ((40 * y) + 100) + i, (GFX_LCD_WIDTH - 30),
                      40 + 1);
      }
    }
    gfx_SwapDraw();
  }
}
void drawAddNameMenu() {
  // dw compiler optimizes constant expressions this is fine
  // edit: i came back to this code at 11pm, no clue wtf it does
  gfx_SwapDraw();
  gfx_SetColor(BLACK);
  gfx_FillScreen(WHITE);
  gfx_SetTextScale(2, 2);
  gfx_SetTextXY(0, 0);
  while (true) {
    gfx_SetDrawBuffer();
    gfx_FillScreen(WHITE);
    gfx_PrintStringXY("Enter the name ", 5, 50);
    gfx_Rectangle(5, GFX_LCD_HEIGHT / 3, 304, 24);
    lastKeyCode = os_GetCSC();
    // process input
    switch (lastKeyCode) {
    case sk_Clear:
      gfx_End();
      exit(0);
      return;
    case sk_Del:
      for (uint8_t i = 0; i < 20; i++) {
        tempNameBuffer[i] = 0;
      }
      cursorPos = 0;
      break;
    case sk_Enter:
      if (tempNameBuffer[0] != '\0') {
        // Instead of using strlen, lets just do this lmao
        cursorPos = 0;
        return;
      }
      break;
    case sk_Left:
      if (cursorPos > 0) {
        cursorPos--;
      }
      break;
    case sk_Right:
      if (cursorPos < 19) {
        cursorPos++;
      }
      break;
    }
    if (chars[lastKeyCode] && cursorPos < 19) {
      tempNameBuffer[cursorPos] = chars[lastKeyCode];
      cursorPos++;
    }
    char size[10];
    sprintf(size, "%u/%u", cursorPos, 19);
    gfx_PrintStringXY(size, 200, 125);
    gfx_PrintStringXY(tempNameBuffer, 8, (GFX_LCD_HEIGHT / 3) + 4);
    gfx_SwapDraw();
  }
}