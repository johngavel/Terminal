/*
  terminalclass.h - Terminal - Processing, Parsing, Executing, and Output Control
  For Terminals.
  Copyright (c) 2025 John J. Gavel.  All right reserved.
*/
#ifndef __TERMINAL_CLASS
#define __TERMINAL_CLASS

#include "features.h"
#include "output_interface.h"
#include "termcmd.h"
#include "utility/commandBuffer.h"
#include "utility/queue.h"

#include <Stream.h>

namespace TerminalLibrary {

typedef enum { NO_PROCESSING, HELP_FUNCTION_CALLED, EMPTY_STRING, ERROR_NO_CMD_FOUND } ReadLineReturn;

class Terminal : public OutputInterface {
public:
  Terminal(Stream* __stream) : inputStream(__stream), outputStream(__stream) { initialize(); };
  Terminal(Stream* __inputStream, Stream* __outputStream) : inputStream(__inputStream), outputStream(__outputStream) { initialize(); };
  void initialize() {
    memset(parameterParsing, 0, MAX_INPUT_LINE);
    memset(tokenizer, 0, MAX_INPUT_LINE);
    setTokenizer(" ");
    terminalCommandPtr = TERM_CMD;
  };
  void configure(OutputInterface* terminal);
  void setTerminalCommand(TerminalCommand* __terminalCommandPtr) { terminalCommandPtr = __terminalCommandPtr; };
  TerminalCommand* getTerminalCommand() { return terminalCommandPtr; };
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
  void prompt();
  void setTokenizer(String token);
  char* getTokenizer() { return tokenizer; };
  char* readParameter();
  void invalidParameter();
  void setEcho(bool __echo) { echo = __echo; };
  bool getEcho() { return echo; };
#ifdef TERMINAL_COLORS
  void setColor(bool __usecolor) { usecolor = __usecolor; };
  bool getColor() { return usecolor; };
#endif
  void setPrompt(bool __useprompt) { useprompt = __useprompt; };
  bool getPrompt() { return useprompt; };
  void setPromptString(String __prompt) { promptString = __prompt; };
  String getPromptString() { return promptString; };
#ifdef TERMINAL_BANNER
  void banner();
  void (*getBannerFunction())(OutputInterface*) { return bannerFunction; };
  void setBannerFunction(void (*function)(OutputInterface*)) { bannerFunction = function; };
#endif
  void clearScreen();
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
  void clearHistory();
#endif
  TerminalUtility::Queue* getLastBuffer() { return lastBuffer; };

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
  void (*bannerFunction)(OutputInterface*) = nullptr;
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

  bool readCharAvailable(unsigned int numberOfChars, unsigned int timeout);
  bool readCharAvailable(unsigned int numberOfChars) { return readCharAvailable(numberOfChars, 0); };
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
  char tokenizer[MAX_INPUT_LINE];

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
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
  TerminalUtility::Queue* lastBuffer = new TerminalUtility::Queue(HISTORY_BUFFER, MAX_INPUT_LINE);
#endif
  TerminalCommand* terminalCommandPtr = nullptr;
};
} // namespace TerminalLibrary
#endif
