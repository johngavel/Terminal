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
#define VT100_SET_CURSOR_BEGIN "\x1B[80D" // The 80 is specified because on the MAX_INPUT_LINE of the command_buffer.h
#define VT100_LEFT_CURSOR "\x1B[1D"
#define VT100_RIGHT_CURSOR "\x1B[1C"
#define VT100_ERASE_LINE "\x1B[2K"

#define TERMINAL_SHOW_CURSOR "\x1B[?25h"

#endif