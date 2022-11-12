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
const byte ACTION_5 = 5;
//
const byte ACTION_6 = 6;
const byte ACTION_7 = 7;
//
const byte ACTION_8 = 8; // Not used

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
  /*buttons*/     { KitchenHoodRemote.button1, Remote1.button1, Remote2.button1 }
};
//------------------------------------------------
Action ActionVent2 {
  /*name*/        "Vent-2",
  /*description*/ "Vent Speed 2",
  /*actionCode*/  ACTION_2,
  /*function*/    0,
  /*buttons*/     { KitchenHoodRemote.button2, Remote2.button2 }
};
//------------------------------------------------
Action ActionVent3 {
  /*name*/        "Vent-3",
  /*description*/ "Vent Speed 3",
  /*actionCode*/  ACTION_3,
  /*function*/    0,
  /*buttons*/     { KitchenHoodRemote.button3, Remote2.button3 }
};
//------------------------------------------------
//------------------------------------------------
Action ActionVentOff {
  /*name*/        "Vent-0ff",
  /*description*/ "Stop Ventilation",
  /*actionCode*/  ACTION_4,
  /*function*/    0,
  /*buttons*/     {  }
};
//------------------------------------------------
Action ActionVentOn {
  /*name*/        "Vent-On",
  /*description*/ "Start Ventilation",
  /*actionCode*/  ACTION_5,
  /*function*/    0,
  /*buttons*/     {  }
};
//------------------------------------------------
//------------------------------------------------
Action ActionVent3Vent1Short {
  /*name*/        "VS3 1m->VS1",
  /*description*/ "VentS3(*1m) -> VentS1",
  /*actionCode*/  ACTION_6,
  /*function*/    0,
  /*buttons*/     { Remote1.button2, ProgRemote1.button1 }
};
//------------------------------------------------
Action ActionVent3Vent1Long {
  /*name*/        "VS3 5m->VS1",
  /*description*/ "VentS3(*5m) -> VentS1",
  /*actionCode*/  ACTION_7,
  /*function*/    0,
  /*buttons*/     { Remote1.button3, Remote1.button4, ProgRemote1.button2 }
};
//------------------------------------------------



//------------------------------------------------
const Action *Actions[] = { &ActionVent1, &ActionVent2, &ActionVent3, &ActionVentOff, &ActionVentOn, &ActionVent3Vent1Short, &ActionVent3Vent1Long };
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
