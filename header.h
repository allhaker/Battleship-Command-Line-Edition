#define MAX 21//Max size of battlefielf

typedef char battlefield;
typedef int battleship;
typedef int submarine;
typedef int partrol_boat;


//Function prototypes
void printMainMenu();
void printSettings();
void startNewGame();
void goToSettings();
void goToHelp();
void handover();
int covertToDigit(char x);
void changeShipsNumber();
void printBattlefield(char *prompt, int rowsColumns, battlefield myBattlefield[MAX][MAX]);
void gridInit (int rowsColumns, battlefield myBattlefield[MAX][MAX]);
void putFleet(battlefield myBattlefield[MAX][MAX], int rowsColumns);
void killship(battlefield myMap[MAX][MAX], battlefield enemyMap[MAX][MAX], int rowsColumns, int X, int Y);
void changeLayout();
int putShip(battlefield myBattlefield[MAX][MAX], int x, char Y, char z, int size, int rowsColumns);
int checkScore(battlefield myBattlefield[MAX][MAX], int rowsColumns);
int checkShip(battlefield myBattlefield[MAX][MAX], int X, int y, char z, int size, int rowsColumns);
int playerFires(battlefield myMap[MAX][MAX], battlefield myShooting[MAX][MAX], battlefield enemymap[MAX][MAX], int rowsColumns);
int isShipDead(battlefield myMap[MAX][MAX], int rowsColumns, int X, int Y);