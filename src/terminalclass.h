/*
  terminalclass.h - Terminal - Processing, Parsing, Executing, and Output Control
  For Terminals.
  Copyright (c) 2025 John J. Gavel.  All right reserved.
*/
#ifndef __TERMINAL_CLASS
#define __TERMINAL_CLASS

#include "features.h"
#include "utility/commandBuffer.h"
#include "utility/queue.h"

#include <Stream.h>

#ifdef TERMINAL_LOGGING
typedef enum { TRACE, INFO, WARNING, ERROR, HELP, PASSED, FAILED, PROMPT } PRINT_TYPES;
#endif

typedef enum { Normal = 0, Black = 30, Red, Green, Yellow, Blue, Magenta, Cyan, White } COLOR;

typedef enum { NO_PROCESSING, HELP_FUNCTION_CALLED, EMPTY_STRING, ERROR_NO_CMD_FOUND } ReadLineReturn;

class Terminal {
public:
  Terminal(Stream* __stream) : inputStream(__stream), outputStream(__stream) { initialize(); };
  Terminal(Stream* __inputStream, Stream* __outputStream) : inputStream(__inputStream), outputStream(__outputStream) { initialize(); };
  void initialize() { memset(parameterParsing, 0, MAX_INPUT_LINE); };
  void configure(Terminal* terminal);
  void setStream(Stream* __stream) {
    inputStream = __stream;
    outputStream = __stream;
  };
  void setStream(Stream* __inputStream, Stream* __outputStream) {
    inputStream = __inputStream;
    outputStream = __outputStream;
  };
  Stream* getInput() { return inputStream; };
  Stream* getOutput() { return outputStream; };
  void setup();
  void loop();
  void println();
#ifdef TERMINAL_COLORS
  void print(COLOR color, String line);
#else
  inline void print(COLOR color, String line) { __print(line); };
#endif
#ifdef TERMINAL_LOGGING
  void print(PRINT_TYPES type, String line);
  void print(PRINT_TYPES type, String line, String line2);
  void println(PRINT_TYPES type, String line);
  void println(PRINT_TYPES type, String line, String line2);
#else
  inline void print(String line) { __print(line); };
  inline void println(String line) { __println(line); };
#endif
#ifdef TERMINAL_HEX_STRING
  void hexdump(unsigned char* buffer, unsigned long length);
#endif
#ifdef TERMINAL_BANNER
  void banner();
#endif
  void prompt();
  char* readParameter();
  void invalidParameter();
  void setEcho(bool __echo) { echo = __echo; };
  bool getEcho() { return echo; };
#ifdef TERMINAL_COLORS
  void useColor(bool __usecolor) { usecolor = __usecolor; };
#endif
  void usePrompt(bool __useprompt) { useprompt = __useprompt; };
  void setPrompt(String __prompt) { promptString = __prompt; };
#ifdef TERMINAL_BANNER
  void setBannerFunction(void function(Terminal*)) { bannerFunction = function; };
#endif
  void clearScreen();
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
  void clearHistory();
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_CONFIGURATION
  static void terminalConfig(Terminal* terminal);
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
  TerminalUtility::Queue lastBuffer = TerminalUtility::Queue(HISTORY_BUFFER, MAX_INPUT_LINE);
#endif

private:
  Stream* inputStream = nullptr;
  Stream* outputStream = nullptr;
  bool echo = true;
#ifdef TERMINAL_COLORS
  bool usecolor = false;
#endif
  bool useprompt = true;
  String promptString = "PROGRAM:\\> ";
#ifdef TERMINAL_BANNER
  void (*bannerFunction)(Terminal*) = nullptr;
#endif

#ifdef TERMINAL_COLORS
  void printColor(COLOR color);
#else
  inline void printColor(COLOR color) {};
#endif
  void __print(String line);
  void __print(char character);
  void __println(String line);
  void __println(char character);

  ReadLineReturn readline();
  ReadLineReturn callFunction();
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
  char* lastCmd();
  int lastCmdIndex = -1;
  unsigned long historyIndex = 0;
#endif

  TerminalUtility::CommandBuffer cmdBuffer;
  char parameterParsing[MAX_INPUT_LINE];
  char* parameterParseSave = nullptr;

  void clearCommandLine();
  void printCommandLine();
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
  void upArrow();
  void downArrow();
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_EDITING
  void rightArrow();
  void leftArrow();
#endif
#ifdef TERMINAL_TAB
  void tab();
#endif
};

#endif
