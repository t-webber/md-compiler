#include "reading_state.hpp"
#include <iostream>
#include <list>

void printState(ReadingState* state) {
  std::cout << "* = " << state->star << "; B = " << state->bold << "; I = "
            << state->italic << "; ";
}

void initState(ReadingState* state, TagState* tag) {
  state->header = 0;
  state->itemize = 0;
  state->enumerate = 0;
  state->spaces = 0;
  state->accent = 0;
  state->sharp = 0;
  state->last_nb = '!';
  state->blockquote = false;
  state->lineChange = false;
  state->verbatim = false;
  state->inline_verbatim = false;
  state->star = false;
  state->bold = false;
  state->italic = false;
  
  tag->tag = "";
  tag->readingOpenTagName = false;
  tag->readingCloseTagName = false;
  tag->openedTags = {};
}

bool is_number(char c) {
  return (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9');
}