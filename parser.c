#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "models.c"

void parseFile(char* fileName, bool isUnitType, bool isResearch, bool isHero, bool isScenerio, bool isCreature);

/*
 nesneler olusacak ancak uygulamada kullanilmasi icin bir veri yapisina kayit edilmeli
 */
void parseFile(char* fileName, bool isUnitType, bool isResearch, bool isHero, bool isScenerio, bool isCreature) {

    FILE *dosya;
    char buffer[2500]; 

    dosya = fopen(fileName, "r");

    if (dosya == NULL) {
        printf("Dosya açılamadı.\n");
        return 1;
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
            char* value = getValue(buffer);
            if (key == "insan_imparatorlugu" || key == "ork_legi") {
                kind = key;
                foundKind = true;
            } else if (key == "piyadeler" || key == "okcular" || key == "suvariler" || key == "kusatma_makineleri" || key == "ork_dovusculeri" || key == "mizrakcilar" || key == "varg_binicileri" || key == "troller") {
                name = key;
                foundName = true;
            } else if (key == "saldiri") {
                attack = atoi(value);
                foundAttack = true;
            } else if (key == "savunma") {
                defence = atoi(value);
                foundDefence = true;
            } else if (key == "saglik") {
                hp = atoi(value);
                foundHp = true;
            } else if (key == "kritik_sans") {
                criticalRate = atoi(value);
                foundCriticalRate = true;
            }
            if (foundHp && foundDefence && foundCriticalRate && foundAttack) {
                foundHp = false;
                foundDefence = false;
                foundCriticalRate = false;
                foundAttack = false;
                createUnitType(kind, name, attack, defence, hp, criticalRate);
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
            char* value = getValue(buffer);
            if (key == "savunma_ustaligi" || key == "saldiri_gelistirmesi" || key == "elit_egitim" || key == "kusatma_ustaligi") {
                name = key;
            } else if (key == "seviye_1") {
                level = 1;
            } else if (key == "seviye_2") {
                level = 2;
            } else if (key == "seviye_3") {
                level = 3;
            } else if (key == "deger") {
                bonusValue = atoi(value);
                foundValue = true;
            } else if (key = "aciklama") {
                explanation = value;
                foundExplanation = true;
            }
            if (foundValue && foundExplanation) {
                foundValue = false;
                foundExplanation = false;
                createResearch(name, level, bonusValue, explanation);
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
            char* value = getValue(buffer);
            if (key == "ork_legi" || key == "insan_imparatorlugu") {
                kind = key;
            } else if (key == "Alparslan" || key == "Yavuz_Sultan_Selim" || key == "Fatih_Sultan_Mehmet" || key == "Tugrul_Bey" || key == "Goruk_Vahsi" || key == "Thruk_Kemikkiran" || key == "Vrog_Kafakiran" || key == "Ugar_Zalim") {
                name = key;
            } else if (key == "bonus_turu") {
                bonusKind = value;
                foundBonusKind = true;
            } else if (key == "etkiledigi_birim") {
                affectedUnitName = value;
                foundAffectedUnitName = true;
            } else if (key == "bonus_degeri") {
                bonusAmount = atoi(value);
                foundBonusAmount = true;
            } else if (key == "aciklama") {
                explanation = value;
                foundExplanation = true;
            }
            if (foundAffectedUnitName && foundBonusAmount && foundBonusKind && foundExplanation) {
                foundAffectedUnitName = false;
                foundBonusAmount = false;
                foundBonusKind = false;
                foundExplanation = false;
                createHero(kind, name, bonusAmount, affectedUnitName, explanation, bonusKind);
            }
        }
    } else if (isCreature) {
        char* kind;
        char* name;
        int bonusAmount;
        char* affectionType;
        char* explanation;
        bool foundBonusAmount = false ;
        bool foundAffectionType = false ;
        bool foundExplanation = false ;
        while (fgets(buffer, sizeof (buffer), dosya)) {
            char* key = getKey(buffer);
            char* value = getValue(buffer);
            if (key == "ork_legi" || key == "insan_imparatorlugu") {
                kind = key;
            }
            else if (key == "Ejderha" || key == "Agri_Dagi_Devleri" || key == "Tepegoz" || key == "Karakurt" key == "Samur" || key == "Kara_Troll" || key == "Golge_Kurtlari" || key == "Camur_Devleri" || key == "Ates_Iblisi" || key == "Buz_Devleri") {
                name = key ;
            }
            else if (key == "etki_degeri") {
                bonusAmount = atoi(value) ;
                foundBonusAmount = true ;
            }
            else if (key == "etki_turu") {
                affectionType = value ;
                foundAffectionType = true ;
            }
            else if (key == "aciklama") {
                explanation = value ;
                foundExplanation = true ;
            }
            if (foundAffectionType && foundAffectionType && foundExplanation) {
                foundAffectionType = false ;
                foundBonusAmount = false ;
                foundExplanation = false ;
                createCreature(kind,name,bonusAmount,affectionType,explanation) ;
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
    }

    return result;

}

/*
 Value sayi ise problemli olacak.
 */
char* getValue(char* row) {

    char* result = NULL;
    size_t length = strlen(row);
    int firstQuatationIndexAfterColon = -1;
    int lastQuatationIndexAfterColon = -1;
    int colonIndex = -1;
    for (int i = 0; i < length; i++) {
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
        for (int i = firstQuatationIndexAfterColon; i < lastQuatationIndexAfterColon; i++) {
            result[counter] = row[i];
            counter++;
        }
    }
    return result;
}




