#include <fstream>
#include <iostream>
#include <string>
#include <cctype>

#include "inline_char.hpp"
#include "reading_state.hpp"

void endOfSuccessive(std::ofstream* output, ReadingState* readState,
                     char current) {
    if (readState->sharp) {
        *output << std::endl << "<h" << readState->sharp << ">";
        readState->header = readState->sharp;
        readState->sharp = 0;
        readState->lineChange = false;
        readState->newline = readState->newline.value_or(false);
    }
}

void endBlocQuote(std::ofstream* output, ReadingState* readState) {
    if (readState->blockquote) {
        *output << "</div>" << std::endl;
        readState->blockquote = false;
        readState->newline = readState->newline.value_or(false);
    }
}

void endUl(std::ofstream* output, ReadingState* readState) {
    if (readState->itemize) {
        *output << "</li>" << std::endl << "</ul>" << std::endl;
        readState->itemize--;
        readState->newline = readState->newline.value_or(false);
    }
}

void endOl(std::ofstream* output, ReadingState* readState) {
    if (readState->last_nb != 'z') {
        readState->lineChange = false;
        readState->newline = readState->newline.value_or(false);
        writeDefaultChar(output, readState->last_nb, readState);
        readState->last_nb = 'z';
    }
    if (readState->enumerate) {
        // std::cout << "closing...";
        *output << "</li>" << std::endl << "</ol>" << std::endl;
        readState->enumerate--;
        readState->newline = readState->newline.value_or(false);
    }
}

void writeNewlineChar(std::ofstream* output, char current,
                      ReadingState* readState) {
    // std::cout << "N; ";
    switch (current) {
        case '`':
            endOfSuccessive(output, readState, current);
            // std::cout << "increment accent";
            endUl(output, readState);
            endOl(output, readState);
            endBlocQuote(output, readState);
            readState->newline = readState->newline.value_or(false);
            readState->accent++;
            break;
        case '>':
            endOfSuccessive(output, readState, current);
            endUl(output, readState);
            endOl(output, readState);
            *output << std::endl;
            if (!readState->blockquote) {
                *output << "<div class='blockquote'>" << std::endl;
            }
            readState->blockquote = true;
            readState->lineChange = false;
            readState->newline = readState->newline.value_or(false);
            break;
        case '-':
            endOfSuccessive(output, readState, current);
            // std::cout << "found li; ";
            endOl(output, readState);
            endBlocQuote(output, readState);
            if (!readState->itemize) {
                *output << std::endl << "<ul>" << std::endl << "\t<li>";
                readState->itemize++;
            } else {
                *output << "</li>" << std::endl << "\t<li>";
            }
            readState->lineChange = false;
            readState->newline = readState->newline.value_or(false);
            break;
        case '#':
            endUl(output, readState);
            endOl(output, readState);
            endBlocQuote(output, readState);
            readState->sharp++;
            readState->newline = readState->newline.value_or(false);
            break;
        case ' ':
            readState->spaces++;
            break;
        case '\n':
            // std::cout << "found newline !!!!!!!!!!!!!!!!!!!!";
            if (readState->lastNewline.has_value() &&
                readState->lastNewline.value()) {
                *output << "<br>";
            } if (readState->lastNewline.has_value()) {
                // std::cout << "newline with N = " <<
                // readState->lastNewline.value() << " ! ";
            } else {
                // std::cout << "newline with N = empty ! ";
            }

            readState->spaces = 0;
            readState->sharp = 0; readState->lastNewline = readState->newline;
            readState->newline = {};
            break;
        default:
            if (int(current) == 13) {
                // std::cout << "wrong newline" << std::endl;
                writeNewlineChar(output, '\n', readState);
                readState->newline = readState->newline.value_or(false);
                break;
            }
            endOfSuccessive(output, readState, current);
            if (std::isdigit(current)) {
                readState->last_nb = current;
                readState->newline = readState->newline.value_or(false);
                break;
            }
                endUl(output, readState);
                endBlocQuote(output, readState);
            if (!std::isalnum(current)) {
                if (readState->last_nb != 'z') {
                readState->last_nb = 'z';
                // std::cout << "found OL ";
                if (readState->enumerate) {
                    *output << "</li>" << std::endl << "\t<li>";
                } else {
                    *output << "<ol>" << std::endl << "\t<li>";
                    readState->enumerate++;
                }
                readState->lineChange = false;
                readState->newline = readState->newline.value_or(false);
                break;
            }
                endOl(output, readState);
                readState->lineChange = false;
                readState->newline = readState->newline.value_or(true);
                writeDefaultChar(output, current, readState);
            }
            break;
    }
}
