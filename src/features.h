#ifndef __TERMINAL_FEATURES
#define __TERMINAL_FEATURES

#define TERMINAL_COLORS
#define TERMINAL_TAB
#define TERMINAL_LOGGING
#define TERMINAL_BANNER
#define TERMINAL_HEX_STRING

#define TERMINAL_STANDARD_COMMANDS_TERMINAL_HELP
#define TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
#define TERMINAL_STANDARD_COMMANDS_TERMINAL_CONTROL
#define TERMINAL_STANDARD_COMMANDS_TERMINAL_CONFIGURATION

#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
#define HISTORY_BUFFER 10
#endif

/*-------------------------------------------------------
Notes:
1. Refactor for MAX_INPUT_LINE into this file
2. Add custom parameterizaiton
3. Get all of the Features working
--------------------------------------------------------*/

#endif