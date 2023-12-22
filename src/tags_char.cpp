#include "tags_char.hpp"

#include <fstream>
#include <iostream>
#include <list>
#include <string>

void writeTagsChar(
    std::ofstream* output, char current,
    ReadingState* readState) {  // FIXME: never exists tab writing mode
    std::cout << "T; ";
    switch (current) {
        case '>':
            if (!readState->tagState->readingTagName) {
                *output << current;
            } else {
                *output << "<";
                if (readState->tagState->closeTag) {
                    *output << "/";
                    if (readState->tagState->openedTags.empty() || readState->tagState->openedTags.back() != readState->tagState->tag) {
                        std::cerr << "Error: closing tag " << readState->tagState->tag << " without opening it" << std::endl;
                        exit(3);
                    }
                    readState->tagState->openedTags.pop_back();
                } else {
                    readState->tagState->openedTags.push_back(
                        readState->tagState->tag);
                }
                *output << readState->tagState->tag << ">";
                readState->tagState->tag = "";
                readState->tagState->readingTagName = false;
                readState->tagState->closeTag = false;
            }
            break;
        case '<':
            readState->tagState->readingTagName = true;
            readState->tagState->closeTag = false;
            readState->tagState->tag = "";
            break;
        case '/':
            if (readState->tagState->readingTagName) {
                readState->tagState->closeTag = true;
                readState->tagState->tag = "";
            } else {
                *output << current;
            }
            break;
        case '\n':
            *output << readState->tagState->tag << std::endl;
            readState->tagState->tag = "";
            break;
        default:
            if (readState->tagState->readingTagName) {
                readState->tagState->tag += current;
            } else {
                *output << current;
            }
            break;
    }
}