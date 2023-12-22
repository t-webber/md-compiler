#ifndef TYPE_HPP
#define TYPE_HPP
#include <iostream>
#include <list>




struct TagState {
  std::string tag;
  bool readingOpenTagName;
  bool readingCloseTagName;
  std::list<std::string> openedTags;
};
typedef TagState TagState;

struct ReadingState {
  int header;
  int itemize;
  int enumerate;
  int spaces;
  int accent;
  int sharp;
  char last_nb;
  bool readingTagName;
  bool insideTag;
  bool blockquote;
  bool lineChange;
  bool verbatim; 
  bool inline_verbatim;
  bool star;
  bool bold;
  bool italic;
  TagState tagState;
};
typedef ReadingState ReadingState;

void printState(ReadingState* state);

void initState(ReadingState* state, TagState* tag);

bool is_number(char c);

#endif