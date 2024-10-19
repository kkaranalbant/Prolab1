#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "models.h"
#define DEFINE_VARIABLES
#include "teamlist.h" 

struct Team* head = NULL;

void addTeam(struct Team* team) {
    if (head == NULL) {
        head = team;
        head->next = NULL ;
        return;
    }

    struct Team* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = team;
    team->next = NULL;
}

bool isEmpty() {
    return head == NULL;
}

void freeTeams() {
    struct Team* current = head;
    while (current != NULL) {
        struct Team* next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
}

void printTeams () {
    struct Team* team = head ;
    while (team != NULL) {
        printf("id : %d\nAmount : %d\nType : %s\n",team->id,team->amount,team->unitType->name) ;
        team = team->next ;
    }
}
