#include "commandBuffer.h"

#include <string.h>

CommandBuffer::CommandBuffer() {
  clearBuffer();
  memset(blankLine, ' ', MAX_INPUT_LINE);
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
  if (cmdBufferIndex < MAX_INPUT_LINE) {
    cmdBuffer[cmdBufferIndex] = character;
    cmdBufferIndex++;
    added = true;
  }
  return added;
}

bool CommandBuffer::deleteCharacter() {
  bool deleted = false;
  if (cmdBufferIndex > 0) {
    cmdBufferIndex--;
    cmdBuffer[cmdBufferIndex] = 0;
    deleted = true;
  }
  return deleted;
}

void CommandBuffer::backIndex() {}

void CommandBuffer::forwardIndex() {}
