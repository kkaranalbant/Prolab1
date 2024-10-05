#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "models.h"
#include "parser.h"

/*
 nesneler olusacak ancak uygulamada kullanilmasi icin bir veri yapisina kayit edilmeli
 */
void parseFile(char* fileName, bool isUnitType, bool isResearch, bool isHero, bool isScenerio, bool isCreature) {

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
                attack = atoi(value);
                foundAttack = true;
            } else if (strcmp(key, "savunma") == 0) {
                defence = atoi(value);
                foundDefence = true;
            } else if (strcmp(key, "saglik") == 0) {
                hp = atoi(value);
                foundHp = true;
            } else if (strcmp(key, "kritik_sans") == 0) {
                criticalRate = atoi(value);
                foundCriticalRate = true;
            }

            if (foundHp && foundDefence && foundCriticalRate && foundAttack) {
                foundHp = false;
                foundDefence = false;
                foundCriticalRate = false;
                foundAttack = false;
                struct UnitType unitType;
                unitType = createUnitType(kind, name, attack, defence, hp, criticalRate);
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
                struct Research research;
                research = createResearch(name, level, bonusValue, explanation);
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
            printf("%s:%s\n", key, value);

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
            } else if (strcmp(key, "etkiledigi_birim") == 0) {
                affectedUnitName = value;
                foundAffectedUnitName = true;
            } else if (strcmp(key, "bonus_degeri") == 0) {
                bonusAmount = atoi(value);
                foundBonusAmount = true;
            } else if (strcmp(key, "aciklama") == 0) {
                explanation = value;
                foundExplanation = true;
            }

            if (foundAffectedUnitName && foundBonusAmount && foundBonusKind && foundExplanation) {
                foundAffectedUnitName = false;
                foundBonusAmount = false;
                foundBonusKind = false;
                foundExplanation = false;
                struct Hero hero;
                hero = createHero(kind, name, bonusAmount, affectedUnitName, explanation, bonusKind);
            }
        }
    } else if (isCreature) {
        char* kind;
        char* name;
        int bonusAmount;
        char* affectionType;
        char* explanation;
        bool foundBonusAmount = false;
        bool foundAffectionType = false;
        bool foundExplanation = false;

        while (fgets(buffer, sizeof (buffer), dosya)) {
            char* key = getKey(buffer);
            if (key == NULL) continue;
            char* value = getValue(buffer);
            printf("%s:%s\n", key, value);

            if (strcmp(key, "ork_legi") == 0 || strcmp(key, "insan_imparatorlugu") == 0) {
                kind = key;
            } else if (strcmp(key, "Ejderha") == 0 || strcmp(key, "Agri_Dagi_Devleri") == 0 ||
                    strcmp(key, "Tepegoz") == 0 || strcmp(key, "Karakurt") == 0 ||
                    strcmp(key, "Samur") == 0 || strcmp(key, "Kara_Troll") == 0 ||
                    strcmp(key, "Golge_Kurtlari") == 0 || strcmp(key, "Camur_Devleri") == 0 ||
                    strcmp(key, "Ates_Iblisi") == 0 || strcmp(key, "Buz_Devleri") == 0) {
                name = key;
            } else if (strcmp(key, "etki_degeri") == 0) {
                bonusAmount = atoi(value);
                foundBonusAmount = true;
            } else if (strcmp(key, "etki_turu") == 0) {
                affectionType = value;
                foundAffectionType = true;
            } else if (strcmp(key, "aciklama") == 0) {
                explanation = value;
                foundExplanation = true;
            }

            if (foundAffectionType && foundBonusAmount && foundExplanation) {
                foundAffectionType = false;
                foundBonusAmount = false;
                foundExplanation = false;
                struct Creature creature;
                creature = createCreature(kind, name, bonusAmount, affectionType, explanation);
            }
        }

    }

    fclose(dosya);

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