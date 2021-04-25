#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>

#include <ctime>
#include "Population.h"
#include "PopulationDeep.h"

bool draw = false;

bool processEvents(SDL_Window *window){
    if (window == NULL){
        return true;
    }
    bool done = false;
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        //printf("Event type: %d\n", event.type);
        if (event.type == SDL_WINDOWEVENT){
            if (event.window.event == SDL_WINDOWEVENT_CLOSE){
                done = true;
            }
        } else if (event.type == SDL_KEYDOWN){
            if (event.key.keysym.sym == SDLK_ESCAPE){
                done = true;
            }
        } else if (event.type == SDL_QUIT){
            done = true;
        }
    }

    const Uint8 *keyState = SDL_GetKeyboardState(NULL);
    if(keyState[SDL_SCANCODE_G]){
        draw = true;
    }
    if(keyState[SDL_SCANCODE_H]){
        draw = false;
    }
    return done;
}

void quitSDL(SDL_Window *window, SDL_Renderer *renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void clearRender(SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

void flushRender(SDL_Renderer *renderer){
    SDL_RenderPresent(renderer);
}

void renderEntity(SDL_Renderer *renderer, Entity *entity){
    Snake *snake = &entity->snake;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_Rect playerRect = {snake->x * SIZE, snake->y * SIZE, SIZE, SIZE};
    SDL_RenderFillRect(renderer, &playerRect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    for (unsigned int i = 0; i < snake->coda.size(); i++){
        SDL_Rect codaRect = {snake->coda[i].x * SIZE, snake->coda[i].y * SIZE, SIZE, SIZE};
        SDL_RenderFillRect(renderer, &codaRect);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_Rect fruttoRect = {snake->xFrutto * SIZE, snake->yFrutto * SIZE, SIZE, SIZE};
    SDL_RenderFillRect(renderer, &fruttoRect);

}

void renderEntityDeep(SDL_Renderer *renderer, EntityDeep *entity){
    Snake *snake = &entity->snake;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_Rect playerRect = {snake->x * SIZE, snake->y * SIZE, SIZE, SIZE};
    SDL_RenderFillRect(renderer, &playerRect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    for (unsigned int i = 0; i < snake->coda.size(); i++){
        SDL_Rect codaRect = {snake->coda[i].x * SIZE, snake->coda[i].y * SIZE, SIZE, SIZE};
        SDL_RenderFillRect(renderer, &codaRect);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_Rect fruttoRect = {snake->xFrutto * SIZE, snake->yFrutto * SIZE, SIZE, SIZE};
    SDL_RenderFillRect(renderer, &fruttoRect);

}

void renderPopulation(SDL_Renderer *renderer, Population *population){
    clearRender(renderer);

    for (int i = 0; i < 1; i++){
        if (population->entities[i]->dead == false){
            renderEntity(renderer, population->entities[i]);
        }
    }

    flushRender(renderer);
}

void renderPopulationDeep(SDL_Renderer *renderer, PopulationDeep *population){
    clearRender(renderer);

    for (int i = 0; i < 1; i++){
        if (population->entities[i]->dead == false){
            renderEntityDeep(renderer, population->entities[i]);
        }
    }

    flushRender(renderer);
}

void allena(SDL_Window *window, SDL_Renderer *renderer, Population *population, int maxTempo){
    int epoch = 0;
    bool fine = false;
    while(!fine){
        bool done = false;
        int tempo = 0;
        epoch++;
        while(!done && tempo < maxTempo){
            tempo++;
            done = true;
            if (processEvents(window)){
                fine = true;
            }
            for (unsigned int i = 0; i < population->entities.size(); i++){
                population->entities[i]->update();
                if (population->entities[i]->dead == false){
                    done = false;
                }
            }
            if (draw){
                renderPopulation(renderer, population);
            }
        }
        population->rank();
        printf("Epoch %d maxFitness %f\n", epoch, population->entities[0]->fitness);
        population->evolve();
        population->restart();
    }
}

void allenaDeep(SDL_Window *window, SDL_Renderer *renderer, PopulationDeep *population, int maxTempo){
    int epoch = 0;
    bool fine = false;
    while(!fine){
        bool done = false;
        int tempo = 0;
        epoch++;
        while(!done && tempo < maxTempo){
            tempo++;
            done = true;
            if (processEvents(window)){
                fine = true;
            }
            for (unsigned int i = 0; i < population->entities.size(); i++){
                population->entities[i]->update();
                if (population->entities[i]->dead == false){
                    done = false;
                }
            }
            if (draw){
                renderPopulationDeep(renderer, population);
            }
        }
        population->rank();
        printf("Epoch %d maxFitness %f\n", epoch, population->entities[0]->fitness);
        population->evolve();
        population->restart();
    }
}

void guarda(SDL_Window *window, SDL_Renderer *renderer, Population *population){
    while(true){
        bool done = false;
        while(!done){
            done = true;
            if (processEvents(window)){
                return;
            }
            population->entities[0]->update();
            if (population->entities[0]->dead == false){
                done = false;
            }
            renderPopulation(renderer, population);
        }
        population->entities[0]->restart();
    }
}

void guardaDeep(SDL_Window *window, SDL_Renderer *renderer, PopulationDeep *population){
    bool fine = false;
    while(!fine){
        bool done = false;
        while(!done){
            done = true;
            if (processEvents(window)){
                fine = true;
            }
            population->entities[0]->update();
            if (population->entities[0]->dead == false){
                done = false;
            }
            renderPopulationDeep(renderer, population);
        }
        population->entities[0]->restart();
    }
}

int main(){
    srand(time(NULL));

    SDL_Window *window;
    SDL_Renderer *renderer;

    Population *population = new Population();

    int maxTempo = 400;

    char fai[10];
    do{
        printf("Cosa vuoi fare?\na allena, v guarda migliore\n");
        scanf("%s", fai);
    } while(strcmp(fai, "a") != 0 && strcmp(fai, "v") != 0);

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (strcmp(fai, "a") == 0){
        //allenaDeep(window, renderer, population, maxTempo);
        allena(window, renderer, population, maxTempo);
    } else if (strcmp(fai, "v") == 0){
        //guardaDeep(window, renderer, population);
        guarda(window, renderer, population);
    }

    quitSDL(window, renderer);

    population->save();

    return 0;
}
