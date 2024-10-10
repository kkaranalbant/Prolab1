/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.c to edit this template
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define DEFINE_VARIABLES
#include "models.h"

struct UnitType* createUnitType(char* kind, char* name, float attack, float defence, int hp, float criticalRate) {
    struct UnitType* unitType = malloc(sizeof(struct UnitType));
    unitType->kind = kind;
    unitType->name = name;
    unitType->attack = attack;
    unitType->defence = defence;
    unitType->maxDefence = defence ;
    unitType->hp = hp;
    unitType->criticalRate = criticalRate;
    return unitType;
}

struct Research* createResearch(char* name, int level, int value, char* explanation, bool isActive, bool isForHuman , bool isForOrcs) {
    struct Research* research = malloc(sizeof(struct Research));
    research->name = name;
    research->level = level;
    research->value = value;
    research->explanation = explanation;
    research->isActive = isActive;
    research->isForHuman = isForHuman;
    research->isForOrcs = isForOrcs ;
    return research;
}

struct Hero* createHero(char* kind, char* name, int bonusAmount, char* affectedUnitName, char* explanation, char* bonusKind, bool isActive) {
    struct Hero* hero = malloc(sizeof(struct Hero)) ;
    hero->name = name;
    hero->kind = kind;
    hero->bonusAmount = bonusAmount;
    hero->explanation = explanation;
    hero->affectedUnitName = affectedUnitName;
    hero->bonusKind = bonusKind;
    hero->isActive = isActive;
    return hero;
}

struct Creature* createCreature(char* kind, char* name, int bonus, char* affectionType, char* explanation, bool isActive) {
    struct Creature* creature = malloc(sizeof(struct Creature));
    creature->kind = kind;
    creature->name = name;
    creature->bonusAmount = bonus;
    creature->affectionType = affectionType;
    creature->explanation = explanation;
    creature->isActive = isActive;
    return creature;
}

struct Team* createTeam(int id, struct UnitType* unitType, bool isHumanUnit, int amount) {
    struct Team* team = malloc (sizeof(struct Team));
    team->id = id;
    team->unitType = unitType;
    team->isHumanUnit = isHumanUnit;
    team->amount = amount;
    return team;
}

struct UnitType* findUnitTypeByName(char* name) {
    struct UnitType* result = NULL ;
    for (int i = 0; i < 8; i++) {
        if (strcmp(unitTypes[i]->name, name) == 0) {
            result = unitTypes[i];
            break;
        }
    }
    return result;
}


void initModelArrays () {
    for (int i = 0 ; i < 8 ; i++) {
        unitTypes[i] = malloc(sizeof(struct UnitType)) ;
    }
    for (int i = 0 ; i < 12 ; i++) {
        researches[i] = malloc(sizeof(struct Research)) ;
    }
    for (int i = 0 ; i < 8 ; i++) {
        heroes[i] = malloc(sizeof(struct Hero)) ;
    }
    for (int i = 0 ; i < 10 ; i++) {
        creatures[i] = malloc(sizeof(struct Creature)) ;
    }
}
