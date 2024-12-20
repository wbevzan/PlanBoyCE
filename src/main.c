/*
LGPLv3 Only, see https://www.gnu.org/licenses/gpl-3.0.txt
*/
#include "./calendar/calendarBackend.h"
#include "./calendar/calendarFrontEnd.h"
#include "./todo/todobackend.h"
#include "./todo/todomenu.h"
#include "color.h"
#include "inputandstuff.h"
#include "mainmenu.h"
#include <graphx.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/timers.h>
#include <ti/getcsc.h>
#include <ti/getkey.h>
#include <time.h>
extern Time *currentTime;
/// @brief cursor position
extern uint8_t cursorPos;
/// @brief check if screen has to be redrawn
/// @brief how do i use doxygen?
extern bool somethingChanged;
extern bool canBackspace;
extern char stepBuffer[5][20];
extern size_t temporaryGoalAmount;
/// @brief last key code hit by the calculator
extern uint8_t lastKeyCode;
extern int8_t currentMenu;
extern uint16_t nextOpenLine;

int main(void) {
  Date *today = setupTime();
  initData();
  readLength();
  gfx_Begin();
  while (lastKeyCode != sk_Clear) {
    // TRANS RIGHTS!
    processInput();
    getClockTime(currentTime);
    if (lastKeyCode == sk_Del && canBackspace) {
      currentMenu = MAINMENU;
      cursorPos = 0;
      goto redraw;
    }
  redraw:
    gfx_SetDrawBuffer();
    gfx_FillScreen(WHITE);
    switch (currentMenu) {
    case MAINMENU: {
      /*
      Main menu code, from here, you can go to the todolist, calendar
      and grades menu
      */
      if (lastKeyCode == sk_Enter) {
        // if somethings selected change to it
        switch (cursorPos) {
        case 0: {
          // todomenu was selected
          currentMenu = TODOMENU;
          lastKeyCode = 0;
          cursorPos = 0;
          readLength();
          goto redraw;
        } break;
        case 2: {
          // calendar menu was selected
          currentMenu = CALENDAR;
          lastKeyCode = 0;
          cursorPos = 0;
          goto redraw;
        } break;
        case 1: {
          // grades menu selected
          currentMenu = GRADES;
          cursorPos = 0;
          lastKeyCode = 0;
          goto redraw;
        } break;
        }
      }
    }
      drawMainMenu();
      break;
    case TODOMENU: {
      drawTodoMenu();
      if (lastKeyCode == sk_Enter) {
        switch (cursorPos) {
        case 1:
          // add was selected
          currentMenu = ADDNAMEMENU;
          cursorPos = 0;
          enterPromptMenu();
          goto redraw;
          break;
        }
      }
      break;
    case CALENDAR: {
      // calendar menu
      drawCalendar(today);
    } break;
    case ADDNAMEMENU: {
      enterPromptMenu();
      drawAddNameMenu();
      currentMenu = ADDGOALMENU;
    } break;
    case ADDGOALMENU: {
      drawAddProgressMenu();
      switch (cursorPos) {
      case 0:
        stepBuffer[0][0] = '\0'; // no step
        temporaryGoalAmount = 1;
        break;
      case 1:
        drawAddStepMenu(drawGetStepsMenu());
        break;
      case 2:
        drawGetNumericalValue();
        break;
      }
      createTask();
      exitPromptMenu();
      currentMenu = TODOMENU;
      break;
    }
    }
    }
    gfx_SwapDraw();
    somethingChanged = false;
  }
  // exit the program
  gfx_End();
  // I don't actually know if i need to free this stuff but why not
  free(currentTime);
  free(today);
  return 0;
}