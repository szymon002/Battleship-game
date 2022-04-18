#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;

#define CAR 0
#define BAT 1
#define CRU 2
#define DES 3
#define maxNumberOfShips 10
#define COMMAND_LENGTH 50
#define typesOfShip 4
#define xZeroLimit 0
#define xMaxLimit 1
#define yZeroLimit 2
#define yMaxLimit 3
#define maxMoveCar 2
#define maxMove 3
#define minBoard 0
#define maxPlaneSquare 150
#define squareSize 2


typedef struct {
    int hasShot;
    int isAllowed[4];
    int numberOfShips[typesOfShip]; //CAR, BAT, CRU, DES
    int shipNumber[typesOfShip][maxNumberOfShips];
    int countParts;
    int actualShips[typesOfShip];
    int actualX[typesOfShip][maxNumberOfShips];
    int actualY[typesOfShip][maxNumberOfShips];
    int hasMoved[typesOfShip][maxNumberOfShips];
    int numOfShoots[typesOfShip][maxNumberOfShips];
    int numOfPlanes[maxNumberOfShips];
    char planesTab[maxPlaneSquare][maxPlaneSquare];
    char actualDir[typesOfShip][maxNumberOfShips];


} Player;

typedef struct {
        int y;
        int x;
        char direct;
        int numberOfShip;
        char shipClass[typesOfShip];
        int lengthOfShips[typesOfShip];
        int shipParts;
} Place;


void createBoard(char **board, int sizeY, int sizeX);

void createNewBoard(char **board, int sizeY, int sizeX);

void printBasicBoard(char **board, int sizeY, int sizeX, int aCounter, int bCounter);

void printBoard(char **board, int sizeY, int sizeX, int aCounter, int bCounter);

void changeNumberOfShips(char instr[], int tab[]);

void placeShip(char instr[], Player &P, Place &ship, char **board, int sizeY, int sizeX, int Num); 

void placeCarShip(Player &P, Place ship, char **board, int &dir, char instr[], int Num);

void placeBatShip(Player &P, Place ship, char **board, int &dir, char instr[], int Num);

void placeCruShip(Player &P, Place ship, char **board, int &dir, char instr[], int Num);

void placeDesShip(Player &P, Place ship, char **board, int &dir, char instr[], int Num);

void shoot(char instr[], int y, int x, char **board, int sizeY, int sizeX, Player &P);

void checkWinner(Player pA, Player pB);

void initPosition(char player, int aIsAllowed[], int bIsAllowed[], int y1, int x1, int y2, int x2);

void setReef(char instr[], char **board, int y, int x, int boardSizeY, int boardSizeX);

void deleteShip(int sClass, Player &P, Place &ship, char **board);

void moveShip(char instr[], Player &P, Place &ship, char **board, int sizeY, int sizeX, char course, int Num, bool extended);

void moveShipForward(char instr[], Player &P, Place &ship, char **board, int sizeY, int sizeX, int dir, int Num);

void moveShipRight(char instr[], Player &P, Place &ship, char **board, int sizeY, int sizeX, int dir, int Num);

void moveShipLeft(char instr[], Player &P, Place &ship, char **board, int sizeY, int sizeX, int dir, int Num);

void createPlayerParametrs(Player &P);

void cleanMoves(Player &P);

void cleanShoots(Player &P);

void printPlayerBoard(Player P, Place &ship, char **board, int sizeX, int sizeY, int x);

void extendedShoot(char instr[], int y, int x, Place ship, Player &P, Player &targetP, char **board, int boardSizeY, int boardSizeX);

void setShipSegments(int sClass, Player P, Place ship, int x, int y, char **board);

void setRadar(char **board, char **tempBoard, int sizeY, int sizeX, Player P, Place ship);

void sendSpyPlane(Player &P,Place ship, int y, int x, char **board, char instr[], int sizeY, int sizeX);

void isMyShip(int x, int y, char dir, int sClass, Place ship, char **board, char **tempBoard);

bool checkIsAllowed(int x, int y, int isAllowed[]);

bool checkIfAllShipsPlaced(int aNumberOfShips[], int bNumberOfShips[], char **board, int length[], int sizeY, int sizeX);

bool isSomeoneWon(Player pA, Player pB);

bool checkBrokenParts(int i, int shipParts, int currentLength);

bool isOnReef(int y, int x, char **board);

bool isTooClose(char **board, int sClass, int dir, int sizeX, int sizeY, Place &ship, Player &P);

bool isInBoard(char instr[], int y, int x, int sizeY, int sizeX);

bool hasMoved(int sClass, Player P, Place ship);

bool areTooManyShoots(Player P, Place ship, int sClass);

bool isCannonDestroyed(Player P, Place ship, char **board, int sClass);

bool isInRange(Player P, int sClass, int y1, int x1, Place ship);

bool isEngineBroken(Player P, Place ship, char **board, int sClass);

int chooseNextPlayer(char player);

int checkCourse(char course);

int checkShipClass(char shipClass[]);

int checkNumberOfCols(int sizeY);

int checkNumberOfRows(int sizeX);

int main() {
    int num;
    int boardSizeY = 21;
    int boardSizeX = 10;
    int byDefault = 0;
    char command[COMMAND_LENGTH];
    char instruction[COMMAND_LENGTH];
    int endOfTurn = 0;

    Place ship;
    ship.lengthOfShips[CAR] = 5;
    ship.lengthOfShips[BAT] = 4;
    ship.lengthOfShips[CRU] = 3;
    ship.lengthOfShips[DES] = 2;
    ship.shipParts = 11111;


    Player pA;
    pA.isAllowed[xZeroLimit] = 0;
    pA.isAllowed[xMaxLimit] = boardSizeX;
    pA.isAllowed[yZeroLimit] = 0;
    pA.isAllowed[yMaxLimit] = boardSizeY/2;

    Player pB;
    pB.isAllowed[xZeroLimit] = 0;
    pB.isAllowed[xMaxLimit] = boardSizeX;
    pB.isAllowed[yZeroLimit] = boardSizeY/2;
    pB.isAllowed[yMaxLimit] = boardSizeY;
    createPlayerParametrs(pB);
    createPlayerParametrs(pA);

    char **board = new char*[boardSizeY];
    for(int i = 0; i < boardSizeY; i++) {

        board[i] = new char [boardSizeX];

    }

    bool gameover = false;
    bool extended = false;

    createBoard(board,boardSizeY,boardSizeX);

    while(!gameover) {

        char comm[COMMAND_LENGTH];
        cin >> command;
        if (strcmp(command,"[state]") == 0) {
            do {
                cin.getline(instruction, COMMAND_LENGTH);
                sscanf(instruction,"%49s", comm);

                if (strcmp(comm, "BOARD_SIZE") == 0) {
                    int y,x;
                    sscanf(instruction,"%*s %d %d", &y, &x);
                    boardSizeX = x;
                    boardSizeY = y;
                    createNewBoard(board,boardSizeY,boardSizeX);
                }
                else if(strcmp(comm, "PRINT") == 0) {
                    int x;
                    sscanf(instruction, "%*s %d", &x);
                    if (x == 0) {
                        printBasicBoard(board, boardSizeY, boardSizeX, pA.countParts, pB.countParts);
                    } else if(x == 1) {
                        printBoard(board, boardSizeY, boardSizeX, pA.countParts, pB.countParts);
                    }
                }

                else if(strcmp(comm, "SET_FLEET") == 0) {
                    char player;
                    sscanf(instruction, "%*s %c", &player);

                    if (player == 'A') {
                        changeNumberOfShips(instruction, pA.numberOfShips);                                                              
                    }

                    else if (player == 'B'){
                        changeNumberOfShips(instruction, pB.numberOfShips);                                                              
                    } 
                }
                else if (strcmp(comm, "INIT_POSITION") == 0) {
                    char player;
                    int y1, y2, x1, x2;
                    sscanf(instruction, "%*s %c %d %d %d %d", &player, &y1, &x1, &y2, &x2);
                    initPosition(player, pA.isAllowed, pB.isAllowed, y1, x1, y2, x2);

                }
                else if (strcmp(comm, "REEF") == 0) {
                    int y, x;
                    sscanf(instruction, "%*s %d %d", &y, &x);
                    setReef(instruction, board, y, x, boardSizeY, boardSizeX);
                }

                else if(strcmp(comm, "SHIP") == 0) {
                    num = 0;
                    char player;
                    sscanf(instruction, "%*s %c %d %d %c %d %3s %d", &player, &ship.y, &ship.x, &ship.direct, &ship.numberOfShip, (char*)&ship.shipClass, &ship.shipParts);
                    int sClass = checkShipClass(ship.shipClass);
                    if (player == 'A') {
                    pA.actualY[sClass][ship.numberOfShip] = ship.y;
                    pA.actualX[sClass][ship.numberOfShip] = ship.x;
                    pA.actualDir[sClass][ship.numberOfShip] = ship.direct; 
                    placeShip(instruction, pA, ship, board, boardSizeY, boardSizeX, num); 
                    }                  
                    else if (player == 'B') {
                    pB.actualY[sClass][ship.numberOfShip] = ship.y;
                    pB.actualX[sClass][ship.numberOfShip] = ship.x;
                    pB.actualDir[sClass][ship.numberOfShip] = ship.direct; 
                    placeShip(instruction, pB, ship, board, boardSizeY, boardSizeX, num);  
                    }                 
                }

                else if(strcmp(comm, "NEXT_PLAYER") == 0) {
                    char player;
                    sscanf(instruction, "%*s %c", &player);
                    byDefault = chooseNextPlayer(player);
                }
                else if (strcmp(comm, "EXTENDED_SHIPS") == 0) {
                    extended = true;
                }

            } while(strcmp(instruction, "[state]") != 0);

        }
        else if (strcmp(command,"[playerA]") == 0) {
            if (byDefault == 0) {
            do {
                cin.getline(instruction, COMMAND_LENGTH);
                sscanf(instruction,"%49s", comm);
                if(strcmp(comm, "PLACE_SHIP") == 0) {
                    num = 1;
                    sscanf(instruction, "%*s %d %d %c %d %3s", &ship.y, &ship.x, &ship.direct, &ship.numberOfShip, (char*)&ship.shipClass );
                    int sClass = checkShipClass(ship.shipClass);
                    pA.actualY[sClass][ship.numberOfShip] = ship.y;
                    pA.actualX[sClass][ship.numberOfShip] = ship.x;
                    pA.actualDir[sClass][ship.numberOfShip] = ship.direct;                 
                    placeShip(instruction, pA, ship, board, boardSizeY, boardSizeX, num);                   
                }
                else if(strcmp(comm,"MOVE") == 0) {
                    num = 0;
                    char course;
                    sscanf(instruction, "%*s %d %3s %c", &ship.numberOfShip, (char*)&ship.shipClass, &course);
                    moveShip(instruction, pA, ship, board, boardSizeY, boardSizeX, course, num, extended);
                }
                else if (strcmp(comm, "SHOOT") == 0) {
                    if (checkIfAllShipsPlaced(pA.numberOfShips, pB.numberOfShips, board, ship.lengthOfShips, boardSizeY, boardSizeX)) {
                        if (!extended) {
                            if (pA.hasShot == 0){
                                    int y,x;    
                                    sscanf(instruction,"%*s %d %d", &y, &x);
                                    shoot(instruction, y, x, board, boardSizeY, boardSizeX, pB);
                                    pA.hasShot = 1; 
                                    pB.hasShot = 0;  
                                }                   
                            else {
                                printf("INVALID OPERATION \"%s\": TOO MANY SHOOTS", instruction);
                                exit(0);
                            }
                        }
                        else {
                            int y, x;
                            sscanf(instruction, "%*s %d %3s %d %d", &ship.numberOfShip, (char*)&ship.shipClass, &y, &x);
                            extendedShoot(instruction, y, x, ship, pA, pB, board, boardSizeY, boardSizeX);
                        }
                    }                  
                    else {
                        printf("INVALID OPERATION \"%s\": NOT ALL SHIPS PLACED", instruction);
                        exit(0);
                    }
                }
                else if (strcmp(comm, "SPY") == 0) {
                    int y, x;
                    sscanf(instruction, "%*s %d %d %d", &ship.numberOfShip, &y, &x);
                    sendSpyPlane(pA, ship, y, x, board, instruction, boardSizeY, boardSizeX);
                }
                else if (strcmp(comm, "PRINT") == 0) {
                    int x;
                    sscanf(instruction, "%*s %d", &x);
                    printPlayerBoard(pA, ship, board, boardSizeX, boardSizeY, x);
                }
                else if(strcmp(instruction,"[playerA]") == 0) {
                    endOfTurn++;
                    cleanShoots(pA);
                    cleanMoves(pA);
                    byDefault = 1;
                }
            } while(strcmp(instruction, "[playerA]") != 0);
            }
            else {
                printf("INVALID OPERATION \"%s \": THE OTHER PLAYER EXPECTED", command);
                exit(0);
            } 
        }   
        else if (strcmp(command,"[playerB]") == 0) {
            if (byDefault == 1) {
            do {
                byDefault = 0;
                cin.getline(instruction, COMMAND_LENGTH);
                sscanf(instruction,"%49s", comm);
                if(strcmp(comm, "PLACE_SHIP") == 0) {
                    num = 1;
                    sscanf(instruction, "%*s %d %d %c %d %3s", &ship.y, &ship.x, &ship.direct, &ship.numberOfShip, (char*)&ship.shipClass );
                    int sClass = checkShipClass(ship.shipClass);
                    pB.actualY[sClass][ship.numberOfShip] = ship.y;
                    pB.actualX[sClass][ship.numberOfShip] = ship.x;
                    pB.actualDir[sClass][ship.numberOfShip] = ship.direct; 
                    placeShip(instruction, pB, ship, board, boardSizeY, boardSizeX, num);                   
                }
                else if(strcmp(comm,"MOVE") == 0) {
                    num = 0;
                    char course;
                    sscanf(instruction, "%*s %d %3s %c", &ship.numberOfShip, (char*)&ship.shipClass, &course);
                    moveShip(instruction, pB, ship, board, boardSizeY, boardSizeX, course, num, extended);
                }
                else if (strcmp(comm, "SHOOT") == 0) {
                if (checkIfAllShipsPlaced(pA.numberOfShips, pB.numberOfShips, board, ship.lengthOfShips, boardSizeY, boardSizeX)) {
                    if (!extended) {
                        if (pB.hasShot == 0){
                                int y,x;    
                                sscanf(instruction,"%*s %d %d", &y, &x);
                                shoot(instruction, y, x, board, boardSizeY, boardSizeX, pA);
                                pB.hasShot = 1; 
                                pA.hasShot = 0;  
                            }                   
                        else {
                            printf("INVALID OPERATION \"%s\": TOO MANY SHOOTS", instruction);
                            exit(0);
                        }
                    }
                    else {
                        int y, x;
                        sscanf(instruction, "%*s %d %3s %d %d", &ship.numberOfShip, (char*)&ship.shipClass, &y, &x);
                        extendedShoot(instruction, y, x, ship, pB, pA, board, boardSizeY, boardSizeX);
                    }
                }                  
                else {
                    printf("INVALID OPERATION \"%s\": NOT ALL SHIPS PLACED", instruction);
                    exit(0);
                }
                }
                else if (strcmp(comm, "SPY") == 0) {
                    int y, x;
                    sscanf(instruction, "%*s %d %d %d",&ship.numberOfShip, &y, &x);
                    sendSpyPlane(pB, ship, y, x, board, instruction,boardSizeY, boardSizeX);
                }
                else if (strcmp(comm, "PRINT") == 0) {
                    int x;
                    sscanf(instruction, "%*s %d", &x);
                    printPlayerBoard(pB, ship, board, boardSizeX, boardSizeY, x);
                }
                else if(strcmp(instruction,"[playerB]") == 0) {
                    endOfTurn++;
                    cleanShoots(pB);                    
                    cleanMoves(pB);
                    byDefault = 0;
                }   
            } while(strcmp(instruction, "[playerB]") != 0);
            }
            else {
                printf("INVALID OPERATION \"%s \": THE OTHER PLAYER EXPECTED", command);
                exit(0);
            }
        }

        else if (cin.peek() == EOF)
               exit(0);

        if(endOfTurn >= 2)
        gameover = isSomeoneWon(pA, pB);
    }

    checkWinner(pA, pB);

    for(int i = 0; i < boardSizeY; i++) {

        delete [] board[i];
    }
    
    delete [] board;

    return 0;
}


void createBoard(char **board, int sizeY, int sizeX) {

    for (int i = 0; i<sizeY; i++) {
        for (int j = 0; j<sizeX; j++) {
            board[i][j] = ' ';
        }
    }
}

void createNewBoard(char **board, int sizeY, int sizeX){
    for(int i = 0; i < sizeY; i++) {

        board[i] = new char [sizeX];

    }

    createBoard(board,sizeY,sizeX);
}

 
void printBasicBoard(char **board, int sizeY, int sizeX, int aCounter, int bCounter) {

    for (int i = 0; i<sizeY; i++) {
        for (int j = 0; j<sizeX; j++) {
            if (board[i][j] == '@' || board[i][j] == '%' || board[i][j] == '!') printf("%c", '+');
            else
            printf("%c", board[i][j]);

        }
        printf("\n");
    }
    printf("PARTS REMAINING:: A : %d B : %d \n", aCounter, bCounter);
}

void printBoard(char **board, int sizeY, int sizeX, int aCounter, int bCounter) {

        int k = checkNumberOfCols(sizeY-1);
        int w = checkNumberOfRows(sizeX-1);
        
        for (int i = 0; i<sizeY+1; i++) {
            for (int j = 0; j<sizeX+k; j++) {
            if(i < w && j >=k) printf("%d", j-k);

            else if(j == 0 && i >= w)  {printf("%0*d",k, i-1);}

            else if (i>0 && j>=1) printf("%c", board[i-1][j-1]);

            else printf("%c", ' ');
            }
            printf("\n");
        }
        printf("PARTS REMAINING:: A : %d B : %d \n", aCounter, bCounter);
}

void printPlayerBoard(Player P, Place &ship, char **board, int sizeX, int sizeY, int x) {
         
    int k = checkNumberOfCols(sizeY-1);
    int w = checkNumberOfRows(sizeX-1);

    char **tempBoard = new char*[sizeY];
    for(int i = 0; i < sizeY; i++) {
        tempBoard[i] = new char [sizeX];
    }

    for (int i=0; i<sizeY; i++) {
        for(int j=0; j<sizeX; j++) {
            tempBoard[i][j] = '?';
        }
    }
    
    for (int i=0; i<sizeY; i++) {
        for(int j=0; j<sizeX; j++) {
            if(P.planesTab[i][j] == '+' || P.planesTab[i][j] == 'x' ||P.planesTab[i][j] == ' '
            || P.planesTab[i][j] == '!' || P.planesTab[i][j] == '@' || P.planesTab[i][j] == '%')
            tempBoard[i][j] = P.planesTab[i][j];
        }
    }

    for (int i=0; i<sizeY; i++) {
        for(int j=0; j<sizeX; j++) {
            if(board[i][j] == '#')
            tempBoard[i][j] = board[i][j];
        }
    }
    setRadar(board, tempBoard, sizeY, sizeX, P, ship);
    if (x == 0) {
    for (int i=0; i<sizeY; i++) {
        for(int j=0; j<sizeX; j++) {
            if (tempBoard[i][j] == '@' || tempBoard[i][j] == '%' || tempBoard[i][j] == '!') printf("%c", '+');
            else printf("%c",tempBoard[i][j]);
        }
        printf("\n");
    }
    } else if (x == 1) {
        for (int i=0; i<sizeY; i++) {
            for(int j=0; j<sizeX; j++) {
            if(i < w && j >=k) printf("%d", j-k);

            else if(j == 0 && i >= w)  {printf("%0*d",k, i-1);}

            else if (i>0 && j>=1) printf("%c", tempBoard[i-1][j-1]);

            else printf("%c", ' ');            }
            printf("\n");
        } 
    }

       for(int i = 0; i < sizeY; i++) {
        delete [] tempBoard[i];
    }
    
    delete [] tempBoard;
}

int chooseNextPlayer(char player) {

        if (player == 'A') return 0;
        else return 1;
}

void createPlayerParametrs(Player &P) {
    P.numberOfShips[CAR] = 1;
    P.numberOfShips[BAT] = 2;
    P.numberOfShips[CRU] = 3;
    P.numberOfShips[DES] = 4;
    P.hasShot = 0;
    P.countParts = 0;
    P.actualShips[CAR] = 0;
    P.actualShips[BAT] = 0;
    P.actualShips[CRU] = 0;
    P.actualShips[DES] = 0;
    cleanMoves(P);
    cleanShoots(P);
    for (int i = 0; i < typesOfShip; i++) {
        for (int j = 0; j<maxNumberOfShips; j++) {
            P.shipNumber[i][j] = 0;
        }
    }

        for (int i = 0; i<maxNumberOfShips; i++) {
        P.numOfPlanes[i] = 0;
    }
}

void changeNumberOfShips(char instr[], int tab[]) {
    char p;
    sscanf(instr, "%*s %c %d %d %d %d", &p, &tab[0], &tab[1], &tab[2], &tab[3]);
}

void placeShip(char instr[], Player &P, Place &ship, char **board, int sizeY, int sizeX, int Num) {
    int dir;
    int sClass;

    sClass = checkShipClass(ship.shipClass);
    if (P.actualDir[sClass][ship.numberOfShip] == 'N' || P.actualDir[sClass][ship.numberOfShip]  == 'W') {
        dir = 1;
    } 
    else dir = -1;

    if(strcmp(instr,"") == 0) return;
     else {
        if(P.shipNumber[sClass][ship.numberOfShip] == 0) {
            if (ship.numberOfShip > (P.numberOfShips[sClass] - 1)){
                printf("INVALID OPERATION \"%s\": ALL SHIPS OF THE CLASS ALREADY SET", instr);
                exit(0);
            }
            else {
                if(sClass == CAR) placeCarShip(P, ship, board, dir, instr, Num);
                if(sClass == BAT) placeBatShip(P, ship, board, dir, instr, Num);
                if(sClass == CRU) placeCruShip(P, ship, board, dir, instr, Num);
                if(sClass == DES) placeDesShip(P, ship, board, dir, instr, Num);
                P.shipNumber[sClass][ship.numberOfShip] = 1;
                P.actualShips[sClass]++;
                if(isTooClose(board, sClass, dir, sizeX, sizeY, ship, P)) {
                    printf("INVALID OPERATION \"%s\": PLACING SHIP TOO CLOSE TO OTHER SHIP", instr);
                    exit(0);
                }
            }
        }
        else {
            printf("INVALID OPERATION \"%s\": SHIP ALREADY PRESENT", instr);
            exit(0);
        }
    }
     ship.shipParts = 11111;   
}                  

void placeCarShip(Player &P, Place ship, char **board, int &dir, char instr[], int Num) {

    int y = P.actualY[CAR][ship.numberOfShip];
    int x = P.actualX[CAR][ship.numberOfShip];        
    if (P.actualDir[CAR][ship.numberOfShip] == 'N' || P.actualDir[CAR][ship.numberOfShip] == 'S') {
        for(int i = 0; i<ship.lengthOfShips[CAR]; i++, y += dir) {

            if(checkIsAllowed(x,y,P.isAllowed) || Num == 0) {
                    if (!isOnReef(y ,x , board)) {
                        if(!checkBrokenParts(i, ship.shipParts, ship.lengthOfShips[CAR])) {
                            board[y][x] = '+';
                            P.countParts++;
                            setShipSegments(CAR, P, ship, x, y, board);
                        }
                        else {
                            board[y][x] = 'x';
                        }
                    }
                    else {
                        printf("INVALID OPERATION  \"%s\": PLACING SHIP ON REEF", instr);
                        exit(0);

                    }
            }   
            else {
                printf("INVALID OPERATION  \"%s\": NOT IN STARTING POSITION", instr);
                exit(0);
            }
        }
    }

    else  {
        for(int i = 0; i<ship.lengthOfShips[CAR]; i++, x += dir) {
            if(checkIsAllowed(x,y,P.isAllowed) || Num == 0) {
                    if (!isOnReef(y,x, board)) {
                        if(!checkBrokenParts(i, ship.shipParts, ship.lengthOfShips[CAR])) {
                            board[y][x] = '+';
                            P.countParts++;
                            setShipSegments(CAR, P, ship, x, y, board);
                        }
                        else {
                            board[y][x] = 'x';
                        }
                    }
                    else {
                        printf("INVALID OPERATION  \"%s\": PLACING SHIP ON REEF", instr);
                        exit(0);

                    }
            }   
            else {
                printf("INVALID OPERATION  \"%s\": NOT IN STARTING POSITION", instr);
                exit(0);
            }
        }   
    }
}

void placeBatShip(Player &P, Place ship, char **board, int &dir, char instr[], int Num) {

    int y = P.actualY[BAT][ship.numberOfShip];
    int x = P.actualX[BAT][ship.numberOfShip];  
        
    if (P.actualDir[BAT][ship.numberOfShip] == 'N' || P.actualDir[BAT][ship.numberOfShip] == 'S') {
        for(int i = 0; i<ship.lengthOfShips[BAT]; i++, y += dir) {

            if(checkIsAllowed(x,y,P.isAllowed) || Num == 0) {
                    if (!isOnReef(y, x, board)) {
                        if(!checkBrokenParts(i, ship.shipParts, ship.lengthOfShips[BAT])) {
                            board[y][x] = '+';
                            P.countParts++;
                            setShipSegments(BAT, P, ship, x, y, board);
                        }
                        else {
                            board[y][x] = 'x';
                        }
                    }
                    else {
                        printf("INVALID OPERATION  \"%s\": PLACING SHIP ON REEF", instr);
                        exit(0);

                    }
            }   
            else {
                printf("INVALID OPERATION  \"%s\": NOT IN STARTING POSITION", instr);
                exit(0);
            }
        }
    }

    else  {
        for(int i = 0; i<ship.lengthOfShips[BAT]; i++, x += dir) {
            if(checkIsAllowed(x,y,P.isAllowed) || Num == 0) {
                    if (!isOnReef(y, x, board)) {
                        if(!checkBrokenParts(i, ship.shipParts, ship.lengthOfShips[BAT])) {
                            board[y][x] = '+';
                            P.countParts++;
                            setShipSegments(BAT, P, ship, x, y, board);
                        }
                        else {
                            board[y][x] = 'x';
                        }
                    }
                    else {
                        printf("INVALID OPERATION  \"%s\": PLACING SHIP ON REEF", instr);
                        exit(0);

                    }
            }   
            else {
                printf("INVALID OPERATION  \"%s\": NOT IN STARTING POSITION", instr);
                exit(0);
            }
        }   
    }
}

void placeCruShip(Player &P, Place ship, char **board, int &dir, char instr[], int Num) {

    int y = P.actualY[CRU][ship.numberOfShip];
    int x = P.actualX[CRU][ship.numberOfShip];  
        
    if (P.actualDir[CRU][ship.numberOfShip] == 'N' || P.actualDir[CRU][ship.numberOfShip] == 'S') {
        for(int i = 0; i<ship.lengthOfShips[CRU]; i++, y += dir) {

            if(checkIsAllowed(x,y,P.isAllowed) || Num == 0) {
                    if (!isOnReef(y,x, board)) {
                        if(!checkBrokenParts(i, ship.shipParts, ship.lengthOfShips[CRU])) {
                            board[y][x] = '+';
                            P.countParts++;
                            setShipSegments(CRU, P, ship, x, y, board);
                        }
                        else {
                            board[y][x] = 'x';
                        }
                    }
                    else {
                        printf("INVALID OPERATION  \"%s\": PLACING SHIP ON REEF", instr);
                        exit(0);

                    }
            }   
            else {
                printf("INVALID OPERATION  \"%s\": NOT IN STARTING POSITION", instr);
                exit(0);
            }
        }
    }

    else  {
        for(int i = 0; i<ship.lengthOfShips[CRU]; i++, x += dir) {
            if(checkIsAllowed(x,y,P.isAllowed) || Num == 0) {
                    if (!isOnReef(y,x, board)) {
                        if(!checkBrokenParts(i, ship.shipParts, ship.lengthOfShips[CRU])) {
                            board[y][x] = '+';
                            P.countParts++;
                            setShipSegments(CRU, P, ship, x, y, board);
                        }
                        else {
                            board[y][x] = 'x';
                        }
                    }
                    else {
                        printf("INVALID OPERATION  \"%s\": PLACING SHIP ON REEF", instr);
                        exit(0);

                    }
            }   
            else {
                printf("INVALID OPERATION  \"%s\": NOT IN STARTING POSITION", instr);
                exit(0);
            }
        }   
    }
}

void placeDesShip(Player &P, Place ship, char **board, int &dir, char instr[], int Num) {\

    int y = P.actualY[DES][ship.numberOfShip];
    int x = P.actualX[DES][ship.numberOfShip];  
        
    if (P.actualDir[DES][ship.numberOfShip] == 'N' || P.actualDir[DES][ship.numberOfShip] == 'S') {
        for(int i = 0; i<ship.lengthOfShips[DES]; i++, y += dir) {

            if(checkIsAllowed(x, y,P.isAllowed) || Num == 0) {
                    if (!isOnReef(y,x, board)) {
                        if(!checkBrokenParts(i, ship.shipParts, ship.lengthOfShips[DES])) {
                            board[y][x] = '+';
                            P.countParts++;
                            setShipSegments(DES, P, ship, x, y, board);
                        }
                        else {
                            board[y][x] = 'x';
                        }
                    }
                    else {
                        printf("INVALID OPERATION  \"%s\": PLACING SHIP ON REEF", instr);
                        exit(0);

                    }
            }   
            else {
                printf("INVALID OPERATION  \"%s\": NOT IN STARTING POSITION", instr);
                exit(0);
            }
        }
    }

    else  {
        for(int i = 0; i<ship.lengthOfShips[DES]; i++, x += dir) {
            if(checkIsAllowed(x,y,P.isAllowed) || Num == 0) {
                    if (!isOnReef(y, x, board)) {   
                        if(!checkBrokenParts(i, ship.shipParts, ship.lengthOfShips[DES])) {
                            board[y][x] = '+';
                            P.countParts++;
                            setShipSegments(DES, P, ship, x, y, board);
                        }
                        else {
                            board[y][x] = 'x';
                        }
                    }
                    else {
                        printf("INVALID OPERATION  \"%s\": PLACING SHIP ON REEF", instr);
                        exit(0);

                    }
            }   
            else {
                printf("INVALID OPERATION  \"%s\": NOT IN STARTING POSITION", instr);
                exit(0);
            }
        }   
    }
}


bool checkIsAllowed(int x, int y, int isAllowed[]) {
 
    if((y>=isAllowed[yZeroLimit] && y<=isAllowed[yMaxLimit]) && (x>=isAllowed[xZeroLimit] && x<=isAllowed[xMaxLimit]))
    return true;
    else return false;
}

bool checkIfAllShipsPlaced(int aNumberOfShips[], int bNumberOfShips[], char **board,  int length[], int sizeY, int sizeX) {
    int counter0 = 0;
    int counter = 0;

    for (int i = 0; i<sizeY; i++) {
        for (int j = 0; j<sizeX; j++) {
            if(board[i][j] == '+' || board[i][j] == 'x' || board[i][j] == '@' || board[i][j] == '!' || board[i][j] == '%') 
                counter++;
        }
    }
    counter0 = ((aNumberOfShips[CAR]+bNumberOfShips[CAR]) * length[CAR]) + ((aNumberOfShips[BAT]+bNumberOfShips[BAT]) * length[BAT])
     + ((aNumberOfShips[CRU]+bNumberOfShips[CRU]) * length[CRU]) + ((aNumberOfShips[DES]+bNumberOfShips[DES]) * length[DES]);

    if (counter0 == counter) return true;
    else return false;

}
void extendedShoot(char instr[], int y, int x, Place ship, Player &P, Player &targetP, char **board, int boardSizeY, int boardSizeX) {

    int sClass = checkShipClass(ship.shipClass);
    if (!areTooManyShoots(P, ship, sClass)) {
        if(!isCannonDestroyed(P, ship, board, sClass)) {
            if(isInRange(P, sClass, y, x, ship)) {
                shoot(instr, y, x, board, boardSizeY, boardSizeX, targetP);
                P.numOfShoots[sClass][ship.numberOfShip]++;
            }
            else {
                printf("INVALID OPERATION  \"%s\": SHOOTING TOO FAR", instr);
                exit(0);
            }
        }
        else {
            printf("INVALID OPERATION  \"%s\": SHIP CANNOT SHOOT", instr);
            exit(0);
        }
    }
    else {
        printf("INVALID OPERATION  \"%s\": TOO MANY SHOOTS", instr);
        exit(0);
    }
}

void shoot(char instr[], int y, int x, char **board, int sizeY, int sizeX, Player &P) {

    if(y>=sizeY || x>=sizeX || x<minBoard || y<minBoard) {
        printf("INVALID OPERATION  \"%s\": FIELD DOES NOT EXIST", instr);
        exit(0);
    }
    else if (board[y][x] =='+' || board[y][x] =='@' || board[y][x] =='!' || board[y][x] =='%') {
        board[y][x] = 'x';
        P.countParts--;
    }

}

bool isSomeoneWon(Player pA, Player pB) {
    if (pA.countParts == 0 || pB.countParts == 0) return true;
    else return false;
}

void checkWinner(Player pA, Player pB) {
    
    if (pA.countParts == 0)
    printf("B won"); 
    else if (pB.countParts == 0)
    printf("A won");

}

void initPosition(char player, int aIsAllowed[], int bIsAllowed[], int y1, int x1, int y2, int x2) {

    if (player == 'A') {
        aIsAllowed[xZeroLimit] = x1;
        aIsAllowed[xMaxLimit] = x2;
        aIsAllowed[yZeroLimit] = y1;
        aIsAllowed[yMaxLimit] = y2;
    } else if (player == 'B') {
        bIsAllowed[xZeroLimit] = x1;
        bIsAllowed[xMaxLimit] = x2;
        bIsAllowed[yZeroLimit] = y1;
        bIsAllowed[yMaxLimit] = y2;
    }

}

void setReef(char instr[], char **board, int y, int x, int boardSizeY, int boardSizeX) {

    if (y >= boardSizeY || x>=boardSizeX) {
        printf("INVALID OPERATION  \"%s\": REEF IS NOT PLACED ON BOARD", instr);
        exit(0);
    }
    else board[y][x] = '#';
}

bool checkBrokenParts(int i, int shipParts, int currentLength){
    int *tab = new int[currentLength];
    for(int j=0; j<currentLength; j++) {
        tab[currentLength - 1 - j] = shipParts % 10;
        shipParts = shipParts / 10;
    }
    if (tab[i] == 0) {
        delete [] tab; 
        return true ;
    }
    else {
        delete [] tab;
        return false;
}
}

bool isOnReef(int y, int x, char **board) {

    if(board[y][x] == '#') return true;
    else return false;

}

bool isTooClose(char **board, int sClass, int dir, int sizeX, int sizeY, Place &ship, Player &P) {
    
    int counter = 0;
    int x = P.actualX[sClass][ship.numberOfShip];
    int y = P.actualY[sClass][ship.numberOfShip];
    if (P.actualDir[sClass][ship.numberOfShip] == 'N' || P.actualDir[sClass][ship.numberOfShip] == 'S') {
        for (int i = -1; i<ship.lengthOfShips[sClass] + 1; i++) {
            for (int j = -1; j<squareSize ; j++) {
                if (x+j < 0 || x+j >= sizeX || y+(i*dir) >=sizeY || y+(i*dir) < 0) {
                    continue;
                }
                else {
                        if (board[y+(i*dir)][x+j] == '+' || board[y+(i*dir)][x+j] == 'x' || board[y+(i*dir)][x+j] == '@'
                        || board[y+(i*dir)][x+j] == '%' || board[y+(i*dir)][x+j] == '!') counter++;
                }
            }
        }
        if (counter > ship.lengthOfShips[sClass]) return true;
        else return false;
    }
    else {
        for (int i = -1; i<squareSize; i++) {
            for (int j = -1; j<ship.lengthOfShips[sClass] + 1; j++) {
                if (x+(j*dir) < 0 || x+(j*dir) >= sizeX || y+i >=sizeY || y+i < 0) {
                    continue;
                }
                else {
                        if (board[y+i][x+(j*dir)] == '+' || board[y+i][x+(j*dir)] == 'x' || board[y+i][x+(j*dir)] == '!'
                        || board[y+i][x+(j*dir)] == '@' || board[y+i][x+(j*dir)] == '%') counter++;;
                }
            }
        }
        if (counter > ship.lengthOfShips[sClass]) return true;
        else return false;
    }
}

int checkShipClass(char shipClass[]) {

    if (strcmp(shipClass, "CAR") == 0) {
        return 0;
    }
    else if(strcmp(shipClass, "BAT") == 0) {
        return 1;
    }
    else if(strcmp(shipClass, "CRU") == 0) {
        return 2;
    }
    else if(strcmp(shipClass, "DES") == 0) {
        return 3;
    }
    else return -1;
}

void deleteShip(int sClass, Player &P, Place &ship, char **board) {
    
    int dir;

    P.shipNumber[sClass][ship.numberOfShip] = 0;

    if (P.actualDir[sClass][ship.numberOfShip] == 'N' || P.actualDir[sClass][ship.numberOfShip] == 'W') {
        dir = 1;
    } 
    else dir = -1;

    int x = P.actualX[sClass][ship.numberOfShip];
    int y = P.actualY[sClass][ship.numberOfShip];


    if (P.actualDir[sClass][ship.numberOfShip] == 'N' || P.actualDir[sClass][ship.numberOfShip] == 'S') {
        for(int i = 0; i<ship.lengthOfShips[sClass]; i++, y += dir) {
            board[y][x] = ' ';
            P.countParts--;
            }
    }
    else  {
        for(int i = 0; i<ship.lengthOfShips[sClass]; i++, x += dir) {
            board[y][x] = ' ';
            P.countParts--;
        }
    }

}

void moveShip(char instr[], Player &P, Place &ship, char **board, int sizeY, int sizeX, char course, int Num, bool extended) {

    int dir;
    int sClass = checkShipClass(ship.shipClass);

    if(ship.direct == 'N' || ship.direct == 'W') {
        dir = -1;
    } else dir = 1;
    if (!isEngineBroken(P, ship, board, sClass) || !extended) {
        if (!hasMoved(sClass, P, ship)) { 
            deleteShip(sClass, P, ship, board);     
            P.hasMoved[sClass][ship.numberOfShip]++;
            if (course == 'F') {
                moveShipForward(instr, P, ship, board, sizeY, sizeX, dir, Num);
            }
            else if(course == 'R') {
                moveShipRight(instr, P, ship, board, sizeY, sizeX, dir, Num);
            }
            else if (course == 'L') {
                moveShipLeft(instr, P, ship, board, sizeY, sizeX, dir, Num);
            }
        }
        else {
            printf("INVALID OPERATION  \"%s\": SHIP MOVED ALREADY", instr);
            exit(0);
        }
    }
    else {
        printf("INVALID OPERATION  \"%s\": SHIP CANNOT MOVE", instr);
        exit(0);       
    }

}

void moveShipForward(char instr[], Player &P, Place &ship, char **board, int sizeY, int sizeX, int dir, int Num) {
    
    int sClass = checkShipClass(ship.shipClass);
    int newHeadY;
    int newHeadX;
    if (P.actualDir[sClass][ship.numberOfShip] == 'N' || P.actualDir[sClass][ship.numberOfShip] == 'S') {
        P.actualY[sClass][ship.numberOfShip] += dir;
        newHeadY = P.actualY[sClass][ship.numberOfShip];
        newHeadX = P.actualX[sClass][ship.numberOfShip];
        if (isInBoard(instr, newHeadY, newHeadX, sizeY, sizeX))
        placeShip(instr, P, ship, board, sizeY, sizeX, Num);
    } else {
        P.actualX[sClass][ship.numberOfShip] += dir;
        newHeadY = P.actualY[sClass][ship.numberOfShip];
        newHeadX = P.actualX[sClass][ship.numberOfShip];
        if (isInBoard(instr, newHeadY, newHeadX, sizeY, sizeX))
        placeShip(instr, P, ship, board, sizeY, sizeX, Num);
    }
}

void moveShipRight(char instr[], Player &P, Place &ship, char **board, int sizeY, int sizeX, int dir, int Num) {
    int newHeadX;
    int newHeadY;
    int sClass = checkShipClass(ship.shipClass);
    if (P.actualDir[sClass][ship.numberOfShip] == 'N') {
        P.actualY[sClass][ship.numberOfShip] += dir;
        P.actualX[sClass][ship.numberOfShip] += (ship.lengthOfShips[sClass]-1);
        P.actualDir[sClass][ship.numberOfShip] = 'E';
        newHeadX = P.actualX[sClass][ship.numberOfShip];
        newHeadY = P.actualY[sClass][ship.numberOfShip];
        if (isInBoard(instr, newHeadY, newHeadX, sizeY, sizeX))
        placeShip(instr, P, ship, board, sizeY, sizeX, Num);
    }
    else if(P.actualDir[sClass][ship.numberOfShip] == 'S') {
        P.actualY[sClass][ship.numberOfShip] += dir;
        P.actualX[sClass][ship.numberOfShip] -= (ship.lengthOfShips[sClass]-1);
        P.actualDir[sClass][ship.numberOfShip] = 'W';
        newHeadX = P.actualX[sClass][ship.numberOfShip];
        newHeadY = P.actualY[sClass][ship.numberOfShip];
        if (isInBoard(instr, newHeadY, newHeadX, sizeY, sizeX))      
        placeShip(instr, P, ship, board, sizeY, sizeX, Num); 
    }
    else if(P.actualDir[sClass][ship.numberOfShip] == 'W') {
        P.actualX[sClass][ship.numberOfShip] += dir;
        P.actualY[sClass][ship.numberOfShip] -= (ship.lengthOfShips[sClass]-1);
        P.actualDir[sClass][ship.numberOfShip] = 'N';
        newHeadX = P.actualX[sClass][ship.numberOfShip];
        newHeadY = P.actualY[sClass][ship.numberOfShip];
        if (isInBoard(instr, newHeadY, newHeadX, sizeY, sizeX))       
        placeShip(instr, P, ship, board, sizeY, sizeX, Num); 
    }
    else if(P.actualDir[sClass][ship.numberOfShip] == 'E') {
        P.actualX[sClass][ship.numberOfShip] += dir;
        P.actualY[sClass][ship.numberOfShip] += (ship.lengthOfShips[sClass]-1);
        P.actualDir[sClass][ship.numberOfShip] = 'S';
        newHeadX = P.actualX[sClass][ship.numberOfShip];
        newHeadY = P.actualY[sClass][ship.numberOfShip];
        if (isInBoard(instr, newHeadY, newHeadX, sizeY, sizeX))        
        placeShip(instr, P, ship, board, sizeY, sizeX, Num); 
    }
}

void moveShipLeft(char instr[], Player &P, Place &ship, char **board, int sizeY, int sizeX, int dir, int Num) {

    int sClass = checkShipClass(ship.shipClass);
    int newHeadX;
    int newHeadY;
    if (P.actualDir[sClass][ship.numberOfShip] == 'N') {
        P.actualY[sClass][ship.numberOfShip] += dir;
        P.actualX[sClass][ship.numberOfShip] -= (ship.lengthOfShips[sClass]-1);
        P.actualDir[sClass][ship.numberOfShip] = 'W';
        newHeadX = P.actualX[sClass][ship.numberOfShip];
        newHeadY = P.actualY[sClass][ship.numberOfShip];
        if (isInBoard(instr, newHeadY, newHeadX, sizeY, sizeX))
        placeShip(instr, P, ship, board, sizeY, sizeX, Num);
    }
    else if(P.actualDir[sClass][ship.numberOfShip] == 'S') {
        P.actualY[sClass][ship.numberOfShip] += dir;
        P.actualX[sClass][ship.numberOfShip] += (ship.lengthOfShips[sClass]-1);
        P.actualDir[sClass][ship.numberOfShip] = 'E';
        newHeadX = P.actualX[sClass][ship.numberOfShip];
        newHeadY = P.actualY[sClass][ship.numberOfShip];
        if (isInBoard(instr, newHeadY, newHeadX, sizeY, sizeX))       
        placeShip(instr, P, ship, board, sizeY, sizeX, Num); 
    }
    else if(P.actualDir[sClass][ship.numberOfShip] == 'W') {
        P.actualX[sClass][ship.numberOfShip] += dir;
        P.actualY[sClass][ship.numberOfShip] += (ship.lengthOfShips[sClass]-1);
        P.actualDir[sClass][ship.numberOfShip] = 'S';
        newHeadX = P.actualX[sClass][ship.numberOfShip];
        newHeadY = P.actualY[sClass][ship.numberOfShip];
        if (isInBoard(instr, newHeadY, newHeadX, sizeY, sizeX))       
        placeShip(instr, P, ship, board, sizeY, sizeX, Num); 
    }
    else if(P.actualDir[sClass][ship.numberOfShip] == 'E') {
        P.actualX[sClass][ship.numberOfShip] += dir;
        P.actualY[sClass][ship.numberOfShip] -= (ship.lengthOfShips[sClass]-1);
        P.actualDir[sClass][ship.numberOfShip] = 'N';  
        newHeadX = P.actualX[sClass][ship.numberOfShip];
        newHeadY = P.actualY[sClass][ship.numberOfShip];
        if (isInBoard(instr, newHeadY, newHeadX, sizeY, sizeX))     
        placeShip(instr, P, ship, board, sizeY, sizeX, Num); 
    }
}

bool isInBoard(char instr[], int y, int x, int sizeY, int sizeX) {
    if(y>=sizeY || y<minBoard || x>=sizeX || x<minBoard) {
        printf("INVALID OPERATION  \"%s\": SHIP WENT FROM BOARD", instr);
        exit(0);
    }
    return true; 
}

bool hasMoved(int sClass, Player P, Place ship) {
    
    if (sClass == CAR) {
        if (P.hasMoved[sClass][ship.numberOfShip] >= maxMoveCar) return true;
        return false;
    }
    else {
        if (P.hasMoved[sClass][ship.numberOfShip] >= maxMove) return true;
        return false;
    }
}

void cleanMoves(Player &P) {
    for(int i = 0; i < typesOfShip; i++) {
        for(int j = 0; j<maxNumberOfShips; j++) {
            P.hasMoved[i][j] = 0;
        }
    }
}

void cleanShoots(Player &P) {
    for(int i = 0; i < typesOfShip; i++) {
        for(int j = 0; j<maxNumberOfShips; j++) {
            P.numOfShoots[i][j] = 0;
        }
    }

}
bool areTooManyShoots(Player P, Place ship, int sClass) {
    if(P.numOfShoots[sClass][ship.numberOfShip] >= ship.lengthOfShips[sClass]) return true;
    return false;
}

bool isCannonDestroyed(Player P, Place ship, char **board, int sClass) {
    
    int dir;

    if (P.actualDir[sClass][ship.numberOfShip] == 'N' || P.actualDir[sClass][ship.numberOfShip]  == 'W') {
        dir = 1;
    } 
    else dir = -1;

    int x = P.actualX[sClass][ship.numberOfShip];
    int y = P.actualY[sClass][ship.numberOfShip];

    if (P.actualDir[sClass][ship.numberOfShip] == 'N' || P.actualDir[sClass][ship.numberOfShip] == 'S') {
        if(board[y+dir][x] == 'x') return true;
    }
    if (P.actualDir[sClass][ship.numberOfShip] == 'W' || P.actualDir[sClass][ship.numberOfShip] == 'E') {
        if(board[y][x+dir] == 'x') return true;
    }

    return false;
}

bool isEngineBroken(Player P, Place ship, char **board, int sClass) {
       
    int dir;

    if (P.actualDir[sClass][ship.numberOfShip] == 'N' || P.actualDir[sClass][ship.numberOfShip]  == 'W') { // check that ship segments are in left/right/top/bottom
        dir = ship.lengthOfShips[sClass]-1; 
    } 
    else dir = -(ship.lengthOfShips[sClass]-1);

    int x = P.actualX[sClass][ship.numberOfShip];
    int y = P.actualY[sClass][ship.numberOfShip];

    if (P.actualDir[sClass][ship.numberOfShip] == 'N' || P.actualDir[sClass][ship.numberOfShip] == 'S') {
        if(board[y+dir][x] == 'x') return true;
    }
    if (P.actualDir[sClass][ship.numberOfShip] == 'W' || P.actualDir[sClass][ship.numberOfShip] == 'E') {
        if(board[y][x+dir] == 'x') return true;
    }

    return false;
}

bool isInRange(Player P, int sClass, int y1, int x1, Place ship) {
    int dir;
    int distance =0;
    if (sClass == CAR) {
        return true;
    }
    if (P.actualDir[sClass][ship.numberOfShip] == 'N' || P.actualDir[sClass][ship.numberOfShip]  == 'W') {
        dir = 1;
    } 
    else dir = -1;

    int x = P.actualX[sClass][ship.numberOfShip];
    int y = P.actualY[sClass][ship.numberOfShip];

    if (P.actualDir[sClass][ship.numberOfShip] == 'N' || P.actualDir[sClass][ship.numberOfShip] == 'S') {
        y += dir;
        distance = ((y1-y) * (y1-y)) + ((x1 - x) * (x1 - x)); // y1, x1 - target ; y,x - source
    }
    if (P.actualDir[sClass][ship.numberOfShip] == 'W' || P.actualDir[sClass][ship.numberOfShip] == 'E') {
        x += dir;
        distance = ((y1-y) * (y1-y)) + ((x1 - x) * (x1 - x)); // y1, x1 - target ; y,x - source
    }

    if (distance <= (ship.lengthOfShips[sClass] * ship.lengthOfShips[sClass])) return true;

    return false;
}

void setShipSegments(int sClass, Player P, Place ship, int x, int y, char **board) {

    char dir = P.actualDir[sClass][ship.numberOfShip];

    int locationCannon;
    int locationEngine;

    if (P.actualDir[sClass][ship.numberOfShip] == 'N' || P.actualDir[sClass][ship.numberOfShip]  == 'W') {
        locationCannon = 1;
        locationEngine = ship.lengthOfShips[sClass]-1; 
    } 
    else {
        locationEngine = -(ship.lengthOfShips[sClass]-1);
        locationCannon = -1;
    }


    if(y == P.actualY[sClass][ship.numberOfShip] && x == P.actualX[sClass][ship.numberOfShip])
        board[y][x] = '@';

    if (dir == 'N' || dir == 'S') {
        if ((y == P.actualY[sClass][ship.numberOfShip] + locationEngine) && x == P.actualX[sClass][ship.numberOfShip])
            board[y][x] = '%';
        else if((y == P.actualY[sClass][ship.numberOfShip] + locationCannon) && x == P.actualX[sClass][ship.numberOfShip])
            board[y][x] = '!';
        
    }
    else if (dir == 'W' || dir == 'E') {
        if ((x == P.actualX[sClass][ship.numberOfShip] + locationEngine) && y == P.actualY[sClass][ship.numberOfShip])
            board[y][x] = '%';

        else if((x == P.actualX[sClass][ship.numberOfShip] + locationCannon) && y == P.actualY[sClass][ship.numberOfShip])
            board[y][x] = '!';
        
    }

}

int checkNumberOfCols(int sizeY) {
    int k = 0;
    for(int i = 0; sizeY > 0; i++ )
    {
        sizeY = sizeY/10;
        k++;
    }
    return k;
}

int checkNumberOfRows(int sizeX) {
    int w = 0;
    for(int i = 0; sizeX > 0; i++ )
    {
        sizeX = sizeX/10;
        w++;
    }
    return w;
}
void setRadar(char **board, char **tempBoard, int sizeY, int sizeX, Player P, Place ship) {


    for(int i=0; i<typesOfShip; i++) {
        for(int j=0; j<maxNumberOfShips; j++) {
            if (j<P.numberOfShips[i] && P.actualShips[i] > 0) { 
            if (board[P.actualY[i][j]][P.actualX[i][j]] == '@') {
            int x = P.actualX[i][j];
            int y = P.actualY[i][j];
            char dir = P.actualDir[i][j];
            isMyShip(x, y, dir, i, ship, board, tempBoard);
            int r = ship.lengthOfShips[i] * ship.lengthOfShips[i];
                for (int k=0; k<sizeY; k++) {
                    for(int l=0; l<sizeX; l++) {
                        if (((y-k) * (y-k)) + ((x-l)*(x-l)) <= r)
                            tempBoard[k][l] = board[k][l];
                    }
                }     
            }
            }
        }
    }
}

void isMyShip(int x, int y, char dir, int sClass, Place ship, char **board, char **tempBoard) {

    int counter = 0;
    int location;
    if (dir == 'N' || dir == 'W') {
        location = 1;
    } 
    else location = -1;

   if (dir == 'N' || dir == 'S') {
        for(int i = 0; i<ship.lengthOfShips[sClass]; i++, y += location) {
            if(board[y][x] == '+' || board[y][x] == '@' || board[y][x] == 'x'
            || board[y][x] == '!' || board[y][x] == '%' ) 
            counter++;
            }
        if (counter == ship.lengthOfShips[sClass]) {
            for(int i = 0; i<ship.lengthOfShips[sClass]; i++) {
                tempBoard[i][x] = board[i][x];
            }
        }
    }
    else  {
        for(int i = 0; i<ship.lengthOfShips[sClass]; i++, x += location) {
            if(board[y][x] == '+' || board[y][x] == '@' || board[y][x] == 'x'
            || board[y][x] == '!' || board[y][x] == '%' ) 
            counter++;
        }
        if (counter == ship.lengthOfShips[sClass] == 0) {
            for(int i = 0; i<ship.lengthOfShips[sClass]; i++) {
                tempBoard[y][i] = board[y][i];
            }
        }
    }

}

void sendSpyPlane(Player &P,Place ship, int y, int x, char **board, char instr[], int sizeY, int sizeX) {

    if (!isCannonDestroyed(P, ship, board, CAR)) {
        if (P.numOfPlanes[ship.numberOfShip] <= 5) {
             P.numOfPlanes[ship.numberOfShip]++;
            for (int i = -1; i < squareSize; i++) {
                for (int j = -1; j < squareSize; j++) {
                if (x+j < 0 || x+j >= sizeX || y+i >=sizeY || y+i < 0) {
                    continue;                
                }
                else P.planesTab[y+i][x+j] = board[y+i][x+j];
                }
            }
        }
        else {
            printf("INVALID OPERATION  \"%s\": ALL PLANES SENT", instr);
            exit(0);           
        }
    }
    else {
            printf("INVALID OPERATION  \"%s\": CANNOT SEND PLANE", instr);
            exit(0);
    }

}