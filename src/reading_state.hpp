#ifndef TYPE_HPP
#define TYPE_HPP
#include <iostream>
#include <list>
#include <string>
#include <optional>


struct TagState {
  std::string tag;
  bool readingTagName;
  bool closeTag;
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
  std::string lang;
  TagState* tagState;
  std::optional<bool> newline;
  std::optional<bool> lastNewline;
};
typedef ReadingState ReadingState;

void printState(ReadingState* state);

void initState(ReadingState* state, TagState* tag);


#endif