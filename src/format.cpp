#include "format.hpp"

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
  *output << 
  "<!DOCTYPE html>\n"
  "<html lang='en'>\n"
  "<head>\n"
  "  <meta charset='UTF-8'>\n"
  "  <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
  "  <style>\n"
  "    body {\n"
  "      font-family: sans-serif;\n"
  "      font-size: 14px;\n"
  "      line-height: 1.6;\n"
  "    }\n"
  "    .blockquote {\n"
  "      border-left: 3px solid #ccc;\n"
  "      margin-left: 0;\n"
  "      margin: 1em;\n"
  "      padding: 0.5em;\n"
  "      padding-left: 1em;\n"
  "      background-color: #eee;\n"
  "    }\n"
  "  </style>\n"
  "</head>\n"
  "<body>\n";
}

void addEnd(std::ofstream* output) {
  *output << "\n</body>" << std::endl;
  *output << "</html>" << std::endl;
}