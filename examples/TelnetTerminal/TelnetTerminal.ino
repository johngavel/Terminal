/*
  TelnetTerminal.ino - PicoW Telnet Terminal Example
  Modified from the WiFiServer.ino Example Placed in the public domain by Earle F. Philhower, III, 2022
  Copyright (c) 2025 John J. Gavel.  All right reserved.
*/

/* This program will open a serial terminal on the usb port of the PicoW */
/* It will open a socket for the telnet program to access on the wifi.*/

#include <Terminal.h>
#include <WiFi.h>

// Replace the following with your network
#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK "your-password"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

int port = 23;

WiFiServer server(port);
WiFiClient client;
Terminal telnet(&client);
bool isTelnetConnected = false;

// Serial Port Terminal Initialized with the Serial Stream
Terminal terminal(&Serial);

/******* Terminal Commands ***************/

// Five Commands added to the Terminal
// ipconfig, exit, reboot, upload, and slowCount
// Functions must be in the form of:
// void functionName(OutputInterface* terminal)

void wifiIFConfig(OutputInterface* terminal) {
  IPAddress ipAddress = WiFi.localIP();
  bool linked = WiFi.isConnected();
  terminal->println(INFO, "Network: " + WiFi.SSID() + ((linked) ? " Connected" : " Unconnected"));
  terminal->println(INFO, "  IP Address:  " + String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") +
                              String(ipAddress[2]) + String(".") + String(ipAddress[3]));
  ipAddress = WiFi.subnetMask();
  terminal->println(INFO, "  Subnet Mask: " + String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") +
                              String(ipAddress[2]) + String(".") + String(ipAddress[3]));
  ipAddress = WiFi.gatewayIP();
  terminal->println(INFO, "  Gateway:     " + String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") +
                              String(ipAddress[2]) + String(".") + String(ipAddress[3]));
  ipAddress = WiFi.dnsIP();
  terminal->println(INFO, "  DNS Server:  " + String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") +
                              String(ipAddress[2]) + String(".") + String(ipAddress[3]));
  terminal->prompt();
}

// Reboot the Adruino from the Terminal
void reboot(OutputInterface* terminal) {
  terminal->println(WARNING, "Pico Rebooting.....");
  delay(100);
  rp2040.reboot();
}

// Reboots the Pico into Upload Code Mode
void uploadPico(OutputInterface* terminal) {
  terminal->println(WARNING, "Rebooting in USB Mode....");
  delay(100);
  rp2040.rebootToBootloader();
}

// Exits the Telnet Session
void exitTelnet(OutputInterface* terminal) {
  if (terminal == &telnet) {
    terminal->println(WARNING, "Closing Telnet Session....");
    delay(100);
    client.stop();
  } else {
    terminal->println(ERROR, "Not Supported on this terminal.");
    terminal->prompt();
  }
}

// Scan the Wifi for networks
static const char* macToString(uint8_t mac[6]) {
  static char s[20];
  sprintf(s, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return s;
}

static const char* encToString(uint8_t enc) {
  switch (enc) {
  case ENC_TYPE_NONE: return "NONE";
  case ENC_TYPE_TKIP: return "WPA";
  case ENC_TYPE_CCMP: return "WPA2";
  case ENC_TYPE_AUTO: return "AUTO";
  }
  return "UNKN";
}

void wifiScan(OutputInterface* terminal) {
  terminal->println(INFO, "Beginning Scan.........");
  auto cnt = WiFi.scanNetworks();
  if (!cnt) {
    terminal->println(INFO, "No Networks Found.");
  } else {
    char line[80];
    sprintf(line, "Found %d networks\n", cnt);
    terminal->println(INFO, line);
    sprintf(line, "%32s %5s %17s %2s %4s", "SSID", "ENC", "BSSID        ", "CH", "RSSI");
    terminal->println(INFO, line);
    for (auto i = 0; i < cnt; i++) {
      uint8_t bssid[6];
      WiFi.BSSID(i, bssid);
      sprintf(line, "%32s %5s %17s %2d %4ld", WiFi.SSID(i), encToString(WiFi.encryptionType(i)), macToString(bssid),
              WiFi.channel(i), WiFi.RSSI(i));
      terminal->println(INFO, line);
    }
  }
  terminal->println(PASSED, "Scan Complete");
  terminal->prompt();
}

// Slow Count - Example Command added to the Terminal, Slowing count up from the parameter given in the command
void slowCount(OutputInterface* terminal) {
  bool passed = false;
  String value = terminal->readParameter(); // Read the Parameter from the command line
  if (value != NULL) {
    int count = value.toInt();
    if ((count > 0) && (count <= 60)) {
      passed = true;
      for (int i = 0; i < count; i++) {
        terminal->print(INFO, String(i + 1) + " "); // Output to the terminal
        delay(1000);
      }
    } else {
      terminal->println(ERROR,
                        "Parameter " + String(count) + " is not between 1 and 60!"); // Error Output to the Terminal
    }
  } else
    terminal->invalidParameter();
  terminal->println();
  terminal->println((passed) ? PASSED : FAILED,
                    "Slow Count Complete"); // Indication to the Terminal that the command has passed or failed.
  terminal->prompt();                       // Prompt the user for the next command
}
/******* End Terminal Commands ***************/

// Custom Banner - Added to the start of the Terminal and Help Command
void banner(OutputInterface* terminal) {
  terminal->println();
  terminal->println(PROMPT, "PicoW Telnet Example Program");
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

void setupSerialPort() {
  // Setup of the Serial Terminal
  Serial.begin(115200);    // Setup your serial line
  terminal.setup();        // Setup the Terminal
  terminal.setColor(true); // Output color to the Terminal
  terminal.setPrompt("example://>");
  terminal.setBannerFunction(banner);
  terminal.banner();

  terminal.println();
  terminal.println(INFO, "Connected to WiFi");
  terminal.println();
  terminal.println(INFO, "Connect to server at " + WiFi.localIP().toString() + ":" + String(port));
  // Print the banner for Startup - This banner can be overridden with "setBannerFunction"
  // for a custom banner
  terminal.banner();
  // Setup is complete - print a prompt for the user to get started.
  terminal.println(PASSED, "Setup Complete");
  terminal.prompt();
}

void setupTelnet() {
  // Setup the Telnet Terminal
  telnet.setup();
  telnet.setColor(true);
  telnet.setPrompt("telnet://>");
  telnet.setBannerFunction(banner);
  telnet.setEcho(true);
}

void setup() {
  delay(10000);
  setupTelnet();

  // Setup of the Wifi
  WiFi.mode(WIFI_STA);
  WiFi.setHostname("PicoW2");
  terminal.println(INFO, "Connecting to '" + String(ssid) + "' with '" + String(password) + "'");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    terminal.print(INFO, ".");
    delay(100);
  }
  server.begin();
  // Adds to standard commands to the terminal:
  // "history" - Listing of the last 10 commands given to the terminal.
  // "help" and "?" - Help, listing of all the commands added to the Terminal
  addStandardTerminalCommands(TERM_CMD);

  // Add Program Specific Commands
  // reboot and slowCount defined above.

  // addCmd parameters are as follows:
  // 1st = String that is the command to be parsed and executed from the command line
  // 2nd = String that is a listing of all parameters in this command, only listed in help
  // 3rd = String that is a description of the command, only listed in help
  // 4th = function to be called when command is received on the Stream.
  TERM_CMD->addCmd("ifconfig", "", "IP Configuration", wifiIFConfig);
  TERM_CMD->addCmd("reboot", "", "Restarts the Pico", reboot);
  TERM_CMD->addCmd("upload", "", "Restarts the Pico in Upload Mode", uploadPico);
  TERM_CMD->addCmd("exit", "", "Close the Terminal", exitTelnet);
  TERM_CMD->addCmd("wifiscan", "", "Scans the Wifi for Networks", wifiScan);
  TERM_CMD->addCmd("slow", "[n]", "1 - 60 Seconds to Count.", slowCount);

  setupSerialPort();
}

void loop() {
  // Process the telnet session
  if (!isTelnetConnected) {
    client = server.accept();
    if (client.connected()) {
      isTelnetConnected = true;
      telnet.setStream(&client);
      telnet.println(INFO, "Starting Telnet Session.");
      telnet.banner();
      telnet.prompt();
      client.flush();
    }
  } else {
    if (client.connected()) {
      telnet.loop();
      client.flush();
    } else {
      isTelnetConnected = false;
    }
  }

  terminal.loop();
  delay(10);
}
