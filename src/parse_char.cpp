#include "type.hpp"
#include "parse_char.hpp"

#include <fstream>
#include <iostream>
#include <string>




void printState(ReadingState* state) {
  std::cout << "H = " << state->header << "; E = " << state->enumerate
            << "; ` = " << state->accent << "; V = " << state->verbatim
            << "  :   ";
}

void addChars(std::ofstream* output, int number, char value) {
  for (int i = 0; i < number; i++) {
    *output << value;
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


void endBlocQuote(std::ofstream* output, ReadingState* readState){
  if (readState->blockquote) {
    *output << "</div>" << std::endl;
    readState->blockquote = false;
  }
}


void endUl(std::ofstream* output, ReadingState* readState){
  if (readState->itemize) {
    *output << "</li>\n</ul>\n" << std::endl;
    readState->itemize--;
  }
}



void endOl(std::ofstream* output, ReadingState* readState){
  if (readState->last_nb != '!') {
    readState->lineChange = false;
    writeDefaultChar(output, readState->last_nb, readState);
    readState->last_nb = '!';
  }
  if (readState->enumerate) {
    std::cout << "closing...";
    *output << "</ol>\n</ul>\n" << std::endl;
    readState->enumerate--;
  }
}


bool is_number(char c) {
  return (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9');
}


void writeCharOutput(std::ofstream* output, char current,
                     ReadingState* readState) {
  printState(readState);
  std::cout << "C = " << current << "; ";
  if (readState->verbatim) {
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
  } else if (readState->lineChange) {
    std::cout << "N; ";
      switch (current) {
      case ' ':
        readState->spaces++;
        if (readState->sharp) {
          *output << "\n<h" << readState->sharp << ">";
          readState->header = readState->sharp;
          readState->sharp = 0;
          readState->lineChange = false;
        } else if (readState->accent) {
          addChars(output, 3 - readState->accent, '`');
          readState->verbatim = !readState->verbatim;
          readState->accent = 0;
        } else {
          *output << current;
        }
        break;
      case '>':
        endUl(output, readState);
        endOl(output, readState);
        *output << "\n";
        if (!readState->blockquote) {
          *output << "\n<div class='blockquote'>";
        } 
          readState->blockquote = true;
          readState->lineChange = false;
          break;
      case '.':
        endUl(output, readState);
        endBlocQuote(output, readState);
        if (readState->last_nb != '!') {
          readState->last_nb = '!';
          std::cout << "found OL ";
          if (readState->enumerate) {
            *output << "</ol>\n\t<ol>";
          } else {
            *output << "<ul>\n\t<ol>";
            readState->enumerate++;
          }
          readState->lineChange = false;
          break;
        }
      case '-':
        std::cout << "found li; ";
        endOl(output, readState);
        endBlocQuote(output, readState);
        if (!readState->itemize) {
          *output << "\n<ul>\n\t<li>";
          readState->itemize++;
        } else {
          *output << "</li>\n\t<li>";
        }
        readState->lineChange = false;
        break;
      case '#':
        endUl(output, readState);
        endOl(output, readState);
        endBlocQuote(output, readState);
        readState->sharp++;
        break;
      case '`':
        endUl(output, readState);
        endOl(output, readState);
        endBlocQuote(output, readState);
        readState->accent++;
        break;
      default:
        if (is_number(current)){
          readState->last_nb = current;
        } else {
          endOl(output, readState);
          endUl(output, readState);
          endBlocQuote(output, readState);
          readState->lineChange = false;
          writeDefaultChar(output, current, readState);
        }
        break;
    }
  } else {
    writeDefaultChar(output, current, readState);
  }
  std::cout << "!" << std::endl;
}

void checkVerbatim(std::ofstream* output, ReadingState* readState, bool is_accent) {
  if (!is_accent){
    if (readState->accent >= 3) {
      std::cerr << "that's weird";
    }
    addChars(output, readState->accent, '`');
    readState->accent = 0;
    return;
  } else {
    readState->accent++;
  }

  if (readState->accent > 3) {
    addChars(output, readState->accent - 3, '`');
    readState->accent = 3;
  }

  if (readState->accent < 3) {
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