/*
  terminal.cpp - Terminal - Processing, Parsing, Executing, and Output Control
  For Terminals.
  Copyright (c) 2024 John J. Gavel.  All right reserved.
*/

#include "stdtermcmd.h"
#include "termcmd.h"
#include "terminalclass.h"

#include <Arduino.h>

#define BS_CHAR (char) 0x08
#define NL_CHAR (char) 0x0A
#define CR_CHAR (char) 0x0D
#define DEL_CHAR (char) 0x7F
#define ESC_CHAR (char) 0x1B

void Terminal::banner() {
  if (bannerFunction == nullptr) {
    println();
    println(PROMPT, "Program Starting......");
  } else {
    (*bannerFunction)(this);
  }
}

void Terminal::prompt() {
  if (useprompt) print(PROMPT, promptString);
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

void Terminal::print(PRINT_TYPES type, String line) {
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
  case WARNING: printColor(Magenta); break;
  case HELP:
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
    print(type, line);
    printColor(Yellow);
    __print(line2);
    printColor(Normal);
    break;
  case ERROR:
    print(type, line);
    print(WARNING, line2);
    break;
  case PASSED:
  case FAILED:
    print(type, line);
    print(INFO, line2);
    break;
  default:
    print(type, line);
    print(type, line2);
    break;
  }
}
void Terminal::println() {
  __println(String(""));
}

void Terminal::println(PRINT_TYPES type, String line) {
  print(type, line);
  println();
}

void Terminal::println(PRINT_TYPES type, String line, String line2) {
  print(type, line, line2);
  println();
}

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
    println(TRACE, String(string));
    string = (char*) printBuffer(nullptr, 0);
  }
}

void Terminal::loop() {
  ReadLineReturn ret;
  ret = readline();

  if (ret == ERROR_NO_CMD_FOUND) {
    println();
    print(ERROR, "Unrecognized command: ");
    println(WARNING, lastCmd());
    println(INFO, "Enter \'?\' or \'help\' for a list of commands.");
    prompt();
  }
}

char* Terminal::readParameter() {
  return strtok_r(NULL, " ", &parameterParseSave);
}

void Terminal::invalidParameter() {
  println();
  println(ERROR, "Unrecognized parameter: " + TERM_CMD->getParameter(lastCmdIndex) + ": ");
  println(WARNING, lastCmd());
  println(INFO, "Enter \'?\' or \'help\' for a list of commands.");
}

void Terminal::configure(Terminal* terminal) {
  echo = terminal->echo;
  usecolor = terminal->usecolor;
  usebackspace= terminal->usebackspace;
  usedelete= terminal->usedelete;
  useprompt= terminal->useprompt;
  promptString= terminal->promptString;
  bannerFunction= terminal->bannerFunction;
}

void Terminal::useBS(bool __usebackspace) { 
  usebackspace = __usebackspace; 
  if (usebackspace) usedelete = false;
};

void Terminal::useDel(bool __usedelete) { 
  usedelete = __usedelete; 
  if (usedelete) usebackspace = false;
};

void Terminal::setup() {
  memset(cmdBuffer, 0, MAX_INPUT_LINE);
  memset(parameterParsing, 0, MAX_INPUT_LINE);
}

ReadLineReturn Terminal::callFunction() {
  ReadLineReturn functionCalled = NO_PROCESSING;
  if (cmdBufferIndex > 0) {
    char* cmdName;
    functionCalled = ERROR_NO_CMD_FOUND;
    if (lastBuffer.size() >= HISTORY) { lastBuffer.pop(); }
    lastBuffer.push(cmdBuffer);
    historyIndex = lastBuffer.size();
    memset(parameterParsing, 0, MAX_INPUT_LINE);
    memcpy(parameterParsing, cmdBuffer, MAX_INPUT_LINE);
    memset(cmdBuffer, 0, MAX_INPUT_LINE);
    cmdBufferIndex = 0;
    cmdName = strtok_r(parameterParsing, " ", &parameterParseSave);
    int cmdIndex = TERM_CMD->findCmd(String(cmdName));
    if (cmdIndex != -1) {
      lastCmdIndex = cmdIndex;
      functionCalled = HELP_FUNCTION_CALLED;
      TERM_CMD->callFunction(cmdIndex, this);
    }
  } else {
    functionCalled = EMPTY_STRING;
    prompt();
  }
  return functionCalled;
}

ReadLineReturn Terminal::readline() {
  char readChar[3];
  int available = 0;
  ReadLineReturn functionCalled = NO_PROCESSING;
  if (inputStream == nullptr) return functionCalled;
  available = inputStream->available();
  if (available > 0) {
    inputStream->readBytes(&readChar[0], 1);
    if (isPrintable(readChar[0])) {
      if (cmdBufferIndex < MAX_INPUT_LINE) {
        if (echo) __print(readChar[0]);
        cmdBuffer[cmdBufferIndex] = readChar[0];
        cmdBufferIndex++;
      }
    } else if (readChar[0] == CR_CHAR) {
      if (echo) println();
      functionCalled = callFunction();
    } else if (readChar[0] == NL_CHAR) {
      if (cmdBufferIndex > 0) {
        if (echo) println();
        functionCalled = callFunction();
      } else if (echo)
        __print(NL_CHAR);
    } else if ((usedelete) && (readChar[0] == DEL_CHAR)) {
      if (cmdBufferIndex > 0) {
        cmdBufferIndex--;
        cmdBuffer[cmdBufferIndex] = 0;
        if (echo) __print(DEL_CHAR);
      }
    } else if ((usebackspace) && (readChar[0] == BS_CHAR)) {
      if (cmdBufferIndex > 0) {
        cmdBufferIndex--;
        cmdBuffer[cmdBufferIndex] = 0;
        __print(" ");
        __print(BS_CHAR);
      } else
        __print(" ");
    } else if (readChar[0] == ESC_CHAR) {
      while (inputStream->available() < 2)
        ;
      inputStream->readBytes(&readChar[0], 2);
      if (readChar[0] == '[') {
        if (readChar[1] == 'A') { // UP Arrow
          if (lastBuffer.size() > 0) {
            if (usedelete)
              for (unsigned long i = 0; i < cmdBufferIndex; i++) __print(DEL_CHAR);
            if (usebackspace) {
              for (unsigned long i = 0; i < cmdBufferIndex; i++) __print(BS_CHAR);
              for (unsigned long i = 0; i < cmdBufferIndex; i++) __print(' ');
              for (unsigned long i = 0; i < cmdBufferIndex; i++) __print(BS_CHAR);
            }
            memset(cmdBuffer, 0, MAX_INPUT_LINE);
            cmdBufferIndex = 0;
            if (historyIndex > 0) historyIndex--;
            lastBuffer.get(historyIndex, cmdBuffer);
            cmdBufferIndex = strlen(cmdBuffer);
            __print(cmdBuffer);
          }
        } else if (readChar[1] == 'B') { // Down Arrow
          if (lastBuffer.size() > 0) {
            if (usedelete)
              for (unsigned long i = 0; i < cmdBufferIndex; i++) __print(DEL_CHAR);
            if (usebackspace) {
              for (unsigned long i = 0; i < cmdBufferIndex; i++) __print(BS_CHAR);
              for (unsigned long i = 0; i < cmdBufferIndex; i++) __print(' ');
              for (unsigned long i = 0; i < cmdBufferIndex; i++) __print(BS_CHAR);
            }
            memset(cmdBuffer, 0, MAX_INPUT_LINE);
            cmdBufferIndex = 0;
            if (historyIndex < (lastBuffer.size() - 1)) historyIndex++;
            lastBuffer.get(historyIndex, cmdBuffer);
            cmdBufferIndex = strlen(cmdBuffer);
            __print(cmdBuffer);
          }
        }
      }
    }
  }
  return functionCalled;
}

char* Terminal::lastCmd() {
  return (char*) lastBuffer.get(lastBuffer.size() - 1);
}

void Terminal::helpHist(Terminal* terminal) {
  terminal->println(INFO, "Command History");
  for (unsigned long i = 0; i < terminal->lastBuffer.size(); i++) terminal->println(HELP, String(i) + ". ", (char*) terminal->lastBuffer.get(i));
  terminal->println(PASSED, "Command History");
  terminal->prompt();
}
