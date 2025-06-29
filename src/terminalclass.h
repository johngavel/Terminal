/*
  terminalclass.h - Terminal - Processing, Parsing, Executing, and Output Control
  For Terminals.
  Copyright (c) 2025 John J. Gavel.  All right reserved.
*/
#ifndef __TERMINAL_CLASS
#define __TERMINAL_CLASS

#define HISTORY 10

#include "utility/commandBuffer.h"
#include "utility/queue.h"

#include <Stream.h>

typedef enum { TRACE, INFO, WARNING, ERROR, HELP, PASSED, FAILED, PROMPT } PRINT_TYPES;

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
  void print(COLOR color, String line);
  void print(PRINT_TYPES type, String line);
  void print(PRINT_TYPES type, String line, String line2);
  void println(PRINT_TYPES type, String line);
  void println(PRINT_TYPES type, String line, String line2);
  void hexdump(unsigned char* buffer, unsigned long length);
  void banner();
  void prompt();
  char* readParameter();
  void invalidParameter();
  void setEcho(bool __echo) { echo = __echo; };
  bool getEcho() { return echo; };
  void useColor(bool __usecolor) { usecolor = __usecolor; };
  void useBS(bool __usebackspace);
  void useDel(bool __usedelete);
  void usePrompt(bool __useprompt) { useprompt = __useprompt; };
  void setPrompt(String __prompt) { promptString = __prompt; };
  void setBannerFunction(void function(Terminal*)) { bannerFunction = function; };
  void clearScreen();
  void clearHistory();
  Queue lastBuffer = Queue(HISTORY, MAX_INPUT_LINE);

private:
  Stream* inputStream = nullptr;
  Stream* outputStream = nullptr;
  bool echo = true;
  bool usecolor = false;
  bool usebackspace = false;
  bool usedelete = true;
  bool useprompt = true;
  String promptString = "PROGRAM:\\> ";
  void (*bannerFunction)(Terminal*) = nullptr;

  void printColor(COLOR color);
  void __print(String line);
  void __print(char character);
  void __println(String line);
  void __println(char character);

  ReadLineReturn readline();
  ReadLineReturn callFunction();
  char* lastCmd();
  int lastCmdIndex = -1;

  CommandBuffer cmdBuffer;
  char parameterParsing[MAX_INPUT_LINE];
  char* parameterParseSave = nullptr;
  unsigned long historyIndex = 0;

  void upArrow();
  void downArrow();
  void rightArrow();
  void leftArrow();
  void tab();

  static void helpHist(Terminal* terminal);
};

#endif
