/*
  termcmd.h - Terminal Commands - Provides a Singular Class that holds
  all of the commands to be executed by any terminal.
  Copyright (c) 2025 John J. Gavel.  All right reserved.
*/

#ifndef __TERMINAL_COMMAND
#define __TERMINAL_COMMAND

#include "features.h"
#include "output_interface.h"

#include <Arduino.h>
#include <functional>

#define TERM_CMD terminalCommand_Ptr

namespace TerminalLibrary {
class CmdItem {
public:
  String command;
  String description;
  String parameter;
  void* function;
  std::function<void(OutputInterface*)> handler;
};

class TerminalCommand {
public:
  TerminalCommand();
  int addCmd(String command, String parameterDesc, String description, void function(OutputInterface*));
  int addCmd(String command, String parameterDesc, String description, std::function<void(OutputInterface*)> handler);

  int getCmdCount() { return countCmd; };
  int findCmd(String command);
  String getCmd(int index);
  String getParameter(int index);
  String getDescription(int index);
  void callFunction(int index, OutputInterface* terminal);

private:
  CmdItem list[MAX_TERM_CMD];
  int countCmd;
};
} // namespace TerminalLibrary

extern TerminalLibrary::TerminalCommand* terminalCommand_Ptr;
#endif
