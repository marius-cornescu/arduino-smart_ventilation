#ifndef _HEADERFILE_ACTIONS
#define _HEADERFILE_ACTIONS

//= CONSTANTS ======================================================================================
//------------------------------------------------
typedef void (*ActionFunction) ();
//------------------------------------------------
struct Action {
  const char *name;
  const char *description;
  const byte actionCode;
  ActionFunction function;
  const unsigned long buttons[4];
};
//------------------------------------------------

const byte ACTION_MAX_VALID = 90; // any action with higher value will be ignored
//
const byte ACTION_NOP = 0;
//
const byte ACTION_1 = 1;
const byte ACTION_2 = 2;
const byte ACTION_3 = 3;
//
const byte ACTION_4 = 4;
const byte ACTION4_DELAY = 5; // not used
//
const byte ACTION_5 = 5;
const byte ACTION_6 = 6;
//
const byte ACTION_7 = 7; // not used

//------------------------------------------------
Action NoAction {
  /*name*/        "NOP",
  /*description*/ "No Action",
  /*actionCode*/  ACTION_NOP,
  /*function*/    0,
  /*buttons*/     {  }
};
//------------------------------------------------

//------------------------------------------------
Action ActionVent1 {
  /*name*/        "Vent-1",
  /*description*/ "Vent Speed 1",
  /*actionCode*/  ACTION_1,
  /*function*/    0,
  /*buttons*/     {  }
};
//------------------------------------------------
Action ActionVent2 {
  /*name*/        "Vent-2",
  /*description*/ "Vent Speed 2",
  /*actionCode*/  ACTION_2,
  /*function*/    0,
  /*buttons*/     {  }
};
//------------------------------------------------
Action ActionVent3 {
  /*name*/        "Vent-3",
  /*description*/ "Vent Speed 3",
  /*actionCode*/  ACTION_3,
  /*function*/    0,
  /*buttons*/     {  }
};
//------------------------------------------------
Action ActionVent3Vent1 {
  /*name*/        "VSp3->VSp1",
  /*description*/ "Vent-S3(*5m) -> Vent-S1",
  /*actionCode*/  ACTION_4,
  /*function*/    0,
  /*buttons*/     {  }
};
//------------------------------------------------
//------------------------------------------------
Action ActionVentOff {
  /*name*/        "Vent-0ff",
  /*description*/ "Stop Ventilation",
  /*actionCode*/  ACTION_5,
  /*function*/    0,
  /*buttons*/     {  }
};
//------------------------------------------------
Action ActionVentOn {
  /*name*/        "Vent-On",
  /*description*/ "Start Ventilation",
  /*actionCode*/  ACTION_6,
  /*function*/    0,
  /*buttons*/     {  }
};
//------------------------------------------------


//------------------------------------------------
const Action *Actions[] = { &ActionVent1, &ActionVent2, &ActionVent3, &ActionVent3Vent1, &ActionVentOff, &ActionVentOn };
//==================================================================================================
const Action *getActionByActionCode(byte actionCode) {
  for (byte act = 0; act < ARRAY_LEN(Actions); act++) {
    const Action *action = Actions[act];
    if (action->actionCode == actionCode) {
      return action;
    }
  }
  return &NoAction;
}
//==================================================================================================
#endif // _HEADERFILE_ACTIONS
