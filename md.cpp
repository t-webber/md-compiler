#include <fstream>
#include <iostream>
#include <string>

#define UL 1
#define LI 2
#define H1 3
#define H2 4
#define H3 5
#define H4 6
#define H5 7
#define H6 8
#define DIV 9
#define BLOCKQUOTE 10

// Missing: itemize and enumerate

typedef struct {
  int header;
  int listing;
  int spaces;
  int accent;
  int sharp;
  bool blockquote;
  bool inlisting;
  bool lineChange;
  bool verbatim;
} ReadingState;

void printState(ReadingState* state) {
  std::cout << "H = " << state->header << "; # = " << state->sharp
            << "; ` = " << state->accent << "; V = " << state->verbatim
            << "  :   ";
}

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
  *output << "</body>" << std::endl;
  *output << "</html>" << std::endl;
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
        *output << ">";
        readState->header = 0;
      }
      readState->lineChange = true;
      readState->spaces = 0;
      *output << current;
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

void endLi(std::ofstream* output, ReadingState* readState){
  if (readState->listing == 1) {
    *output << "</li>" << std::endl;
    readState->listing = 2;
  }
}


void endUl(std::ofstream* output, ReadingState* readState){
  if (readState->listing == 1) {
    *output << "</ul>" << std::endl;
    readState->listing = 2;
  }
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
          *output << "<h" << readState->sharp << ">";
          readState->header = readState->sharp;
          readState->sharp = 0;
          readState->lineChange = false;
        } else if (readState->accent) {
          addChars(output, 3 - readState->accent, '`');
          readState->verbatim = !readState->verbatim;
          readState->accent = 0;
        }
        break;
      case '>':
        if (!readState->blockquote) {
          *output << "<div class='blockquote'>\n";
        } 
          readState->blockquote = true;
          readState->lineChange = false;
          break;
      case '-':
        endBlocQuote(output, readState);
        if (readState->listing == 0) {
          *output << "<ul>\n\t<li>";
          readState->listing = 1;
        }
      case '#':
        endBlocQuote(output, readState);
        endUl(output, readState);
        readState->sharp++;
        break;
      case '`':
        endBlocQuote(output, readState);
        readState->accent++;
        break;
      default:
        endBlocQuote(output, readState);
          readState->lineChange = false;
          writeDefaultChar(output, current, readState);
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

void readNChange(std::ifstream* input, std::ofstream* output) {
  addInit(output);

  ReadingState readState;
  readState.header = 0;
  readState.listing = 0;
  readState.spaces = 0;
  readState.accent = 0;
  readState.sharp = 0;
  readState.blockquote = false;
  readState.inlisting = false;
  readState.lineChange = true;
  readState.verbatim = false;

  while ((*input).good()) {
    char current = (*input).get();
    bool current_is_accent = (current == '`');
    if (!current_is_accent) {
      writeCharOutput(output, current, &readState);
    }
    checkVerbatim(output, &readState, current_is_accent);
  }

  writeCharOutput(output, ' ', &readState);
  checkVerbatim(output, &readState, false);
  endBlocQuote(output, &readState);
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
  return 0;
}