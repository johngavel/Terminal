#ifndef __TERMINAL_COMMAND
#define __TERMINAL_COMMAND

#include "terminalclass.h"

#define TERM_CMD TerminalCommand::get()

#define MAX_TERM_CMD 10

class CmdItem {
public:
  String command;
  String description;
  String parameter;
  void* function;
};

class TerminalCommand {
public:
  static TerminalCommand* get();
  int addCmd(String command, String parameterDesc, String description, void function(Terminal*));
  int getCmdCount() { return countCmd; };
  int findCmd(String command);
  String getCmd(int index);
  String getParameter(int index);
  String getDescription(int index);
  void callFunction(int index, Terminal* terminal);

private:
  TerminalCommand();
  static TerminalCommand* terminalCommand;
  CmdItem list[MAX_TERM_CMD];
  int countCmd;
};

#endif
