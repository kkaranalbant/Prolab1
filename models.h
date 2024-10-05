#ifndef MODELS_H
#define MODELS_H

struct UnitType {
    char* kind;
    char* name;
    int attack;
    int defence;
    int hp;
    int criticalRate;
};

struct Research {
    char* name;
    int level;
    int value;
    char* explanation;
};

struct Hero {
    char* kind;
    char* name;
    char* bonusKind;
    char* affectedUnitName;
    int bonusAmount;
    char* explanation;
};

struct Creature {
    char* kind;
    char* name;
    int bonusAmount;
    char* affectionType;
    char* explanation;
};

struct UnitType createUnitType(char* kind, char* name, int attack, int defence, int hp, int criticalRate);
struct Research createResearch(char* name, int level, int value, char* explanation);
struct Hero createHero(char* kind, char* name, int bonusAmount, char* affectedUnitName, char* explanation, char* bonusKind);
struct Creature createCreature(char* kind, char* name, int bonus, char* affectionType, char* explanation);

#endif
