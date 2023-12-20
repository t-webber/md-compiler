#pragma once

#include <fstream>
#include <iostream>
#include <string>

void checkExtension(char* filename);

void makeOutputPath(char* outputPath, char* filename);

void addInit(std::ofstream* output);

void addEnd(std::ofstream* output);
