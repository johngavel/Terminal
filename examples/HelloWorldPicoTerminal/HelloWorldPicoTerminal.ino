/*
  HelloWorldPicoTerminal.ino - Pico Serial Terminal Example
  Copyright (c) 2025 John J. Gavel.  All right reserved.
*/
#include <Terminal.h>

// Actual Terminal Initialized with the Serial Stream
Terminal terminal(&Serial1);

// Simple Hello World Example Command
void hello(Terminal* terminal) {
  terminal->println(INFO, "Hello World!");
  terminal->prompt();
}

void setup() {
  Serial1.begin(115200); // Setup your serial line
  terminal.setup();      // Setup the Terminal
  addStandardTerminalCommands();
  // addCmd parameters are as follows:
  // 1st = String that is the command to be parsed and executed from the command line
  // 2nd = String that is a listing of all parameters in this command, only listed in help
  // 3rd = String that is a description of the command, only listed in help
  // 4th = function to be called when command is received on the Stream.
  TERM_CMD->addCmd("hello", "", "Prints Hello World!", hello);
  terminal.prompt();
}

void loop() {
  // Process the terminal
  terminal.loop();
  delay(10);
}
