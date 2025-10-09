/*
  ArduinoTerminal.ino - Arduino Serial Terminal Example
  Copyright (c) 2025 John J. Gavel.  All right reserved.

  This can compile on a Arduino Uno:
  Sketch uses 17172 bytes (53%) of program storage space. Maximum is 32256 bytes.
  Global variables use 1635 bytes (79%) of dynamic memory, leaving 413 bytes for local variables. Maximum is 2048 bytes.

  I however recommend turning off all of the features and decreasing the buffer sizes located in the features.h file
  Sketch uses 7604 bytes (23%) of program storage space. Maximum is 32256 bytes.
  Global variables use 624 bytes (30%) of dynamic memory, leaving 1424 bytes for local variables. Maximum is 2048 bytes.
*/
#include <Terminal.h>

// Actual Terminal Initialized with the Serial Stream
Terminal terminal(&Serial);

// Two Commands added to the Terminal
// reboot and slowCount
// Functions must be in the form of:
// void functionName(Terminal* terminal)

// Reboot the Adruino from the Terminal
void (*resetFunc)(void) = 0; // declare reset function at address 0
void reboot(Terminal* terminal) {
#ifdef TERMINAL_LOGGING
  terminal->println(WARNING, "Arduino Uno Rebooting.....");
#else
  terminal->println("Arduino Uno Rebooting.....");
#endif  
  delay(100);
  resetFunc();
}

// Slow Count - Example Command added to the Terminal, Slowing count up from the parameter given in the command
void slowCount(Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter(); // Read the Parameter from the command line
  if (value != NULL) {
    int count = value.toInt();
    if ((count > 0) && (count <= 60)) {
      passed = true;
      for (int i = 0; i < count; i++) {
#ifdef TERMINAL_LOGGING        
        terminal->print(INFO, String(i + 1) + " "); // Output to the terminal
#else
        terminal->print(String(i + 1) + " "); // Output to the terminal
#endif        
        delay(1000);
      }
    } else {
#ifdef TERMINAL_LOGGING        
      terminal->println(ERROR, "Parameter " + String(count) + " is not between 1 and 60!"); // Error Output to the Terminal
#else
      terminal->println("Parameter " + String(count) + " is not between 1 and 60!"); // Error Output to the Terminal
#endif      
    }
  } else
    terminal->invalidParameter();
  terminal->println();
#ifdef TERMINAL_LOGGING        
  terminal->println((passed) ? PASSED : FAILED, "Slow Count Complete"); // Indication to the Terminal that the command has passed or failed.
#else
  terminal->println(String((passed) ? "PASSED" : "FAILED") + " - Slow Count Complete"); // Indication to the Terminal that the command has passed or failed.
#endif
  terminal->prompt();                                                   // Prompt the user for the next command
}

void setup() {
  Serial.begin(9600); // Setup your serial line
  terminal.setup();   // Setup the Terminal
  // Adds to standard commands to the terminal:
  // "history" - Listing of the last 10 commands given to the terminal.
  // "help" and "?" - Help, listing of all the commands added to the Terminal
  addStandardTerminalCommands();

  // Add Program Specific Commands
  // reboot and slowCount defined above.

  // addCmd parameters are as follows:
  // 1st = String that is the command to be parsed and executed from the command line
  // 2nd = String that is a listing of all parameters in this command, only listed in help
  // 3rd = String that is a description of the command, only listed in help
  // 4th = function to be called when command is received on the Stream.
  TERM_CMD->addCmd("reboot", "", "Restarts the Arduino Uno", reboot);
  TERM_CMD->addCmd("slow", "[n]", "1 - 60 Seconds to Count.", slowCount);

#ifdef TERMINAL_BANNER
  // Print the banner for Startup - This banner can be overridden with "setBannerFunction"
  // for a custom banner  
  terminal.banner();
#endif  
  // Setup is complete - print a prompt for the user to get started.
#ifdef TERMINAL_LOGGING          
  terminal.println(PASSED, "Setup Complete");
#else
  terminal.println("PASSED Setup Complete");
#endif  
  terminal.prompt();
}

void loop() {
  // Process the terminal
  terminal.loop();
  delay(10);
}
