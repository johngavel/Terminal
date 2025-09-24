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

/* Weaknesses / Limitations

⚠️ Tokenizer is fixed – only splits on spaces; no quotes/escaped args.
⚠️ Global singleton (TERM_CMD) – simplifies API but reduces modularity.
⚠️ Blocking I/O assumption – designed for loop() polling, not async.
⚠️ Minimal error handling – bad input mostly ignored.
⚠️ No namespaces – commands live in a flat registry; collisions possible.
⚠️ Memory footprint – holds command history; not tuned for very low-RAM MCUs.
 */
#endif