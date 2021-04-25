#ifndef POPULATION_H_INCLUDED
#define POPULATION_H_INCLUDED

#include "NeuralNetwork.h"
#include <string.h>

struct Entity{
    NeuralNetwork<INPUT, HIDDEN, OUTPUT> brain;
    Snake snake;
    double fitness;
    bool dead;

    static float mutationRate;

    Entity()
    {
        restart();
    }
    void initializeRandom()
    {
        brain.initializeRandom();
    }
    void restart()
    {
        snake.restart();
        dead = false;
    }
    void mutate()
    {
        static auto mut = [=](float x){
            if (frand() < Entity::mutationRate){
                x += frand(-0.2, 0.2);
                if (x < -1){
                    x = -1;
                } else if (x > 1){
                    x = 1;
                }
                return x;
            } else{
                return x;
            }
        };
        brain.hWeights.map(mut);
        brain.oWeights.map(mut);
        brain.hBias.map(mut);
        brain.oBias.map(mut);
    }

    void calculateFitness()
    {
        int steps = snake.steps;
        int apples = snake.apples;
        fitness = steps + (pow(2, apples) + pow(apples, 2.1f) * 500) - (pow(apples, 1.2f) * pow((0.2 * steps), 1.3f));
    }

    void update(){
        if (dead){
            return;
        }
        dead = snake.update();
        //float *data = getData(snake);
        //float *data = getDataMondo(entity->snake);
        float *data = getLook(snake);
        float *result = brain.feedForward(data);
        int indMax = 0;
        for (int i = 1; i < brain.outputNodes; i++){
            if (result[i] > result[indMax]){
                indMax = i;
            }
        }
        delete[] result;
        delete[] data;
        int newDir = indMax;
        snake.updateDir((Dir)newDir);
    }

    void save(FILE *fp){
        for (int r = 0; r < brain.hiddenNodes; r++){
            for (int c = 0; c < brain.inputNodes; c++){
                fprintf(fp, "%f ", brain.hWeights.data[r][c]);
            }
        }
        for (int r = 0; r < brain.outputNodes; r++){
            for (int c = 0; c < brain.hiddenNodes; c++){
                fprintf(fp, "%f ", brain.oWeights.data[r][c]);
            }
        }
        for (int r = 0; r < brain.hiddenNodes; r++){
            fprintf(fp, "%f ", brain.hBias.data[r][0]);
        }
        for (int r = 0; r < brain.outputNodes; r++){
            fprintf(fp, "%f ", brain.oBias.data[r][0]);
        }
        fprintf(fp, "\n");
    }

    void load(FILE *fp){
        for (int r = 0; r < brain.hiddenNodes; r++){
            for (int c = 0; c < brain.inputNodes; c++){
                fscanf(fp, "%f", &(brain.hWeights.data[r][c]));
            }
        }
        for (int r = 0; r < brain.outputNodes; r++){
            for (int c = 0; c < brain.hiddenNodes; c++){
                fscanf(fp, "%f", &(brain.oWeights.data[r][c]));
            }
        }
        for (int r = 0; r < brain.hiddenNodes; r++){
            fscanf(fp, "%f", &(brain.hBias.data[r][0]));
        }
        for (int r = 0; r < brain.outputNodes; r++){
            fscanf(fp, "%f", &(brain.oBias.data[r][0]));
        }
    }

    static void crossOverEntity(Entity &ec, Entity &e1, Entity &e2)
    {
        if (e1.brain.inputNodes != e2.brain.inputNodes || e1.brain.hiddenNodes != e2.brain.hiddenNodes || e1.brain.outputNodes != e2.brain.outputNodes){
            printf("ERROR crossOver\n");
        }
        NeuralNetwork<INPUT, HIDDEN, OUTPUT> &a = e1.brain;
        NeuralNetwork<INPUT, HIDDEN, OUTPUT> &b = e2.brain;
        NeuralNetwork<INPUT, HIDDEN, OUTPUT> &c = ec.brain;
        int i = 0;
        int randomCut = irand(c.hiddenNodes);
        for (; i < randomCut; i++)
        {
            for (int col = 0; col < c.inputNodes; col++)
            {
                c.hWeights.data[i][col] = a.hWeights.data[i][col];
            }
        }
        for (; i < c.hiddenNodes; i++)
        {
            for (int col = 0; col < c.inputNodes; col++)
            {
                c.hWeights.data[i][col] = b.hWeights.data[i][col];
            }
        }
        randomCut = irand(c.outputNodes);
        for (i = 0; i < randomCut; i++)
        {
            for (int col = 0; col < c.hiddenNodes; col++)
            {
                c.oWeights.data[i][col] = a.oWeights.data[i][col];
            }
        }
        for (; i < c.outputNodes; i++)
        {
            for (int col = 0; col < c.hiddenNodes; col++)
            {
                c.oWeights.data[i][col] = b.oWeights.data[i][col];
            }
        }
        randomCut = irand(c.hiddenNodes);
        for (i = 0; i < randomCut; i++)
        {
            c.hBias.data[i][0] = a.hBias.data[i][0];
        }
        for (; i < c.hiddenNodes; i++)
        {
            c.hBias.data[i][0] = b.hBias.data[i][0];
        }
        randomCut = irand(c.outputNodes);
        for (i = 0; i < randomCut; i++)
        {
            c.oBias.data[i][0] = a.oBias.data[i][0];
        }
        for (; i < c.outputNodes; i++)
        {
            c.oBias.data[i][0] = b.oBias.data[i][0];
        }
    }
};

float Entity::mutationRate = MUTATION_RATE;

struct Population
{
    std::vector<Entity *> entities;
    int numberElite;

    Population(int sizePopulation, float mutationRate = MUTATION_RATE)
    {
        Entity::mutationRate = mutationRate;
        entities = std::vector<Entity *>(sizePopulation);
        for (int i = 0; i < sizePopulation; i++)
        {
            entities[i] = new Entity();
            entities[i]->initializeRandom();
        }
        numberElite = sizePopulation / 5;
        if (numberElite < 2){
            numberElite = 2;
        }
    }

    Population(float mutationRate = MUTATION_RATE)
    {
        Entity::mutationRate = mutationRate;
        char scelta[20];
        char nomeCompleto[20] = "res/";
        printf("Quale file vuoi loadare? ");
        scanf("%s", scelta);
        strcat(nomeCompleto, scelta);
        FILE *fp = fopen(nomeCompleto, "r");
        if (fp == NULL){
            printf("Non riesco ad aprire il file\n");
            exit(EXIT_FAILURE);
        } else{
            printf("Sto aprendo il file\n");
        }
        int sizePopulation;
        fscanf(fp, "%d %*d %*d %*d %*f", &sizePopulation);
        entities = std::vector<Entity *>(sizePopulation);
        for (int i = 0; i < sizePopulation; i++)
        {
            entities[i] = new Entity();
            entities[i]->load(fp);
        }
        printf("File letto con successo\n");
        numberElite = sizePopulation / 5;
        if (numberElite < 2){
            numberElite = 2;
        }
    }

    void rank()
    {
        calculateFitness();
        std::vector<Entity *> vecchio = entities;
        for (unsigned int i = 0; i < entities.size(); i++){
            float maxFitness = -1.0f;
            int indMax = -1;
            for (unsigned int s = 0; s < entities.size(); s++){
                if (vecchio[s] != nullptr && vecchio[s]->fitness > maxFitness){
                    maxFitness = vecchio[s]->fitness;
                    indMax = s;
                }
            }
            entities[i] = vecchio[indMax];
            vecchio[indMax] = nullptr;
        }
    }

    void mutate()
    {
        for (unsigned int i = numberElite; i < entities.size(); i++)
        {
            entities[i]->mutate();
        }
    }

    void crossOver()
    {
        int numberEntities = entities.size();
        int quantiNuovi = numberEntities / 3;
        for (int i = numberEntities - 2 * quantiNuovi; i < numberEntities - quantiNuovi; i++){
            int aIndex, bIndex;
            while((aIndex = irand(numberElite)) == i);
            while((bIndex = irand(numberElite)) == i);
            if (frand() > 0.5f){
                Entity::crossOverEntity(*entities[i], *entities[aIndex], *entities[bIndex]);
            } else{
                Entity::crossOverEntity(*entities[i], *entities[bIndex], *entities[aIndex]);
            }
        }
        for (int i = numberEntities - quantiNuovi; i < numberEntities; i++){
            int aIndex, bIndex;
            while((aIndex = irand(numberElite)) == i);
            while((bIndex = irand(numberEntities)) == i);
            if (frand() > 0.5f){
                Entity::crossOverEntity(*entities[i], *entities[aIndex], *entities[bIndex]);
            } else{
                Entity::crossOverEntity(*entities[i], *entities[bIndex], *entities[aIndex]);
            }
        }
    }

    void crossOverNuovo()
    {
        int numberEntities = entities.size();
        int quantiNuovi = numberEntities / 2;
        for (int i = 1; i < quantiNuovi; i++){
            int aIndex, bIndex;
            while((aIndex = irand(numberElite)) == i);
            while((bIndex = irand(numberEntities)) == i);
            if (frand() > 0.5f){
                Entity::crossOverEntity(*entities[i], *entities[aIndex], *entities[bIndex]);
            } else{
                Entity::crossOverEntity(*entities[i], *entities[bIndex], *entities[aIndex]);
            }
        }
        for (int i = quantiNuovi; i < numberEntities; i++){
            int aIndex, bIndex;
            while((aIndex = irand(numberEntities)) == i);
            while((bIndex = irand(numberEntities)) == i);
            if (frand() > 0.5f){
                Entity::crossOverEntity(*entities[i], *entities[aIndex], *entities[bIndex]);
            } else{
                Entity::crossOverEntity(*entities[i], *entities[bIndex], *entities[aIndex]);
            }
        }
    }

    void evolve()
    {
        //crossOver();
        crossOverNuovo();
        mutate();
    }

    void restart()
    {
        for (unsigned int i = 0; i < entities.size(); i++)
        {
            entities[i]->restart();
        }
    }

    void calculateFitness()
    {
        for (unsigned int i = 0; i < entities.size(); i++)
        {
            entities[i]->calculateFitness();
        }
    }
    void save()
    {
        char nome[20];
        char nomeCompleto[20] = "res/";
        printf("Come vuoi salvarlo? ");
        scanf("%s", nome);
        strcat(nomeCompleto, nome);
        FILE *fp = fopen(nomeCompleto, "w");
        if (fp == NULL){
            printf("Errore nell'aprire il file\n");
        } else{
            printf("Inizio a salvare\n");
        }
        fprintf(fp, "%d %d %d %d %f\n", (int) entities.size(), INPUT, HIDDEN, OUTPUT, MUTATION_RATE);
        for (unsigned int i = 0; i < entities.size(); i++){
            entities[i]->save(fp);
        }
        fclose(fp);
        printf("%s salvato con successo\n", nome);
    }
};

#endif // POPULATION_H_INCLUDED
