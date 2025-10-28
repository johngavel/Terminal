[![Arduino-lint](https://github.com/johngavel/Terminal/actions/workflows/lint.yml/badge.svg)](https://github.com/johngavel/Terminal/actions/workflows/lint.yml)
[![Compile Examples](https://github.com/johngavel/Terminal/actions/workflows/build.yml/badge.svg)](https://github.com/johngavel/Terminal/actions/workflows/build.yml)
[![CPP Check](https://github.com/johngavel/Terminal/actions/workflows/cpp-check.yml/badge.svg)](https://github.com/johngavel/Terminal/actions/workflows/cpp-check.yml)
[![Check License status](https://github.com/johngavel/Terminal/actions/workflows/check-license.yml/badge.svg)](https://github.com/johngavel/Terminal/actions/workflows/check-license.yml)

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/johngavel/Terminal/blob/master/LICENSE)
[![GitHub issues](https://img.shields.io/github/issues/johngavel/Terminal.svg)](https://github.com/johngavel/Terminal/issues)
[![Spell Check](https://github.com/johngavel/Terminal/actions/workflows/spell-check.yml/badge.svg)](https://github.com/johngavel/Terminal/actions/workflows/spell-check.yml)

[![GitHub release](https://img.shields.io/github/release/johngavel/Terminal.svg?maxAge=3600)](https://github.com/johngavel/Terminal/releases)

# Terminal v. 1.0.12
Terminal for Serial Ports, Telnet, and other Streams. Processing, Parsing, Executing, and Output Control for Terminals. Provides the ability to easily add commands to Terminals, Execute commands, and Parse and Tokenize the Input. 

The Terminal handles left/right arrows for editing, up/down arrows for history, and tab for auto-complete.

The default tokenizer for commands is a space (' '). This is configurable via the setTokenizer method.

Some simple commands that I have implemented, include rebooting the device, setting the device to Upload Mode (For the Raspberry Pi Pico, type in the command and you don't have to push any buttons or reset the device), simple status commands of the code in progress. You can read parameters for the command line for turning on and off gpio lines.

Please see the Telnet Example for how to setup a Telent "like" session using this Terminal.

If you have any ideas, suggestions, or problems; please let me know in the Discussions page!

Written by John J. Gavel
## Release
* 1.0.12 - Updated Version Numbers
  * Replaced Terminal Commands from a Singleton to a global. Allowing the user to create different Terminal Commands for each different Terminal or use the same one.
  * BREAKING - Changed the format of the Terminal Command Function. Instead of being passed the called terminal, they are now passed an "OutputInterface*". This change allowed the seperation of the Terminal and the Terminal Commands. OutputInterface contains all of the methods that Terminal had so no other code change is necessary. (Maybe?!?).
    * void hello(Terminal* terminal); --> void hello(OutputInterface* terminal);
  * Added ASCII Table utility, that allows the terminal to print nice multi-color tables.
  * Removed terminal configuration help command
  * Standardized get/set methods
* 1.0.11 - Updated Version Numbers
  * Fixing and updating Arduino UNO Example
  * Updated stty command, to control Echo, Color, and Prompt. Changed Feature Name.
* 1.0.10 - Updated Version Numbers
  * Added custom Tokenizer
  * Changed the reading of escape codes to be non-blocking
  * Added CI tools to GitHub
  * Fixed issue with Arduino Terminal Example not compiling
  * Added Terminal Memory Diagnostics Helper in features.h
* 1.0.9 - Updated Version Numbers
  * Added features.h - Allows the user to customize the Terminal library by adding and removing Terminal Features. Such as Color, History, Logging, and each of the standard terminal commands. Just comment out the unwanted feature.
  * Updated Terminal Utilities by adding a destructor, and removing the copy constructor
  * Add namespaces TerminalUtility and TerminalLibrary. Uses TerminalLibrary in Terminal.h.
* 1.0.8 - Updated Version Numbers
  * Added Left/Right Arrow Support
  * Added echo enable/disable command
  * Updated Escape Codes for erasing lines on the terminal
* 1.0.7 - Updated Version Numbers
  * Lint Fixes
  * Add Auto-complete for TAB functionality
  * Reformatted Up/Down Arrow Key 
* 1.0.5 - Updated Version Numbers
  * Added Clear and Reset Terminal Commands
* 1.0.4 - Updated Version Numbers
  * Added Example Code
* 1.0.3 - Updated Version Numbers
  * Added dynamic echo setting
  * Added Example Code
* 1.0.2 - Updated Version Numbers
  * Formatting and Lint Fixes
* 1.0.1 - Updated Version Numbers
  * Setting up a new Terminal, based on an old Terminal
  * Fixing Backspace and Delete 
* 1.0.0 - Initial Revision
## Future Work
- [X] Tokenizer is fixed – only splits on spaces; no quotes/escaped args.
- [ ] Global singleton (TERM_CMD) – simplifies API but reduces modularity. This will require code changes when fixed.
- [ ] Minimal error handling – bad input mostly ignored.
- [X] No namespaces – commands live in a flat registry; collisions possible.
- [X] Memory footprint – holds command history; not tuned for very low-RAM MCUs. Add the ability to turn off Terminal Features via #defines

## Simple Example
I have set this example up on a Raspberry Pi Pico and set the USB to connect to the PC as a Serial Device. 
```
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
  Serial.begin(); // Setup your serial line
  terminal.setup(); // Setup the Terminal
  addStandardTerminalCommands(); 
  TERM_CMD->addCmd("hello", "", "Prints Hello World!", hello);
  terminal.banner();
  terminal.prompt();
}

void loop() {
  // Process the terminal
  terminal.loop();
  delay(10);
}
```
Serial Port Output
```
PROGRAM:\>
PROGRAM:\> help

Arduino Program
?        - Print Help
help     - Print Help
history  - Command History
hello    - Prints Hello World!

PROGRAM:\> hello
Hello World!
PROGRAM:\>
```
## Classes
The Terminal library provides 2 classes. A Terminal Class for Processing, Parsing, Executing, and Output Control. And the TerminalCommand class for handling and storing the terminal commands.

## Terminal Class
### Setup
* Terminal - Constructor, this is initialized with a pointer to a Stream. Some common Stream classes are Serial, Serial1, EthernetClient, WiFiClient, and File.
* setup - Initialize the Terminal Classes, call this in the setup function of the Arduino code.
* setPrompt - Sets the prompt in the Terminal that will be displayed to the user
* setBannerFunction - The banner can be displayed and show whatever information you desire.
### Execution
* loop - call this function in the loop function of the Arduino.
### Terminal Configuration
* configure - This uses another terminal setup and configuration for itself.
* setTokenizer - This overrides the default tokenizer of " " with user supplied tokens.
* setEcho - This configures the terminal class to echo the incoming data.
* getEcho - Returns the echo setting for the terminal.
* setColor - The Terminal Class can use the common color escape codes. However not all terminals process these.
* setPrompt - Allows you to suppress all prompts to the user.
### Terminal Output
* banner - displays the banner for the terminal to the user.
* prompt - displays the prompt for the terminal to the user, indicating input requested.
* print(COLOR, String) - Outputs in the Color you have chosen to the Terminal
  * Normal, Black, Red, Green, Yellow, Blue, Magenta, Cyan, White
* print(PRINT_TYPES, String) - Pre-defined output print types that all output must conform to.
  * INFO - This is the Normal print type
  * TRACE - Debugging print type, will print a [ DEBUG  ] before the line.
  * WARNING - This changes the Color of the output so that Warnings are more noticeable.
  * ERROR - This changes the Color of the output and will print a [ ERROR ] before the output line.
  * HELP - This exists for making the Help Command be better formatted.
  * PASSED - This changes the Color of the output and will print a [   OK   ] before the output line.
  * FAILED - This changes the Color of the output and will print a [ FAILED ] before the output line.
  * PROMPT - This changes the Color of the output. Normally associated with the input prompt.
* println - Outputs and Carriage Return and Newline to the Terminal.
* println(PRINT_TYPES, String) - Pre-defined output print types that all output must conform to.
* clearScreen - Sends Escape Commands to Clear the Terminal Screen
* clearHistory - Clears the command history from the terminal
### Terminal Input
Since the Terminal Class is processing and parsing all of the Inputs, these functions are only supposed to be called from within a called command.
* readParamter - Returns the parameter for the command that was typed in. Return NULL if no parameter was typed in. It is up to the user to convert the String parameter to a type for error checking.
* invalidParamter - This is an error condition as determined by the command. This function is called to indicate that command was malformed.

## TerminalCommand Class
This is the class that stores and manages the commands that you have created. This class is a Singleton and only one will exist in the system.
This class is accessed with the TERM_CMD macro.
TERM_CMD->addCmd(String command, String parameterDesc, String description, void function(OutputInterface*));
* command - This is a string the user will type in to run your command. Such as "dir" or "copy".
* paramterDesc - This is a string for the Help or Invalid Parameter, it should describe any parameters that your command has. Such as "filename" or "on/off".
* description - This is a string for the Help, it should be a short blurb about your command. Such as "Directory Listing" or "Turns on or off GPIO Pin 1".
* function - This is the function that will be called by the Terminal Class to run your command.

## Standard Commands
Two commands are already available to the you. A "help"/"?" and "history" commands. These can be added to your code by calling:
* addStandardTerminalCommands()
  * Help - This can be accessed by typing "help" or "?" at the command line. This will print out to the user a formatted listing of all commands available. These commands will described when you added them to the TerminalCommand class.
  * History - This can be accessed by typing "history" at the command line. This will print out to the user the last ten commands entered on this terminal.
  * Clear Screen - This can be accessed by typing "clear" at the command line. This will print out escape commands that will clear the terminal screen.
  * Reset Terminal - This can be accessed by typing "reset" at the command line. This will clear the screen and delete the command history.
  * Enable/Disable Terminal Echo - This can be accessed by type "stty echo" or "stty -echo" at the command line. This will enable or disable the echo of commands sent to this terminal.

## Writing your own Commands
I have provided in the examples some basic commands.

All commands shall be functions within your code. Methods of classes must be static.
The format of the function shall be: void functionName(OutputInterface* terminal)
* void - There is no return processing done by the Terminal class from calling your function.
* functionName - Whatever name you choose to call your function.
* terminal - This a pointer back to the Terminal that called your function, use this terminal to execute and respond back to the user.
  
### Simple Command Example
This is a simple command that will will do a digital read on GPIO Pin 3. I have already pre-supposed that you have setup this Pin 3 as a digital input.
```
void readPin3(OutputInterface* terminal) {
  int val = digitalRead(3);
  if (val == HIGH) terminal->println(INFO, "Pin 3 is HIGH");
  else terminal->println(INFO, "Pin 3 is LOW");
  terminal->prompt();
}
```
> [!NOTE]
> You should prompt the user when you are done processing the commands. Some commands might kick off something, like connect to Wifi. So your prompt would be later in a different section of code.

In the setup section where you are setting up your Terminal you would add this command to the Terminal Command class.
```
TERM_CMD->addCmd("read3", "", "Reads Pin 3 of the device.", readPin3);
```
Now your output from running this command on the serial port would be:
```
program:/> read3
Pin 3 is HIGH
program:/>
```
### Parameter Command Example
This is a simple command that takes one parameter.
```
void slowCount(OutputInterface* terminal) {
  bool passed = false;
  String value = terminal->readParameter();  // Read the Parameter from the command line
  if (value != NULL) {
    int count = value.toInt();
    if ((count > 0) && (count <= 60)) {
      passed = true;
      for (int i = 0; i < count; i++) {
        terminal->print(INFO, String(i + 1) + " ");  // Output to the terminal
        delay(1000);
      }
    } else {
      terminal->println(ERROR, "Parameter " + String(count) + " is not between 1 and 60!");  // Error Output to the Terminal
    }
  } else terminal->invalidParameter();
  terminal->println();
  terminal->println((passed) ? PASSED : FAILED, "Slow Count Complete");  // Indication to the Terminal that the command has passed or failed.
  terminal->prompt();                                                    // Prompt the user for the next command
}
```
In the setup section where you are setting up your Terminal you would add this command to the Terminal Command class.
```
TERM_CMD->addCmd("slow", "[n]", "Counts slowly on output.", slowCount);
```
Some sample outputs from running this command on the serial port would be:
```
program:/> slow 5
1 2 3 4 5
program:/>
```
```
program:/> slow 120
[  ERROR ] Parameter 120 is not between 1 and 60!

[ FAILED ] Slow Count Complete
program:/>
```
```
promgram:/> slow

[  ERROR ] Unrecognized parameter: [n]:
slow
Enter '?' or 'help' for a list of commands.

[ FAILED ] Slow Count Complete
promgram:/>
```
## Sample Commands
### Arduino Reboot Command
Reboot the Arduino Device from the Command Line
```
// Reboot the Adruino from the Terminal
void(* resetFunc) (void) = 0;//declare reset function at address 0
void reboot(OutputInterface* terminal) {
  terminal->println(WARNING, "Arduino Uno Rebooting.....");
  delay(100);
  resetFunc();
}
```
### Raspberry Pi Pico Reboot Command
Reboot the Raspberry Pico from the Command Line
```
// Reboot the Adruino from the Terminal
void reboot(OutputInterface* terminal) {
  terminal->println(WARNING, "Pico Rebooting.....");
  delay(100);
  rp2040.reboot();
}
```
### Raspberry Pi Pico Upload Command
Reboot the Raspberry Pico from the Command Line into the Upload state, prevents you from having to do the double button press or holding the button when powering on.
```
// Reboots the Pico into Upload Code Mode
void uploadPico(OutputInterface* terminal) {
  terminal->println(WARNING, "Rebooting in USB Mode....");
  delay(100);
  rp2040.rebootToBootloader();
}
```
### Batch File Command
One of the neat things that the Terminal Command Class can do is create a batch file command. This command assumes that you have setup a littlefs file system and have some other terminal setup.
```
void runCommand(OutputInterface* terminal) {
  char* value;
  value = terminal->readParameter();
  if (value != NULL) {
    File file = LittleFS.open(value, "r");
    Terminal runTerminal(&file, terminal->getOutput());
    runTerminal.configure(terminal);
    runTerminal.setup();
    runTerminal.setEcho(false);
    runTerminal.setPrompt(false);
    while (file.available()) runTerminal.loop();
  } else {
    terminal->invalidParameter();
  }
  terminal->prompt();
}
```
This is a basic way of storing commands in a file and then having the Terminal code execute and still output the results to the main terminal.
