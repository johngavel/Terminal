/*
  queue.cpp - Utility Library - Provides a classic queue
  Copyright (c) 2025 John J. Gavel.  All right reserved.
*/

#include "queue.h"

#include <stdlib.h>
#include <string.h>

namespace TerminalUtility {
Queue::Queue(unsigned long __capacity, unsigned long __sizeOfElement, void* __memory) {
  allocatedMemory = false;
  capacity = __capacity;
  sizeOfElement = __sizeOfElement;
  if (__memory == nullptr){
    memory = (unsigned char*) malloc(capacity * sizeOfElement);
    allocatedMemory = true;
  }
  else
    memory = (unsigned char*) __memory;
  if (memory == nullptr) {
    sizeOfElement = 0;
    capacity = 0;
  }
  clear();
}

Queue::~Queue() {
  clear();
  if (allocatedMemory)
    free(memory);
}

inline bool Queue::full() {
  return (size() == capacity);
}

inline bool Queue::empty() {
  return (size() == 0);
}

bool Queue::push(void* element) {
  if (!full()) {
    backIndex++;
    backIndex = backIndex % capacity;
    countOfElements++;
    memcpy(&memory[backIndex * sizeOfElement], element, sizeOfElement);
    return true;
  } else {
    return false;
  }
}

bool Queue::pop(void* element) {
  if (!empty()) {
    if (element != nullptr) { memcpy(element, &memory[frontIndex * sizeOfElement], sizeOfElement); }
    memset(&memory[frontIndex * sizeOfElement], 0, sizeOfElement);
    frontIndex++;
    frontIndex = frontIndex % capacity;
    countOfElements--;
    return true;
  } else {
    return false;
  }
}

bool Queue::get(unsigned long index, void* element) {
  void* address = get(index);
  if (address != nullptr) {
    memcpy(element, address, sizeOfElement);
    return true;
  } else
    return false;
}

void* Queue::get(unsigned long index) {
  unsigned long indexPtr = index;
  if ((!empty()) && (index < size())) {
    indexPtr += frontIndex;
    indexPtr = indexPtr % capacity;
    return &memory[indexPtr * sizeOfElement];
  } else
    return nullptr;
}

unsigned long Queue::size() {
  return countOfElements;
}

void Queue::clear() {
  frontIndex = 0;
  backIndex = capacity - 1;
  countOfElements = 0;
  if (memory != nullptr) memset(memory, 0, capacity * sizeOfElement);
}
}
