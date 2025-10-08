#ifndef __TERMINAL_COMMAND_BUFFER
#define __TERMINAL_COMMAND_BUFFER

#include "../features.h"

namespace TerminalUtility {
class CommandBuffer {
public:
  CommandBuffer();
  void clearBuffer();
  void setCommand(char* command);
  char* getCommand();
  char* getBlank();
  unsigned long getCommandLength();
  bool addCharacter(char character);
  bool deleteCharacter();
  bool backIndex();
  bool forwardIndex();
  unsigned long getIndex() { return cmdBufferIndex; };

private:
  char cmdBuffer[MAX_INPUT_LINE];
  char blankLine[MAX_INPUT_LINE];
  char tempLine[MAX_INPUT_LINE];
  unsigned long cmdBufferIndex = 0;
};
} // namespace TerminalUtility

#endif