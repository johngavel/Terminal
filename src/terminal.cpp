/*
  terminal.cpp - Terminal - Processing, Parsing, Executing, and Output Control
  For Terminals.
  Copyright (c) 2025 John J. Gavel.  All right reserved.
*/

#include "stdtermcmd.h"
#include "termcmd.h"
#include "terminalclass.h"
#include "utility/characterCodes.h"

#include <Arduino.h>
namespace TerminalLibrary {
#ifdef TERMINAL_BANNER
void Terminal::banner() {
  if (bannerFunction == nullptr) {
    println();
#ifdef TERMINAL_LOGGING
    println(PROMPT, "Arduino Program");
#else
    println("Arduino Program");
#endif
  } else {
    (*bannerFunction)(this);
  }
}
#endif

void Terminal::prompt() {
  if (useprompt)
#ifdef TERMINAL_LOGGING
    print(PROMPT, promptString + " ");
#else
    print(promptString + " ");
#endif
}

void Terminal::__print(String line) {
  if (outputStream == nullptr) return;
  outputStream->print(line);
}

void Terminal::__print(char character) {
  __print(String(character));
}

void Terminal::__println(String line) {
  if (outputStream == nullptr) return;
  outputStream->println(line);
}

void Terminal::__println(char character) {
  __println(String(character));
}

#ifdef TERMINAL_COLORS
void Terminal::printColor(COLOR color) {
  if (usecolor) {
    char colorString[32];
    sprintf(colorString, "\033[%dm", color);
    __print(colorString);
  }
}

void Terminal::print(COLOR color, String line) {
  printColor(color);
  __print(line);
  printColor(Normal);
}
#endif

#ifdef TERMINAL_LOGGING
void Terminal::printHeader(PRINT_TYPES type) {
  printColor(Normal);
  switch (type) {
  case TRACE:
    __print("[");
    print(Cyan, "  DEBUG ");
    __print("] ");
    printColor(Cyan);
    break;
  case PROMPT: printColor(Green); break;
  case ERROR:
    __print("[");
    print(Red, "  ERROR ");
    __print("] ");
    printColor(Red);
    break;
  case PASSED:
    __print("[");
    print(Green, "   OK   ");
    __print("] ");
    break;
  case FAILED:
    __print("[");
    print(Red, " FAILED ");
    __print("] ");
    break;
  case WARNING:
    __print("[");
    print(Magenta, "  WARN  ");
    __print("] ");
    break;
  case HELP: printColor(Yellow); break;
  case INFO:
  default: break;
  }
}

void Terminal::print(PRINT_TYPES type, String line) {
  printColor(Normal);
  switch (type) {
  case TRACE:
    printColor(Cyan);
    break;
  case PROMPT: printColor(Green); break;
  case ERROR:
    printColor(Red);
    break;
  case PASSED:
    printColor(Green);
    break;
  case FAILED:
    printColor(Red);
    break;
  case WARNING:
    printColor(Magenta);
    break;
  case HELP: printColor(Yellow); break;
  case INFO:
  default: break;
  }
  __print(line);
  printColor(Normal);
}

void Terminal::print(PRINT_TYPES type, String line, String line2) {
  printColor(Normal);
  switch (type) {
  case HELP:
    print(INFO, line);
    print(HELP, line2);
    break;
  default:
    print(type, line);
    print(INFO, line2);
    break;
  }
}
#endif

void Terminal::println() {
  __println(String(""));
}

#ifdef TERMINAL_LOGGING
void Terminal::println(PRINT_TYPES type, String line) {
  printHeader(type);
  print(type, line);
  println();
}

void Terminal::println(PRINT_TYPES type, String line, String line2) {
  printHeader(type);
  print(type, line, line2);
  println();
}
#endif

#ifdef TERMINAL_HEX_STRING
static String hexByteString(unsigned char value) {
  String string = "";
  if (value < 16) string += "0";
  string += String(value, HEX);
  return string;
}

static const char* printBuffer(unsigned char* buffer, unsigned long length) {
  static unsigned char* pBuffer;
  static unsigned long bufferIndex;
  static unsigned long bufferLength;
  static String string;
  string = "";
  unsigned long loop;
  if (buffer != nullptr) {
    pBuffer = buffer;
    bufferIndex = 0;
    bufferLength = length;
  }
  if (bufferIndex >= bufferLength) return nullptr;
  string += hexByteString(bufferIndex >> 24 & 0xFF);
  string += hexByteString(bufferIndex >> 16 & 0xFF);
  string += hexByteString(bufferIndex >> 8 & 0xFF);
  string += hexByteString(bufferIndex & 0xFF);
  string += ": ";
  unsigned long remaining = bufferLength - bufferIndex;
  loop = (remaining < 16) ? remaining : 16;
  loop += bufferIndex;
  for (; bufferIndex < loop; bufferIndex++) { string += hexByteString(pBuffer[bufferIndex]) + " "; }
  return string.c_str();
}

void Terminal::hexdump(unsigned char* buffer, unsigned long length) {
  char* string = (char*) printBuffer(buffer, length);
  while (string != nullptr) {
#ifdef TERMINAL_LOGGING
    println(TRACE, String(string));
#else
    println(String(string));
#endif
    string = (char*) printBuffer(nullptr, 0);
  }
}
#endif

void Terminal::loop() {
  ReadLineReturn ret;
  ret = readline();

  if (ret == ERROR_NO_CMD_FOUND) {
    println();
#ifdef TERMINAL_LOGGING
    char line[80];
    memset(line, 0, sizeof(line));
    strncpy(line, "Unrecognized command: ", sizeof(line));
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
    strncat(line, lastCmd(), sizeof(line));
#endif
    println(ERROR, line);
    println(INFO, "Enter \'?\' or \'help\' for a list of commands.");
#else
    print("Unrecognized command: ");
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
    println(lastCmd());
#endif
    println("Enter \'?\' or \'help\' for a list of commands.");
#endif
    prompt();
  }
}

void Terminal::setTokenizer(String token) {
  memset(tokenizer, 0, MAX_INPUT_LINE);
  strncpy(tokenizer, token.c_str(), MAX_INPUT_LINE - 1);
}

char* Terminal::readParameter() {
  return strtok_r(NULL, tokenizer, &parameterParseSave);
}

void Terminal::invalidParameter() {
  println();
#ifdef TERMINAL_LOGGING
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
  if (terminalCommandPtr)
    println(ERROR, "Unrecognized parameter: " + terminalCommandPtr->getParameter(lastCmdIndex) + ": ");
  else
    println(ERROR, "No Command Processor.");
  println(WARNING, "Command: " + String(lastCmd()));
#endif
  println(INFO, "Enter \'?\' or \'help\' for a list of commands.");
#else
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
  if (terminalCommandPtr)
    println("Unrecognized parameter: " + terminalCommandPtr->getParameter(lastCmdIndex) + ": ");
  else
    println(ERROR, "No Command Processor.");
  println("Command: " + String(lastCmd()));
#else
  println("Unrecognized parameter");
#endif
  println("Enter \'?\' or \'help\' for a list of commands.");
#endif
}

void Terminal::configure(OutputInterface* terminal) {
  setEcho(terminal->getEcho());
  setPrompt(terminal->getPrompt());
  setPromptString(terminal->getPromptString());
  setTokenizer(String(terminal->getTokenizer()));
#ifdef TERMINAL_COLORS
  setColor(terminal->getColor());
#endif
#ifdef TERMINAL_BANNER
  setBannerFunction(terminal->getBannerFunction());
#endif
}

void Terminal::setup() {
  cmdBuffer.clearBuffer();
  memset(parameterParsing, 0, MAX_INPUT_LINE);
}

ReadLineReturn Terminal::callFunction() {
  ReadLineReturn functionCalled = NO_PROCESSING;
  if (cmdBuffer.getCommandLength() > 0) {
    char* cmdName;
    functionCalled = ERROR_NO_CMD_FOUND;
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
    if (lastBuffer->size() >= HISTORY_BUFFER) { lastBuffer->pop(); }
    lastBuffer->push(cmdBuffer.getCommand());
    historyIndex = lastBuffer->size();
#endif
    memset(parameterParsing, 0, MAX_INPUT_LINE);
    memcpy(parameterParsing, cmdBuffer.getCommand(), MAX_INPUT_LINE);
    cmdBuffer.clearBuffer();
    cmdName = strtok_r(parameterParsing, tokenizer, &parameterParseSave);
    if (terminalCommandPtr) {
      int cmdIndex = terminalCommandPtr->findCmd(String(cmdName));
      if (cmdIndex != -1) {
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
        lastCmdIndex = cmdIndex;
#endif
        functionCalled = HELP_FUNCTION_CALLED;
        terminalCommandPtr->callFunction(cmdIndex, this);
      }
    } else {
      functionCalled = ERROR_NO_CMD_FOUND;
    }
  } else {
    functionCalled = EMPTY_STRING;
    prompt();
  }
  return functionCalled;
}

bool Terminal::readCharAvailable(unsigned int numberOfChars, unsigned int timeout) {
  bool available = false;
  unsigned int availableChar = inputStream->available();
  if (timeout > 0) {
    uint32_t start = millis();
    while ((!available) && ((millis() - start) < timeout)) {
      availableChar = inputStream->available();
      available = availableChar >= numberOfChars;
    }
  } else {
    available = availableChar >= numberOfChars;
  }
  return available;
}

ReadLineReturn Terminal::readline() {
  if (inputStream == nullptr) return NO_PROCESSING;

  char readChar[5];
  if (!readCharAvailable(1)) return NO_PROCESSING;

  inputStream->readBytes(&readChar[0], 1);
  char c = readChar[0];

  if (c == HT_CHAR) {
#ifdef TERMINAL_TAB
    if (echo) tab();
#endif
  } else if (isPrintable(c)) {
    if (cmdBuffer.addCharacter(c) && echo) { printCommandLine(); }
  } else if (c == CR_CHAR || (c == NL_CHAR && cmdBuffer.getCommandLength() > 0)) {
    if (echo) println();
    return callFunction();
  } else if (c == DEL_CHAR || c == BS_CHAR) {
    cmdBuffer.deleteCharacter();
    if (echo) printCommandLine();
  } else if (c == ESC_CHAR && echo) {
    // Wait for the next two bytes of the escape sequence
    if (!readCharAvailable(2, 12)) { // We didn't get the rest of the Escape char sequence
      cmdBuffer.deleteCharacter();
      return NO_PROCESSING;
    }
    inputStream->readBytes(&readChar[1], 2);

#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
    if (readChar[1] == VT100_UP_ARROW[1] && readChar[2] == VT100_UP_ARROW[2]) {
      upArrow();
    } else if (readChar[1] == VT100_DOWN_ARROW[1] && readChar[2] == VT100_DOWN_ARROW[2]) {
      downArrow();
#else
    if (0) {
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_EDITING
    } else if (readChar[1] == VT100_RIGHT_ARROW[1] && readChar[2] == VT100_RIGHT_ARROW[2]) {
      rightArrow();
    } else if (readChar[1] == VT100_LEFT_ARROW[1] && readChar[2] == VT100_LEFT_ARROW[2]) {
      leftArrow();
#else
    } else if (0) {
#endif
    } else {
      // Optional: handle unknown escape sequences here
      // __print("ESC " + String(readChar[1]) + String(readChar[2]));
      // while (inputStream->available()) {
      //   inputStream->readBytes(&readChar[0], 1);
      //   __print(String(readChar[0]));
      // }
      // println();
    }
  }

  return NO_PROCESSING;
}

#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
char* Terminal::lastCmd() {
  return (char*) lastBuffer->get(lastBuffer->size() - 1);
}
#endif

void Terminal::clearScreen() {
#ifdef TERMINAL_COLORS
  if (usecolor) {
    __print(VT100_CLEAR_SCREEN);
    __print(VT100_SET_CURSOR_HOME);
  } else {
#else
  if (0) {
#endif
#ifdef TERMINAL_LOGGING
    println(ERROR, "Command not implemented for this terminal.");
#else
    println("Command not implemented for this terminal.");
#endif
  }
}

#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
void Terminal::clearHistory() {
  lastBuffer->clear();
  historyIndex = 0;
}
#endif

} // namespace TerminalLibrary