#ifndef HEADER_H
#define HEADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "models.h"
#include "teamlist.h"
#include "httpsreq.h"
#include "parser.h"

// Yapı tanımları

typedef struct {
    int health;
    int number;
} Entity;

// Renk tanımları
extern SDL_Color blueColor;
extern SDL_Color greenColor;
extern SDL_Color yellowColor;
extern SDL_Color orangeColor;
extern SDL_Color purpleColor;
extern SDL_Color greyColor;
extern SDL_Color brownColor;
extern SDL_Color blackColor;

// Font değişkeni
extern TTF_Font* font;

extern int slidingAmount;
extern int gridDimension;
extern int cellDimension;
extern int enlemBaslamaNoktasi;
extern int boylamInsanBaslamaNoktasi;
extern int boylamOrkBaslamaNoktasi;
extern int enlemOrkBaslamaNoktasi;
extern int infoMesssagePixelChanger;
extern int messageCount;
extern char** messages;
extern bool isInfoMessageRendered;

// İşlev prototipleri
void drawHealthBar(SDL_Renderer* renderer, int x, int y, int cellSize, int health);
void drawFilledDownwardTriangle(SDL_Renderer* renderer, int x, int y, int size, SDL_Color color);
void drawFilledUpwardTriangle(SDL_Renderer* renderer, int x, int y, int size, SDL_Color color);
void renderText(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color color);
void distributeHumans(SDL_Renderer* renderer, int totalWarriors, int startX, int startY, char* type);
void distributeOrks(SDL_Renderer* renderer, int totalOrks, int startX, int startY, char* type);
void drawTeamsOnScreen(SDL_Renderer* renderer, struct Team* head);
bool isMessageExists(char* message);
void addMessage(char* message);
void printMessages(SDL_Renderer* renderer);
void visualize();

#endif
