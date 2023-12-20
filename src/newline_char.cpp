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
   readState->start_was_char = std::isalpha(current);
 } else if (readState->accent) {
   addChars(output, 3 - readState->accent, '`');
   readState->verbatim = !readState->verbatim;
   readState->accent = 0;
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
    readState->start_was_char = false;
    writeDefaultChar(output, readState->last_nb, readState);
    readState->last_nb = '!';
  }
  if (readState->enumerate) {
    std::cout << "closing...";
    *output << "</li>\n</ol>\n" << std::endl;
    readState->enumerate--;
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
          readState->start_was_char = std::isalpha(current);
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
          readState->start_was_char = std::isalpha(current);
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
        readState->start_was_char = std::isalpha(current);
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
      case ' ':
        readState->spaces++;
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
          readState->start_was_char = std::isalpha(current);
          writeDefaultChar(output, current, readState);
        }
        break;
    }
}

