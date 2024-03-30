#ifndef _HEADERFILE_COMMON
#define _HEADERFILE_COMMON
//= CONSTANTS ======================================================================================
#define SW_VERSION "2024.03.30"
//------------------------------------------------
//------------------------------------------------
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

#define SEC 1000L      // 1 second
#define HOUR 3600000L  // 1 hour in milliseconds

#ifdef DEBUG
#define TIME_TICK 40L
#else
#define TIME_TICK 10L
#endif

void (*resetFunc)(void) = 0;  // declare reset fuction at address 0

//------------------------------------------------
//==================================================================================================
#endif  // _HEADERFILE_COMMON
