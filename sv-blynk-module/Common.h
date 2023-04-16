#ifndef _HEADERFILE_COMMON
#define _HEADERFILE_COMMON
//= CONSTANTS ======================================================================================
#define SW_VERSION "2023.04.08"
//------------------------------------------------
//------------------------------------------------
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

#define SEC 1000L  // 1 second

#ifdef DEBUG
#define TIME_TICK 40L
#else
#define TIME_TICK 10L
#endif

//------------------------------------------------

//------------------------------------------------
//==================================================================================================
#endif // _HEADERFILE_COMMON
