#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "httpsreq.h"
#include "models.h"

int main() {
    //parseFile("/home/kaan/Downloads/main_scenario_2/research.json",false,true,false,false,false) ;calisiyor
    //char* data = makeHttpsRequest("https://test-link"); calisiyor
    //printf("%s",data);
    initModelArrays();
    parseFile("/home/kaan/Downloads/Prolab_I_json (2)/unit_types.json", true, false, false, false);
    parseFile("/home/kaan/Downloads/Prolab_I_json (2)/heroes.json", false, false, true, false);
    parseFile("/home/kaan/Downloads/Prolab_I_json (2)/research.json", false, true, false, false);
    parseFile("/home/kaan/Downloads/Prolab_I_json (2)/creatures.json", false, false, false, true);
    parseFile("/home/kaan/Downloads/Prolab_I_json (2)/main_scenario_1.json", false, false, false, false);
//    for (int i = 0; i < 10; i++) {
//        struct Creature* creature;
//        creature = creatures[i];
//        if (creature->isActive) {
//            printf("Aktif Canavar : %s : %s\n", creature->kind, creature->name);
//        }
//    }
//    
//    for (int i = 0 ; i < 8 ; i++) {
//        struct Hero* hero ;
//        hero = heroes[i] ;
//        //if (hero->isActive) {
//            printf("Aktif Hero : %s : %s\n",hero->kind , hero->name) ;
//        //}
//    }
//    
//    for (int i = 0 ; i < 12 ; i++) {
//        struct Research* research ;
//        research = researches[i] ;
//        if (research->isActive) {
//            printf("Aktif research : %s : %d insan icin : %b ork icin : %b \n",research->name , research->level,research->isForHuman,research->isForOrcs) ;
//        }
//    }
    

    war();

    printTeams();

    return 0;
}




