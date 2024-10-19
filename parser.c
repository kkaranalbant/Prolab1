#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "models.h"
#include "parser.h"
#include "teamlist.h"
#include <time.h>

/*
 nesneler olusacak ancak uygulamada kullanilmasi icin bir veri yapisina kayit edilmeli
 */
void parseFile(char* fileName, bool isUnitType, bool isResearch, bool isHero, bool isCreature) {

    FILE *dosya;
    char buffer[2500];

    dosya = fopen(fileName, "r");

    if (dosya == NULL) {
        printf("Dosya açılamadı.\n");
        return;
    }

    if (isUnitType) {
        char* kind = NULL;
        char* name = NULL;
        int attack = 0;
        int defence = 0;
        int hp = 0;
        int criticalRate = 0;
        bool foundAttack = false;
        bool foundDefence = false;
        bool foundHp = false;
        bool foundCriticalRate = false;
        while (fgets(buffer, sizeof (buffer), dosya)) {
            char* key = getKey(buffer);
            if (key == NULL) continue;
            char* value = getValue(buffer);
            if (strcmp(key, "insan_imparatorlugu") == 0 || strcmp(key, "ork_legi") == 0) {
                kind = key;
            } else if (strcmp(key, "piyadeler") == 0 || strcmp(key, "okcular") == 0 ||
                    strcmp(key, "suvariler") == 0 || strcmp(key, "kusatma_makineleri") == 0 ||
                    strcmp(key, "ork_dovusculeri") == 0 || strcmp(key, "mizrakcilar") == 0 ||
                    strcmp(key, "varg_binicileri") == 0 || strcmp(key, "troller") == 0) {
                name = key;
            } else if (strcmp(key, "saldiri") == 0) {
                attack = atof(value);
                foundAttack = true;
            } else if (strcmp(key, "savunma") == 0) {
                defence = atof(value);
                foundDefence = true;
            } else if (strcmp(key, "saglik") == 0) {
                hp = atoi(value);
                foundHp = true;
            } else if (strcmp(key, "kritik_sans") == 0) {
                criticalRate = atof(value);
                foundCriticalRate = true;
            }
            if (foundHp && foundDefence && foundCriticalRate && foundAttack) {
                foundHp = false;
                foundDefence = false;
                foundCriticalRate = false;
                foundAttack = false;
                struct UnitType* unitType = createUnitType(kind, name, attack, defence, hp, criticalRate);
                for (int i = 0; i < 8; i++) {
                    if (unitTypes[i]->name == NULL) {
                        unitTypes[i] = unitType;
                        break;
                    }
                }
            }
        }
    } else if (isResearch) {
        char* name = NULL;
        int level = 0;
        int bonusValue = 0;
        char* explanation = NULL;
        bool foundValue = false;
        bool foundExplanation = false;
        while (fgets(buffer, sizeof (buffer), dosya)) {
            char* key = getKey(buffer);
            if (key == NULL) continue;
            char* value = getValue(buffer);
            if (strcmp(key, "savunma_ustaligi") == 0 || strcmp(key, "saldiri_gelistirmesi") == 0 ||
                    strcmp(key, "elit_egitim") == 0 || strcmp(key, "kusatma_ustaligi") == 0) {
                name = key;
            } else if (strcmp(key, "seviye_1") == 0) {
                level = 1;
            } else if (strcmp(key, "seviye_2") == 0) {
                level = 2;
            } else if (strcmp(key, "seviye_3") == 0) {
                level = 3;
            } else if (strcmp(key, "deger") == 0) {
                bonusValue = atoi(value);
                foundValue = true;
            } else if (strcmp(key, "aciklama") == 0) {
                explanation = value;
                foundExplanation = true;
            }

            if (foundValue && foundExplanation) {
                foundValue = false;
                foundExplanation = false;
                struct Research* research;
                research = createResearch(name, level, bonusValue, explanation, false, false, false);
                for (int i = 0; i < 12; i++) {
                    if (researches[i]->name == NULL) {
                        researches[i] = research;
                        break;
                    }
                }
            }
        }

    } else if (isHero) {
        char* kind;
        char* name;
        char* bonusKind;
        char* affectedUnitName;
        int bonusAmount;
        char* explanation;
        bool foundBonusKind = false;
        bool foundAffectedUnitName = false;
        bool foundBonusAmount = false;
        bool foundExplanation = false;
        while (fgets(buffer, sizeof (buffer), dosya)) {
            char* key = getKey(buffer);
            if (key == NULL) continue;
            char* value = getValue(buffer);
            if (strcmp(key, "ork_legi") == 0 || strcmp(key, "insan_imparatorlugu") == 0) {
                kind = key;
            } else if (strcmp(key, "Alparslan") == 0 || strcmp(key, "Yavuz_Sultan_Selim") == 0 ||
                    strcmp(key, "Fatih_Sultan_Mehmet") == 0 || strcmp(key, "Tugrul_Bey") == 0 ||
                    strcmp(key, "Goruk_Vahsi") == 0 || strcmp(key, "Thruk_Kemikkiran") == 0 ||
                    strcmp(key, "Vrog_Kafakiran") == 0 || strcmp(key, "Ugar_Zalim") == 0) {
                name = key;
            } else if (strcmp(key, "bonus_turu") == 0) {
                bonusKind = value;
                foundBonusKind = true;
            } else if (strcmp(key, "bonus_degeri") == 0) {
                bonusAmount = atoi(value);
                foundBonusAmount = true;
            } else if (strcmp(key, "aciklama") == 0) {
                explanation = value;
                char* firstTwoCharsOfExplanation = malloc(3 * sizeof (char));
                for (int i = 0; i < 2; i++) {
                    firstTwoCharsOfExplanation[i] = explanation[i];
                }
                firstTwoCharsOfExplanation[2] = '\0';
                if (strcmp(firstTwoCharsOfExplanation, "pi") == 0) {
                    affectedUnitName = "piyadeler";
                } else if (strcmp(firstTwoCharsOfExplanation, "ku") == 0) {
                    affectedUnitName = "kusatma_makineleri";
                } else if (strcmp(firstTwoCharsOfExplanation, "su") == 0) {
                    affectedUnitName = "suvariler";
                } else if (strcmp(firstTwoCharsOfExplanation, "ok") == 0) {
                    affectedUnitName = "okcular";
                } else if (strcmp(firstTwoCharsOfExplanation, "or") == 0) {
                    affectedUnitName = "ork_dovusculeri";
                } else if (strcmp(firstTwoCharsOfExplanation, "tr") == 0) {
                    affectedUnitName = "troller";
                } else if (strcmp(firstTwoCharsOfExplanation, "va") == 0) {
                    affectedUnitName = "varg_binicileri";
                } else if (strcmp(firstTwoCharsOfExplanation, "mi") == 0) {
                    affectedUnitName = "mizrakcilar";
                } else if (strcmp(firstTwoCharsOfExplanation, "tu") == 0 && strcmp(kind, "ork_legi") == 0) {
                    affectedUnitName = "all";
                }
                foundAffectedUnitName = true;
            }
            if (foundBonusAmount && foundBonusKind && foundExplanation, foundAffectedUnitName) {
                foundBonusAmount = false;
                foundBonusKind = false;
                foundExplanation = false;
                foundAffectedUnitName = false;

                struct Hero* hero;
                hero = createHero(kind, name, bonusAmount, affectedUnitName, explanation, bonusKind, false);
                for (int i = 0; i < 9; i++) {
                    if (heroes[i]->name == NULL) {
                        heroes[i] = hero;
                        break;
                    }
                }
            }
        }
    } else if (isCreature) {
        char* kind;
        char* name;
        int bonusAmount;
        char* affectionType;
        char* explanation;
        char* affectedUnitName;
        bool foundBonusAmount = false;
        bool foundAffectionType = false;
        bool foundExplanation = false;
        bool foundAffectedUnitName = false;
        while (fgets(buffer, sizeof (buffer), dosya)) {
            char* key = getKey(buffer);
            if (key == NULL) continue;
            char* value = getValue(buffer);
            if (strcmp(key, "ork_legi") == 0 || strcmp(key, "insan_imparatorlugu") == 0) {
                kind = key;
            } else if (strcmp(key, "Ejderha") == 0 || strcmp(key, "Agri_Dagi_Devleri") == 0 ||
                    strcmp(key, "Tepegoz") == 0 || strcmp(key, "Karakurt") == 0 ||
                    strcmp(key, "Samur") == 0 || strcmp(key, "Kara_Troll") == 0 ||
                    strcmp(key, "Golge_Kurtlari") == 0 || strcmp(key, "Camur_Devleri") == 0 ||
                    strcmp(key, "Ates_Iblisi") == 0 || strcmp(key, "Buz_Devleri") == 0 || strcmp(key, "Makrog_Savas_Beyi") == 0) {
                name = key;
            } else if (strcmp(key, "etki_degeri") == 0) {
                bonusAmount = atoi(value);
                foundBonusAmount = true;
            } else if (strcmp(key, "etki_turu") == 0) {
                affectionType = value;
                foundAffectionType = true;
            } else if (strcmp(key, "aciklama") == 0) {
                explanation = value;
                char* firstTwoCharsOfExplanation = malloc(3 * sizeof (char));
                for (int i = 0; i < 2; i++) {
                    firstTwoCharsOfExplanation[i] = explanation[i];
                }
                firstTwoCharsOfExplanation[2] = '\0';
                if (strcmp(firstTwoCharsOfExplanation, "pi") == 0) {
                    affectedUnitName = "piyadeler";
                } else if (strcmp(firstTwoCharsOfExplanation, "ku") == 0) {
                    affectedUnitName = "kusatma_makineleri";
                } else if (strcmp(firstTwoCharsOfExplanation, "su") == 0) {
                    affectedUnitName = "suvariler";
                } else if (strcmp(firstTwoCharsOfExplanation, "ok") == 0) {
                    affectedUnitName = "okcular";
                } else if (strcmp(firstTwoCharsOfExplanation, "or") == 0) {
                    affectedUnitName = "ork_dovusculeri";
                } else if (strcmp(firstTwoCharsOfExplanation, "tr") == 0) {
                    affectedUnitName = "troller";
                } else if (strcmp(firstTwoCharsOfExplanation, "va") == 0) {
                    affectedUnitName = "varg_binicileri";
                } else if (strcmp(firstTwoCharsOfExplanation, "mi") == 0) {
                    affectedUnitName = "mizrakcilar";
                }
                foundAffectedUnitName = true;
                foundExplanation = true;
            }

            if (foundAffectionType && foundBonusAmount && foundExplanation && foundAffectedUnitName) {
                foundAffectionType = false;
                foundBonusAmount = false;
                foundExplanation = false;
                foundAffectedUnitName = false;
                struct Creature* creature;
                creature = createCreature(kind, name, bonusAmount, affectionType, explanation, false, affectedUnitName);
                for (int i = 0; i < 11; i++) {
                    if (creatures[i]->name == NULL) {
                        creatures[i] = creature;
                        break;
                    }
                }
            }
        }

    } else {
        char* kind = NULL;
        while (fgets(buffer, sizeof (buffer), dosya)) {
            char* key = getKey(buffer);
            if (key == NULL) continue;
            if (strcmp(key, "insan_imparatorlugu") == 0 || strcmp(key, "ork_legi") == 0) kind = key;
            if (strcmp(key, "canavarlar") == 0) {
                char** substrings = getSubstringValues(buffer);
                for (int i = 0; substrings[i] != NULL; i++) {
                    char* substring = substrings[i];
                    for (int j = 0; j < 11; j++) {
                        struct Creature* creature;
                        creature = creatures[j];
                        if (creature->name != NULL && strcmp(creature->name, substring) == 0) {
                            creature->isActive = true;
                            break;
                        }

                    }

                }
            } else if (strcmp(key, "kahramanlar") == 0) {
                char** substrings = getSubstringValues(buffer);
                for (int i = 0; substrings[i] != NULL; i++) {
                    char* substring = substrings[i];
                    for (int j = 0; j < 9; j++) {
                        struct Hero* hero;
                        hero = heroes[j];
                        if (hero->name != NULL && strcmp(hero->name, substring) == 0) {
                            hero->isActive = true;
                            break;
                        }
                    }
                }

            } else if (strcmp(key, "piyadeler") == 0 || strcmp(key, "okcular") == 0 || strcmp(key, "suvariler") == 0 || strcmp(key, "kusatma_makineleri") == 0) {
                char* value = getValue(buffer);
                struct UnitType* unitType;
                unitType = findUnitTypeByName(key);
                int valueDecimal = atoi(value);
                int teamAmount = valueDecimal / 100;
                srand(time(0));
                for (int i = 0; i < teamAmount; i++) {
                    int randomId = rand() % 1000;
                    struct Team* team = createTeam(randomId, unitType, true, 100);
                    addTeam(team);
                }
                if (valueDecimal % 100 != 0) {
                    int lastTeamAmount = valueDecimal % 100;
                    struct Team* team;
                    team = createTeam(rand() % 1000, unitType, true, lastTeamAmount);
                    addTeam(team);
                }
            } else if (strcmp(key, "ork_dovusculeri") == 0 || strcmp(key, "mizrakcilar") == 0 || strcmp(key, "varg_binicileri") == 0 || strcmp(key, "troller") == 0) {
                char* value = getValue(buffer);
                struct UnitType* unitType;
                unitType = findUnitTypeByName(key);
                int valueDecimal = atoi(value);
                int teamAmount = valueDecimal / 100;
                srand(time(0));
                for (int i = 0; i < teamAmount; i++) {
                    struct Team* team;
                    team = createTeam(rand() % 1000, unitType, false, 100);
                    addTeam(team);
                }
                if (valueDecimal % 100 != 0) {
                    int lastTeamAmount = valueDecimal % 100;
                    struct Team* team;
                    team = createTeam(rand() % 1000, unitType, false, lastTeamAmount);
                    addTeam(team);
                }
            } else if (strcmp(key, "savunma_ustaligi") == 0 || strcmp(key, "saldiri_gelistirmesi") == 0 || strcmp(key, "elit_egitim") == 0 || strcmp(key, "kusatma_ustaligi") == 0) {
                char* value = getValue(buffer);
                for (int i = 0; i < 12; i++) {
                    struct Research* research;
                    research = researches[i];
                    if (research->name != NULL && strcmp(key, research->name) == 0 && research->level == atoi(value)) {
                        research->isActive = true;
                        if (kind[0] != NULL && strcmp(kind, "ork_legi") == 0) {
                            research->isForHuman = false;
                            research->isForOrcs = true;
                        } else {
                            research->isForHuman = true;
                            research->isForOrcs = false;
                        }
                        break;
                    }
                }
            }

        }

        fclose(dosya);

    }
}

char* getKey(char* row) {

    char* result = NULL;

    size_t length = strlen(row);

    int firstQuatationIndex = -1;
    int lastQuatationIndex = -1;

    for (int i = 0; i < length; i++) {
        if (row[i] == '"' && firstQuatationIndex == -1) {
            firstQuatationIndex = i;
            continue;
        }
        if (row[i] == ':') {
            lastQuatationIndex = i - 1;
        }
    }
    if (firstQuatationIndex != -1 && lastQuatationIndex != -1) {
        result = (char*) malloc((lastQuatationIndex - firstQuatationIndex) * sizeof (char));
        int counter = 0;
        for (int i = firstQuatationIndex + 1; i < lastQuatationIndex; i++) {
            result[counter] = row[i];
            counter++;
        }
        result[counter] = '\0';
    }
    return result;

}

char* getValue(char* row) {

    char* result = NULL;
    size_t length = strlen(row);
    int firstQuatationIndexAfterColon = -1;
    int lastQuatationIndexAfterColon = -1;
    int colonIndex = -1;
    for (int i = 0; i < length; i++) {
        if (row[i] == '{') {
            result = "";
            return result;
        }
        if (row[i] == ':' && colonIndex == -1) {
            colonIndex = i;
            continue;
        }
        if (colonIndex != -1 && i > colonIndex && row[i] == '"' && firstQuatationIndexAfterColon == -1) {
            firstQuatationIndexAfterColon = i;
            continue;
        }
        if (colonIndex != -1 && i > colonIndex && row[i] == '"' && lastQuatationIndexAfterColon == -1) {
            lastQuatationIndexAfterColon = i;
            continue;
        }
    }

    if (lastQuatationIndexAfterColon != -1 && firstQuatationIndexAfterColon != -1) {
        result = (char*) malloc((lastQuatationIndexAfterColon - firstQuatationIndexAfterColon) * sizeof (char));
        int counter = 0;
        for (int i = firstQuatationIndexAfterColon + 1; i < lastQuatationIndexAfterColon; i++) {
            result[counter] = row[i];
            counter++;
        }
        result[counter] = '\0';
    } else if (lastQuatationIndexAfterColon == -1 && firstQuatationIndexAfterColon == -1) {
        int counter = 0;
        for (int i = colonIndex + 2; (row[i] != ',' || row[i] != ' ') && row[i] != '\0'; i++) {
            counter++;
        }
        result = (char*) malloc((counter + 1) * sizeof (char));
        int j = 0;
        for (int i = colonIndex + 2; i < colonIndex + counter; i++) {
            if (i == colonIndex + counter - 1 && row[i] == ',') {
                break;
            }
            result[j] = row[i];
            j++;
        }
        result[j] = '\0';
    }
    return result;
}

char** getSubstringValues(char* row) {
    size_t length = strlen(row);
    int colonIndex = 0;

    // Find the colon index
    for (int i = 0; i < length; i++) {
        if (row[i] == ':') {
            colonIndex = i;
            break;
        }
    }

    int lastBracketIndex = 0;
    // Find the closing bracket index
    for (int i = colonIndex + 2; i < length; i++) {
        if (row[i] == ']') {
            lastBracketIndex = i;
            break;
        }
    }

    // Extract substring inside brackets
    char* inBrackets = (char*) malloc((lastBracketIndex - (colonIndex + 2)) * sizeof (char));
    if (!inBrackets) return NULL; // Check for allocation failure
    int counter = 0;

    for (int i = colonIndex + 3; i < lastBracketIndex; i++) {
        inBrackets[counter] = row[i];
        counter++;
    }
    inBrackets[counter] = '\0';

    // Count the number of commas to determine the number of substrings
    size_t inBracketsLength = strlen(inBrackets);
    int commaCounter = 0;
    for (int i = 0; i < inBracketsLength; i++) {
        if (inBrackets[i] == ',') commaCounter++;
    }

    // Allocate memory for subStrings array
    char** subStrings = (char**) malloc((commaCounter + 1) * sizeof (char*));
    if (!subStrings) {
        free(inBrackets); // Clean up in case of allocation failure
        return NULL;
    }

    // Split the string into substrings
    int startIndex = 0;
    int finishIndex = 0;
    int subStringCounter = 0;

    for (int i = 0; i < inBracketsLength; i++) {
        if (inBrackets[i] == ',' || i == inBracketsLength - 1) {
            finishIndex = i;
            subStrings[subStringCounter] = (char*) malloc((finishIndex - startIndex + 1) * sizeof (char));
            int subStringIndexCounter = 0;
            for (int i = startIndex; i < finishIndex; i++) {
                if (inBrackets[i] == '"') continue;
                subStrings[subStringCounter][subStringIndexCounter] = inBrackets[i];
                subStringIndexCounter++;
            }
            subStrings[subStringCounter][subStringIndexCounter] = '\0'; // Null-terminate
            subStringCounter++;
            startIndex = finishIndex + 1;
            if (inBrackets[startIndex] == ' ') {
                startIndex++;
            }
            i = startIndex;
        }
    }
    subStrings[subStringCounter] = '\0';

    free(inBrackets); // Clean up temporary string
    return subStrings; // Return allocated array

}
