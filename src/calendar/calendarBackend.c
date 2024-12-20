/*
Why is this file so much more commented than anything else in my program lol
*/
#include <math.h>
#include <stdlib.h>
#include <tice.h>
uint8_t MonthFocused = 1;
uint16_t yearFocused = 2015;
uint8_t cusorPosX;
uint8_t cursorPosY;
#include "../inputandstuff.h"
extern bool somethingChanged;
typedef struct time {
  uint8_t minute;
  uint8_t second;
  uint8_t hour; // 24 hour time
} Time;
Time *currentTime;
typedef struct date {
  uint16_t year;
  uint8_t month;
  uint8_t day;
} Date;
Date *setupTime() {
  // sets up time variables, probably really bad lol
  currentTime = malloc(sizeof(Time));
  uint8_t day;
  uint8_t month;
  uint16_t year;
  boot_GetDate(&day, &month, &year);
  //I probably don't need to malloc this, but malloc makes me feel cool
  Date *today = malloc(sizeof(Date));
  today->year = year;
  today->month = month;
  today->day = day;
  MonthFocused = month;
  yearFocused = year;
  return today;
}
void getClockTime(Time *time) {
  /*
  Takes a pointer to the current time (malloc'ed on heap)
  and updates it
  Could prob be optimized a lot but hey we have a lot of spare clock cycles
  */
  uint8_t seconds, minutes, hours; //mom look i didn't use 3 lines!!!
  boot_GetTime(&seconds, &minutes, &hours);
  time->second = seconds;
  time->minute = minutes;
  time->hour = hours;
  somethingChanged = true;
}
uint8_t calculateDayOfWeek(Date *date) {
  /*
  Calculates the day of the week given a date parameter
  uses Zeller's congruence which is a supa cool formula
  */
  // setting up those pesky variables
  uint8_t day = (date->day + 1) % 7;
  uint8_t month = date->month;
  uint16_t year = date->year; // apparently unused, womp womp too lazy to fix
  if (month <= 2) {
    // January and february are 13th and 14th
    month = month + 12;
    year--;
  };
  uint16_t century = date->year / 100;
  uint8_t yearOfCentury = date->year % 100;
  // The geeks for geeks page for this USES LETTERS INSTEAD OF READABLE VARAIBLE
  // NAMES GUHQWGIUOQIWOUGOIUQWEGUIHQWOIGEUOQIWEGOIH
  return (((day + 13 * (month + 1) / 5 + yearOfCentury + yearOfCentury / 4 +
            century / 4 + 5 * century) %
           7) +
          5) %
         7;
}