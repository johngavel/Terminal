#include "termcmd.h"
#include "terminalclass.h"
#include "utility/characterCodes.h"

void Terminal::tab() {
  String blankLine = "                         ";
  if (echo && (cmdBuffer.getCommandLength() > 0)) {
    // Find number of commands that match
    int cmdCount = 0;
    int cmdIndex = -1;
    String buffer = cmdBuffer.getCommand();
    for (int i = 0; i < TERM_CMD->getCmdCount(); i++) {
      String readCommand = TERM_CMD->getCmd(i);
      readCommand = readCommand.substring(0, cmdBuffer.getCommandLength());
      if (readCommand.compareTo(buffer) == 0) {
        cmdCount++;
        cmdIndex = i;
      }
    }
    // If single command then replace command with found one.
    if (cmdCount == 1) {
      String readCommand = TERM_CMD->getCmd(cmdIndex);
      readCommand = readCommand.substring(cmdBuffer.getCommandLength());
      __print(readCommand);
      cmdBuffer.clearBuffer();
      cmdBuffer.setCommand((char*) TERM_CMD->getCmd(cmdIndex).c_str());
      __print(" ");
      cmdBuffer.addCharacter(' ');
    } else { // If multiple commands, print commands and replace prompt.
      cmdCount = 0;
      for (int i = 0; i < TERM_CMD->getCmdCount(); i++) {
        String readCommand = TERM_CMD->getCmd(i);
        readCommand = readCommand.substring(0, cmdBuffer.getCommandLength());
        if (readCommand.compareTo(buffer) == 0) {
          if (cmdCount % 3 == 0) println();
          __print(TERM_CMD->getCmd(i));
          String line = blankLine.substring(0, 16 - TERM_CMD->getCmd(i).length());
          __print(line);
          cmdCount++;
        }
      }
      println();
      prompt();
      __print(cmdBuffer.getCommand());
    }
  }
}
void Terminal::upArrow() {
  char tempBuffer[MAX_INPUT_LINE];
  memset(tempBuffer, 0, MAX_INPUT_LINE);
  if (lastBuffer.size() > 0) {
    if (usedelete)
      for (unsigned long i = 0; i < cmdBuffer.getCommandLength(); i++) __print(DEL_CHAR);
    if (usebackspace) {
      for (unsigned long i = 0; i < cmdBuffer.getCommandLength(); i++) __print(BS_CHAR);
      for (unsigned long i = 0; i < cmdBuffer.getCommandLength(); i++) __print(' ');
      for (unsigned long i = 0; i < cmdBuffer.getCommandLength(); i++) __print(BS_CHAR);
    }
    cmdBuffer.clearBuffer();
    if (historyIndex > 0) historyIndex--;
    lastBuffer.get(historyIndex, tempBuffer);
    cmdBuffer.setCommand(tempBuffer);
    __print(cmdBuffer.getCommand());
  }
}

void Terminal::downArrow() {
  char tempBuffer[MAX_INPUT_LINE];
  memset(tempBuffer, 0, MAX_INPUT_LINE);
  if (lastBuffer.size() > 0) {
    if (usedelete)
      for (unsigned long i = 0; i < cmdBuffer.getCommandLength(); i++) __print(DEL_CHAR);
    if (usebackspace) {
      for (unsigned long i = 0; i < cmdBuffer.getCommandLength(); i++) __print(BS_CHAR);
      for (unsigned long i = 0; i < cmdBuffer.getCommandLength(); i++) __print(' ');
      for (unsigned long i = 0; i < cmdBuffer.getCommandLength(); i++) __print(BS_CHAR);
    }
    cmdBuffer.clearBuffer();
    if (historyIndex < (lastBuffer.size() - 1)) historyIndex++;
    lastBuffer.get(historyIndex, tempBuffer);
    cmdBuffer.setCommand(tempBuffer);
    __print(cmdBuffer.getCommand());
  }
}

void Terminal::rightArrow() {}

void Terminal::leftArrow() {}
