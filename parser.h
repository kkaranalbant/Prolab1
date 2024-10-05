#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

void parseFile(char* fileName, bool isUnitType, bool isResearch, bool isHero, bool isScenerio, bool isCreature);

char* getKey(char* row);
char* getValue(char* row);

#endif
