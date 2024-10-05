/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.c to edit this template
 */

#include <stdlib.h>
#include <stdio.h>

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


UnitType createUnitType (char* kind , char* name , int attack , int defence , int hp , int criticalRate) {
    struct UnitType unitType ;
    unitType.kind = kind ;
    unitType.name = name ;
    unitType.attack = attack ;
    unitType.defence = defence ;
    unitType.hp = hp ;
    unitType.criticalRate = criticalRate ;
    return unitType ;
}

Research createResearch (char* name , int level , int value  ,  char* explanation) {
    struct Research research ;
    research.name = name ;
    research.level = level ;
    research.value = value ;
    research.explanation = explanation ;
    return research ;
}

Hero createHero (char* kind , char* name , int bonusAmount , char* affectedUnitName , char* explanation , char* bonusKind) {
    struct Hero hero ;
    hero.name = name ;
    hero.kind = kind ;
    hero.bonusAmount = bonusAmount ;
    hero.explanation = explanation ;
    hero.affectedUnitName = affectedUnitName ;
    hero.bonusKind = bonusKind ;
    return hero ;
}

Creature createCreature (char* kind , char* name , int bonus , char* affectionType , char* explanation) {
    struct Creature creature ;
    creature.kind = kind ;
    creature.name = name ;
    creature.bonusAmount = bonus ;
    creature.affectionType = affectionType ;
    creature.explanation = explanation ;
    return creature ;
}