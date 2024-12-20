#include <graphx.h>
#ifndef CALENDARBACKEND_H
typedef struct date {
  uint16_t year;
  uint8_t month;
  uint8_t day;
} Date;
typedef struct time {
  uint8_t minute;
  uint8_t second;
  uint8_t hour; // 24 hour time
} Time;
void getClockTime(Time *time);
uint8_t calculateDayOfWeek(Date *today);
Date *setupTime();
#define CALENDARBACKEND_H
#endif