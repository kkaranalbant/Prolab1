#ifndef MODELS_H
#define MODELS_H
#include <stdbool.h>

struct UnitType {
    char* kind;
    char* name;
    float attack;
    float defence;
    int maxDefence ;
    int hp;
    float criticalRate;
};

struct Research {
    char* name;
    int level;
    int value;
    char* explanation;
    bool isActive;
    bool isForHuman;
    bool isForOrcs ;
};

struct Hero {
    char* kind;
    char* name;
    char* bonusKind;
    char* affectedUnitName;
    int bonusAmount;
    char* explanation;
    bool isActive;
};

struct Creature {
    char* kind;
    char* name;
    int bonusAmount;
    char* affectionType;
    char* explanation;
    bool isActive;
};

struct Team {
    int id;
    struct UnitType* unitType;
    bool isHumanUnit;
    int amount;
    struct Team* next;
};


#ifdef DEFINE_VARIABLES

    struct UnitType* unitTypes[8] ;
    struct Research* researches[12];
    struct Hero* heroes[8];
    struct Creature* creatures[10];
#else
    extern struct UnitType* unitTypes[8];
    extern struct Research* researches[12];
    extern struct Hero* heroes[8];
    extern struct Creature* creatures[10];
#endif


struct UnitType* createUnitType(char* kind, char* name, float attack, float defence, int hp, float criticalRate);
struct Research* createResearch(char* name, int level, int value, char* explanation, bool isActive, bool isForHuman , bool isForOrcs);
struct Hero* createHero(char* kind, char* name, int bonusAmount, char* affectedUnitName, char* explanation, char* bonusKind, bool isActive);
struct Creature* createCreature(char* kind, char* name, int bonus, char* affectionType, char* explanation, bool isActive);
struct UnitType* findUnitTypeByName(char* name);
struct Team* createTeam (int id , struct UnitType* unitType , bool isHumanUnit , int amount);
void initModelArrays () ;
#endif
