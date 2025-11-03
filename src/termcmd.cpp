/*
  termcmd.cpp - Terminal Commands - Provides a Singular Class that holds
  all of the commands to be executed by any terminal.
  Copyright (c) 2025 John J. Gavel.  All right reserved.
*/

#include "termcmd.h"

static TerminalLibrary::TerminalCommand terminalCommand;
TerminalLibrary::TerminalCommand* terminalCommand_Ptr = &terminalCommand;

namespace TerminalLibrary {

TerminalCommand::TerminalCommand() {
  countCmd = 0;
  for (int i = 0; i < MAX_TERM_CMD; i++) {
    list[i].command = "";
    list[i].parameter = "";
    list[i].description = "";
    list[i].function = NULL;
#ifndef ARDUINO_ARCH_AVR
    list[i].handler = nullptr;
#endif
  }
}

int TerminalCommand::addCmd(String command, String parameterDesc, String description, void function(OutputInterface*)) {
  int returnInt = -1;
  if (findCmd(command) == -1) {
    if (countCmd < MAX_TERM_CMD) {
      list[countCmd].command = command;
      list[countCmd].parameter = parameterDesc;
      list[countCmd].description = description;
      list[countCmd].function = (void*) function;
#ifndef ARDUINO_ARCH_AVR      
      list[countCmd].handler = nullptr;
#endif      
      returnInt = countCmd;
      countCmd++;
    }
  }
  return returnInt;
}

#ifndef ARDUINO_ARCH_AVR
int TerminalCommand::addCmd(String command, String parameterDesc, String description, std::function<void(OutputInterface*)> handler) {
  int returnInt = -1;
  if (findCmd(command) == -1) {
    if (countCmd < MAX_TERM_CMD) {
      list[countCmd].command = command;
      list[countCmd].parameter = parameterDesc;
      list[countCmd].description = description;
      list[countCmd].function = nullptr;
      list[countCmd].handler = handler;
      returnInt = countCmd;
      countCmd++;
    }
  }
  return returnInt;
}
#endif

int TerminalCommand::findCmd(String command) {
  int cmdIndex = -1;
  for (int i = 0; i < countCmd; i++) {
    if (command == list[i].command) cmdIndex = i;
  }
  return cmdIndex;
}

String TerminalCommand::getCmd(int index) {
  return list[index].command;
}

String TerminalCommand::getParameter(int index) {
  return list[index].parameter;
}

String TerminalCommand::getDescription(int index) {
  return list[index].description;
}

void TerminalCommand::callFunction(int index, OutputInterface* terminal) {
  if (list[index].function) {
    void (*cmd)(OutputInterface*) = (void (*)(OutputInterface*)) list[index].function;
    (*cmd)(terminal);
  }
#ifndef ARDUINO_ARCH_AVR  
  if (list[index].handler) { list[index].handler(terminal); }
#endif
}
} // namespace TerminalLibrary