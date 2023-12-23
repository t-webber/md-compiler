#include "newline_char.hpp"
#include "inline_char.hpp"
#include "reading_state.hpp"
#include "format.hpp"
#include "tags_char.hpp"


#include <fstream>
#include <iostream>
#include <string>


void readNChange(std::ifstream* input, std::ofstream* output);
void writeCharOutput(std::ofstream* output, char current, ReadingState* readState);


int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: ./p <filename>" << std::endl;
    return 1;
  }

  char outputPath[44];
  checkExtension(argv[1]);
  makeOutputPath(outputPath, argv[1]);

  std::ifstream input;
  std::ofstream output;
  input.open(argv[1]);
  output.open(outputPath);

  readNChange(&input, &output);
  
  input.close();
  output.close();

  return 0;
}


void readNChange(std::ifstream* input, std::ofstream* output) {
  addInit(output);

  ReadingState readState;
  TagState tagState;
  initState(&readState, &tagState);

  bool current_is_accent;

  while ((*input).good()) {
    char current = (*input).get();
    if (current == EOF) {
      break;
    }

    current_is_accent = (current == '`');
    checkVerbatim(output, input, &readState, current_is_accent);
    writeCharOutput(output, current, &readState);
  }
 
  writeCharOutput(output, ' ', &readState);
  checkVerbatim(output, input, &readState, false);
  endBlocQuote(output, &readState);
  endUl(output, &readState);
  endOl(output, &readState);
  addEnd(output);
}


void writeCharOutput(std::ofstream* output, char current,
                     ReadingState* readState) {
  std::cout << "C = " << current << "; ";
  printState(readState);
  if (current == '<' || readState->tagState->openedTags.size() > 0 || readState->tagState->readingTagName) {
    writeTagsChar(output, current, readState);
  } else if (readState->verbatim) {
    writeVerbatimChar(output, current, readState);
  } else if (readState->lineChange) {
    writeNewlineChar(output, current, readState);
  } else {
    writeDefaultChar(output, current, readState);
  }
  std::cout << "endl" << std::endl;
}