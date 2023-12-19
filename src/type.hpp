#ifndef TYPE_HPP
#define TYPE_HPP

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
};

typedef ReadingState ReadingState;

#endif