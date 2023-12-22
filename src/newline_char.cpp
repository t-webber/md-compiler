#include <fstream>
#include <iostream>
#include <string>

#include "inline_char.hpp"
#include "reading_state.hpp"

void endOfSuccessive(std::ofstream* output, ReadingState* readState,
                     char current) {
    if (readState->sharp) {
        *output << std::endl << "<h" << readState->sharp << ">";
        readState->header = readState->sharp;
        readState->sharp = 0;
        readState->lineChange = false;
    }
}

void endBlocQuote(std::ofstream* output, ReadingState* readState) {
    if (readState->blockquote) {
        *output << "</div>" << std::endl;
        readState->blockquote = false;
        
    }
}

void endUl(std::ofstream* output, ReadingState* readState) {
    if (readState->itemize) {
        *output << "</li>" << std::endl << "</ul>" << std::endl;
        readState->itemize--;
        
    }
}

void endOl(std::ofstream* output, ReadingState* readState) {
    if (readState->last_nb != '!') {
        readState->lineChange = false;
        
        writeDefaultChar(output, readState->last_nb, readState);
        readState->last_nb = '!';
    }
    if (readState->enumerate) {
        std::cout << "closing...";
        *output << "</li>" << std::endl << "</ol>" << std::endl;
        readState->enumerate--;
        
    }
}

void writeNewlineChar(std::ofstream* output, char current,
                      ReadingState* readState) {
    std::cout << "N with " << current << "><; ";
    switch (current) {
        case '`':
            std::cout << "increment accent";
            endUl(output, readState);
            endOl(output, readState);
            endBlocQuote(output, readState);
            
            readState->accent++;
            break;
        case '>':
            endUl(output, readState);
            endOl(output, readState);
            *output << std::endl;
            if (!readState->blockquote) {
                *output << "<div class='blockquote'>" << std::endl;
            }
            readState->blockquote = true;
            readState->lineChange = false;
            break;
        case '.':
            endUl(output, readState);
            endBlocQuote(output, readState);
            if (readState->last_nb != '!') {
                readState->last_nb = '!';
                std::cout << "found OL ";
                if (readState->enumerate) {
                    *output << "</li>" << std::endl << "\t<li>";
                } else {
                    *output << "<ol>" << std::endl << "\t<li>";
                    readState->enumerate++;
                }
                readState->lineChange = false;
                
                break;
            }
        case '-':
            std::cout << "found li; ";
            endOl(output, readState);
            endBlocQuote(output, readState);
            if (!readState->itemize) {
                *output << std::endl << "<ul>" << std::endl << "\t<li>";
                readState->itemize++;
            } else {
                *output << "</li>" << std::endl << "\t<li>";
            }
            readState->lineChange = false;
            
            break;
        case '#':
            endUl(output, readState);
            endOl(output, readState);
            endBlocQuote(output, readState);
            
            readState->sharp++;
            break;
        case ' ':
            readState->spaces++;
            
            break;
        case '\n':
            std::cout << "found newline !!!!!!!!!!!!!!!!!!!!";
            *output << "<br>";
            readState->spaces = 0;
            readState->sharp = 0;
            break;
        default:
            if (int(current) == 13) {
                std::cout << "wrong newline" << std::endl;
                writeNewlineChar(output, '\n', readState);
            }
            endOfSuccessive(output, readState, current);
            if (is_number(current)) {
                readState->last_nb = current;
            } else {
                endOl(output, readState);
                endUl(output, readState);
                endBlocQuote(output, readState);
                readState->lineChange = false;
                writeDefaultChar(output, current, readState);
            }
            break;
    }
}
