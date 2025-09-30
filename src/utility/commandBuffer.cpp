#include "commandBuffer.h"

#include <string.h>

namespace TerminalUtility {
CommandBuffer::CommandBuffer() {
  clearBuffer();
  memset(blankLine, ' ', MAX_INPUT_LINE);
  memset(tempLine, 0, MAX_INPUT_LINE);
  blankLine[MAX_INPUT_LINE - 1] = 0;
}

void CommandBuffer::clearBuffer() {
  memset(cmdBuffer, 0, MAX_INPUT_LINE);
  cmdBufferIndex = 0;
}

void CommandBuffer::setCommand(char* command) {
  clearBuffer();
  strncpy(cmdBuffer, command, MAX_INPUT_LINE - 1);
  cmdBufferIndex = strnlen(cmdBuffer, MAX_INPUT_LINE);
}

char* CommandBuffer::getCommand() {
  return cmdBuffer;
}

unsigned long CommandBuffer::getCommandLength() {
  return strnlen(cmdBuffer, MAX_INPUT_LINE);
}

bool CommandBuffer::addCharacter(char character) {
  bool added = false;
  if (cmdBufferIndex < MAX_INPUT_LINE - 1) {
    memset(tempLine, 0, MAX_INPUT_LINE);
    memcpy(tempLine, &cmdBuffer[cmdBufferIndex], MAX_INPUT_LINE - cmdBufferIndex);
    cmdBuffer[cmdBufferIndex] = character;
    memcpy(&cmdBuffer[cmdBufferIndex + 1], tempLine, MAX_INPUT_LINE - cmdBufferIndex);
    cmdBuffer[MAX_INPUT_LINE - 1] = 0;
    cmdBufferIndex++;
    added = true;
  }
  return added;
}

bool CommandBuffer::deleteCharacter() {
  bool deleted = false;
  if (cmdBufferIndex > 0) {
    memset(tempLine, 0, MAX_INPUT_LINE);
    memcpy(tempLine, &cmdBuffer[cmdBufferIndex], MAX_INPUT_LINE - cmdBufferIndex);
    memcpy(&cmdBuffer[cmdBufferIndex - 1], tempLine, MAX_INPUT_LINE - cmdBufferIndex);
    cmdBuffer[MAX_INPUT_LINE - 1] = 0;
    cmdBufferIndex--;
    deleted = true;
  }
  return deleted;
}

bool CommandBuffer::backIndex() {
  if (cmdBufferIndex > 0) {
    cmdBufferIndex--;
    return true;
  }
  return false;
}

bool CommandBuffer::forwardIndex() {
  if (cmdBufferIndex < getCommandLength()) {
    cmdBufferIndex++;
    return true;
  }
  return false;
}
}