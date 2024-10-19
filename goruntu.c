#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

typedef struct {
    int health;  
    int number;  
} Entity;

int mazgalBoyutu = 54;
int hucreBoyutu = 54;
int enlemBaslamaNoktasi = 440;
int boylamInsanBaslamaNoktasi = 0;
int boylamOrkBaslamaNoktasi = 1080;
int enlemOrkBaslamaNoktasi = 1466;

SDL_Color blueColor = { 0, 0, 255, 255 };
SDL_Color greenColor = { 0, 255, 0, 255 };
SDL_Color yellowColor = { 255, 255, 0, 255 };
SDL_Color orangeColor = { 255, 165, 0, 255 };
SDL_Color purpleColor = { 128, 0, 128, 255 };
SDL_Color greyColor = { 169, 169, 169, 255 };
SDL_Color brownColor = { 165, 42, 42, 255 };
SDL_Color blackColor = { 0, 0, 0, 255 };

TTF_Font* font = NULL;

void drawHealthBar(SDL_Renderer* renderer, int x, int y, int cellSize, int health) {
    SDL_Rect healthBar = { x + 5, y + 5, cellSize - 10, cellSize / 4 };
    
    
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

    SDL_Rect destRect = { x, y, textWidth, textHeight };
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_DestroyTexture(texture);
}

void distributeHumans(SDL_Renderer* renderer, int totalWarriors, int startX, int startY, int type, int show) {
    int warriorsPerSquare = 100;  
    int numSquares = (totalWarriors + warriorsPerSquare - 1) / warriorsPerSquare;  

    SDL_Color triangleColor;
    if (type == 1) {
        triangleColor = blueColor;  
    } else if (type == 2) {
        triangleColor = yellowColor;  
    } else if (type == 3) {
        triangleColor = greenColor; 
    } else if (type == 4) {
        triangleColor = orangeColor;  
    }

    for (int i = 0; i < numSquares; ++i) {
        int warriorsInSquare = (i == numSquares - 1) ? (totalWarriors % warriorsPerSquare) : warriorsPerSquare;
        warriorsInSquare = warriorsInSquare == 0 ? warriorsPerSquare : warriorsInSquare;  

        int currentY = startY + (type - 1) * hucreBoyutu;  
        
        drawHealthBar(renderer, startX + i * hucreBoyutu, currentY, hucreBoyutu, 100);

        SDL_Color whiteColor = { 0, 0, 0, 255 };  
        char countStr[10];  
        sprintf(countStr, "%d", warriorsInSquare);
        
        if (show==1) {
            strcat(countStr, " ^+");
        }else if(show==-1)
            strcat(countStr, " ^-");

        renderText(renderer, countStr, startX + i * hucreBoyutu + 15, currentY + 40, whiteColor);  

        
        drawFilledDownwardTriangle(renderer, startX + i * hucreBoyutu + 15, currentY + 20, 20, triangleColor);
    }
}

void distributeOrks(SDL_Renderer* renderer, int totalOrks, int startX, int startY, int type,int show) {
    int orksPerSquare = 100;  
    int numSquares = (totalOrks + orksPerSquare - 1) / orksPerSquare;  

    SDL_Color triangleColor;
    if (type == 1) {
        triangleColor = blackColor;  
    } else if (type == 2) {
        triangleColor = brownColor;  
    } else if (type == 3) {
        triangleColor = greyColor;  
    } else if (type == 4) {
        triangleColor = purpleColor;  
    }

    for (int i = 0; i < numSquares; ++i) {
        int orksInSquare = (i == numSquares - 1) ? (totalOrks % orksPerSquare) : orksPerSquare;
        orksInSquare = orksInSquare == 0 ? orksPerSquare : orksInSquare;   
        
        int currentY = boylamOrkBaslamaNoktasi - (type - 1) * hucreBoyutu; 
        drawHealthBar(renderer, enlemOrkBaslamaNoktasi - i * hucreBoyutu, currentY  -54, hucreBoyutu, 100);  

        SDL_Color whiteColor = { 0, 0, 0, 255 };  
        char countStr[10];  
        sprintf(countStr, "%d", orksInSquare);

        if (show==1) {
            strcat(countStr, " ^+");
        }else if(show==-1)
            strcat(countStr, " ^-");

        renderText(renderer, countStr, enlemOrkBaslamaNoktasi - i * hucreBoyutu + 15, currentY - 15, whiteColor);  

        drawFilledUpwardTriangle(renderer, enlemOrkBaslamaNoktasi - i * hucreBoyutu + 15, currentY-35, 20, triangleColor);
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();  // Initialize TTF
    
    // Load a font (make sure you have a .ttf file in the specified path)
    font = TTF_OpenFont("/home/yunus/Desktop/Programlama Laboratuvarı - I/JMH Typewriter-Thin.ttf", 12);
    if (!font) {
        printf("Font hatasi %s\n", TTF_GetError());
        return -1;
    }

    SDL_Window* ekran = SDL_CreateWindow("OYUN EKRANI", 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (1960/2), (1080/2), SDL_WINDOW_SHOWN); 
    
    SDL_Renderer* ekranYazdir = SDL_CreateRenderer(ekran, -1, SDL_RENDERER_ACCELERATED);

    int ekranEni, ekranBoyu;
    SDL_GetWindowSize(ekran, &ekranEni, &ekranBoyu);
    SDL_SetWindowFullscreen(ekran, SDL_WINDOW_FULLSCREEN_DESKTOP);

    int oyunDevam = 1;

    int totalWarriorsType1 = 130;
    int totalWarriorsType2 = 75;  
    int totalWarriorsType3 = 130;
    int totalWarriorsType4 = 75;  
    int totalOrks = 333; 

    while (oyunDevam) {
    
        SDL_SetRenderDrawColor(ekranYazdir, 255, 0, 0, 255);
        SDL_RenderClear(ekranYazdir);

        SDL_SetRenderDrawColor(ekranYazdir, 255, 255, 255, 255);
        SDL_Rect beyazAlan = {enlemBaslamaNoktasi, boylamInsanBaslamaNoktasi, 1080, 1080};
        SDL_RenderFillRect(ekranYazdir, &beyazAlan);

        SDL_SetRenderDrawColor(ekranYazdir, 0, 0, 0, 255);
        for (int i = 0; i <= 20; ++i) {
            SDL_RenderDrawLine(ekranYazdir, enlemBaslamaNoktasi, boylamInsanBaslamaNoktasi + i * hucreBoyutu, 1520, boylamInsanBaslamaNoktasi + i * hucreBoyutu);
            SDL_RenderDrawLine(ekranYazdir, enlemBaslamaNoktasi + i * hucreBoyutu, boylamInsanBaslamaNoktasi, enlemBaslamaNoktasi + i * hucreBoyutu, boylamInsanBaslamaNoktasi + mazgalBoyutu * hucreBoyutu);
        }

        distributeHumans(ekranYazdir, totalWarriorsType1, enlemBaslamaNoktasi, boylamInsanBaslamaNoktasi, 1,-1);
        distributeHumans(ekranYazdir, totalWarriorsType2, enlemBaslamaNoktasi, boylamInsanBaslamaNoktasi , 2,-1);
        distributeHumans(ekranYazdir, totalWarriorsType3, enlemBaslamaNoktasi, boylamInsanBaslamaNoktasi , 3,1);
        distributeHumans(ekranYazdir, totalWarriorsType4, enlemBaslamaNoktasi, boylamInsanBaslamaNoktasi, 4,0);

        distributeOrks(ekranYazdir, totalOrks, enlemOrkBaslamaNoktasi, boylamOrkBaslamaNoktasi,2,-1);
        distributeOrks(ekranYazdir, 232, enlemOrkBaslamaNoktasi, boylamOrkBaslamaNoktasi,1,-1);
        distributeOrks(ekranYazdir, 65, enlemOrkBaslamaNoktasi, boylamOrkBaslamaNoktasi,3,1);
        distributeOrks(ekranYazdir, 346, enlemOrkBaslamaNoktasi, boylamOrkBaslamaNoktasi,4,1);

        SDL_RenderPresent(ekranYazdir);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                oyunDevam = 0; 
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) { 
                    oyunDevam = 0; 
                }
            }
        }

    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(ekranYazdir);
    SDL_DestroyWindow(ekran);
    SDL_Quit();
    
    return 0;
}
