#include <stdlib.h>
#include <stdio.h>
#include "models.h"
#include "teamlist.h"
#include <string.h>
#include <stdbool.h>


bool isHumansHaveActiveTeam();
bool isOrcsHaveActiveTeam();

float calculateHumanAttackPower(int stepNumber) {
    struct Team* currentTeam = head;
    float result = 0;
    while (currentTeam != NULL) {
        if (currentTeam->isHumanUnit && currentTeam->isActiveTeam) {
            int criticalHitStepNumber = 100 / currentTeam->unitType->criticalRate;
            if (stepNumber % criticalHitStepNumber == 0) {
                result += currentTeam->amount * currentTeam->unitType->attack * 1.5;
            } else {
                result += currentTeam->amount * currentTeam->unitType->attack;
            }
        }
        currentTeam = currentTeam->next;
    }
    return result;
}

float calculateOrcAttackPower(int stepNumber) {
    struct Team* currentTeam = head;
    float result = 0;
    while (currentTeam != NULL) {
        if (!currentTeam->isHumanUnit && currentTeam->isActiveTeam) {
            int criticalHitStepNumber = 100 / currentTeam->unitType->criticalRate;
            if (stepNumber % criticalHitStepNumber == 0) {
                result += currentTeam->amount * currentTeam->unitType->attack * 1.5;
            } else {
                result += currentTeam->amount * currentTeam->unitType->attack;
            }
        }
        currentTeam = currentTeam->next;
    }
    return result;
}

float calculateHumanDefenceAmount() {
    struct Team* currentTeam = head;
    float result = 0;
    while (currentTeam != NULL) {
        if (currentTeam->isHumanUnit && currentTeam->isActiveTeam) {
            result += currentTeam->amount * currentTeam->unitType->defence;
        }
        currentTeam = currentTeam->next;
    }
    return result;
}

float calculateHumanMaxDefenceAmount() {
    struct Team* currentTeam = head;
    float result = 0;
    while (currentTeam != NULL) {
        if (currentTeam->isHumanUnit && currentTeam->isActiveTeam) {
            result += currentTeam->amount * currentTeam->unitType->maxDefence;
        }
        currentTeam = currentTeam->next;
    }
    return result;
}

float calculateOrcDefenceAmount() {
    struct Team* currentTeam = head;
    float result = 0;
    while (currentTeam != NULL) {
        if (!currentTeam->isHumanUnit && currentTeam->isActiveTeam) {
            result += currentTeam->amount * currentTeam->unitType->defence;
        }
        currentTeam = currentTeam->next;
    }
    return result;
}

void doDamageToHumanTeam(float totalDamage) {
    struct Team* currentTeam = head;

    float humanDefenceAmount = calculateHumanDefenceAmount();

    float netDamageForAllHumanTeam = totalDamage * (1 - (humanDefenceAmount / totalDamage));

    char firstMessage[256];

    snprintf(firstMessage, sizeof (firstMessage),
            "Orc Takimi Insan Takimina Saldiriyor.\nVerilen Toplam Hasar: %.2f\nVerilen Net Hasar: %.2f\nInsan Imparatorlugunun Defansi: %.2f\n",
            totalDamage, netDamageForAllHumanTeam, humanDefenceAmount);

    addMessageToFile(firstMessage);

    if (netDamageForAllHumanTeam > 0) {
        while (currentTeam != NULL) {
            if (currentTeam->isHumanUnit && currentTeam->isActiveTeam) {
                float defenceAmount = currentTeam->amount * currentTeam->unitType->defence;

                float netDamageForTeam = netDamageForAllHumanTeam * (defenceAmount / humanDefenceAmount);

                int deadAmount = (int) (netDamageForTeam / (currentTeam->hp / currentTeam->amount));
                if (deadAmount > currentTeam->amount) {
                    deadAmount = currentTeam->amount;
                }

                int newTeamAmount = currentTeam->amount - deadAmount;
                float newTeamHp = currentTeam->hp - netDamageForTeam;

                if (newTeamAmount > 0 && newTeamHp > 0) {
                    currentTeam->amount = newTeamAmount;
                    currentTeam->hp = newTeamHp;
                } else {
                    currentTeam->amount = 0;
                    currentTeam->hp = 0;
                    currentTeam->isActiveTeam = false;
                }

                char secondaryMessage[256];

                snprintf(secondaryMessage, sizeof (secondaryMessage),
                        "Orclar %d id li ve %s birlik tipine sahip insan takimina %.2f net hasar vurdu.\n"
                        "Takimin yeni cani: %.2f, Takimin yeni sayisi: %d\n",
                        currentTeam->id, currentTeam->unitType->name, netDamageForTeam, currentTeam->hp, currentTeam->amount);

                addMessageToFile(secondaryMessage);
            }
            currentTeam = currentTeam->next;
        }
    }
}

void doDamageToOrcTeam(float totalDamage) {
    struct Team* currentTeam = head;

    float orcDefenceAmount = calculateOrcDefenceAmount();

    float netDamageForAllOrcTeam = totalDamage * (1 - (orcDefenceAmount / totalDamage));

    char firstMessage[256];

    snprintf(firstMessage, sizeof (firstMessage),
            "Insan Takimi Orc Takimina Saldiriyor.\nVerilen Toplam Hasar: %.2f\nVerilen Net Hasar: %.2f\nOrc Leginin Defansi: %.2f\n",
            totalDamage, netDamageForAllOrcTeam, orcDefenceAmount);

    addMessageToFile(firstMessage);

    if (netDamageForAllOrcTeam > 0) {
        while (currentTeam != NULL) {
            if (!currentTeam->isHumanUnit && currentTeam->isActiveTeam) {
                float defenceAmount = currentTeam->amount * currentTeam->unitType->defence;

                float netDamageForTeam = netDamageForAllOrcTeam * (defenceAmount / orcDefenceAmount);

                int deadAmount = (int) (netDamageForTeam / (currentTeam->hp / currentTeam->amount));
                if (deadAmount > currentTeam->amount) {
                    deadAmount = currentTeam->amount;
                }

                int newTeamAmount = currentTeam->amount - deadAmount;
                float newTeamHp = currentTeam->hp - netDamageForTeam;

                if (newTeamAmount > 0 && newTeamHp > 0) {
                    currentTeam->amount = newTeamAmount;
                    currentTeam->hp = newTeamHp;
                } else {
                    currentTeam->amount = 0;
                    currentTeam->hp = 0;
                    currentTeam->isActiveTeam = false;
                }

                char secondaryMessage[256];

                snprintf(secondaryMessage, sizeof (secondaryMessage),
                        "Insan Imparatorlugu %d id li ve %s birlik tipine sahip takima %.2f net hasar vurdu.\n"
                        "Takimin yeni cani: %.2f, Takimin yeni sayisi: %d\n",
                        currentTeam->id, currentTeam->unitType->name, netDamageForTeam, currentTeam->hp, currentTeam->amount);

                addMessageToFile(secondaryMessage);
            }
            currentTeam = currentTeam->next;
        }
    }
}

void tireAllOfTeams() {
    struct Team* currentTeam = head;
    bool choosenSuvari = false;
    bool choosenOkcu = false;
    bool choosenPiyade = false;
    bool choosenKusatma = false;
    bool choosenDovuscu = false;
    bool choosenTrol = false;
    bool choosenMizrakci = false;
    bool choosenBinici = false;
    while (currentTeam != NULL) {
        if (strcmp(currentTeam->unitType->name, "piyadeler"), !choosenPiyade) {
            currentTeam->unitType->attack *= 0.9;
            currentTeam->unitType->defence *= 0.9;
            choosenPiyade = true;
        } else if (strcmp(currentTeam->unitType->name, "okcular"), !choosenOkcu) {
            currentTeam->unitType->attack *= 0.9;
            currentTeam->unitType->defence *= 0.9;
            choosenOkcu = true;
        } else if (strcmp(currentTeam->unitType->name, "suvariler"), !choosenSuvari) {
            currentTeam->unitType->attack *= 0.9;
            currentTeam->unitType->defence *= 0.9;
            choosenSuvari = true;
        } else if (strcmp(currentTeam->unitType->name, "kusatma_makineleri"), !choosenKusatma) {
            currentTeam->unitType->attack *= 0.9;
            currentTeam->unitType->defence *= 0.9;
            choosenKusatma = true;
        } else if (strcmp(currentTeam->unitType->name, "ork_dovusculeri"), !choosenDovuscu) {
            currentTeam->unitType->attack *= 0.9;
            currentTeam->unitType->defence *= 0.9;
            choosenDovuscu = true;
        } else if (strcmp(currentTeam->unitType->name, "mizrakcilar"), !choosenMizrakci) {
            currentTeam->unitType->attack *= 0.9;
            currentTeam->unitType->defence *= 0.9;
            choosenMizrakci = true;
        } else if (strcmp(currentTeam->unitType->name, "varg_binicileri"), !choosenBinici) {
            currentTeam->unitType->attack *= 0.9;
            currentTeam->unitType->defence *= 0.9;
            choosenBinici = true;
        } else if (strcmp(currentTeam->unitType->name, "troller"), !choosenTrol) {
            currentTeam->unitType->attack *= 0.9;
            currentTeam->unitType->defence *= 0.9;
            choosenTrol = true;
        }

        currentTeam = currentTeam->next;
    }
}

void initHeroes() {
    for (int i = 0; i < 8; i++) {
        if (heroes[i]->isActive) {
            addHeroBonus(heroes[i]);
        }
    }
}

void addHeroBonus(struct Hero* hero) {
    if (strcmp(hero->bonusKind, "saldiri") == 0) {
        addAttackBonus(hero);
    } else if (strcmp(hero->bonusKind, "kritik") == 0) {
        addCriticalBonus(hero);
    } else if (strcmp(hero->bonusKind, "savunma") == 0) {
        addDefenceBonus(hero);
    }
}

void addAttackBonus(struct Hero* hero) {
    struct Team* team = head;
    char firstTwoCharsOfExplanation [2];
    char* explanation = hero->explanation;
    for (int i = 0; i < 2; i++) {
        firstTwoCharsOfExplanation[i] = explanation[i];
    }
    if (strcmp(firstTwoCharsOfExplanation, "pi") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "pi") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "ku") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "ku") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "su") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "su") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "ok") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "ok") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "or") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "or") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "tr") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "tr") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "va") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "va") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "mi") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "mi") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "tu") == 0 && strcmp(hero->kind, "ork_legi")) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "or") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "va") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "tr") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "mi") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "tu") == 0 && strcmp(hero->kind, "insan_imparatorlugu")) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "pi") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "ku") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "su") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "ok") == 0) {
                team->unitType->attack *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    }
}

void addDefenceBonus(struct Hero* hero) {
    struct Team* team = head;
    char firstTwoCharsOfExplanation [2];
    char* explanation = hero->explanation;
    for (int i = 0; i < 2; i++) {
        firstTwoCharsOfExplanation[i] = explanation[i];
    }
    if (strcmp(firstTwoCharsOfExplanation, "pi") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "pi") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "ku") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "ku") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "su") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "su") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "ok") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "ok") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "or") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "or") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "tr") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "tr") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "va") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "va") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "mi") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "mi") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "tu") == 0 && strcmp(hero->kind, "ork_legi")) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "or") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "va") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "tr") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "mi") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "tu") == 0 && strcmp(hero->kind, "insan_imparatorlugu")) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "pi") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "ku") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "su") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "ok") == 0) {
                team->unitType->defence *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    }
}

void addCriticalBonus(struct Hero* hero) {
    struct Team* team = head;
    char firstTwoCharsOfExplanation [2];
    char* explanation = hero->explanation;
    for (int i = 0; i < 2; i++) {
        firstTwoCharsOfExplanation[i] = explanation[i];
    }
    if (strcmp(firstTwoCharsOfExplanation, "pi") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "piyadeler") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "ku") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "kusatma_makineleri") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "su") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "suvariler") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "ok") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "okcular") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "or") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "ork_dovusculeri") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "tr") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "troller") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "va") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "varg_binicileri") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "mi") == 0) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "mizrakcilar") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "tu") == 0 && strcmp(hero->kind, "ork_legi")) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "ork_dovusculeri") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "varg_binicileri") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "troller") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "mizrakcilar") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    } else if (strcmp(firstTwoCharsOfExplanation, "tu") == 0 && strcmp(hero->kind, "insan_imparatorlugu")) {
        while (team != NULL) {
            if (strcmp(team->unitType->name, "piyadeler") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "kusatma_makineleri") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "suvariler") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
        while (team != NULL) {
            if (strcmp(team->unitType->name, "okcular") == 0) {
                team->unitType->criticalRate *= (100 + hero->bonusAmount) / 100;
                break;
            }
            team = team->next;
        }
    }
}

void addCreatureBonuses() {

    for (int i = 0; i < 10; i++) {
        if (creatures[i]->isActive) {
            struct Team* currentTeam = head;
            char* firstTwoCharactersOfExplanation = malloc(3 * sizeof (char));
            firstTwoCharactersOfExplanation[0] = creatures[i]->explanation[0];
            firstTwoCharactersOfExplanation[1] = creatures[i]->explanation[1];
            firstTwoCharactersOfExplanation[2] = '\0';
            if (strcmp(creatures[i]->kind, "insan_imparatorlugu") == 0) {
                if (strcmp(creatures[i]->affectionType, "saldiri") == 0) {
                    if (strcmp(firstTwoCharactersOfExplanation, "pi")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "piyadeler") == 0) {
                                currentTeam->unitType->attack *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "su")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "suvariler") == 0) {
                                currentTeam->unitType->attack *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "ok")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "okcular") == 0) {
                                currentTeam->unitType->attack *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "ku")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "kusatma_makineleri") == 0) {
                                currentTeam->unitType->attack *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    }
                } else if (strcmp(creatures[i]->affectionType, "savunma") == 0) {
                    if (strcmp(firstTwoCharactersOfExplanation, "pi")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "piyadeler") == 0) {
                                currentTeam->unitType->defence *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "su")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "suvariler") == 0) {
                                currentTeam->unitType->defence *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "ok")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "okcular") == 0) {
                                currentTeam->unitType->defence *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "ku")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "kusatma_makineleri") == 0) {
                                currentTeam->unitType->defence *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    }
                } else if (strcmp(creatures[i]->affectionType, "hasar") == 0) {


                } else if (strcmp(creatures[i]->affectionType, "kritik") == 0) {
                    if (strcmp(firstTwoCharactersOfExplanation, "pi")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "piyadeler") == 0) {
                                currentTeam->unitType->criticalRate *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "su")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "suvariler") == 0) {
                                currentTeam->unitType->criticalRate *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "ok")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "okcular") == 0) {
                                currentTeam->unitType->criticalRate *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "ku")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "kusatma_makineleri") == 0) {
                                currentTeam->unitType->criticalRate *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                        }
                        currentTeam = currentTeam->next;
                    }
                }
            } else {
                if (strcmp(creatures[i]->affectionType, "saldiri") == 0) {
                    if (strcmp(firstTwoCharactersOfExplanation, "or")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "ork_dovusculeri") == 0) {
                                currentTeam->unitType->attack *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "mi")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "mizrakcilar") == 0) {
                                currentTeam->unitType->attack *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "va")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "varg_binicileri") == 0) {
                                currentTeam->unitType->attack *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "tr")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "troller") == 0) {
                                currentTeam->unitType->attack *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    }
                } else if (strcmp(creatures[i]->affectionType, "savunma") == 0) {
                    if (strcmp(firstTwoCharactersOfExplanation, "mi")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "mizrakcilar") == 0) {
                                currentTeam->unitType->defence *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "or")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "ork_dovusculeri") == 0) {
                                currentTeam->unitType->defence *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "va")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "varg_binicileri") == 0) {
                                currentTeam->unitType->defence *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "tr")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "troller") == 0) {
                                currentTeam->unitType->defence *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    }
                } else if (strcmp(creatures[i]->affectionType, "hasar") == 0) {


                } else if (strcmp(creatures[i]->affectionType, "kritik") == 0) {
                    if (strcmp(firstTwoCharactersOfExplanation, "or")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "ork_dovusculeri") == 0) {
                                currentTeam->unitType->criticalRate *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;

                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "mi")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "mizrakcilar") == 0) {
                                currentTeam->unitType->criticalRate *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;

                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "tr")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "troller") == 0) {
                                currentTeam->unitType->criticalRate *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;

                        }
                    } else if (strcmp(firstTwoCharactersOfExplanation, "va")) {
                        while (currentTeam != NULL) {
                            if (strcmp(currentTeam->unitType->name, "varg_binicileri") == 0) {
                                currentTeam->unitType->criticalRate *= ((100 + creatures[i]->bonusAmount) / 100);
                                break;
                            }
                            currentTeam = currentTeam->next;
                        }
                    }
                }
            }
        }
    }
}

void initExperiments() {
    for (int i = 0; i < 12; i++) {
        if (researches[i]->isActive) {
            struct Team* currentTeam = head;
            if (researches[i]->isForHuman) {
                bool increasedPiyade = false;
                bool increasedKusatma = false;
                bool increasedOkcu = false;
                bool increasedSuvari = false;
                if (strcmp(researches[i]->name, "savunma_ustaligi") == 0) {
                    while (currentTeam != NULL) {
                        if (currentTeam->isHumanUnit) {
                            if (strcmp(currentTeam->unitType->name, "piyadeler") == 0 && !increasedPiyade) {
                                currentTeam->unitType->defence *= ((100 + researches[i]->value) / 100);
                                increasedPiyade = true;
                            } else if (strcmp(currentTeam->unitType->name, "okcular") == 0 && !increasedOkcu) {
                                currentTeam->unitType->defence *= ((100 + researches[i]->value) / 100);
                                increasedOkcu = true;
                            } else if (strcmp(currentTeam->unitType->name, "suvariler") == 0 && !increasedSuvari) {
                                currentTeam->unitType->defence *= ((100 + researches[i]->value) / 100);
                                increasedSuvari = true;
                            } else if (strcmp(currentTeam->unitType->name, "kusatma_makineleri") == 0 && !increasedKusatma) {
                                currentTeam->unitType->defence *= ((100 + researches[i]->value) / 100);
                                increasedKusatma = true;
                            }
                        }
                        currentTeam = currentTeam->next;
                    }
                } else if (strcmp(researches[i]->name, "saldiri_gelistirmesi") == 0) {
                    while (currentTeam != NULL) {
                        if (currentTeam->isHumanUnit) {
                            if (strcmp(currentTeam->unitType->name, "piyadeler") == 0 && !increasedPiyade) {
                                currentTeam->unitType->attack *= ((100 + researches[i]->value) / 100);
                                increasedPiyade = true;
                            } else if (strcmp(currentTeam->unitType->name, "okcular") == 0 && !increasedOkcu) {
                                currentTeam->unitType->attack *= ((100 + researches[i]->value) / 100);
                                increasedOkcu = true;
                            } else if (strcmp(currentTeam->unitType->name, "suvariler") == 0 && !increasedSuvari) {
                                currentTeam->unitType->attack *= ((100 + researches[i]->value) / 100);
                                increasedSuvari = true;
                            } else if (strcmp(currentTeam->unitType->name, "kusatma_makineleri") == 0 && !increasedKusatma) {
                                currentTeam->unitType->attack *= ((100 + researches[i]->value) / 100);
                                increasedKusatma = true;
                            }
                        }
                        currentTeam = currentTeam->next;
                    }
                } else if (strcmp(researches[i]->name, "elit_egitim") == 0) {
                    while (currentTeam != NULL) {
                        if (currentTeam->isHumanUnit) {
                            if (strcmp(currentTeam->unitType->name, "piyadeler") == 0 && !increasedPiyade) {
                                currentTeam->unitType->criticalRate *= ((100 + researches[i]->value) / 100);
                                increasedPiyade = true;
                            } else if (strcmp(currentTeam->unitType->name, "okcular") == 0 && !increasedOkcu) {
                                currentTeam->unitType->criticalRate *= ((100 + researches[i]->value) / 100);
                                increasedOkcu = true;
                            } else if (strcmp(currentTeam->unitType->name, "suvariler") == 0 && !increasedSuvari) {
                                currentTeam->unitType->criticalRate *= ((100 + researches[i]->value) / 100);
                                increasedSuvari = true;
                            } else if (strcmp(currentTeam->unitType->name, "kusatma_makineleri") == 0 && !increasedKusatma) {
                                currentTeam->unitType->criticalRate *= ((100 + researches[i]->value) / 100);
                                increasedKusatma = true;
                            }
                        }
                        currentTeam = currentTeam->next;
                    }
                } else if (strcmp(researches[i]->name, "kusatma_ustaligi") == 0) {
                    while (currentTeam != NULL) {
                        if (currentTeam->isHumanUnit && strcmp(currentTeam->unitType->name, "kusatma_makineleri") == 0) {
                            currentTeam->unitType->attack *= ((100 + researches[i]->value) / 100);
                            break;
                        }
                        currentTeam = currentTeam->next;
                    }
                }

            } else if (researches[i]->isForOrcs) {
                bool increasedDovuscu = false;
                bool increasedMizrakci = false;
                bool increasedBinici = false;
                bool increasedTroller = false;
                if (strcmp(researches[i]->name, "savunma_ustaligi") == 0) {
                    while (currentTeam != NULL) {
                        if (!currentTeam->isHumanUnit) {
                            if (strcmp(currentTeam->unitType->name, "ork_dovusculeri") == 0 && !increasedDovuscu) {
                                currentTeam->unitType->defence *= ((100 + researches[i]->value) / 100);
                                increasedDovuscu = true;
                            } else if (strcmp(currentTeam->unitType->name, "mizrakcilar") == 0 && !increasedMizrakci) {
                                currentTeam->unitType->defence *= ((100 + researches[i]->value) / 100);
                                increasedMizrakci = true;
                            } else if (strcmp(currentTeam->unitType->name, "varg_binicileri") == 0 && !increasedBinici) {
                                currentTeam->unitType->defence *= ((100 + researches[i]->value) / 100);
                                increasedBinici = true;
                            } else if (strcmp(currentTeam->unitType->name, "troller") == 0 && !increasedTroller) {
                                currentTeam->unitType->defence *= ((100 + researches[i]->value) / 100);
                                increasedTroller = true;
                            }
                        }
                        currentTeam = currentTeam->next;
                    }
                } else if (strcmp(researches[i]->name, "saldiri_gelistirmesi") == 0) {
                    while (currentTeam != NULL) {
                        if (!currentTeam->isHumanUnit) {
                            if (strcmp(currentTeam->unitType->name, "ork_dovusculeri") == 0 && !increasedDovuscu) {
                                currentTeam->unitType->attack *= ((100 + researches[i]->value) / 100);
                                increasedDovuscu = true;
                            } else if (strcmp(currentTeam->unitType->name, "mizrakcilar") == 0 && !increasedMizrakci) {
                                currentTeam->unitType->attack *= ((100 + researches[i]->value) / 100);
                                increasedMizrakci = true;
                            } else if (strcmp(currentTeam->unitType->name, "varg_binicileri") == 0 && !increasedBinici) {
                                currentTeam->unitType->attack *= ((100 + researches[i]->value) / 100);
                                increasedBinici = true;
                            } else if (strcmp(currentTeam->unitType->name, "troller") == 0 && !increasedTroller) {
                                currentTeam->unitType->attack *= ((100 + researches[i]->value) / 100);
                                increasedTroller = true;
                            }
                        }
                        currentTeam = currentTeam->next;
                    }
                } else if (strcmp(researches[i]->name, "elit_egitim") == 0) {
                    while (currentTeam != NULL) {
                        if (!currentTeam->isHumanUnit) {
                            if (strcmp(currentTeam->unitType->name, "ork_dovusculeri") == 0 && !increasedDovuscu) {
                                currentTeam->unitType->criticalRate *= ((100 + researches[i]->value) / 100);
                                increasedDovuscu = true;
                            } else if (strcmp(currentTeam->unitType->name, "mizrakcilar") == 0 && !increasedMizrakci) {
                                currentTeam->unitType->criticalRate *= ((100 + researches[i]->value) / 100);
                                increasedMizrakci = true;
                            } else if (strcmp(currentTeam->unitType->name, "varg_binicileri") == 0 && !increasedBinici) {
                                currentTeam->unitType->criticalRate *= ((100 + researches[i]->value) / 100);
                                increasedBinici = true;
                            } else if (strcmp(currentTeam->unitType->name, "troller") == 0 && !increasedTroller) {
                                currentTeam->unitType->criticalRate *= ((100 + researches[i]->value) / 100);
                                increasedTroller = true;
                            }
                        }
                        currentTeam = currentTeam->next;
                    }

                }
            }
        }
    }
}

void prepareWar() {
    initHeroes();
    initExperiments();
    addCreatureBonuses();
}

void war() {
    prepareWar();
    for (int i = 1; i <= 100; i++) {
        if (i % 5 == 0) {
            tireAllOfTeams();
        }
        if (i % 2 == 1) {
            float humanAttackPower = calculateHumanAttackPower(i);
            doDamageToOrcTeam(humanAttackPower);
            if (!isOrcsHaveActiveTeam()) {
                char message[70];
                snprintf(message, sizeof (message), "Orclarin Tamami Oldu.\nSavasin bittigi adim sayisi : %d", i);
                addMessageToFile(message);
                break;
            }
        } else {
            int orcAttackPower = calculateOrcAttackPower(i);
            doDamageToHumanTeam(orcAttackPower);
            if (!isHumansHaveActiveTeam()) {
                char message[70];
                snprintf(message, sizeof (message), "Insanlarin Tamami Oldu.\nSavasin bittigi adim sayisi : %d", i);
                addMessageToFile(message);
                break;
            }
        }
    }
}

void addMessageToFile(char* message) {
    const char* filePath = "/home/kaan/Desktop/Info.txt";
    FILE* file = fopen(filePath, "a");
    fprintf(file, "%s\n", message);
    fclose(file);
}

bool isHumansHaveActiveTeam() {
    struct Team* currentTeam = head;
    while (currentTeam != NULL) {
        if (currentTeam->isActiveTeam && currentTeam->isHumanUnit) {
            return true;
        }
        currentTeam = currentTeam->next;
    }
    return false;
}

bool isOrcsHaveActiveTeam() {
    struct Team* currentTeam = head;
    while (currentTeam != NULL) {
        if (currentTeam->isActiveTeam && !currentTeam->isHumanUnit) {
            return true;
        }
        currentTeam = currentTeam->next;
    }
    return false;
}



