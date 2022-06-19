#ifndef _HEADERFILE_ACTIONS
#define _HEADERFILE_ACTIONS

#define ARRAY_LEN(array) (sizeof(array)/sizeof(array[0]))

#ifdef DEBUG
#define ARRAY_PRINT(array) (sizeof(array)/sizeof(array[0]))
#endif

//= CONSTANTS ======================================================================================
//------------------------------------------------
const byte ACTION_MAX_VALID = 90; // any action with higher value will be ignored
const byte ACTION_UNKNOWN = 99;
const byte ACTION_NOP = 100;
//
const byte ACTION_1 = 1;
const byte ACTION_2 = 2;
const byte ACTION_3 = 3;
//
const byte ACTION_4 = 4;
//
const byte ACTION_5 = 5;
const byte ACTION_6 = 6;

//------------------------------------------------
struct Action {
  char *name;
  byte actionCode;
  unsigned int buttonCnt;
  unsigned long buttons[4];
};

//------------------------------------------------
const Action Action1 {
  /*name*/        "Vent-1",
  /*actionCode*/  ACTION_1,
  /*buttonCnt*/   3,
  /*buttons*/     { Remote1.button1, Remote2.button1, Remote3.button1 }
};
//------------------------------------------------
const Action Action2 {
  /*name*/        "Vent-2",
  /*actionCode*/  ACTION_2,
  /*buttonCnt*/   3,
  /*buttons*/     { Remote1.button2, Remote2.button2, Remote3.button2 }
};
//------------------------------------------------
const Action Action3 {
  /*name*/        "Vent-3",
  /*actionCode*/  ACTION_3,
  /*buttonCnt*/   3,
  /*buttons*/     { Remote1.button3, Remote2.button3, Remote3.button3 }
};
//------------------------------------------------
const Action Action4 {
  /*name*/        "Vent-3(*5m)->Vent-1",
  /*actionCode*/  ACTION_4,
  /*buttonCnt*/   1,
  /*buttons*/     { Remote2.button4 }
};
//------------------------------------------------
//------------------------------------------------

//------------------------------------------------
//==================================================================================================
Action *Actions[4] = { &Action1,  &Action2, &Action3, &Action4 };
//==================================================================================================
#endif // _HEADERFILE_ACTIONS
