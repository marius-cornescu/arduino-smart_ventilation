#ifndef _HEADERFILE_COMM_COMMON
#define _HEADERFILE_COMM_COMMON
//= CONSTANTS ======================================================================================
//------------------------------------------------
// 2 bytes for 0 to 99 number
const byte ACTION_PAYLOAD_SIZE = 2;
const byte PAYLOAD_ACTION_START = 0;

// 25 bytes for Action Label
const byte LABEL_PAYLOAD_SIZE = 25;
const byte PAYLOAD_LABEL_START = ACTION_PAYLOAD_SIZE + 1;
// 
const byte PAYLOAD_SIZE = ACTION_PAYLOAD_SIZE + 1 + LABEL_PAYLOAD_SIZE + 1;
//==================================================================================================
#endif  // _HEADERFILE_COMM_PROTOCOL