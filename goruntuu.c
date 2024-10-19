#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "models.h"
#include "teamlist.h"
#include <stdbool.h>
#include <string.h>
#include "httpsreq.h"
#include "parser.h"
#include <stdlib.h>
#include "goruntu.h"

SDL_Color blueColor = {0, 0, 255, 255};
SDL_Color greenColor = {0, 255, 0, 255};
SDL_Color yellowColor = {255, 255, 0, 255};
SDL_Color orangeColor = {255, 165, 0, 255};
SDL_Color purpleColor = {128, 0, 128, 255};
SDL_Color greyColor = {169, 169, 169, 255};
SDL_Color brownColor = {165, 42, 42, 255};
SDL_Color blackColor = {0, 0, 0, 255};

TTF_Font* font = NULL;

int slidingAmount = -1;
int gridDimension = 54;
int cellDimension = 54;
int enlemBaslamaNoktasi = 440;
int boylamInsanBaslamaNoktasi = 0;
int boylamOrkBaslamaNoktasi = 1024;
int enlemOrkBaslamaNoktasi = 1466;
int infoMesssagePixelChanger = 500;
int messageCount = 0;
char** messages = NULL;
bool isInfoMessageRendered = false;


void drawHealthBar(SDL_Renderer* renderer, int x, int y, int cellSize, int health) {
    SDL_Rect healthBar = {x + 5, y + 5, cellSize - 10, cellSize / 4};


    if (health > 80) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
    else if (health > 20) SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // yellow
    else SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red

    healthBar.w = (cellSize - 10) * health / 100;
    SDL_RenderFillRect(renderer, &healthBar);
}

void drawFilledDownwardTriangle(SDL_Renderer* renderer, int x, int y, int size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int i = 0; i <= size; i++) {
        int left = x + size / 2 - (i * (size / 2) / size);
        int right = x + size / 2 + (i * (size / 2) / size);
        SDL_RenderDrawLine(renderer, left, y + size - i, right, y + size - i);
    }
}

void drawFilledUpwardTriangle(SDL_Renderer* renderer, int x, int y, int size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int i = 0; i <= size; i++) {
        int left = x + size / 2 - (i * (size / 2) / size);
        int right = x + size / 2 + (i * (size / 2) / size);
        SDL_RenderDrawLine(renderer, left, y + i, right, y + i);
    }
}

void renderText(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color color) {

    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_FreeSurface(surface);

    SDL_Rect destRect = {x, y, textWidth, textHeight};
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_DestroyTexture(texture);
}

void distributeHumans(SDL_Renderer* renderer, int totalWarriors, int startX, int startY, char* type) {
    int typeInt = 0;
    if (strcmp("piyadeler", type) == 0) {
        typeInt = 1;
    } else if (strcmp("okcular", type) == 0) {
        typeInt = 2;
    } else if (strcmp("suvariler", type) == 0) {
        typeInt = 3;
    } else if (strcmp("kusatma_makineleri", type) == 0) {
        typeInt = 4;
    }
    slidingAmount += 1;
    SDL_Color triangleColor;
    if (typeInt == 1) {
        triangleColor = blueColor;
    } else if (typeInt == 2) {
        triangleColor = yellowColor;
    } else if (typeInt == 3) {
        triangleColor = greenColor;
    } else if (typeInt == 4) {
        triangleColor = orangeColor;
    }

    int currentY = startY + (typeInt - 1) * cellDimension;

    drawHealthBar(renderer, startX + slidingAmount * cellDimension, currentY, cellDimension, 100);

    SDL_Color whiteColor = {0, 0, 0, 255};

    char countStr [10];
    sprintf(countStr, "%d", totalWarriors);

    for (int i = 0; i < 9; i++) {
        if (heroes[i]->isActive && strcmp(heroes[i]->kind, "insan_imparatorlugu") == 0 && strcmp(type, heroes[i]->affectedUnitName) == 0) {
            for (int j = 0; j < 11; j++) {
                struct Creature* creature = creatures[j];
                if (creature->isActive && strcmp(creature->kind, "insan_imparatorlugu") == 0 && strcmp(creature->affectedUnitName, type) == 0) {
                    char infoMessage1 [100];
                    sprintf(infoMessage1, "%s : %s", heroes[i]->name, heroes[i]->explanation);
                    addMessage(infoMessage1);
                    char infoMessage2 [100];
                    sprintf(infoMessage2, "%s : %s", creature->name, creature->explanation);
                    addMessage(infoMessage2);
                    sprintf(countStr, "%d %s", totalWarriors, " +HC");
                    renderText(renderer, countStr, startX + slidingAmount * cellDimension + 15, currentY + 40, whiteColor);
                    drawFilledDownwardTriangle(renderer, startX + slidingAmount * cellDimension + 15, currentY + 20, 20, triangleColor);
                    return;
                }
            }
        }
    }

    for (int i = 0; i < 9; i++) {
        if (heroes[i]->isActive && strcmp(heroes[i]->kind, "insan_imparatorlugu") == 0 && strcmp(type, heroes[i]->affectedUnitName) == 0) {
            char infoMessage [100];
            sprintf(infoMessage, "%s : %s", heroes[i]->name, heroes[i]->explanation);
            addMessage(infoMessage);
            sprintf(countStr, "%d %s", totalWarriors, " +H");
        } else if (heroes[i]->isActive && strcmp(heroes[i]->kind, "insan_imparatorlugu") == 0) {
            char infoMessage [100];
            sprintf(infoMessage, "%s : %s", heroes[i]->name, heroes[i]->explanation);
            addMessage(infoMessage);
        }
    }


    for (int i = 0; i < 11; i++) {
        struct Creature* creature = creatures[i];
        if (creature->isActive && strcmp(creature->kind, "insan_imparatorlugu") == 0 && strcmp(creature->affectedUnitName, type) == 0) {
            char infoMessage [100];
            sprintf(infoMessage, "%s : %s", creature->name, creature->explanation);
            addMessage(infoMessage);
            sprintf(countStr, "%d %s", totalWarriors, " +C");
        } else if (creature->isActive && strcmp(creature->kind, "insan_imparatorlugu") == 0) {
            char infoMessage [100];
            sprintf(infoMessage, "%s : %s", creature->name, creature->explanation);
            addMessage(infoMessage);
        }
    }

    renderText(renderer, countStr, startX + slidingAmount * cellDimension + 15, currentY + 40, whiteColor);
    drawFilledDownwardTriangle(renderer, startX + slidingAmount * cellDimension + 15, currentY + 20, 20, triangleColor);

}

void distributeOrks(SDL_Renderer* renderer, int totalOrks, int startX, int startY, char* type) {


    SDL_Color triangleColor;
    int typeInt = 0;
    if (strcmp("ork_dovusculeri", type) == 0) {
        typeInt = 1;
    } else if (strcmp("mizrakcilar", type) == 0) {
        typeInt = 2;
    } else if (strcmp("varg_binicileri", type) == 0) {
        typeInt = 3;
    } else if (strcmp("troller", type) == 0) {
        typeInt = 4;
    }
    slidingAmount += 1;
    if (typeInt == 1) {
        triangleColor = blackColor;
    } else if (typeInt == 2) {
        triangleColor = brownColor;
    } else if (typeInt == 3) {
        triangleColor = greyColor;
    } else if (typeInt == 4) {
        triangleColor = purpleColor;
    }

    int currentY = startY - (typeInt - 1) * cellDimension;
    drawHealthBar(renderer, startX - slidingAmount * cellDimension, currentY - 54, cellDimension, 100);

    SDL_Color whiteColor = {0, 0, 0, 255};
    char countStr[10];
    sprintf(countStr, "%d", totalOrks);

    for (int i = 0; i < 9; i++) {
        if ((heroes[i]->isActive && strcmp(heroes[i]->kind, "ork_legi") == 0 && strcmp(type, heroes[i]->affectedUnitName) == 0) || (heroes[i]->isActive && strcmp(heroes[i]->name, "Ugar_Zalim") == 0)) {
            for (int j = 0; j < 11; j++) {
                struct Creature* creature = creatures[j];
                if ((creature->isActive && strcmp(creature->kind, "ork_legi") == 0 && strcmp(creature->affectedUnitName, type) == 0)) {
                    char infoMessage1 [100];
                    sprintf(infoMessage1, "%s : %s", heroes[i]->name, heroes[i]->explanation);
                    addMessage(infoMessage1);
                    char infoMessage2 [100];
                    sprintf(infoMessage2, "%s : %s", creature->name, creature->explanation);
                    addMessage(infoMessage2);
                    sprintf(countStr, "%d %s", totalOrks, " +HC");
                    renderText(renderer, countStr, startX - slidingAmount * cellDimension + 15, currentY - 15, whiteColor);
                    drawFilledUpwardTriangle(renderer, startX - slidingAmount * cellDimension + 15, currentY - 35, 20, triangleColor);
                    return;
                }
            }
        }
    }

    for (int i = 0; i < 9; i++) {
        if ((heroes[i]->isActive && strcmp(heroes[i]->kind, "ork_legi") == 0 && strcmp(type, heroes[i]->affectedUnitName) == 0) || (heroes[i]->isActive && strcmp("Ugar_Zalim", heroes[i]->name) == 0)) {
            char infoMessage [100];
            sprintf(infoMessage, "%s : %s", heroes[i]->name, heroes[i]->explanation);
            addMessage(infoMessage);
            sprintf(countStr, "%d %s", totalOrks, " +H");
        } else if (heroes[i]->isActive && strcmp(heroes[i]->kind, "ork_legi") == 0) {
            char infoMessage [100];
            sprintf(infoMessage, "%s : %s", heroes[i]->name, heroes[i]->explanation);
            addMessage(infoMessage);
        }
    }


    for (int i = 0; i < 11; i++) {
        struct Creature* creature = creatures[i];
        if ((creature->isActive && strcmp(creature->kind, "ork_legi") == 0 && strcmp(creature->affectedUnitName, type) == 0)) {
            char infoMessage [100];
            sprintf(infoMessage, "%s : %s", creature->name, creature->explanation);
            addMessage(infoMessage);
            sprintf(countStr, "%d %s", totalOrks, " +C");
        } else if (creature->isActive && strcmp(creature->kind, "ork_legi") == 0) {
            char infoMessage [100];
            sprintf(infoMessage, "%s : %s", creature->name, creature->explanation);
            addMessage(infoMessage);
        }
    }


    renderText(renderer, countStr, startX - slidingAmount * cellDimension + 15, currentY - 15, whiteColor);

    drawFilledUpwardTriangle(renderer, startX - slidingAmount * cellDimension + 15, currentY - 35, 20, triangleColor);

}

bool isMessageExists(char* message) {
    for (int i = 0; i < messageCount; i++) {
        if (strcmp(messages[i], message) == 0) {
            return true;
        }
    }
    return false;
}

void addMessage(char* message) {
    if (messageCount < 100 && !isMessageExists(message)) {
        strcpy(messages[messageCount], message);
        messageCount++;
    }
}

void printMessages(SDL_Renderer* renderer) {
    messages[messageCount][0] = '\0';
    for (int i = 0; i < messageCount; i++) {
        renderText(renderer, messages[i], 10, infoMesssagePixelChanger, blackColor);
        infoMesssagePixelChanger += 30;
    }
    for (int i = 0; i < 12; i++) {
        if (researches[i]->isActive) {
            char message[130];
            if (researches[i]->isForHuman) {
                snprintf(message, sizeof (message), "%s (Level %d) : %s (Insan Imparatorlugu Icin)",
                        researches[i]->name, researches[i]->level, researches[i]->explanation);
                renderText(renderer, message, 10, infoMesssagePixelChanger, blackColor);
            } else {
                snprintf(message, sizeof (message), "%s (Level %d) : %s (Ork Legi Icin)",
                        researches[i]->name, researches[i]->level, researches[i]->explanation);
                renderText(renderer, message, 10, infoMesssagePixelChanger, blackColor);
            }
            infoMesssagePixelChanger += 30;
        }
    }
}

void drawTeamsOnScreen(SDL_Renderer* ekranYazdir, struct Team * head) {
    SDL_SetRenderDrawColor(ekranYazdir, 255, 255, 255, 255);
    SDL_Rect beyazAlan = {enlemBaslamaNoktasi, boylamInsanBaslamaNoktasi, 1080, 1080};
    SDL_RenderFillRect(ekranYazdir, &beyazAlan);

    SDL_SetRenderDrawColor(ekranYazdir, 0, 0, 0, 255);
    for (int i = 0; i <= 20; ++i) {
        SDL_RenderDrawLine(ekranYazdir, enlemBaslamaNoktasi, boylamInsanBaslamaNoktasi + i * cellDimension, 1520, boylamInsanBaslamaNoktasi + i * cellDimension);
        SDL_RenderDrawLine(ekranYazdir, enlemBaslamaNoktasi + i * cellDimension, boylamInsanBaslamaNoktasi, enlemBaslamaNoktasi + i * cellDimension, boylamInsanBaslamaNoktasi + gridDimension * cellDimension);
    }

    struct Team* team = head;
    bool isDifferentTeam = false;
    while (team != NULL) {
        if (isDifferentTeam) {
            slidingAmount = -1;
        }
        if (team->next != NULL && strcmp(team->unitType->name, team->next->unitType->name) != 0) {
            isDifferentTeam = true;
        } else if (team->next != NULL && strcmp(team->unitType->name, team->next->unitType->name) == 0) {
            isDifferentTeam = false;
        }
        if (team->isHumanUnit) {
            distributeHumans(ekranYazdir, team->amount, enlemBaslamaNoktasi, boylamInsanBaslamaNoktasi, team->unitType->name);
        } else {
            distributeOrks(ekranYazdir, team->amount, enlemOrkBaslamaNoktasi, boylamOrkBaslamaNoktasi, team->unitType->name);
        }
        team = team->next;
    }

    if (!isInfoMessageRendered) {
        printMessages(ekranYazdir);
        isInfoMessageRendered = true;
    }
    else {
        infoMesssagePixelChanger = 500 ;
        printMessages(ekranYazdir);
    }


    SDL_RenderPresent(ekranYazdir);
}

void visualize() {


    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    font = TTF_OpenFont("/home/kaan/CProjects/Prolab/JMH Typewriter-Thin.ttf", 12);


    messages = (char**) malloc(100 * sizeof (char*));

    for (int i = 0; i < 100; i++) {
        messages[i] = (char*) malloc(100 * sizeof (char));
    }


    SDL_Window* ekran = SDL_CreateWindow("OYUN EKRANI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (1920 / 2), (1080 / 2), SDL_WINDOW_SHOWN);
    SDL_Renderer* ekranYazdir = SDL_CreateRenderer(ekran, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(ekranYazdir, 255, 0, 0, 255);
    SDL_RenderClear(ekranYazdir);
    int ekranEni, ekranBoyu;
    SDL_GetWindowSize(ekran, &ekranEni, &ekranBoyu);
    SDL_SetWindowFullscreen(ekran, SDL_WINDOW_FULLSCREEN_DESKTOP);

    drawTeamsOnScreen(ekranYazdir, head);


    Uint32 startTime = SDL_GetTicks(); // Başlangıç zamanını al
    while (SDL_GetTicks() - startTime < 10000) { // 10 saniye (10000 ms)
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyRenderer(ekranYazdir);
                SDL_DestroyWindow(ekran);
                SDL_Quit();
            }
        }
    }


    war();

    slidingAmount = -1;

    drawTeamsOnScreen(ekranYazdir, head);


    Uint32 startTime1 = SDL_GetTicks();
    while (SDL_GetTicks() - startTime1 < 10000) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyRenderer(ekranYazdir);
                SDL_DestroyWindow(ekran);
                SDL_Quit();
            }
        }
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(ekranYazdir);
    SDL_DestroyWindow(ekran);
    SDL_Quit();

}
