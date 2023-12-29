#pragma once

#include "reading_state.hpp"

#include <fstream>
#include <iostream>
#include <string>

std::string readLanguage(std::ifstream* input, ReadingState* readState, char* current);

void checkVerbatim(std::ofstream* output, std::ifstream* input, ReadingState* readState, char* current);

void writeVerbatimChar(std::ofstream* output, char current,
                       ReadingState* readState);

void writeDefaultChar(std::ofstream* output, char current,
                      ReadingState* readState);

void addChars(std::ofstream* output, int number, char value);