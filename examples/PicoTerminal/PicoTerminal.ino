/*
  PicoTerminal.ino - Pico Serial Terminal Example
  Copyright (c) 2024 John J. Gavel.  All right reserved.
*/
#include <Terminal.h>

// Actual Terminal Initialized with the Serial Stream
Terminal terminal(&Serial1);


// Three Commands added to the Terminal
// reboot, upload, and slowCount
// Functions must be in the form of:
// void functionName(Terminal* terminal)

// Reboot the Adruino from the Terminal
void reboot(Terminal* terminal) {
  terminal->println(WARNING, "Pico Rebooting.....");
  delay(100);
  rp2040.reboot();
}

// Reboots the Pico into Upload Code Mode
void uploadPico(Terminal* terminal) {
  terminal->println(WARNING, "Rebooting in USB Mode....");
  delay(100);
  rp2040.rebootToBootloader();
}

// Slow Count - Example Command added to the Terminal, Slowing count up from the parameter given in the command
void slowCount(Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();  // Read the Parameter from the command line
  if (value != NULL) {
    int count = value.toInt();
    if ((count > 0) && (count <= 60)) {
      passed = true;
    for (int i = 0; i < count; i++) {
      terminal->print(INFO, String(i+ 1) + " "); // Output to the terminal
      delay(1000);
    }
    } else {
      terminal->println(ERROR, "Parameter " + String(count) + " is not between 1 and 60!"); // Error Output to the Terminal
    }
  } else terminal->invalidParameter();
  terminal->println();
  terminal->println((passed)?PASSED:FAILED, "Slow Count Complete"); // Indication to the Terminal that the command has passed or failed.
  terminal->prompt(); // Prompt the user for the next command
}

// Custom Banner - Added to the start of the Terminal and Help Command
void banner(Terminal* terminal) {
  terminal->println();
  terminal->println(PROMPT, "Pico Example Program");
  terminal->println(INFO, "Build Date: " + String(__DATE__) + " Time: " + String(__TIME__));
  terminal->println();
  terminal->print(INFO, "Core is running at ");
  terminal->print(INFO, String(rp2040.f_cpu() / 1000000));
  terminal->println(INFO, " Mhz");
  int used = rp2040.getUsedHeap();
  int total = rp2040.getTotalHeap();
  int percentage = (used * 100) / total;
  terminal->print(INFO, "RAM Memory Usage: ");
  terminal->print(INFO, String(used));
  terminal->print(INFO, "/");
  terminal->print(INFO, String(total));
  terminal->print(INFO, " --> ");
  terminal->print(INFO, String(percentage));
  terminal->println(INFO, "%");
  terminal->print(INFO, "CPU Temperature: ");
  terminal->print(INFO, String((9.0 / 5.0 * analogReadTemp()) + 32.0, 0));
  terminal->println(INFO, "°F.");
}


void setup() {
  Serial1.begin(115200); // Setup your serial line
  terminal.setup(); // Setup the Terminal
  terminal.useColor(true); // Output color to the Terminal
  terminal.setPrompt("example://>");
  terminal.setBannerFunction(banner);
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
  TERM_CMD->addCmd("reboot", "", "Restarts the Pico", reboot);
  TERM_CMD->addCmd("upload", "", "Restarts the Pico in Upload Mode", uploadPico);
  TERM_CMD->addCmd("slow", "[n]", "1 - 60 Seconds to Count.", slowCount);

  // Print the banner for Startup - This banner can be overridden with "setBannerFunction"
  // for a custom banner
  terminal.banner();
  // Setup is complete - print a prompt for the user to get started.
  terminal.println(PASSED, "Setup Complete");
  terminal.prompt();
}

void loop() {
  // Process the terminal
  terminal.loop();
  delay(10);
}
