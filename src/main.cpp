#include "newline_char.hpp"
#include "inline_char.hpp"
#include "reading_state.hpp"
#include "format.hpp"


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

  std::cout << "Starting..." << std::endl;

  readNChange(&input, &output);
  
  input.close();
  output.close();

  return 0;
}


void readNChange(std::ifstream* input, std::ofstream* output) {
  addInit(output);

  ReadingState readState;
  readState.header = 0;
  readState.itemize = 0;
  readState.enumerate = 0;
  readState.spaces = 0;
  readState.accent = 0;
  readState.sharp = 0;
  readState.start_was_char = true;
  readState.last_nb = '!';
  readState.blockquote = false;
  readState.lineChange = true;
  readState.verbatim = false;

  bool current_is_accent;

  while ((*input).good()) {
    char current = (*input).get();
    if (current == EOF) {
      break;
    }

    current_is_accent = (current == '`');
    checkVerbatim(output, &readState, current_is_accent);
    writeCharOutput(output, current, &readState);
  }
 
  writeCharOutput(output, ' ', &readState);
  checkVerbatim(output, &readState, false);
  endBlocQuote(output, &readState);
  endUl(output, &readState);
  endOl(output, &readState);
  addEnd(output);
}


void writeCharOutput(std::ofstream* output, char current,
                     ReadingState* readState) {
  printState(readState);
  std::cout << "C = " << current << "; ";
  if (readState->verbatim) {
    writeVerbatimChar(output, current, readState);
  } else if (readState->lineChange) {
    writeNewlineChar(output, current, readState);
  } else {
    writeDefaultChar(output, current, readState);
  }
  std::cout << "!" << std::endl;
}