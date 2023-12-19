#pragma once

#include "type.hpp"

#include <fstream>
#include <iostream>
#include <string>

void printState(ReadingState* state);

void addChars(std::ofstream* output, int number, char value);

void writeDefaultChar(std::ofstream* output, char current, ReadingState* readState);

void endBlocQuote(std::ofstream* output, ReadingState* readState);

void endUl(std::ofstream* output, ReadingState* readState);

void endOl(std::ofstream* output, ReadingState* readState);

bool is_number(char c);

void writeCharOutput(std::ofstream* output, char current, ReadingState* readState);

void checkVerbatim(std::ofstream* output, ReadingState* readState, bool is_accent);