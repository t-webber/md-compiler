#include "inline_char.hpp"
#include "reading_state.hpp"

#include <fstream>
#include <iostream>
#include <string>


void checkVerbatim(std::ofstream* output, ReadingState* readState, bool is_accent) {
  if (!readState->accent || is_accent) {
    return;
  }

  if (readState->accent < 3 && !readState->verbatim) {
    if (!readState->inline_verbatim) {
      *output << "<pre class='inline-pre'>";
    } else {
      *output << "</pre>";  
    }
    readState->inline_verbatim = !readState->inline_verbatim;
    addChars(output, readState->accent-1, '`');
    readState->accent = 0;
    return;
  } else if (readState->accent < 3) {
    return;
  } else {
   if (readState->verbatim) {
      *output << "</pre>";
    } else {
      *output << "<pre class='blockpre'>";
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
      
        std::cout << "found newlineinline !!! ";
      if (readState->header) {
        *output << "</h";
        *output << readState->header;
        *output << ">" << std::endl;
        readState->header = 0;
      }
      // readState->start_was_char = true;
      readState->lineChange = true;
      readState->spaces = 0;
      break;
    case ' ':
      if (readState->sharp && !readState->header) {
        readState->header = readState->sharp;
        // *output << "<h1> ";
        std::cerr << std::endl << " ===== What ? ===== " << std::endl;
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
