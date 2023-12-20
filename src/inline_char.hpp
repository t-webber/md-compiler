#pragma once

#include "reading_state.hpp"

#include <fstream>
#include <iostream>
#include <string>


void checkVerbatim(std::ofstream* output, ReadingState* readState, bool is_accent);

void writeVerbatimChar(std::ofstream* output, char current,
                       ReadingState* readState);

void writeDefaultChar(std::ofstream* output, char current,
                      ReadingState* readState);


void addChars(std::ofstream* output, int number, char value);