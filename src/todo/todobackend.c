#include <fileioc.h>
#include <stdio.h>
#define LINES_PER_TASK 8
/*
FILE FORMAT SPECIFICATION

uint8_t NUMBEROFTASKS + \n
uint8_t nextOpenLine + \n

00\n
00\n

char* __TASKNAME + \n
uint16_t currentProgress + \n
uint16_t maxProgress + \n
char* STEP1 + \n
char* STEP2 + \n
char* STEP3 + \n
char* STEP4 + \n
char* STEP5 + \n
*/
extern char tempNameBuffer[20];
extern char stepBuffer[5][20];
extern uint16_t temporaryGoalAmount;
uint8_t fileHandle;
#define SAVEDATA "TASKDATA"
uint8_t length = 0;
typedef struct TaskProgression {
  uint16_t currentProgress;
  uint16_t max;
  char steps[5][20];
} TaskProgression;

typedef struct task {
  char name[20];
  TaskProgression *progress;
} Task;
extern char tempNameBuffer[20];
extern char stepBuffer[5][20];
extern uint16_t temporaryGoalAmount;
uint16_t nextOpenLine = 2;
// formats a new file properly
void formatFile() {
  const char *firstLine = "00\n";
  for (uint8_t i = 0; i < 3; i++) {
    ti_PutC(firstLine[i], fileHandle);
  }
  ti_Rewind(fileHandle);
}
// creates the task file if it doesn't exist
void createTaskFile() {
  fileHandle = ti_Open(SAVEDATA, "r");
  if (fileHandle == 0) {
    fileHandle = ti_Open(SAVEDATA, "w+");
    formatFile();
  } else {
    fileHandle = ti_Open(SAVEDATA, "r+");
  }
}

uint8_t charToDigit(const char character) {
  // This is now officially bloatware
  switch (character) {
  case '1':
    return 1;
  case '2':
    return 2;
  case '3':
    return 3;
  case '4':
    return 4;
  case '5':
    return 5;
  case '6':
    return 6;
  case '7':
    return 7;
  case '8':
    return 8;
  case '9':
    return 9;
  case '0':
    return 0;
  }
}

char digitToChar(const uint8_t digit) {
  // bloatware lookup table
  switch (digit) {
  case 1:
    return '1';
  case 2:
    return '2';
  case 3:
    return '3';
  case 4:
    return '4';
  case 5:
    return '5';
  case 6:
    return '6';
  case 7:
    return '7';
  case 8:
    return '8';
  case 9:
    return '9';
  case 0:
    return '0';
  }
}

void readLength() {
  length = 0;
  nextOpenLine = 0;
  // read length
  length += charToDigit(ti_GetC(fileHandle));
  if (ti_GetC(fileHandle) == '1') {
    length += 10;
  }

  ti_Seek(1, SEEK_CUR, fileHandle); // ignore newline
  // read next open line
  nextOpenLine += charToDigit(ti_GetC(fileHandle));
  nextOpenLine += charToDigit(ti_GetC(fileHandle)) * 10;
  nextOpenLine += charToDigit(ti_GetC(fileHandle)) * 100;
  // okie we good
  ti_Rewind(fileHandle);
}
void seekToNextTaskSpace() {
  if (length == 0) {
    return; // we're already at the thingy lol
  }
  ti_Seek(6, SEEK_SET, fileHandle);
  uint32_t charsToSeek = 0;
  for (uint16_t i = 0; i < nextOpenLine; i++) {
    // loops through each line until it gets there
    while (ti_GetC(fileHandle) != '\n') {
      charsToSeek++;
    }
    charsToSeek++;
  }
  ti_Seek(charsToSeek, SEEK_CUR, fileHandle);
}

void initData() {
  createTaskFile();
  readLength();
}

void uint16ToStr(const uint16_t num, char *str) {
  uint8_t digit = 0;
  uint16_t temp = num;
  // check for 0
  if (num == 0) {
    for (uint8_t i = 0; i < 5; i++) {
      str[i] = '0';
    }
    str[5] = '\0';
    return;
  }
  for (uint8_t i = 5; i > 0; i--) {
    digit = temp % 10;
    temp /= 10;
    str[i] = digitToChar(digit);
  }
  str[5] = '\0';
}

Task *readTask(uint8_t taskNum) {
  // create variables
  char progressStr[6];
  char goalStr[6];
  uint16_t progress = 0;
  uint16_t goal = 0;
  TaskProgression *taskProgress = malloc(sizeof(TaskProgression));
  Task *task = malloc(sizeof(Task));
  // seek to the task selected
  for (uint8_t i = 0; i < taskNum; i++) {
    seekToNextTaskSpace();
  }
  // yoink name
  for (uint8_t i = 0; i < 19; i++) {
    task->name[i] = ti_GetC(fileHandle);
  }
  task->name[19] = 0;
  // skip newline
  ti_Seek(1, SEEK_CUR, fileHandle);
  // start getting progress
  for (uint8_t i = 0; i < 5; i++) {
    progressStr[i] = ti_GetC(fileHandle);
  }
  progressStr[5] = 0; // add null terminator
  {
    uint8_t place = 1;
    for (uint8_t i = 5; i < 0; i++) {
      progress += charToDigit(progressStr[i - 1]) * place;
      place *= 10;
    }
  }
  ti_Seek(1, SEEK_CUR, fileHandle);
  for (uint8_t i = 0; i < 5; i++) {
    goalStr[i] = ti_GetC(fileHandle);
  }
  goalStr[5] = 0;
  {
    uint8_t place = 1;
    for (uint8_t i = 0; i < 5; i++) {
      goal += charToDigit(goalStr[i]) * place;
      place *= 10;
    }
  }
  // grab steps
  for (uint8_t i = 0; i < 5; i++) {
    for (uint8_t n = 0; n < 19; i++) {
      char temp = ti_GetC(fileHandle);
      if (temp == '\n') {
        goto NEXTLINE;
      }
      taskProgress->steps[i][n] = temp;
    }
  NEXTLINE:
    ti_Seek(1, SEEK_CUR, fileHandle);
  }
}

void createTask() {
  readLength();
  seekToNextTaskSpace();
  // write name
  for (uint8_t i = 0; i < 19; i++) {
    ti_PutC(tempNameBuffer[i], fileHandle);
  }
  ti_PutC('\n', fileHandle);
  {
    // put progress there too
    const char *str = "00000";
    for (uint8_t i = 0; i < 5; i++) {
      ti_PutC(str[i], fileHandle);
    }
  }
  ti_PutC('\n', fileHandle);
  // put goal amount
  {
    char str[6];
    uint16ToStr(temporaryGoalAmount, str);
    for (uint8_t i = 0; i < 5; i++) {
      ti_PutC(str[i], fileHandle);
    }
  }
  ti_PutC('\n', fileHandle);
  // Write steps
  for (uint8_t i = 0; i < 5; i++) {
    for (uint8_t stepIndex = 0; stepIndex < 19; stepIndex++) {
      if (stepBuffer[i][stepIndex] == '0') {
        ti_PutC('\n', fileHandle);
        goto NEWLINE;
      }
      if (stepBuffer[i][stepIndex] == '\n') {
        goto NEWLINE;
      }
      ti_PutC(stepBuffer[i][stepIndex], fileHandle);
    }
  NEWLINE:
    ti_PutC('\n', fileHandle);
  }
}
