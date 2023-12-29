#pragma once

#include "reading_state.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <list>

void writeTagsChar(std::ofstream* output, char current, ReadingState* readState);