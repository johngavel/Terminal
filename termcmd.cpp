#include "termcmd.h"

TerminalCommand* TerminalCommand::terminalCommand = nullptr;

TerminalCommand::TerminalCommand() {
  countCmd = 0;
  for (int i = 0; i < MAX_TERM_CMD; i++) {
    list[i].command = "";
    list[i].parameter = "";
    list[i].description = "";
    list[i].function = NULL;
  }
}

TerminalCommand* TerminalCommand::get() {
  if (terminalCommand == nullptr) terminalCommand = new TerminalCommand();
  return terminalCommand;
}

int TerminalCommand::addCmd(String command, String parameterDesc, String description, void function(Terminal*)) {
  int returnInt = -1;
  if (countCmd < MAX_TERM_CMD) {
    list[countCmd].command = command;
    list[countCmd].parameter = parameterDesc;
    list[countCmd].description = description;
    list[countCmd].function = (void*) function;
    returnInt = countCmd;
    countCmd++;
  }
  return returnInt;
}

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

void TerminalCommand::callFunction(int index, Terminal* terminal) {
  void (*cmd)(Terminal*) = (void (*)(Terminal*)) list[index].function;
  (*cmd)(terminal);
}
