#ifndef __TERMINAL_FEATURES
#define __TERMINAL_FEATURES

/* Defined Feature Macros to turn on/off specific features - This can save memory or computing power */

#define TERMINAL_COLORS      // Gives the Terminal the escape code to print in color
#define TERMINAL_TAB         // Tab Auto-Complete Feature
#define TERMINAL_LOGGING     // Logging features of TRACE, INFO, WARNING, ERROR, HELP, PASSED, FAILED, PROMPT
#define TERMINAL_BANNER      // User generated banner that is used at start-up
#define TERMINAL_HEX_STRING  // Diagnostics prints that do HEX printing for the user

#define TERMINAL_STANDARD_COMMANDS_TERMINAL_HELP
#define TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
#define TERMINAL_STANDARD_COMMANDS_TERMINAL_EDITING
#define TERMINAL_STANDARD_COMMANDS_TERMINAL_CLEAR
#define TERMINAL_STANDARD_COMMANDS_TERMINAL_RESET
#define TERMINAL_STANDARD_COMMANDS_TERMINAL_ECHO
#define TERMINAL_STANDARD_COMMANDS_TERMINAL_CONFIGURATION
#define TERMINAL_STANDARD_COMMANDS_TERMINAL_DIAGNOSTICS

/*******************************************************/

#define MAX_TERM_CMD 50
#define MAX_INPUT_LINE 80

#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
#define HISTORY_BUFFER 10
#endif

#endif