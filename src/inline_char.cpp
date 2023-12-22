#include "inline_char.hpp"

#include <fstream>
#include <iostream>
#include <string>

#include "reading_state.hpp"

void checkVerbatim(std::ofstream *output, ReadingState *readState,
                   bool is_accent) {
    if (!readState->accent || is_accent) {
        return;
    }

    if (readState->accent < 3 && !readState->verbatim) {
        if (!readState->inline_verbatim) {
            *output << "<pre class='pre-inline'>";
        } else {
            *output << "</pre>";
        }
        readState->inline_verbatim = !readState->inline_verbatim;
        addChars(output, readState->accent - 1, '`');
        readState->accent = 0;
        return;
    } else if (readState->accent < 3) {
        return;
    } else {
        if (readState->verbatim) {
            *output << "</pre>";
        } else {
            *output << "<pre class='pre-block'>" << std::endl;
        }
        readState->verbatim = !readState->verbatim;
        readState->accent = 0;
    }
}

void writeVerbatimChar(std::ofstream *output, char current,
                       ReadingState *readState) {
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
}

void endStar(std::ofstream *output, ReadingState *readState) {
    if (readState->star && readState->italic) {
        *output << "</em>";
        readState->italic = false;
    } else if (readState->star) {
        *output << "<em>";
        readState->italic = true;
    };
    readState->star = false;
}

void writeDefaultChar(std::ofstream *output, char current,
                      ReadingState *readState) {
    std::cout << "D; ";
    if (current != '*') {
        endStar(output, readState);
    }
    switch (current) {
        case '`':
            readState->accent++;

            break;
        case '\n':
            // std::cout << "found newlineinline !!! ";
            if (readState->header) {
                *output << "</h";
                *output << readState->header;
                *output << ">" << std::endl;
                readState->header = 0;
            }
            readState->lineChange = true;
            readState->spaces = 0;

            break;
        case ' ':
            if (readState->sharp && !readState->header) {
                readState->header = readState->sharp;
                // *output << "<h1> ";
                std::cerr << std::endl << " ===== What ? ===== " << std::endl;
            } else {
                *output << current;
            }
            break;
        case '*':
            std::cout << "found star ";
            if (readState->star && readState->bold) {
                *output << "</b>";
                readState->bold = false;
                readState->star = false;
            } else if (readState->star) {
                *output << "<b>";
                readState->bold = true;
                readState->star = false;
            } else {
                std::cout << "first star ";
                readState->star = true;
                std::cout << readState->star;
            };
            break;
        default:
            *output << current;
            break;
    }
}

void addChars(std::ofstream *output, int number, char value) {
    for (int i = 0; i < number; i++) {
        *output << value;
    }
}
