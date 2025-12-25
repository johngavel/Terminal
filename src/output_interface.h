/*
  terminalclass.h - Terminal - Processing, Parsing, Executing, and Output Control
  For Terminals.
  Copyright (c) 2025 John J. Gavel.  All right reserved.
*/
#ifndef __OUTPUT_INTERFACE
#define __OUTPUT_INTERFACE

#include "features.h"
#include "utility/queue.h"

#include <Arduino.h>

#define MAX_CONTEXT 4

namespace TerminalLibrary {

#ifdef TERMINAL_LOGGING
typedef enum { TRACE, INFO, WARNING, ERROR, HELP, PASSED, FAILED, PROMPT } PRINT_TYPES;
#endif

typedef enum { Normal = 0, Black = 30, Red, Green, Yellow, Blue, Magenta, Cyan, White } COLOR;

class OutputInterface {
public:
  virtual void println() = 0;
  virtual void print(COLOR color, String line) = 0;
#ifdef TERMINAL_LOGGING
  virtual void print(PRINT_TYPES type, String line) = 0;
  virtual void print(PRINT_TYPES type, String line, String line2) = 0;
  virtual void println(PRINT_TYPES type, String line) = 0;
  virtual void println(PRINT_TYPES type, String line, String line2) = 0;
#else
  virtual void print(String line) = 0;
  virtual void println(String line) = 0;
#endif
#ifdef TERMINAL_HEX_STRING
  virtual void hexdump(unsigned char* buffer, unsigned long length) = 0;
#endif
#ifdef TERMINAL_BANNER
  virtual void banner() = 0;
#endif
  virtual void prompt() = 0;
  virtual char* readParameter() = 0;
  virtual void invalidParameter() = 0;
  virtual void setEcho(bool __echo) = 0;
  virtual bool getEcho() = 0;
  ;
#ifdef TERMINAL_COLORS
  virtual void setColor(bool __usecolor) = 0;
  virtual bool getColor() = 0;
#endif
  virtual void setPrompt(bool __useprompt) = 0;
  virtual bool getPrompt() = 0;
  virtual String getPromptString() = 0;
  virtual void setPromptString(String __prompt) = 0;
  virtual String getTerminalName() = 0;
  virtual void setTerminalName(String __terminalName) = 0;
#ifdef TERMINAL_BANNER
  virtual void (*getBannerFunction())(OutputInterface*) = 0;
  virtual void setBannerFunction(void (*function)(OutputInterface*)) = 0;
#endif
  virtual void clearScreen() = 0;
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
  virtual void clearHistory() = 0;
#endif
  virtual void setTokenizer(String token) = 0;
  virtual char* getTokenizer() = 0;

  virtual TerminalUtility::Queue* getLastBuffer() = 0;

  virtual void setStream(Stream* __stream) = 0;
  virtual void setStream(Stream* __inputStream, Stream* __outputStream) = 0;
  virtual Stream* getInput() = 0;
  virtual Stream* getOutput() = 0;

  virtual void setContext(unsigned int i, void* ptr) = 0;
  virtual void* getContext(unsigned int i) = 0;
};

}; // namespace TerminalLibrary
#endif // __OUTPUT_INTERFACE