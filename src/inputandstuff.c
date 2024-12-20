#include "inputandstuff.h"
#include "./calendar/calendarBackend.h"
#include "color.h"
#include <graphx.h>
#include <stdbool.h>
#include <stdio.h>
#include <ti/getcsc.h>
#include <ti/getkey.h>
#include <time.h>
/// @brief cursor position
uint8_t cursorPos = 0;
/// @brief check if screen has to be redrawn
bool somethingChanged = true;
/// @brief last key code hit by the calculator
uint8_t lastKeyCode;
uint8_t currentMenu = MAINMENU;
// This is such a lazy way to handle this lmao
bool canBackspace = true;
extern uint8_t MonthFocused;
extern uint16_t yearFocused;
// I stole this from one of the code expamples so it probably works
const char *chars = "\0\0\0\0\0\0\0\0\0\0\"WRMH\0\0?[VQLG\0\0:ZUPKFC\0 "
                    "YTOJEB\0\0XSNIDA\0\0\0\0\0\0\0\0";
void processMenuCursor(uint8_t limit) {
  /*
  processes the menu cursor
  */
  if (lastKeyCode == sk_Down) {
    somethingChanged = true;
    if (cursorPos == limit) {
      cursorPos = 0;
    } else {
      cursorPos++;
    }
  } else if (lastKeyCode == sk_Up) {
    somethingChanged = true;
    if (cursorPos == 0) {
      cursorPos = limit;
    } else {
      cursorPos--;
    }
  }
}



void processCalendarInput() {
  if (lastKeyCode == sk_Right) {
    somethingChanged = true;
    MonthFocused++;
    if (MonthFocused == 13) {
      MonthFocused = 1;
      yearFocused++;
    }
  } else if (lastKeyCode == sk_Left) {
    somethingChanged = true;
    MonthFocused--;
    if (MonthFocused == 0) {
      MonthFocused = 12;
      yearFocused--;
    }
  }
}

void processInput() {
  lastKeyCode = os_GetCSC();
  if (lastKeyCode == sk_Enter) {
    somethingChanged = true;
  }

  switch (currentMenu) {
  case MAINMENU:
    processMenuCursor(2);
    break;
  case TODOMENU:
    processMenuCursor(1);
    break;
  case CALENDAR:
    processCalendarInput();
  }
}
