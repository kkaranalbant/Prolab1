/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   teamlist.h
 * Author: kaan
 *
 * Created on October 6, 2024, 2:04 PM
 */

#ifndef TEAMLIST_H
#define TEAMLIST_H

#ifdef DEFINE_VARIABLES
struct Team* head ;

#else
extern struct Team* head ;

#endif

void addTeam (struct Team* team) ;

bool isEmpty () ;

#endif /* TEAMLIST_H */

