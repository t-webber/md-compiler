#include "reading_state.hpp"
#include <iostream>
#include <list>
#include <optional>

void printState(ReadingState* state) {
// std::cout << "New = " << state->newline.value_or("empty") << " ! ";
}

void initState(ReadingState* state, TagState* tag) {
  state->header = 0;
  state->itemize = 0;
  state->enumerate = 0;

  state->spaces = 0;
  state->accent = 0;
  state->sharp = 0;
  state->last_nb = 'z';
  state->star = false;

  state->blockquote = false;
  state->lineChange = true;
  state->verbatim = false;
  state->inline_verbatim = false;
  state->bold = false;
  state->italic = false;
  state->newline = {};
  state->lastNewline = {};
  
  state->tagState = tag;
  tag->tag = "";
  tag->readingTagName = false;
  tag->closeTag = false;
  tag->openedTags = {};
}
