#ifndef _HEADERFILE_COMM_COMMON
#define _HEADERFILE_COMM_COMMON
//= CONSTANTS ======================================================================================
// 1 bytes for 0 to 9 number
const byte SPEED_PAYLOAD_SIZE = 1;
const byte PAYLOAD_SPEED_START = 0;

// 2 bytes for 0 to 99 number
const byte ACTION_PAYLOAD_SIZE = 2;
const byte PAYLOAD_ACTION_START = PAYLOAD_SPEED_START + SPEED_PAYLOAD_SIZE + 1;

// 25 bytes for Action Label
const byte LABEL_PAYLOAD_SIZE = 25;
const byte PAYLOAD_LABEL_START = PAYLOAD_ACTION_START + ACTION_PAYLOAD_SIZE + 1;
// 
const byte PAYLOAD_SIZE = SPEED_PAYLOAD_SIZE + 1 + ACTION_PAYLOAD_SIZE + 1 + LABEL_PAYLOAD_SIZE + 1;

//= VARIABLES ======================================================================================
//
char payload[PAYLOAD_SIZE];
//------------------------------------------------
//==================================================================================================
#endif  // _HEADERFILE_COMM_COMMON
