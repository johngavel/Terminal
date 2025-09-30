/*
  queue.h - Utility Library - Provides a classic queue
  Copyright (c) 2025 John J. Gavel.  All right reserved.
*/

#ifndef __QUEUE
#define __QUEUE
namespace TerminalUtility {
class Queue {
public:
  Queue(unsigned long __capacity, unsigned long __sizeOfElement, void* __memory = nullptr);
  ~Queue();
  Queue(const Queue&) = delete;
  Queue& operator=(const Queue&) = delete;
  bool full();
  bool empty();
  bool push(void* element);
  bool pop(void* element = nullptr);
  bool get(unsigned long index, void* element);
  void* get(unsigned long index);
  unsigned long size();
  void clear();

private:
  unsigned char* memory;
  unsigned long capacity;
  unsigned long sizeOfElement;
  unsigned long frontIndex;
  unsigned long backIndex;
  unsigned long countOfElements;
  bool allocatedMemory;
};
} // namespace TerminalUtility

#endif
