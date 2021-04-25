#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED

#include <vector>
#include <math.h>

#define WIDTH 640
#define HEIGHT 640
#define SIZE 10
#define XMAX (WIDTH / SIZE)
#define YMAX (HEIGHT / SIZE)
#define VEL 1

#define INPUT 24
#define NUMBER_HIDDEN 1
#define HIDDEN 20
#define OUTPUT 4

#define MUTATION_RATE 0.01f
#define SUPER_MUTATION 0.1f
#define ELITE_MUTATION_RATE 0.001f

int irand(int bound);

enum Dir {UP, DOWN, LEFT, RIGHT};

enum Mondo {VUOTO, PLAYER, CODA, FRUTTO};

struct Coda{
    int x, y;
    Coda(int _x, int _y) : x(_x), y(_y) {}
};

struct Snake{
    int x, y;
    int xFrutto, yFrutto;
    int steps, apples;
    Dir dir;
    std::vector<Coda> coda;
    Mondo world[XMAX][YMAX];

    Snake()
    {
        restart();
    }
    void restart()
    {
        x = XMAX / 2;
        y = YMAX / 2;
        dir = UP;

        xFrutto = irand(XMAX);
        yFrutto = irand(YMAX);

        steps = 0;
        apples = 0;

        coda.clear();
        int codaInizio = 2;
        coda.reserve(codaInizio);
        for (int i = 0; i < codaInizio; i++){
            coda.emplace_back(x, y + i + 1);
        }
        calculateWorld();
    }
    void updateDir(Dir newDir){
        if(newDir == UP && dir != DOWN){
            dir = UP;
        } else if(newDir == DOWN && dir != UP){
            dir = DOWN;
        } else if(newDir == LEFT && dir != RIGHT){
            dir = LEFT;
        } else if(newDir == RIGHT && dir != LEFT){
            dir = RIGHT;
        }
    }
    bool update(){
        bool collisione = false;
        clearWorld();
        for (int i = coda.size() - 1; i > 0; i--){
            coda[i].x = coda[i - 1].x;
            coda[i].y = coda[i - 1].y;
        }
        coda[0].x = x;
        coda[0].y = y;
        if (dir == UP){
            y -= VEL;
        } else if (dir == DOWN){
            y += VEL;
        } else if (dir == LEFT){
            x -= VEL;
        } else if (dir == RIGHT){
            x += VEL;
        }
        if (x <= -1){
            x = XMAX - 1;
            collisione = true;
        } else if (x >= XMAX){
            x = 0;
            collisione = true;
        }
        if (y <= -1){
            y = YMAX - 1;
            collisione = true;
        } else if (y >= YMAX){
            y = 0;
            collisione = true;
        }

        for (unsigned int i = 0; i < coda.size(); i++){
            if (x == coda[i].x && y == coda[i].y){
                collisione = true;
                break;
            }
        }

        if (x == xFrutto && y == yFrutto){
            coda.emplace_back(coda[coda.size() - 1].x, coda[coda.size() - 1].y);
            world[xFrutto][yFrutto] = VUOTO;
            xFrutto = irand(XMAX);
            yFrutto = irand(YMAX);
            world[xFrutto][yFrutto] = FRUTTO;
            apples++;
        }
        steps++;
        updateWorld();

        return collisione;
    }

private:
    void calculateWorld(){
        for (int xScorri = 0; xScorri < XMAX; xScorri++){
            for (int yScorri = 0; yScorri < YMAX; yScorri++){
                world[xScorri][yScorri] = VUOTO;
            }
        }
        world[x][y] = PLAYER;
        for (unsigned int i = 0; i < coda.size(); i++){
            world[coda[i].x][coda[i].y] = CODA;
        }
        world[xFrutto][yFrutto] = FRUTTO;
    }
    void clearWorld(){
        world[x][y] = VUOTO;
        for (unsigned int i = 0; i < coda.size(); i++){
            world[coda[i].x][coda[i].y] = VUOTO;
        }
    }
    void updateWorld(){
        world[x][y] = PLAYER;
        for (unsigned int i = 0; i < coda.size(); i++){
            world[coda[i].x][coda[i].y] = CODA;
        }
    }
};

float *getDataVecchio(Snake *snake){
    float *data = new float[INPUT];
    int distanzaMuro[8];
    int x = snake->x;
    int y = snake->y;
    distanzaMuro[0] = XMAX - x;
    distanzaMuro[1] = x;
    distanzaMuro[2] = YMAX - y;
    distanzaMuro[3] = y;
    distanzaMuro[4] = 0;
    while (x < XMAX && y < YMAX){
        x++;
        y++;
        distanzaMuro[4]++;
    }
    x = snake->x;
    y = snake->y;
    distanzaMuro[5] = 0;
    while (x >= 0 && y < YMAX){
        x--;
        y++;
        distanzaMuro[5]++;
    }
    x = snake->x;
    y = snake->y;
    distanzaMuro[6] = 0;
    while (x < XMAX && y >= 0){
        x++;
        y--;
        distanzaMuro[6]++;
    }
    x = snake->x;
    y = snake->y;
    distanzaMuro[7] = 0;
    while (x >= 0 && y >= 0){
        x--;
        y--;
        distanzaMuro[7]++;
    }
    int distanzaCibo[8] = { 0 };
    x = snake->x;
    y = snake->y;
    while (x < XMAX && snake->world[x][y] != FRUTTO){
        x++;
        distanzaCibo[0]++;
    }
    x = snake->x;
    y = snake->y;
    while (x >= 0 && snake->world[x][y] != FRUTTO){
        x--;
        distanzaCibo[1]++;
    }
    x = snake->x;
    y = snake->y;
    while (y < YMAX && snake->world[x][y] != FRUTTO){
        y++;
        distanzaCibo[2]++;
    }
    x = snake->x;
    y = snake->y;
    while (y >= 0 && snake->world[x][y] != FRUTTO){
        y--;
        distanzaCibo[3]++;
    }
    x = snake->x;
    y = snake->y;
    while (x < XMAX && y < YMAX && snake->world[x][y] != FRUTTO){
        x++;
        y++;
        distanzaCibo[4]++;
    }
    x = snake->x;
    y = snake->y;
    while (x >= 0 && y < YMAX && snake->world[x][y] != FRUTTO){
        x--;
        y++;
        distanzaCibo[5]++;
    }
    x = snake->x;
    y = snake->y;
    while (x < XMAX && y >= 0 && snake->world[x][y] != FRUTTO){
        x++;
        y--;
        distanzaCibo[6]++;
    }
    x = snake->x;
    y = snake->y;
    while (x >= 0 && y >= 0 && snake->world[x][y] != FRUTTO){
        x--;
        y--;
        distanzaCibo[7]++;
    }

    int distanzaCoda[8] = { 0 };
    x = snake->x;
    y = snake->y;
    while (x < XMAX && snake->world[x][y] != CODA){
        x++;
        distanzaCoda[0]++;
    }
    x = snake->x;
    y = snake->y;
    while (x >= 0 && snake->world[x][y] != CODA){
        x--;
        distanzaCoda[1]++;
    }
    x = snake->x;
    y = snake->y;
    while (y < YMAX && snake->world[x][y] != CODA){
        y++;
        distanzaCoda[2]++;
    }
    x = snake->x;
    y = snake->y;
    while (y >= 0 && snake->world[x][y] != CODA){
        y--;
        distanzaCoda[3]++;
    }
    x = snake->x;
    y = snake->y;
    while (x < XMAX && y < YMAX && snake->world[x][y] != CODA){
        x++;
        y++;
        distanzaCoda[4]++;
    }
    x = snake->x;
    y = snake->y;
    while (x >= 0 && y < YMAX && snake->world[x][y] != CODA){
        x--;
        y++;
        distanzaCoda[5]++;
    }
    x = snake->x;
    y = snake->y;
    while (x < XMAX && y >= 0 && snake->world[x][y] != CODA){
        x++;
        y--;
        distanzaCoda[6]++;
    }
    x = snake->x;
    y = snake->y;
    while (x >= 0 && y >= 0 && snake->world[x][y] != CODA){
        x--;
        y--;
        distanzaCoda[7]++;
    }
    //printf("muro: ");
    for (int i = 0; i < 8; i++){
        data[i] = 1 / (float)distanzaMuro[i];
        //printf("%d ", distanzaMuro[i]);
    }
    //printf("\ncibo: ");
    for (int i = 0; i < 8; i++){
        data[8 + i] = (float)distanzaCoda[i];
        //printf("%d ", distanzaCibo[i]);
    }
    //printf("\ncoda: ");
    for (int i = 0; i < 8; i++){
        data[16 + i] = (float)distanzaCibo[i];
        //printf("%d ", distanzaCoda[i]);
    }
    data[24] = snake->dir;
    data[25] = snake->x - snake->xFrutto;
    data[26] = snake->y - snake->yFrutto;
    //printf("\n");
    return data;
}

float *lookInDirection(Snake &snake, int xdir, int ydir){
    //set up a temp array to hold the values that are going to be passed to the main vision array
    float *visionInDirection = new float[3];
    visionInDirection[0] = 0;
    visionInDirection[1] = 0;

    int x = snake.x;
    int y = snake.y;
    bool foodIsFound = false;//true if the food has been located in the direction looked
    bool tailIsFound = false;//true if the tail has been located in the direction looked
    float distance = 0;
    //move once in the desired direction before starting
    x += xdir;
    y += ydir;
    distance +=1;

    //look in the direction until you reach a wall
    while (x >= 0 && x < XMAX && y >= 0 && y < YMAX) {

      //check for food at the position
      if (!foodIsFound && x == snake.xFrutto && y == snake.yFrutto) {
        visionInDirection[0] = 1;
        foodIsFound = true; // dont check if food is already found
      }

      //check for tail at the position
      if (!tailIsFound && snake.world[x][y] == CODA) {
        visionInDirection[1] = 1.0f/distance;
        tailIsFound = true; // dont check if tail is already found
      }

      //look further in the direction
      x += xdir;
      y += ydir;
      distance +=1;
    }

    //set the distance to the wall
    visionInDirection[2] = 1.0f/distance;

    return visionInDirection;
}

float *getLook(Snake &snake){
    float *vision = new float[24];
    //look left
    float *tempValues = lookInDirection(snake, -1, 0);
    vision[0] = tempValues[0];
    vision[1] = tempValues[1];
    vision[2] = tempValues[2];
    //look left/up
    delete[] tempValues;
    tempValues = lookInDirection(snake, -1, -1);
    vision[3] = tempValues[0];
    vision[4] = tempValues[1];
    vision[5] = tempValues[2];
    //look up
    delete[] tempValues;
    tempValues = lookInDirection(snake, 0, -1);
    vision[6] = tempValues[0];
    vision[7] = tempValues[1];
    vision[8] = tempValues[2];
    //look up/right
    delete[] tempValues;
    tempValues = lookInDirection(snake, 1, -1);
    vision[9] = tempValues[0];
    vision[10] = tempValues[1];
    vision[11] = tempValues[2];
    //look right
    delete[] tempValues;
    tempValues = lookInDirection(snake, 1, 0);
    vision[12] = tempValues[0];
    vision[13] = tempValues[1];
    vision[14] = tempValues[2];
    //look right/down
    delete[] tempValues;
    tempValues = lookInDirection(snake, 1, 1);
    vision[15] = tempValues[0];
    vision[16] = tempValues[1];
    vision[17] = tempValues[2];
    //look down
    delete[] tempValues;
    tempValues = lookInDirection(snake, 0, 1);
    vision[18] = tempValues[0];
    vision[19] = tempValues[1];
    vision[20] = tempValues[2];
    //look down/left
    delete[] tempValues;
    tempValues = lookInDirection(snake, -1, 1);
    vision[21] = tempValues[0];
    vision[22] = tempValues[1];
    vision[23] = tempValues[2];
    delete[] tempValues;
    return vision;
}

float *getDataMondo(Snake &snake){
    float *data = new float[XMAX * YMAX];
    for (int x = 0; x < XMAX; x++){
        for (int y = 0; y < YMAX; y++){
            int indice = x * YMAX + y;
            if (snake.world[x][y] == PLAYER){
                data[indice] = -XMAX * YMAX;
            } else if (snake.world[x][y] == CODA){
                data[indice] = -2 * XMAX;
            } else if (snake.world[x][y] == FRUTTO){
                data[indice] = XMAX * YMAX;
            } else if (snake.world[x][y] == VUOTO){
                int xdist = x - snake.xFrutto;
                int ydist = y - snake.yFrutto;
                int tot = xdist + ydist;
                data[indice] = XMAX * YMAX - 5 * tot;
            }
        }
    }
    return data;
}


#endif // SNAKE_H_INCLUDED
