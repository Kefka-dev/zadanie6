#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 11
#define SHIP_COUNT 5
#define TRUE 1
#define FALSE 0
#define FAIL 1
#define SHIP_DESTROYED 2

#define SHIP_STATUS_LINE 15
#define ERROR_LINE 13
#define MAIN_IO_LINE 14
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
    int destroyed;
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
void white() {
    printf("\033[37m");
}
//sets the cursor position
void setCursorPos(int x, int y)
{
    printf("\033[%d;%dH", y, x);
}

//clears the terminal
void clearTerminal()
{
    printf("\033[H\033[J");
}

//clears the line from cursor position to the end of the screen
void clearLine()
{
    printf("\033[2K");
    fflush(stdout);
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
void printGrid(char grid[GRID_SIZE][GRID_SIZE], /*POSITION *gridPos*/ POSITION gridPos)
{

    for (int rows = 0; rows < GRID_SIZE; rows++)
    {
        // setCursorPos((*gridPos).x, (*gridPos).y);
        setCursorPos(gridPos.x, gridPos.y);
        for (int cols = 0; cols < GRID_SIZE; cols++)
        {
            if(rows >0 && cols>0)
            {
               if (isdigit(grid[rows][cols]) != FALSE)
               {
                    green();
               }
               else if (grid[rows][cols] == '-')
               {
                    white();
               }
               else if (grid[rows][cols] == 'X')
               {
                    red();
               }
               else
               {
                blue();
               }
               
            }
            else{
                yellow();
            }
            printf("[%c]", grid[rows][cols]);
        }
        reset();
        // (*gridPos).y++;
        gridPos.y++;
    }
    printf("\n");
}

//vypise grid zahaleny "hmlou" (lode niesu vidno)
void printGridWithFog(char grid[GRID_SIZE][GRID_SIZE], /*POSITION *gridPos*/ POSITION gridPos)
{
    for (int rows = 0; rows < GRID_SIZE; rows++)
    {
        // setCursorPos((*gridPos).x, (*gridPos).y);
        setCursorPos(gridPos.x, gridPos.y);
        for (int cols = 0; cols < GRID_SIZE; cols++)
        {
            if(rows >0 && cols>0)
            {
               if (isdigit(grid[rows][cols]) != FALSE)
               {
                    blue();
                    printf("[~]");
               }
               else if (grid[rows][cols] == '-')
               {
                    white();
                    printf("[%c]", grid[rows][cols]);
               }
               else if (grid[rows][cols] == 'X')
               {
                    red();
                    printf("[%c]", grid[rows][cols]);
               }
               else
               {
                    blue();
                    printf("[%c]", grid[rows][cols]);
               }
               
            }
            else{
                yellow();
                printf("[%c]", grid[rows][cols]);
            }
            
        }
        reset();
        // (*gridPos).y++;
        gridPos.y++;
    }
    printf("\n");
}

// ak je lodka polozena na gride vrati 0, ak nie vrati 1
int placeBoat(LOD *boatToPlace, POSITION wantedBridgePos, char orientation ,char grid[GRID_SIZE][GRID_SIZE])
{
    //cislovanie hracej plochy je na nultom riadku a nultom stlpci cize cords treba o 1 posunut
    wantedBridgePos.x++;
    wantedBridgePos.y++;

    if(orientation == 'h')
    {
        //chceck ci sa lod zmesti na sirku, cize poloha bridge+dlzka lode
        if ( ((wantedBridgePos.x - 1 + (*boatToPlace).lenght) < GRID_SIZE ) && wantedBridgePos.x >=1)
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
        if (((wantedBridgePos.y -1 + (*boatToPlace).lenght) < GRID_SIZE ) && wantedBridgePos.y >=1)
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

//na zaklade znaku ktoru lod trafil jej odrata 1 zivot, lod ma zivotou podla dlzky. tj. lod je dlha 5 => ma 5 zivotov
//RETURN 1 ak to bol posledny zivot lode, inac RETURN 0
int dmgShip(LOD ship[SHIP_COUNT], char shipIdentifier)
{
    int ShipCheck;
    //prevod charakteru cisla na integer
    ShipCheck = shipIdentifier - '0';
    ship[ShipCheck].liveTiles--;

    if (ship[ShipCheck].liveTiles == 0)
    {
        return 1;
    }
    return 0;
    // printf("test konverzu %d", ShipCheck);
    
}

//vystreli na grid, RETURN -1 ak je to mimo gridu, RETURN 1 ak trafi lod, RETURN 0 ak netrafi, RETURN 2 ak bola znicena cela lod
int shoot(char grid[GRID_SIZE][GRID_SIZE], POSITION target, LOD ship[SHIP_COUNT])
{
    //cislovanie hracej plochy je na nultom riadku a nultom stlpci cize cords treba o 1 posunut
    target.x++;
    target.y++;
    int destroyedBoat;
    if((target.x >=1 && target.y >=1) && (target.x < 11 && target.y < 11))
    {
        if(grid[target.y][target.x] == '~')
        {
            grid[target.y][target.x] = '-';
            return 0;
        }
        else if(isdigit(grid[target.y][target.x]) != FALSE)
        {
            destroyedBoat = dmgShip(ship, grid[target.y][target.x]);
            grid [target.y][target.x] = 'X';
            if (destroyedBoat == TRUE)
            {
                return 2;
            }
            
            return 1;
        }

    }
    else
    {
        return -1;
    }
    
}

//if ship destroyed return ship index, else -1
int destroyShip(LOD ships[SHIP_COUNT])
{
    for (int ship = 0; ship < SHIP_COUNT; ship++)
    {
        if(ships[ship].destroyed == FALSE && ships[ship].liveTiles == 0)
        {
            ships[ship].destroyed = TRUE;
            return ship;
        }
    }
    
    return -1;
}
//player boats array
LOD lodeHrac[SHIP_COUNT]= {
    {"PatrolBoat",2, 2, FALSE, '0'},
    {"Submarine", 3, 3, FALSE, '1'},
    {"Destroyer", 3, 3, FALSE, '2'},
    {"Battleship", 4, 4, FALSE, '3'},
    {"Carrier", 5, 5, FALSE, '4'}
};
//computer (insane AI) ship array
LOD lodeComputer[SHIP_COUNT]= {
    {"PatrolBoat",2, 2, FALSE, '0'},
    {"Submarine", 3, 3, FALSE, '1'},
    {"Destroyer", 3, 3, FALSE, '2'},
    {"Battleship", 4, 4, FALSE, '3'},
    {"Carrier", 5, 5, FALSE, '4'}
};

int main()
{
    char player1grid[GRID_SIZE][GRID_SIZE];
    char player2grid[GRID_SIZE][GRID_SIZE];
    clearTerminal();
    
    //printf("%s", lodeHrac1[0].name);

    return 0;
}