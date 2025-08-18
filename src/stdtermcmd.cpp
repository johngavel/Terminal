/*
  stdtermcmd.cpp - Standard Terminal Commands - Provides some standard commands to add to the terminal.
  Copyright (c) 2025 John J. Gavel.  All right reserved.
*/

#include "stdtermcmd.h"

#include "termcmd.h"
#include "terminal.h"

void help(Terminal* terminal);
void helpHist(Terminal* terminal);
void clearScreen(Terminal* terminal);
void resetTerminal(Terminal* terminal);

void addStandardTerminalCommands() {
  TERM_CMD->addCmd("?", "", "Print Help", help);
  TERM_CMD->addCmd("clear", "", "Clear the terminal screen", clearScreen);
  TERM_CMD->addCmd("reset", "", "Reset the Terminal", resetTerminal);
  TERM_CMD->addCmd("help", "", "Print Help", help);
  TERM_CMD->addCmd("history", "", "Command History", helpHist);
  TERM_CMD->addCmd("terminal", "", "Terminal Configuration", Terminal::terminalConfig);
}

void help(Terminal* terminal) {
  terminal->banner();

  int maxStringLength = 0;
  for (int i = 0; i < TERM_CMD->getCmdCount(); i++) {
    int length = TERM_CMD->getCmd(i).length() + TERM_CMD->getParameter(i).length() + 2;
    maxStringLength = (maxStringLength > length) ? maxStringLength : length;
  }
  for (int i = 0; i < TERM_CMD->getCmdCount(); i++) {
    String line1 = TERM_CMD->getCmd(i) + " " + TERM_CMD->getParameter(i) + "                                         ";
    String line2 = "- " + TERM_CMD->getDescription(i);
    line1 = line1.substring(0, maxStringLength);
    terminal->println(HELP, line1, line2);
  }

  terminal->println();
  terminal->prompt();
}

void helpHist(Terminal* terminal) {
  terminal->println(INFO, "Command History");
  for (unsigned long i = 0; i < terminal->lastBuffer.size(); i++) terminal->println(HELP, String(i + 1) + ". ", (char*) terminal->lastBuffer.get(i));
  terminal->println(PASSED, "Command History");
  terminal->prompt();
}

void clearScreen(Terminal* terminal) {
  terminal->clearScreen();
  terminal->prompt();
}

void resetTerminal(Terminal* terminal) {
  terminal->clearScreen();
  terminal->clearHistory();
  terminal->banner();
  terminal->prompt();
}
