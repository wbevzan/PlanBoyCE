#include "calendar/calendarBackend.h"
#include "color.h"
#include "inputandstuff.h"
#include <graphx.h>
#include <stdbool.h>
#include <stdio.h>
#include <ti/getcsc.h>
#include <ti/getkey.h>
#include <time.h>
/// @brief cursor position
extern uint8_t cursorPos;
extern uint8_t length;
/// @brief check if screen has to be redrawn
extern bool somethingChanged;
/// @brief last key code hit by the calculator
extern uint8_t lastKeyCode;
extern uint8_t currentMenu;
extern Time *currentTime;

void drawMainMenu() {
  char buf[20]; // probably too big but whatever
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
  gfx_PrintString("Todolist > ");
  if (cursorPos == 1)
    // index while selected
    gfx_SetTextXY(15, 60);
  else
    gfx_SetTextXY(5, 60);
  // second menu item
  gfx_PrintString("Calendar > ");
  // third menu item
  // Now print side info
  gfx_SetTextScale(1, 1);
  char len[20];
  sprintf(len, "To do: %u", length);
  gfx_SetTextXY(180, 150);
  gfx_PrintString("-- Current Time --");

  sprintf(buf, "%u:%u:%u", currentTime->hour, currentTime->minute,
          currentTime->second);
  gfx_SetTextScale(2, 2);
  gfx_PrintStringXY(buf, 195, 165);
  gfx_PrintStringXY(len, 180, 20);
}