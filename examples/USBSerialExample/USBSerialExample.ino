/*
  USBSerialExample.ino - Pico Serial Terminal Example
  Copyright (c) 2025 John J. Gavel.  All right reserved.
*/
#include <Terminal.h>

// Actual Terminal Initialized with the Serial Stream
Terminal terminal(&Serial);

// Simple Hello World Example Command
void hello(OutputInterface* terminal) {
  terminal->println(INFO, "Hello World!");
  terminal->prompt();
}

void setup() {
  Serial.begin();   // Setup your serial line
  terminal.setup(); // Setup the Terminal
  addStandardTerminalCommands(TERM_CMD);
  TERM_CMD->addCmd("hello", "", "Prints Hello World!", hello);
  terminal.banner();
  terminal.prompt();
}

void loop() {
  // Process the terminal
  terminal.loop();
  delay(10);
}