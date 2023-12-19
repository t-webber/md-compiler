#include "parse_char.hpp"
#include "type.hpp"

#include <fstream>
#include <iostream>
#include <string>


void checkExtension(char* filename) {
  char* current = filename;
  while (*current && *current != '.') {
    current++;
  }
  if (*current != '.') {
    std::cerr << "You forgot the extension of the file" << std::endl;
    exit(1);
  }
  current++;
  if (*current != 'm' || *(++current) != 'd' || *(++current)) {
    std::cerr << "Wrong file extension. Supported: .md" << std::endl;
    exit(1);
  }
}

void makeOutputPath(char* outputPath, char* filename) {
  char* current = filename;
  char* writing = outputPath;
  while (*current && *current != '.') {
    *writing = *current;
    current++;
    writing++;
  }
  if (!(*current)) {
    std::cerr << "Problem creating file" << std::endl;
    exit(2);
  }
  *writing = '.';
  *(++writing) = 'h';
  *(++writing) = 't';
  *(++writing) = 'm';
  *(++writing) = 'l';
  *(++writing) = '\0';
}

void addInit(std::ofstream* output) {
  *output << "<!DOCTYPE html>" << std::endl;
  *output << "<html lang='en'>" << std::endl;
  *output << "<head>" << std::endl;
  *output << "  <meta charset='UTF-8'>" << std::endl;
  *output << "  <meta name='viewport' content='width=device-width, "
             "initial-scale=1.0'>"
          << std::endl;
  *output << "</head>" << std::endl;
  *output << "<body>" << std::endl;
}

void addEnd(std::ofstream* output) {
  *output << "\n</body>" << std::endl;
  *output << "</html>" << std::endl;
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
  readState.last_nb = '!';
  readState.blockquote = false;
  readState.lineChange = true;
  readState.verbatim = false;

  while ((*input).good()) {
    char current = (*input).get();
    if (current == EOF) {
      break;
    }
    bool current_is_accent = (current == '`');
    if (!current_is_accent) {
      writeCharOutput(output, current, &readState);
    }
    checkVerbatim(output, &readState, current_is_accent);
  }
 
  // writeCharOutput(output, ' ', &readState);
  checkVerbatim(output, &readState, false);
  endBlocQuote(output, &readState);
  endUl(output, &readState);
  endOl(output, &readState);
  addEnd(output);
}

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