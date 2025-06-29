#ifndef __TERMINAL_COMMAND_BUFFER
#define __TERMINAL_COMMAND_BUFFER

#define MAX_INPUT_LINE 80

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
  void backIndex();
  void forwardIndex();

private:
  char cmdBuffer[MAX_INPUT_LINE];
  char blankLine[MAX_INPUT_LINE];
  unsigned long cmdBufferIndex = 0;
};

#endif