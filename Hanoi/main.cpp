#include <SDL2/SDL.h>
#include <stdio.h>

// Constantes para la ventana
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int DISK_HEIGHT = 20;
const int BASE_HEIGHT = 20;
const int TOWER_WIDTH = 10;
#define NUM_DISKS 3

// Estructura para los discos
typedef struct {
    int width;
    int height;
    int x;
    int y;
} Disk;

Disk disks[NUM_DISKS];

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int tower[3][NUM_DISKS]; // Cada torre puede contener hasta NUM_DISKS discos
int towerHeight[3] = {0, 0, 0}; // Altura actual de cada torre

int initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL no se pudo inicializar. SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow("Torre de Hanoi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("La ventana no se pudo crear. SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("El renderizador no se pudo crear. SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    return 1; // Todo bien
}

void drawTowerBase() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Negro para las torres
    for (int i = 0; i < 3; i++) {
        SDL_Rect base = {WINDOW_WIDTH / 4 * (i + 1) - TOWER_WIDTH / 2, WINDOW_HEIGHT - BASE_HEIGHT, TOWER_WIDTH, BASE_HEIGHT};
        SDL_RenderFillRect(renderer, &base);
    }
}

void drawDisks() {
    for (int i = 0; i < NUM_DISKS; i++) {
        SDL_Rect rect;
        rect.x = disks[i].x;
        rect.y = disks[i].y;
        rect.w = disks[i].width;
        rect.h = disks[i].height;

        SDL_SetRenderDrawColor(renderer, 255 - i * 30, i * 45, i * 30, 255); // Color variable para cada disco
        SDL_RenderFillRect(renderer, &rect);
    }
}

void moveDisk(int fromPeg, int toPeg) {
    if (towerHeight[fromPeg] == 0) return; // No hay discos para mover

    // Encontrar el disco superior en fromPeg
    int diskIndex = tower[fromPeg][towerHeight[fromPeg] - 1];
    towerHeight[fromPeg]--;

    // Actualizar posición x del disco para moverlo a toPeg
    disks[diskIndex].x = WINDOW_WIDTH / 4 * (toPeg + 1) - disks[diskIndex].width / 2;

    // Actualizar posición y del disco basada en la altura de la torre de destino
    disks[diskIndex].y = WINDOW_HEIGHT - BASE_HEIGHT - DISK_HEIGHT * (towerHeight[toPeg] + 1);

    // Agregar el disco a la torre de destino
    tower[toPeg][towerHeight[toPeg]] = diskIndex;
    towerHeight[toPeg]++;
}

void solveHanoi(int n, int fromPeg, int toPeg, int auxPeg) {
    if (n == 1) {
        moveDisk(fromPeg, toPeg);
        return;
    }
    solveHanoi(n - 1, fromPeg, auxPeg, toPeg);
    moveDisk(fromPeg, toPeg);
    solveHanoi(n - 1, auxPeg, toPeg, fromPeg);
}

int main(int argc, char* args[]) {
    if (!initSDL()) {
        SDL_Quit();
        return -1;
    }

    // Inicializar los discos y las torres
    for (int i = 0; i < NUM_DISKS; i++) {
        disks[i].width = 60 - i * 20; // Anchura decreciente
        disks[i].height = DISK_HEIGHT; // Altura constante
        disks[i].x = WINDOW_WIDTH / 4 - (disks[i].width / 2); // Posicionar en la primera torre
        disks[i].y = WINDOW_HEIGHT - BASE_HEIGHT - DISK_HEIGHT * (i + 1); // Apilar de abajo hacia arriba
        tower[0][i] = i; // Llenar la torre 0 con todos los discos
    }
    towerHeight[0] = NUM_DISKS;

    // Bucle de dibujo (simplificado)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Fondo blanco
    SDL_RenderClear(renderer);
    drawTowerBase();
    drawDisks();
    SDL_RenderPresent(renderer);
    SDL_Delay(2000); // Esperar para visualizar el estado inicial

    solveHanoi(NUM_DISKS, 0, 2, 1); // Ajustar a índices basados en 0

    // Visualizar el estado final
    SDL_RenderClear(renderer);
    drawTowerBase();
    drawDisks();
    SDL_RenderPresent(renderer);
    SDL_Delay(2000); // Esperar para visualizar el estado final

    // Liberar recursos y cerrar SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
