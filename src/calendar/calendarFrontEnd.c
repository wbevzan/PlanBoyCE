#include "../color.h"
#include "calendarBackend.h"
#include <graphx.h>
#include <math.h>
#include <stdio.h>
#include <sys/rtc.h>
extern uint16_t yearFocused;
extern uint8_t MonthFocuse;
#define JANUARYDAYS 31
#define FEBRUARYDAYS 28
#define S1LK08DAYS 28
#define MARCHDAYS 31
#define APRILDAYS 30
#define MAYDAYS 31
#define JUNEDAYS 30
#define JULYDAYS 31
#define AUGUSTDAYS 31
#define SEPTEMBERDAYS 30
#define OCTOBERDAYS 31
#define NOVEMBERDAYS 30
#define DECEMBERDAYS 31
char yearName[6]; // year name buffer
// There's a buffer overflow here (I think) but tbh im lazy asf
extern uint8_t cusorPosX;
extern uint8_t cursorPosY;
extern uint8_t MonthFocused;
extern uint16_t yearFocused;
// Stores the offset from 30 days within each month, note that february is
// checked sperately.
const uint8_t monthDayCount[12] = {JANUARYDAYS, FEBRUARYDAYS, MARCHDAYS,
                                   APRILDAYS,   MAYDAYS,      JUNEDAYS,
                                   JULYDAYS,    AUGUSTDAYS,   SEPTEMBERDAYS,
                                   OCTOBERDAYS, NOVEMBERDAYS, DECEMBERDAYS};
// Array of month offsets
const char monthNames[12][10] = {
    "January", "February", "March",     "April",   "May",      "June",
    "July",    "August",   "September", "October", "November", "December"};
// Array of month names
const char dayNames[7][3] = {"Su", "Mo", "Tu", "W", "Th", "Fr", "Sa"};

void drawCalendar() {
  /*
Really large function that draws the calendar menu
I've hated every second ive spent writing this, but its atleast
better than studying for ap physics
*/
  // Draw the top with the year and stuff
  // ooh getting fancy with the register keyword
  register uint8_t currentDay = 0;
  uint8_t firstDayOfWeekOfMonth;
  char buf[4];
  gfx_SetTextScale(2, 2);
  gfx_SetColor(BLACK);
  sprintf(yearName, "%u", yearFocused);
  gfx_PrintStringXY(yearName, 30, 2);
  gfx_PrintStringXY(monthNames[MonthFocused - 1], 100, 2);
  gfx_SetTextScale(1, 1);
  // Draw the separator
  for (uint8_t i = 0; i < 5; i++) {
    gfx_Line(0, 20 + i, GFX_LCD_WIDTH, 20 + i);
  }
  for (uint8_t i = 0; i < 7; i++) {
    gfx_PrintStringXY(dayNames[i], i * 32, 30);
  }
  // Start drawing stuff
  Date _tempday = {yearFocused, MonthFocused, 1};
  uint8_t daysInMonth = monthDayCount[MonthFocused - 1];
  // Leap year
  if (MonthFocused == 2 && yearFocused % 4 == 0) {
    daysInMonth = 29;
  }
  firstDayOfWeekOfMonth = calculateDayOfWeek(&_tempday);
  // begin drawing
  for (uint8_t i = firstDayOfWeekOfMonth; i < 7; i++) {
    currentDay++;
    sprintf(buf, "%u", currentDay);
    gfx_Rectangle((32 * i), 40, 30, 30);
    gfx_PrintStringXY(buf, (32 * i) + 5, 45);
  }
  // this took me so long to figure out omg
  for (uint8_t i = 1; i < 8 /*Set to a large number cuz why not*/; i++) {
    // Draw a row
    for (uint8_t n = 0; n < 7; n++) {
      // draw a box
      if (currentDay >= daysInMonth) {
        return;
      }
      currentDay++;
      sprintf(buf, "%u", currentDay);
      gfx_Rectangle((32 * n), (31 * i) + 40, 30, 30);
      gfx_PrintStringXY(buf, (32 * n) + 5, (31 * i) + 45);
    }
  }
}
// I showed ryan what the calendar looked like and she was impressed :D
// I am now satisfied with life
//(If she saw my spaghetti code i dont think she'd be proud of me)