#ifndef POPULATIONDEEP_H_INCLUDED
#define POPULATIONDEEP_H_INCLUDED

#include "NeuralNetworkDeep.h"
#include <string.h>

struct EntityDeep{
    NeuralNetworkDeep<INPUT, NUMBER_HIDDEN, HIDDEN, OUTPUT> brain;
    Snake snake;
    double fitness;
    bool dead;

    EntityDeep()
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
    void mutate(float mutationRate)
    {
        static auto mut = [=](float x, float mutRate){
            if (frand() < mutRate){
                x += frand(-0.2, 0.2);
                if (x < -1){
                    x = -1;
                } else if (x > 1){
                    x = 1;
                }
            }
            return x;
        };
        brain.iWeights.map1f(mut, mutationRate);
        brain.oWeights.map1f(mut, mutationRate);
        brain.iBias.map1f(mut, mutationRate);
        brain.oBias.map1f(mut, mutationRate);
        for (int i = 0; i < brain.numberHidden; i++){
            brain.hWeights[i].map1f(mut, mutationRate);
            brain.hBias[i].map1f(mut, mutationRate);
        }
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
                fprintf(fp, "%f ", brain.iWeights.data[r][c]);
            }
        }
        for (int r = 0; r < brain.outputNodes; r++){
            for (int c = 0; c < brain.hiddenNodes; c++){
                fprintf(fp, "%f ", brain.oWeights.data[r][c]);
            }
        }
        for (int r = 0; r < brain.hiddenNodes; r++){
            fprintf(fp, "%f ", brain.iBias.data[r][0]);
        }
        for (int r = 0; r < brain.outputNodes; r++){
            fprintf(fp, "%f ", brain.oBias.data[r][0]);
        }
        for (int n = 0; n < brain.numberHidden; n++){
            for (int r = 0; r < brain.hiddenNodes; r++){
                for (int c = 0; c < brain.hiddenNodes; c++){
                    fprintf(fp, "%f ", brain.hWeights[n].data[r][c]);
                }
                fprintf(fp, "%f ", brain.hBias[n].data[r][0]);
            }
        }
        fprintf(fp, "\n");
    }

    void load(FILE *fp){
        for (int r = 0; r < brain.hiddenNodes; r++){
            for (int c = 0; c < brain.inputNodes; c++){
                fscanf(fp, "%f", &(brain.iWeights.data[r][c]));
            }
        }
        for (int r = 0; r < brain.outputNodes; r++){
            for (int c = 0; c < brain.hiddenNodes; c++){
                fscanf(fp, "%f", &(brain.oWeights.data[r][c]));
            }
        }
        for (int r = 0; r < brain.hiddenNodes; r++){
            fscanf(fp, "%f", &(brain.iBias.data[r][0]));
        }
        for (int r = 0; r < brain.outputNodes; r++){
            fscanf(fp, "%f", &(brain.oBias.data[r][0]));
        }
        for (int n = 0; n < brain.numberHidden; n++){
            for (int r = 0; r < brain.hiddenNodes; r++){
                for (int c = 0; c < brain.hiddenNodes; c++){
                    fscanf(fp, "%f", &(brain.hWeights[n].data[r][c]));
                }
                fscanf(fp, "%f", &(brain.hBias[n].data[r][0]));
            }
        }
    }

    static void crossOverEntity(EntityDeep &ec, EntityDeep &e1, EntityDeep &e2)
    {
        if (e1.brain.inputNodes != e2.brain.inputNodes || e1.brain.numberHidden != e2.brain.numberHidden || e1.brain.hiddenNodes != e2.brain.hiddenNodes || e1.brain.outputNodes != e2.brain.outputNodes){
            printf("ERROR crossOver\n");
        }
        auto &a = e1.brain;
        auto &b = e2.brain;
        auto &c = ec.brain;
        int i = 0;
        int randomCut = irand(c.hiddenNodes);
        for (; i < randomCut; i++)
        {
            for (int col = 0; col < c.inputNodes; col++)
            {
                c.iWeights.data[i][col] = a.iWeights.data[i][col];
            }
        }
        for (; i < c.hiddenNodes; i++)
        {
            for (int col = 0; col < c.inputNodes; col++)
            {
                c.iWeights.data[i][col] = b.iWeights.data[i][col];
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
            c.iBias.data[i][0] = a.iBias.data[i][0];
        }
        for (; i < c.hiddenNodes; i++)
        {
            c.iBias.data[i][0] = b.iBias.data[i][0];
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
        for (int n = 0; n < c.numberHidden; n++){
           randomCut = irand(c.hiddenNodes);
            for (i = 0; i < randomCut; i++)
            {
                for (int col = 0; col < c.hiddenNodes; col++)
                {
                    c.hWeights[n].data[i][col] = a.hWeights[n].data[i][col];
                }
            }
            for (; i < c.hiddenNodes; i++)
            {
                for (int col = 0; col < c.hiddenNodes; col++)
                {
                    c.hWeights[n].data[i][col] = b.hWeights[n].data[i][col];
                }
            }
            randomCut = irand(c.hiddenNodes);
            for (i = 0; i < randomCut; i++)
            {
                c.hBias[n].data[i][0] = a.hBias[n].data[i][0];
            }
            for (; i < c.hiddenNodes; i++)
            {
                c.hBias[n].data[i][0] = b.hBias[n].data[i][0];
            }
        }
    }
};


struct PopulationDeep
{
    std::vector<EntityDeep *> entities;
    int numberElite, numberSuperMutate, numberCross;

    float mutationRate;

    PopulationDeep(int sizePopulation, float _mutationRate = MUTATION_RATE)
    {
        mutationRate = _mutationRate;
        entities = std::vector<EntityDeep *>(sizePopulation);
        for (int i = 0; i < sizePopulation; i++)
        {
            entities[i] = new EntityDeep();
            entities[i]->initializeRandom();
        }
        calculateParti();
    }

    PopulationDeep(float _mutationRate = MUTATION_RATE)
    {
        mutationRate = _mutationRate;
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
        entities = std::vector<EntityDeep *>(sizePopulation);
        for (int i = 0; i < sizePopulation; i++)
        {
            entities[i] = new EntityDeep();
            entities[i]->load(fp);
        }
        printf("File letto con successo\n");
        calculateParti();
    }

    void rank()
    {
        calculateFitness();
        std::vector<EntityDeep *> vecchio = entities;
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
        for (int i = 1; i < numberElite; i++){
            entities[i]->mutate(ELITE_MUTATION_RATE);
        }
        for (int i = numberElite; i < numberElite + numberSuperMutate; i++)
        {
            entities[i]->mutate(SUPER_MUTATION);
        }
        for (unsigned int i = numberElite + numberSuperMutate; i < entities.size(); i++)
        {
            entities[i]->mutate(MUTATION_RATE);
        }
    }

    void crossOver()
    {
        int numberEntities = entities.size();
        for (int i = 0; i < numberCross; i++){
            int indice = i + numberElite + numberSuperMutate;
            int aIndex, bIndex;
            aIndex = irand(numberElite);
            bIndex = irand(numberElite);
            if (frand() > 0.5f){
                EntityDeep::crossOverEntity(*entities[indice], *entities[aIndex], *entities[bIndex]);
            } else{
                EntityDeep::crossOverEntity(*entities[indice], *entities[bIndex], *entities[aIndex]);
            }
        }
        for (int i = 0; i < numberCross; i++){
            int indice = i + numberElite + numberSuperMutate + numberCross;
            int aIndex, bIndex;
            aIndex = irand(numberElite);
            while((bIndex = irand(numberEntities)) == indice);
            if (frand() > 0.5f){
                EntityDeep::crossOverEntity(*entities[indice], *entities[aIndex], *entities[bIndex]);
            } else{
                EntityDeep::crossOverEntity(*entities[indice], *entities[bIndex], *entities[aIndex]);
            }
        }
        for (int i = numberElite + numberSuperMutate + 2 * numberCross; i < numberEntities; i++){
            int aIndex, bIndex;
            while((aIndex = irand(numberEntities)) == i);
            while((bIndex = irand(numberEntities)) == i);
            if (frand() > 0.5f){
                EntityDeep::crossOverEntity(*entities[i], *entities[aIndex], *entities[bIndex]);
            } else{
                EntityDeep::crossOverEntity(*entities[i], *entities[bIndex], *entities[aIndex]);
            }
        }
    }

    void evolve()
    {
        crossOver();
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

private:
    void calculateParti(){
        numberElite = entities.size() / 5;
        if (numberElite < 2){
            numberElite = 2;
        }
        numberCross = 2 * entities.size() / 9;
        numberSuperMutate = entities.size() - 3 * numberCross - numberElite;
    }
};

#endif // POPULATIONDEEP_H_INCLUDED
