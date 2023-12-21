#include "inline_char.hpp"
#include "reading_state.hpp"


#include <fstream>
#include <iostream>
#include <string>


void endOfSuccessive(std::ofstream* output, ReadingState* readState, char current) {
  if (readState->sharp) {
   *output << "\n<h" << readState->sharp << ">";
   readState->header = readState->sharp;
   readState->sharp = 0;
   readState->lineChange = false;
   readState->start_was_char = false;
 } else if (readState->accent) {
   addChars(output, 3 - readState->accent, '`');
   readState->start_was_char = false;
   readState->verbatim = !readState->verbatim;
   readState->accent = 0;
 }
}

void endBlocQuote(std::ofstream* output, ReadingState* readState){
  if (readState->blockquote) {
    *output << "</div>" << std::endl;
    readState->blockquote = false;
   readState->start_was_char = false;
  }
}

void endUl(std::ofstream* output, ReadingState* readState){
  if (readState->itemize) {
    *output << "</li>\n</ul>\n" << std::endl;
    readState->itemize--;
   readState->start_was_char = false;
  }
}

void endOl(std::ofstream* output, ReadingState* readState){
  if (readState->last_nb != '!') {
    readState->lineChange = false;
    readState->start_was_char = false;
    writeDefaultChar(output, readState->last_nb, readState);
    readState->last_nb = '!';
  }
  if (readState->enumerate) {
    std::cout << "closing...";
    *output << "</li>\n</ol>\n" << std::endl;
    readState->enumerate--;
   readState->start_was_char = false;
  }
}

void writeNewlineChar(std::ofstream* output, char current,
                      ReadingState* readState) {
    std::cout << "N; ";
      switch (current) {
      case '>':
        endUl(output, readState);
        endOl(output, readState);
        *output << "\n";
        if (!readState->blockquote) {
          *output << "<div class='blockquote'>\n";
        } 
        readState->blockquote = true;
        readState->lineChange = false;
        readState->start_was_char = true;
        break;
      case '.':
        endUl(output, readState);
        endBlocQuote(output, readState);
        if (readState->last_nb != '!') {
          readState->last_nb = '!';
          std::cout << "found OL ";
          if (readState->enumerate) {
            *output << "</li>\n\t<li>";
          } else {
            *output << "<ol>\n\t<li>";
            readState->enumerate++;
          }
          readState->lineChange = false;
          readState->start_was_char = false;
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
        readState->start_was_char = false;
        break;
      case '#':
        endUl(output, readState);
        endOl(output, readState);
        endBlocQuote(output, readState);
        readState->start_was_char = false;
        readState->sharp++;
        break;
      case '`':
        endUl(output, readState);
        endOl(output, readState);
        endBlocQuote(output, readState);
        readState->start_was_char = false;
        readState->accent++;
        break;
      case ' ':
        readState->spaces++;
        readState->start_was_char = false;
        break;
      case '\n':
        readState->spaces = 0;
        readState->sharp = 0;
        readState->accent = 0;
        break;
      default:
        endOfSuccessive(output, readState, current);
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
}

