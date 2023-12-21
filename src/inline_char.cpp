#include "inline_char.hpp"
#include "reading_state.hpp"

#include <fstream>
#include <iostream>
#include <string>


void checkVerbatim(std::ofstream* output, ReadingState* readState, bool is_accent) {
  if (!readState->accent) {
    return;
  }

  if (readState->accent < 3 and !is_accent) {
    addChars(output, readState->accent, '`');
    std::cout << "writing " << readState->accent << " accents";
    readState->accent = 0;
    return;
  } else if (readState->accent < 3) {
    std::cout << "not enough";
    return;
  } else {
    if (readState->verbatim) {
      *output << "</pre>";
      std::cout << "EndPreview" << std::endl;
    } else {
      *output << "<pre>";
      std::cout << "BeginPreview" << std::endl;
    }
    readState->verbatim = !readState->verbatim;
    readState->accent = 0;
  }
}

void writeVerbatimChar(std::ofstream* output, char current,
                       ReadingState* readState) {
  std::cout << "V; ";
    switch (current) {
      case '`':
        readState->accent += 1;
        break;
      case ' ':
        addChars(output, readState->accent, '`');
        *output << ' ';
        readState->accent = 0;
        break;
      default:
        *output << current;
    }
}

void writeDefaultChar(std::ofstream* output, char current,
                      ReadingState* readState) {
  std::cout << "D; ";
  switch (current) {
    case '`':
      readState->accent++;
      break;
    case '\n':
      if (readState->header) {
        *output << "</h";
        *output << readState->header;
        *output << ">\n";
        readState->header = 0;
      }
      if (readState->start_was_char) {
        *output << "<br>";
      }
        readState->start_was_char = true;
      readState->lineChange = true;
      readState->spaces = 0;
      break;
    case ' ':
      if (readState->sharp && !readState->header) {
        readState->header = readState->sharp;
        // *output << "<h1> ";
        std::cout << std::endl << " ===== What ? ===== " << std::endl;
      } else {
        *output << current;
      }
      break;
    default:
      *output << current;
      break;
  }
}


void addChars(std::ofstream* output, int number, char value) {
  for (int i = 0; i < number; i++) {
    *output << value;
  }
}
