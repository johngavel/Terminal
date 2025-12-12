#include "asciitable.h"

namespace ASCIITable {

static String tab(int lengthPrinted, int totalLength) {
  String spaces = "";
  for (int i = lengthPrinted; i < totalLength; i++) spaces += " ";
  return spaces;
}

AsciiTable::AsciiTable(OutputInterface* __terminal) {
  terminal = __terminal;
  for (int i = 0; i < MAX_TABLE_COLUMNS; i++) {
    columnColor[i] = Normal;
    columnWidth[i] = 0;
    columnHeader[i] = "";
  }
  numberOfColumns = 0;
}

void AsciiTable::addColumn(COLOR color, String header, unsigned long width) {
  if (numberOfColumns < MAX_TABLE_COLUMNS) {
    columnColor[numberOfColumns] = color;
    columnWidth[numberOfColumns] = width;
    columnHeader[numberOfColumns] = header;
    numberOfColumns++;
  }
}

static const String dashes = "-------------------------------------------------------------------------------";

void AsciiTable::printCellData(COLOR color, String line, int width) {
  String cell = line.substring(0, width - 2);
  String data = " " + cell + tab(cell.length(), width - 2) + " |";
  terminal->print(color, data);
}

void AsciiTable::printHeader() {
  terminal->println();
  for (int i = 0; i < numberOfColumns; i++) { printCellData(columnColor[i], columnHeader[i], columnWidth[i]); }
  terminal->println();
  for (int i = 0; i < numberOfColumns; i++) {
    terminal->print(columnColor[i], dashes.substring(0, columnWidth[i]));
    terminal->print(columnColor[i], "|");
  }
  terminal->println();
}

void AsciiTable::printData(String line0, String line1, String line2, String line3, String line4, String line5,
                           String line6, String line7, String line8, String line9) {
  if (numberOfColumns > 0) printCellData(columnColor[0], line0, columnWidth[0]);
  if (numberOfColumns > 1) printCellData(columnColor[1], line1, columnWidth[1]);
  if (numberOfColumns > 2) printCellData(columnColor[2], line2, columnWidth[2]);
  if (numberOfColumns > 3) printCellData(columnColor[3], line3, columnWidth[3]);
  if (numberOfColumns > 4) printCellData(columnColor[4], line4, columnWidth[4]);
  if (numberOfColumns > 5) printCellData(columnColor[5], line5, columnWidth[5]);
  if (numberOfColumns > 6) printCellData(columnColor[6], line6, columnWidth[6]);
  if (numberOfColumns > 7) printCellData(columnColor[7], line7, columnWidth[7]);
  if (numberOfColumns > 8) printCellData(columnColor[8], line8, columnWidth[8]);
  if (numberOfColumns > 9) printCellData(columnColor[9], line9, columnWidth[9]);
  terminal->println();
}

void AsciiTable::printDone(String done) {
  terminal->println();
  terminal->println(PASSED, done);
}
} // namespace ASCIITable