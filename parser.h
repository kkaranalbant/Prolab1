#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

void parseFile(char* fileName, bool isUnitType, bool isResearch, bool isHero, bool isCreature);

char* getKey(char* row);
char* getValue(char* row);
char** getSubstringValues(char* row);

#endif
