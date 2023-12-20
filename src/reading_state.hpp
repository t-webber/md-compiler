#ifndef TYPE_HPP
#define TYPE_HPP
#include <iostream>

struct ReadingState {
  int header;
  int itemize;
  int enumerate;
  int spaces;
  int accent;
  int sharp;
  char last_nb;
  bool blockquote;
  bool lineChange;
  bool verbatim; 
  bool start_was_char;
};

typedef ReadingState ReadingState;

void printState(ReadingState* state);

bool is_number(char c);

#endif