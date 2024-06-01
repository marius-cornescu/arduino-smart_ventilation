#ifndef _HEADERFILE_REMOTES
#define _HEADERFILE_REMOTES

//= CONSTANTS ======================================================================================
struct Remote {
  const char* name;
  unsigned long button1;
  unsigned long button2;
  unsigned long button3;
  unsigned long button4;
};
//------------------------------------------------
/*
   TELECOMANDA PATRATA (123)    => Hota
   TELECOMANDA 4 butoane (ABCD) => Toilete
   TELECOMANDA 3 butoane (123)  => Baie
*/
//------------------------------------------------
const Remote KitchenHoodRemote {
  /*name*/      "hota",
  /*button1*/   9094708,
  /*button2*/   9094705,
  /*button3*/   9094706,
  /*button4*/   0
};
//------------------------------------------------
const Remote Remote1 {
  /*name*/      "alba 4 butoane",
  /*button1*/   6145640,
  /*button2*/   6145636,
  /*button3*/   6145634,
  /*button4*/   6145633
};
//------------------------------------------------
const Remote Remote2 {
  /*name*/      "alba 3 butoane",
  /*button1*/   9468385,
  /*button2*/   9468386,
  /*button3*/   9468388,
  /*button4*/   0
};
//------------------------------------------------
const Remote ProgRemote1 {
  /*name*/      "Black Programable remote 4 buttons",
  /*button1*/   9037301, // generated code
  /*button2*/   9037302, // generated code
  /*button3*/   9037303, // generated code
  /*button4*/   9037304  // generated code
};
//------------------------------------------------
const Remote ProgRemote2 {
  /*name*/      "Black Programable remote 4 buttons",
  /*button1*/   9037305, // generated code
  /*button2*/   9037306, // generated code
  /*button3*/   9037307, // generated code
  /*button4*/   9037308  // generated code
};
//------------------------------------------------
const Remote RemoteWithAntenna {
  /*name*/      "alba 4 butoane - antenna",
  /*button1*/   2658088,
  /*button2*/   2658084,
  /*button3*/   2658082,
  /*button4*/   2658081
};
//------------------------------------------------

//------------------------------------------------
//==================================================================================================
#endif // _HEADERFILE_REMOTES
