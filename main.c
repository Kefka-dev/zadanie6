#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 11
#define SHIP_COUNT 5
#define TRUE 1
#define FALSE 0
#define FAIL 1
typedef struct position
{
    int x;
    int y;
} POSITION;

typedef struct lod
{
    char name[50];
    int lenght;
    int liveTiles;
    char displayChar;
    //bridge = najlavejsi alebo najvissi bod dole
    POSITION bridgePosition;
} LOD;

void reset() {
    printf("\033[0m");
}
void red() {
    printf("\033[31m");
}
void green() {
    printf("\033[32m");
}
void blue() {
    printf("\033[36m");
}
void yellow() {
    printf("\033[33m");
}
//nastavi poziciu cursora
void setCursorPos(int x, int y)
{
    printf("\033[%d;%dH", y, x);
}

//clearne terminal
void clearTerminal()
{
    printf("\033[H\033[J");
}

//naplni grid vodou
void fillWithWater(char grid[GRID_SIZE][GRID_SIZE])
{
    grid[0][0] = ' ';
    for (int cols = 1; cols < GRID_SIZE; cols++)
    {
        grid[0][cols] = 47+cols;
    }

    for (int rows = 1; rows < GRID_SIZE; rows++)
    {
        grid[rows][0] = 47+rows;
    }

    for (int rows = 1; rows < GRID_SIZE; rows++)
    {
        for (int cols = 1; cols < GRID_SIZE; cols++)
        {
            grid[rows][cols] = '~';
        }
    }

}

//vypise grid
void printGrid(char grid[GRID_SIZE][GRID_SIZE], POSITION *gridPos)
{

    for (int rows = 0; rows < GRID_SIZE; rows++)
    {
        setCursorPos((*gridPos).x, (*gridPos).y);
        for (int cols = 0; cols < GRID_SIZE; cols++)
        {
            if(rows >0 && cols>0)
            {
               blue();
               if (isdigit(grid[rows][cols]) != FALSE)
               {
                    green();
               }
               
            }
            else{
                yellow();
            }
            printf("[%c]", grid[rows][cols]);
        }
        reset();
        (*gridPos).y++;
    }
    printf("\n");
}

int placeBoat(LOD *boatToPlace, POSITION wantedBridgePos, char orientation ,char grid[GRID_SIZE][GRID_SIZE])
{
    //cislovanie hracej plochy je na nultom riadku a nultom stlpci cize cords treba o 1 posunut
    wantedBridgePos.x++;
    wantedBridgePos.y++;

    if(orientation == 'h')
    {
        //chceck ci sa lod zmesti na sirku, cize poloha bridge+dlzka lode
        if ( (wantedBridgePos.x - 1 + (*boatToPlace).lenght) < GRID_SIZE)
        {

            (*boatToPlace).bridgePosition = wantedBridgePos;

            //check ci sa tam lod zmesti(ci tam uz nie je ina)
            for (int piece = 0; piece < (*boatToPlace).lenght; piece++)
            {
                if (grid[(*boatToPlace).bridgePosition.y][(*boatToPlace).bridgePosition.x + piece] != '~')
                {
                    return 1;
                }
            }
 
            //samotne umiestnenie lode na grid
            for (int piece = 0; piece < (*boatToPlace).lenght; piece++)
            {
                grid[(*boatToPlace).bridgePosition.y][(*boatToPlace).bridgePosition.x + piece] = (*boatToPlace).displayChar;
            }
            
        }
        else
        {
            //fprintf(stderr, "nezmesti sa\n");
            return 1;
        }
    }
    else if (orientation == 'v')
    {
        //chceck ci sa lod zmesti na vysku, cize poloha bridge+dlzka lode
        if ((wantedBridgePos.y -1 + (*boatToPlace).lenght) < GRID_SIZE)
        {
            (*boatToPlace).bridgePosition = wantedBridgePos;    
            
            //check ci sa tam lod zmesti(ci tam uz nie je ina)
            for (int piece = 0; piece < (*boatToPlace).lenght; piece++)
            {
                if (grid[(*boatToPlace).bridgePosition.y+piece][(*boatToPlace).bridgePosition.x] != '~')
                {
                    return 1;
                }
            }
            
            //samotne vertikalne umiestnenie lode na grid
            for (int piece = 0; piece < (*boatToPlace).lenght; piece++)
            {
                grid[(*boatToPlace).bridgePosition.y+piece][(*boatToPlace).bridgePosition.x] = (*boatToPlace).displayChar;
            }
            
        }
        else
        {
            //fprintf(stderr, "nezmesti sa\n");
            return 1;
        }
        
    }
    else
    {
        //nespravny znak pre orientaciu, musi byt 'h' alebo 'v'
        return 1;
    }

    return 0;
}
// nahodne rozlozi lodicky po gride
void rndBoatSpread(LOD boatsToSpread[SHIP_COUNT], char grid[GRID_SIZE][GRID_SIZE])
{
    srand(time(NULL));
    POSITION rndPos;
    int coinflip;
    char orientation;
    for (int ship = 0; ship < SHIP_COUNT; ship++)
    {
        rndPos.x = rand()%10;
        rndPos.y = rand()%10;
        coinflip = rand()%2;

        if (coinflip == 1)
        {
            orientation = 'h';
        }
        else
        {
            orientation = 'v';
        }
        
        if(placeBoat(&boatsToSpread[ship], rndPos, orientation, grid) == FAIL)
        {
            ship--;
        }
    }
    
    
}


LOD lodeHrac1[SHIP_COUNT]= {
    {"Cln",2, 2, '0'},
    {"Ponorka", 3, 3, '1'},
    {"Torpedoborec", 3, 3, '2'},
    {"Kriznik", 4, 3, '3'},
    {"LietadlovaLod", 5, 3, '4'}
};

LOD lodeHrac2[SHIP_COUNT]= {
    {"Cln",2, 2, '0'},
    {"Ponorka", 3, 3, '1'},
    {"Torpedoborec", 3, 3, '2'},
    {"Kriznik", 4, 4, '3'},
    {"LietadlovaLod", 5, 5, '4'}
};

int main()
{
    char player1grid[GRID_SIZE][GRID_SIZE];
    char player2grid[GRID_SIZE][GRID_SIZE];
    clearTerminal();
    fillWithWater(player1grid);
    fillWithWater(player2grid);
    POSITION cursorPosition;
    cursorPosition.x=1;
    cursorPosition.y=2;
    //polozenie lodky
    POSITION tuDajLod = {1,1};
    POSITION tuDajLod2 = {4,5};
    placeBoat(&lodeHrac1[1], tuDajLod, 'h', player1grid);
    placeBoat(&lodeHrac1[4], tuDajLod2, 'h', player1grid);
    printGrid(player1grid, &cursorPosition);

    // cursorPosition.x=61;
    // cursorPosition.y=1;
    // printGrid(player2grid, &cursorPosition);
    
    //printf("%s", lodeHrac1[0].name);

    return 0;
}