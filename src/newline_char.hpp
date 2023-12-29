#pragma once

#include "inline_char.hpp"
#include "reading_state.hpp"


#include <fstream>
#include <iostream>
#include <string>


void endOfSuccessive(std::ofstream* output, ReadingState* readState, char current);

void endBlocQuote(std::ofstream* output, ReadingState* readState);

void endUl(std::ofstream* output, ReadingState* readState); 

void endOl(std::ofstream* output, ReadingState* readState);

void writeNewlineChar(std::ofstream* output, char current, ReadingState* readState);