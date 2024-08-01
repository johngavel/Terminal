# Terminal v. 1.0.3
Terminal for Serial Ports, Telnet, and other Streams. Processing, Parsing, Executing, and Output Control for Terminals. Provides the abilty to easily add commands to Terminals, Execute commands, and Parse and Tokenize the Input. 
The Terminal handles backspace and up/down arrows from history. All commands typed into the Terminal a space delimited.

Written by John J. Gavel

## Simple Example
I have set this example up on a Rasberry Pi Pico and connected Pins 1 and 2 for the serial port. This is a basic TTY cable connected to PUTTY on my main computer.
```
/*
  HelloWorldPicoTerminal.ino - Pico Serial Terminal Example
  Copyright (c) 2024 John J. Gavel.  All right reserved.
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
Arduino Program
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
* setEcho - This configures the terminal class to echo the incoming data.
* getEcho - Returns the echo setting for the terminal.
* useColor - The Terminal Class can use the common color escape codes. However not all terminals process these.
* useBS - Depending on the Terminal you use it will send a BackSpace character or a Delete character when you press the Backspace Key. Configure for your terminal.
* useDel - Depending on the Terminal you use it will send a BackSpace character or a Delete character when you press the Backspace Key. Configure for your terminal.
* usePrompt - Allows you to supress all prompts to the user.
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
### Terminal Input
Since the Terminal Class is processing and parsing all of the Inputs, these functions are only supposed to be called from within a called command.
* readParamter - Returns the parameter for the command that was typed in. Return NULL if no paramter was typed in. It is up to the user to convert the String paramter to a type for error checking.
* invalidParamter - This is an error condition as determined by the command. This function is called to indicate that command was malformed.

## TerminalCommand Class
This is the class that stores and manages the commands that you have created. This class is a Singleton and only one will exist in the system.
This class is accessed with the TERM_CMD macro.
TERM_CMD->addCmd(String command, String parameterDesc, String description, void function(Terminal*));
* command - This is a string the user will type in to run your command. Such as "dir" or "copy".
* paramterDesc - This is a string for the Help or Invalid Parameter, it should describe any paramters that your command has. Such as "filename" or "on/off".
* description - This is a string for the Help, it should be a short blurb about your command. Such as "Directory Listing" or "Turns on or off GPIO Pin 1".
* function - This is the function that will be called by the Terminal Class to run your command.

## Standard Commands
Two commands are already available to the you. A "help"/"?" and "history" commands. These can be added to your code by calling:
* addStandardTerminalCommands()
Help - This can be accessed by typing "help" or "?" at the command line. This will print out to the user a formated listing of all commands available. These commands will described when you added them to the TerminalCommand class.
History - This can be accessed by typing "history" at the command line. This will print out to the user the last ten commands entered on this terminal.

## Writing your own Commands
I have provided in the examples some basic commands.

All commands shall be functions within your code. Methods of classes must be static.
The format of the function shall be: void functionName(Terminal* terminal)
* void - There is no return processing done by the Terminal class from calling your function.
* functionName - Whatever name you choose to call your function.
* terminal - This a pointer back to the Terminal that called your function, use this terminal to execute and respond back to the user.
  
### Simple Command Example
This is a simple command that will will do a digital read on GPIO Pin 3. I have already pre-supposed that you have setup this Pin 3 as a digital input.
```
void readPin3(Terminal* terminal) {
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
void slowCount(Terminal* terminal) {
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
## Batch File Command
One of the neat things that the Terminal Command Class can do is create a batch file command. This command assumes that you have setup a littlefs file system and have some other terminal setup.
```
void runCommand(Terminal* terminal) {
  char* value;
  value = terminal->readParameter();
  if (value != NULL) {
    File file = LittleFS.open(value, "r");
    Terminal runTerminal(&file, terminal->getOutput());
    runTerminal.configure(terminal);
    runTerminal.setup();
    runTerminal.setEcho(false);
    runTerminal.usePrompt(false);
    while (file.available()) runTerminal.loop();
  } else {
    terminal->invalidParameter();
  }
  terminal->prompt();
}
```
This is a basic way of storing commands in a file and then having the Terminal code execute and still output the results to the main terminal.
