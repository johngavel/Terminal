/*
  stdtermcmd.cpp - Standard Terminal Commands - Provides some standard commands to add to the terminal.
  Copyright (c) 2025 John J. Gavel.  All right reserved.
*/

#include "stdtermcmd.h"

#include "output_interface.h"
#include "termcmd.h"

namespace TerminalLibrary {
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HELP
void help(OutputInterface* terminal);
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
void history(OutputInterface* terminal);
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_CLEAR
void clearScreen(OutputInterface* terminal);
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_RESET
void resetTerminal(OutputInterface* terminal);
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_STTY
void sttyCommand(OutputInterface* terminal);
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_DIAGNOSTICS
void diagCommand(OutputInterface* terminal);
#endif

void addStandardTerminalCommands(TerminalCommand* __termCmd) {
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HELP
  __termCmd->addCmd("?", "", "Print Help", help);
  __termCmd->addCmd("help", "", "Print Help", help);
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_CLEAR
  __termCmd->addCmd("clear", "", "Clear the terminal screen", clearScreen);
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_RESET
  __termCmd->addCmd("reset", "", "Reset the Terminal", resetTerminal);
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_STTY
  __termCmd->addCmd("stty", "echo|-echo|color|-color|prompt|-prompt",
                    "Enables/Disables Terminal Echo, Color, or Prompt", sttyCommand);
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
  __termCmd->addCmd("history", "", "Command History", history);
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_DIAGNOSTICS
  __termCmd->addCmd("diag", "", "Memory Diagnostics of the Terminal Library", diagCommand);
#endif
}

#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HELP
namespace {
constexpr size_t kMaxCmdLen = 48;
constexpr size_t kMaxParamLen = 64;
constexpr size_t kMaxDescLen = 96;
constexpr size_t kMaxLhsLen = kMaxCmdLen + 1 + kMaxParamLen; // "cmd param"
} // namespace

void help(OutputInterface* terminal) {
#ifdef TERMINAL_BANNER
  terminal->banner();
#endif

  const int count = TERM_CMD->getCmdCount();
  size_t maxWidth = 0;

  // First pass: compute display width (clamped)
  for (int i = 0; i < count; ++i) {
    const String cmd = TERM_CMD->getCmd(i);
    const String param = TERM_CMD->getParameter(i);
    const bool hasParam = param.length() > 0;

    size_t width = cmd.length() + (hasParam ? (1 + param.length()) : 0);
    if (width > kMaxLhsLen) width = kMaxLhsLen; // clamp to buffer capacity
    if (width > maxWidth) maxWidth = width;
  }

  // Second pass: build and print
  for (int i = 0; i < count; ++i) {
    const String cmd = TERM_CMD->getCmd(i);
    const String param = TERM_CMD->getParameter(i);
    const String desc = TERM_CMD->getDescription(i);

    char lhs[kMaxLhsLen + 1];
    size_t pos = 0;

    // Copy cmd
    for (size_t j = 0; j < cmd.length() && pos < kMaxLhsLen; ++j) lhs[pos++] = cmd[j];

    // Optional space + param
    if (param.length() && pos < kMaxLhsLen) {
      lhs[pos++] = ' ';
      for (size_t j = 0; j < param.length() && pos < kMaxLhsLen; ++j) lhs[pos++] = param[j];
    }

    // Pad to maxWidth
    while (pos < maxWidth && pos < kMaxLhsLen) lhs[pos++] = ' ';
    lhs[pos] = '\0';

#ifdef TERMINAL_LOGGING
    // " - " + desc into rhs buffer
    char rhs[3 + kMaxDescLen + 1];
    rhs[0] = ' ';
    rhs[1] = '-';
    rhs[2] = ' ';
    size_t rpos = 2;
    for (size_t j = 0; j < desc.length() && j < kMaxDescLen; ++j) rhs[rpos++] = desc[j];
    rhs[rpos] = '\0';

    terminal->println(HELP, lhs, rhs);
#else
    terminal->print(lhs);
    terminal->println(String("- ") + desc);
#endif
  }

  terminal->println();
  terminal->prompt();
}
#endif

#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
void history(OutputInterface* terminal) {
#ifdef TERMINAL_LOGGING
  terminal->println(INFO, "Command History");
  for (unsigned long i = 0; i < terminal->getLastBuffer()->size(); i++)
    terminal->println(HELP, String(i + 1) + ". ", (char*) terminal->getLastBuffer()->get(i));
  terminal->println(PASSED, "Command History");
#else
  terminal->println("Command History");
  for (unsigned long i = 0; i < terminal->getLastBuffer()->size(); i++) {
    terminal->print(String(i + 1) + ". ");
    terminal->println((char*) terminal->getLastBuffer()->get(i));
  }
  terminal->println("Command History");
#endif
  terminal->prompt();
}
#endif

#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_CLEAR
void clearScreen(OutputInterface* terminal) {
  terminal->clearScreen();
  terminal->prompt();
}
#endif

#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_RESET
void resetTerminal(OutputInterface* terminal) {
  terminal->clearScreen();
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
  terminal->clearHistory();
#endif
#ifdef TERMINAL_BANNER
  terminal->banner();
#endif
  terminal->prompt();
}
#endif

#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_STTY
void sttyCommand(OutputInterface* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value != NULL) {
    if (value.equals("echo")) {
      terminal->setEcho(true);
      passed = true;
    }
    if (value.equals("-echo")) {
      terminal->setEcho(false);
      passed = true;
    }
    if (value.equals("color")) {
      terminal->setColor(true);
      passed = true;
    }
    if (value.equals("-color")) {
      terminal->setColor(false);
      passed = true;
    }
    if (value.equals("prompt")) {
      terminal->setPrompt(true);
      passed = true;
    }
    if (value.equals("-prompt")) {
      terminal->setPrompt(false);
      passed = true;
    }
  }
  if (!passed) terminal->invalidParameter();
  terminal->prompt();
}
#endif

#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_DIAGNOSTICS
void diagCommand(OutputInterface* terminal) {
  terminal->println();
#ifdef TERMINAL_LOGGING
  terminal->println(PROMPT, "Terminal Diagnostics");
  terminal->print(INFO, "Standard Commands: ");

#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HELP
  terminal->print(HELP, "help, ");
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
  terminal->print(HELP, "history, ");
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_CLEAR
  terminal->print(HELP, "clear, ");
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_RESET
  terminal->print(HELP, "reset, ");
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_STTY
  terminal->print(HELP, "stty, ");
#endif
  terminal->println(HELP, "diag.");

  terminal->println(HELP, "Maximum Input String: ", String((int) MAX_INPUT_LINE));
  terminal->println(HELP, "Current Number of Commands: ", String(TERM_CMD->getCmdCount()));
  terminal->println(HELP, "Maximum Commands Allowed: ", String((int) MAX_TERM_CMD));
  terminal->println(HELP, "RAM Usage Terminal: ", String(sizeof(*terminal)) + " bytes");
  terminal->println(HELP, "RAM Usage Commands: ", String(sizeof(*TERM_CMD)) + " bytes");
  terminal->println();
  terminal->println(PASSED, "Terminal Diagnostics");
#else
  terminal->println("Terminal Diagnostics");
  terminal->print("Standard Commands: ");

#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HELP
  terminal->print("help, ");
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_HISTORY
  terminal->print("history, ");
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_CLEAR
  terminal->print("clear, ");
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_RESET
  terminal->print("reset, ");
#endif
#ifdef TERMINAL_STANDARD_COMMANDS_TERMINAL_STTY
  terminal->print("stty, ");
#endif
  terminal->println("diag.");

  terminal->print("Maximum Input String: ");
  terminal->println(String((int) MAX_INPUT_LINE));
  terminal->print("Current Number of Commands: ");
  terminal->println(String(TERM_CMD->getCmdCount()));
  terminal->print("Maximum Commands Allowed: ");
  terminal->println(String((int) MAX_TERM_CMD));
  terminal->print("RAM Usage Terminal: ");
  terminal->println(String(sizeof(*terminal)) + " bytes");
  terminal->print("RAM Usage Commands: ");
  terminal->println(String(sizeof(*TERM_CMD)) + " bytes");
  terminal->println();
  terminal->println("Terminal Diagnostics");
#endif
  terminal->prompt();
}
#endif
} // namespace TerminalLibrary