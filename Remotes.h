#ifndef _HEADERFILE_REMOTES
#define _HEADERFILE_REMOTES

//= CONSTANTS ======================================================================================
struct Remote {
  unsigned long buttonCnt;
  char *name;
  unsigned long button1;
  unsigned long button2;
  unsigned long button3;
  unsigned long button4;
};
//------------------------------------------------
/*
   TELECOMANDA PATRATA (123)    => Hota
   TELECOMANDA 4 butoane (ABCD) => Baie
   TELECOMANDA 3 butoane (123)  => Toilete
*/
//------------------------------------------------
const Remote Remote1 {
  /*buttonCnt*/ 3,
  /*name*/      "hota",
  /*button1*/   9094708,
  /*button2*/   9094705,
  /*button3*/   9094706,
  /*button4*/   0
};
//------------------------------------------------
const Remote Remote2 {
  /*buttonCnt*/ 4,
  /*name*/      "alba 4 butoane",
  /*button1*/   6145640,
  /*button2*/   6145636,
  /*button3*/   6145634,
  /*button4*/   6145633
};
//------------------------------------------------
const Remote Remote3 {
  /*buttonCnt*/ 3,
  /*name*/      "alba 3 butoane",
  /*button1*/   9468385,
  /*button2*/   9468386,
  /*button3*/   9468388,
  /*button4*/   0
};
//------------------------------------------------

//------------------------------------------------
//==================================================================================================
#endif // _HEADERFILE_REMOTES
