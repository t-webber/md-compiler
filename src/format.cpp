#include "format.hpp"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

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

void copyFile(std::ofstream* output, std::string path){
  std::ifstream input;
  input.open(path); 
  std::string content;
  while (std::getline(input, content)){;
    *output << content << std::endl;
  }
}

void addInit(std::ofstream* output) {
  copyFile(output, "./src/html_templates/header.html");
}


void addEnd(std::ofstream* output) {
  copyFile(output, "./src/html_templates/footer.html");
}
