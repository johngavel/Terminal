#ifndef GAVEL_ASCII_TABLE
#define GAVEL_ASCII_TABLE

#define MAX_TABLE_COLUMNS 10

#include "../terminalclass.h"

namespace ASCIITable {
using namespace TerminalLibrary;

class AsciiTable {
public:
  AsciiTable(OutputInterface* __terminal);
  void addColumn(COLOR color, String header, unsigned long width);
  void printHeader();
  void printCellData(COLOR color, String line, int width);
  void printData(String line0 = "", String line1 = "", String line2 = "", String line3 = "", String line4 = "", String line5 = "", String line6 = "",
                 String line7 = "", String line8 = "", String line9 = "");
  void printDone(String done);

private:
  COLOR columnColor[MAX_TABLE_COLUMNS];
  unsigned long columnWidth[MAX_TABLE_COLUMNS];
  String columnHeader[MAX_TABLE_COLUMNS];
  int numberOfColumns;
  OutputInterface* terminal;
};
}; // namespace ASCIITable

using namespace ASCIITable;
#endif
