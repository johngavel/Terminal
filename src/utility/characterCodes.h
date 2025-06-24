#ifndef __TERMINAL_CHARACTER_CODES
#define __TERMINAL_CHARACTER_CODES

#define BS_CHAR (char) 0x08
#define HT_CHAR (char) 0x09
#define NL_CHAR (char) 0x0A
#define CR_CHAR (char) 0x0D
#define DEL_CHAR (char) 0x7F
#define ESC_CHAR (char) 0x1B

#define VT100_UP_ARROW "\x1B[A"
#define VT100_DOWN_ARROW "\x1B[B"
#define VT100_RIGHT_ARROW "\x1B[C"
#define VT100_LEFT_ARROW "\x1B[D"
#define VT100_CLEAR_SCREEN "\x1B[2J"
#define VT100_SET_CURSOR_HOME "\x1B[0;0H"

#endif