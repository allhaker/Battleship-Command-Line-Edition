#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

//global variables
battleship globalBattleships = 1;
submarine globalSubmarines = 2;
partrol_boat globalPartrol_boats = 3;
int globalrowsColumns = 10;


void main(void) {
    int gameMode;
    system("cls");//cleaning console screen
    printf("Welcome to BATTLESHIP - The CMD Edition!\nPress 'Maximize' for better Experience\n");
    printMainMenu();
    scanf("%d", &gameMode);
    switch (gameMode) //picking up option from main menu user has choosen
    {
        case 0:
            exit(1);
        case 1:
            startNewGame(); //New Game
            break;
        case 2:
            goToSettings(); //Settings
            break;
        case 3:
            goToHelp(); //Help
            break;
    }
}

void goToSettings(){
    int setting;
    char back;
    system("cls");//cleaning console screen
    
    //Settings body
    printf("Welcome to BATTLESHIP - Settings!\n\n");
    printSettings();
    scanf("%d", &setting);
    switch (setting) //picking up option from settings menu user has choosen
    {
        case 0:
            main();
        case 1:
            printf("\nCurrent size of the Battlefield is %d cells\n", globalrowsColumns);
            changeLayout();
            printf("Success! Press Enter to continue\n");
            fflush(stdin);
            scanf("%c", &back);
            goToSettings();
            break;
        case 2:
            printf("\nCurrent size of the fleet:\n\nBattleships (4 cells): %d;\nSumbarine (3 cells): %d;\nPatrol Boat (2 cells): %d;\n", globalBattleships, globalSubmarines, globalPartrol_boats);
            changeShipsNumber(); //change number of ships
            printf("Success! Press Enter to continue\n");
            fflush(stdin);
            scanf("%c", &back);
            goToSettings();
            break;
    }
}

void goToHelp(){
    char back;
    system("cls");
    printf("Welcome to BATTLESHIP - Help!\n\nIn this CMD edition the game is designed for 2 players behind one computer\n\n1) You will have to allocate your fleet on a map:\nBattleship - 4 cells\nSubmarine - 3 cells\nPatrol"
           " boat - 2 cells\n\n2) The game goes before somebody destroyed all opponents ships\n\n3) If player wants to surrender he/she should type 666 into next X firing\ncoordinate\n");
    printf("\nLEGEND\n~ - empty cell\n# - ship\no - missed shot\nX- shot on target/damaged ship\n");
    
    printf("Press Enter to go to main menu.");
    fflush(stdin);
    scanf("%c", &back);
    main();
}

void printSettings(){ //Main menu function
    printf("Change Battlefield Layout (1)\n");
    printf("Change number of ships (2)\n");
    printf("Back to main menu (0)\n");
}

void printMainMenu(){//Settigs menu function
    printf("\nNew Game (1)\n");
    printf("Settings (2)\n");
    printf("Help (3)\n");
    printf("Exit (0)\n");
}


void startNewGame(){
    system("cls");
    
    //setting instance variables
    int rowsColumns = globalrowsColumns, gameOver = 0, winner, hit = 3;
    rowsColumns += 1;//incrementing by one as there is an extra row on the battlefield, which has to be taken into account
    battlefield myMap1[MAX][MAX], shootingRange1[MAX][MAX],	myMap2[MAX][MAX], shootingRange2[MAX][MAX];
    
    //intialization of all 4 grids
    gridInit(rowsColumns, myMap1);
    gridInit(rowsColumns, myMap2);
    gridInit(rowsColumns, shootingRange1);
    gridInit(rowsColumns, shootingRange2);
    
    //-------------------------------Starting the game--------------------------------//
    
    //Printing defaulf battefield and starting filling the fleet Player 1
    printBattlefield("My Battlefield - Player 1\n\n", rowsColumns, myMap1);
    putFleet(myMap1, rowsColumns);
    
    //Computer handover screen to another player
    handover();
    system("cls");
    
    //Printing defaulf battefield and starting filling the fleet Player 2
    printBattlefield("My Battlefield - Player 2\n\n", rowsColumns, myMap2);
    putFleet(myMap2, rowsColumns);
    
    while (gameOver < 1){//main game loop
        handover();
        while (hit != 0)//Player 1 shooting loop
        {
            system("cls");
            printf("\nPlayer 1 turn to fire\n");
            
            //Printing the messages if enemy ship is sank or hit
            if (hit == 1) printf("STATUS UPDATE: Enemy ship HIT!\n");
            else if (hit == 2) printf("STATUS UPDATE: Enemy ship SANK!\n");
            
            hit = playerFires(myMap1, shootingRange1, myMap2, rowsColumns);//Handling the shot of the player itself
            gameOver = checkScore(myMap2, rowsColumns);//checking whether enemy still has alive ships
            
            //breaking loop upon serveral conditions
            if (gameOver == 1) break;//if Player 1 won
            if (hit == 0) break;//if Player 1 missed
            if (hit == 5) {//if player 1 surrendered
                gameOver = 2;
                break;
            }
        }
        
        if (gameOver == 1){//checking the winner if there is
            winner = 1;
            break;
        }
        
        if (gameOver == 2){//checking the winner if there is
            winner = 2;
            break;
        }
        handover();
        
        hit = 3;
        while (hit != 0)//Player 2 shooting loop
        {
            system("cls");
            printf("\nPlayer 2 turn to fire\n");
            
            //Printing the messages if enemy ship is sank or hit
            if (hit == 1) printf("STATUS UPDATE: Enemy ship HIT!\n");
            else if (hit == 2) printf("STATUS UPDATE: Enemy ship SANK!\n");
            
            hit = playerFires(myMap2, shootingRange2, myMap1, rowsColumns);//Handling the shot of the player itself
            gameOver = checkScore(myMap1, rowsColumns);//checking whether enemy still has alive ships
            
            //breaking loop upon serveral conditions
            if (gameOver == 1) break;//if Player 2 won
            if (hit == 0) break;//if Player 2 missed
            if (hit == 5) {//if Player 2 surrendered
                gameOver = 2;
                break;
            }
        }
        
        if (gameOver == 1){//checking the winner if there is
            winner = 2;
            break;
        }
        if (gameOver == 2){//checking the winner if there is
            winner = 1;
            break;
        }
    }
    system("cls");
    
    //Game over screen printing the winner
    printf("Player %d won!!!\n", winner);
    fflush(stdin);
    printf("Press Enter to go to main menu.");
    scanf("%c", &winner);
    main();
}

int playerFires(battlefield myMap[MAX][MAX], battlefield myShooting[MAX][MAX], battlefield enemyMap[MAX][MAX], int rowsColumns){
    //Instance variables
    int X, Y, check = 1;
    char y;
    
    //Printing Game interface
    printBattlefield("My Ships\n", rowsColumns, myMap);
    printBattlefield("My shooting range\n", rowsColumns, myShooting);
    printf("Choose shooting area!\n");
    
    while (check > 0){//checking if shot is successul or not loop
        //Acquiring coordinates
        printf("X axis (Decimals only): ");
        fflush(stdin);
        scanf("%d", &X);
        if (X == 666){//Surrender functionality
            return 5;
        }
        printf("Y axis (Capitals only): ");
        fflush(stdin);
        scanf("%c", &y);
        Y = covertToDigit(y);
        
        if (X <= 0 || X > rowsColumns || Y <= 0 || Y > rowsColumns) printf("You are out of the range! Please, try again.\n");//Checking if coordinated are good
        else {
            check--;
        }
    }
    
    if (enemyMap[X][Y] != '#'){//checking if shot was on target or not
        //printing missed shot on both players maps
        myShooting[X][Y] = 'o';
        enemyMap[X][Y] = 'o';
        return 0;
    }
    else{
        check = isShipDead(enemyMap, rowsColumns, X, Y);//if shot is successful - checking if ship is dead
        if (check == 1){
            //shipIsAlive handling
            myShooting[X][Y] = 'X';
            enemyMap[X][Y] = 'X';
            return check;
        }
        else
        {
            //shipIsDead handling
            myShooting[X][Y] = 'X';
            enemyMap[X][Y] = 'X';
            killship(myShooting, enemyMap, rowsColumns, X, Y);//killing ship
            return check;
        }
    }
}

void gridInit(int rowsColumns, battlefield myBattlefield[MAX][MAX]){//initial filling of battlefield matrix
    int i,j;
    myBattlefield[0][0] = ' ';
    for (i = 1; i < rowsColumns; i++){
        myBattlefield[0][i] = 64 + i;//Capitals Y axis
    }
    for (i = 1; i < rowsColumns; i++){
        myBattlefield[i][0] = i;//Decimals X axis
    }
    for (i = 1; i < rowsColumns; i++) {
        for (j = 1; j < rowsColumns; j++)
            myBattlefield[i][j] = '~';//Empty cells symbol filling
    }
}

void printBattlefield(char *prompt, int rowsColumns, battlefield myBattlefield[MAX][MAX]){
    printf(prompt);//Prompt message
    for (int i = 0; i < rowsColumns; i++) {
        for (int j = 0; j < rowsColumns; j++){
            if (j == 0 && i != 0){//checking battlefield in order to find what to printf char or int
                if (i > 9)	printf("%d ", myBattlefield[i][j]);//if row is more than 10 than print with one less space
                else printf("%d  ", myBattlefield[i][j]);
            }
            else printf("%c  ", myBattlefield[i][j]);
        }
        printf("\n\n");
    }
}

int putShip(battlefield myBattlefield[MAX][MAX], int x, char Y, char z, int size, int rowsColumns){//function, which handles locating the ship onto battlefield
    int y = covertToDigit(Y);//converting Y axis letter to digit
    checkShip(myBattlefield, x, y, z, size, rowsColumns);//checking if ship is suitable for putting at the current location
    printBattlefield("\n", rowsColumns, myBattlefield);
    if (checkShip(myBattlefield, x, y, z, size, rowsColumns) == 1)	{
        if (z == 'v' || z == 'V') for (int i = 0; i < size; i++) myBattlefield[x + i][y] = '#';
        else for (int i = 0; i < size; i++) myBattlefield[x][y + i] = '#';
        return 1;
    }
    else return 0;
}

int checkShip(battlefield myBattlefield[MAX][MAX], int x, int y, char z, int size, int rowsColumns){
    
    if (x <= 0 || x> rowsColumns || y <= 0 || y>rowsColumns) return 0; //checking if ship is within battlefield borders
    //decrementing X and Y in order to enable following check working correctly
    x--;
    y--;
    
    if (z != 'H' && z != 'h' && z != 'V' && z != 'v')  return 0; //Checking if Z coordinate is correct
    else{
        if ((z == 'H' || z == 'h') && ((y +size <= rowsColumns) && (y >= 0))){//checking if there are neighbouring ships within less than one cell from ship potential location (ship is horizontal)
            for (int i = 0; i < size + 2; i++){
                for (int j = 0; j < 3; j++){
                    if (myBattlefield[x + j][y + i] == '#') return 0; //returing error if existing ship is found nearby
                }
            }
            return 1;
        }
        
        if ((z == 'V' || z == 'v') && ((x + size <= rowsColumns) && (x >= 0))){//checking if there are neighbouring ships within less than one cell from ship potential location (ship is vertical)
            for (int i = 0; i < size + 2; i++){
                for (int j = 0; j < 3; j++){
                    if (myBattlefield[x + i][y + j] == '#') return 0;
                }
            }
            return 1;
        }
    }
    return 0;
}

int covertToDigit(char x){//Function which coverts capitals A-J to decimal
    return (x - 64);
}

void putFleet(battlefield myBattlefield[MAX][MAX], int rowsColumns){//initializing players fleet function
    //instance variables
    battleship Battleships = globalBattleships;
    submarine Submarines = globalSubmarines;
    partrol_boat Partrol_boats = globalPartrol_boats;
    
    int i = 1, checkSuccess = 0, x;
    char y, z;
    printf("Please, put your ships at the battlefield!\n");
    while (Battleships>0)//Battleships loop, which works yet there are available battleships
    {
        //Scanning X, Y and Z dimensions (Z - vertical or horizontal)
        printf("\nBattleship %d position X axis (1,2,3...): ", i);
        fflush(stdin);
        scanf("%d:", &x);
        printf("\nY axis (Capitals only): ");
        fflush(stdin);
        scanf("%c:", &y);
        printf("\nPut ship vertically or horizontally(h/v): ");
        fflush(stdin);
        scanf("%c:", &z);
        //Checking if ship is allowed to be put
        checkSuccess = putShip(myBattlefield, x, y, z, 4, rowsColumns);
        system("cls");
        //Checking the result of putShip function
        if (checkSuccess != 1){//Handling insuccessful ship allocation
            printf("The ship cannot be put there! Please, try again.\n");
            printBattlefield("\n", rowsColumns, myBattlefield);
            continue;
        }
        else {//Handling successful ship allocation
            printBattlefield("\n", rowsColumns, myBattlefield);
            Battleships--;
            i++;
        }
    }
    i = 1;
    while (Submarines>0)//Submarines loop, which works yet there are available Submarines
    {
        //Scanning X, Y and Z dimensions (Z - vertical or horizontal)
        printf("\nSubmarines %d position X axis (1,2,3...): ", i);
        fflush(stdin);
        scanf("%d:", &x);
        printf("\nY axis (Capitals only): ");
        fflush(stdin);
        scanf("%c:", &y);
        printf("\nPut ship vertically or horizontally(h/v): ");
        fflush(stdin);
        scanf("%c:", &z);
        //Checking if ship is allowed to be put
        checkSuccess = putShip(myBattlefield, x, y, z, 3, rowsColumns);
        system("cls");
        //Checking the result of putShip function
        if (checkSuccess != 1){//Handling insuccessful ship allocation
            printf("The ship cannot be put there! Please, try again.\n");
            printBattlefield("\n", rowsColumns, myBattlefield);
            continue;
        }
        else {//Handling successful ship allocation
            Submarines--;
            printBattlefield("\n", rowsColumns, myBattlefield);
            i++;
        }
    }
    i = 1;
    while (Partrol_boats>0)//Partrol boats loop, which works yet there are available Partrol boats
    {
        //Scanning X, Y and Z dimensions (Z - vertical or horizontal)
        printf("\nPatrol Boats %d position X axis (1,2,3...): ", i);
        fflush(stdin);
        scanf("%d:", &x);
        printf("\nY axis (Capitals only): ");
        fflush(stdin);
        scanf("%c:", &y);
        printf("\nPut ship vertically or horizontally(h/v): ");
        fflush(stdin);
        scanf("%c:", &z);
        //Checking if ship is allowed to be put
        checkSuccess = putShip(myBattlefield, x, y, z, 2, rowsColumns);
        system("cls");
        //Checking the result of putShip function
        if (checkSuccess != 1){//Handling insuccessful ship allocation
            printf("The ship cannot be put there! Please, try again.\n");
            printBattlefield("\n", rowsColumns, myBattlefield);
            continue;
        }
        else {//Handling successful ship allocation
            Partrol_boats--;
            printBattlefield("\n", rowsColumns, myBattlefield);
            i++;
        }
    }
}

void handover(){//handover function which clears the screen and prints the message
    char enter;
    system("cls");
    fflush(stdin);
    printf("Its other player's turn now!\nPlease, hanodover the device to the next player and then press Enter.\n");
    scanf("%c", &enter);
}

int checkScore(battlefield myBattlefield[MAX][MAX], int rowsColumns){//function which looks for alive ships
    for (int i = 0; i < rowsColumns; i++){
        for (int j = 0; j < rowsColumns; j++){
            if (myBattlefield[i][j] == '#') return 0;
        }
    }
    return 1;
}

int isShipDead(battlefield myMap[MAX][MAX], int rowsColumns, int X, int Y){//function checks all four directions for alive parts of ships
    //instance variables
    int i = 1;
    
    while ((myMap[X + i][Y] != '~') && (myMap[X + i][Y] != 'o') && (X+i < rowsColumns))//checks if there are alive parts of ships in asending X order
    {
        if (myMap[X + i][Y] == '#') return 1;
        i++;
    }
    i = 1;
    while ((myMap[X - i][Y] != '~') && (myMap[X - i][Y] != 'o') && (X + 1 - i > 1))//checks if there are alive parts of ships in descending X order
    {
        if (myMap[X - i][Y] == '#') return 1;
        i++;
    }
    
    i = 1;
    while ((myMap[X][Y + i] != '~') && (myMap[X][Y + i] != 'o') && (Y + i < rowsColumns))//checks if there are alive parts of ships in asending Y order
    {
        if (myMap[X][Y + i] == '#') return 1;
        i++;
    }
    
    i = 1;
    while ((myMap[X][Y - i] != '~') && (myMap[X][Y - i] != 'o') && (Y + 1 - i > 1))//checks if there are alive parts of ships in descending Y order
    {
        if (myMap[X][Y - i] == '#') return 1;
        i++;
    }
    
    return 2;
}

void killship(battlefield myMap[MAX][MAX], battlefield enemyMap[MAX][MAX], int rowsColumns, int X, int Y) {//function killing the ship - drowing 'o' around dead ship on both players maps
    //instance variables
    int i = 1, myIndexX, myIndexY;
    
    //these four loops are looking for ship size and orientation (horizontal or vertical) and replacing surrounding cells with 'o'.
    
    while ((myMap[X + i][Y] != '~') && (myMap[X + i][Y] != 'o') && (X + i < rowsColumns))//checks descending X
    {
        for (int k = -2; k < 2; k++){//these loops are filling the cells with 'o'
            for (int j = -1; j < 2; j++){
                myIndexX = X + k + i;//temp battlefield matrix index storage
                myIndexY = Y + j;
                
                if (myMap[myIndexX][myIndexY] != 'X' && myIndexX > 0 && myIndexX < rowsColumns && myIndexY > 0 && myIndexY < rowsColumns) {//checking if 'o' will not replace axis titles
                    enemyMap[myIndexX][myIndexY] = 'o';
                    myMap[myIndexX][myIndexY] = 'o';
                    
                }
            }
        }
        i++;
    }
    
    i = 1;//resetting i variable
    while ((myMap[X - i][Y] != '~') && (myMap[X - i][Y] != 'o') && (X - i > 1))//checks asending X
    {
        for (int k = -2; k < 2; k++){//these loops are filling the cells with 'o'
            for (int j = -1; j < 2; j++){
                myIndexX = X + k + 1 - i;//temp battlefield matrix index storage
                myIndexY = Y + j;
                
                if (myMap[myIndexX][myIndexY] != 'X' && myIndexX > 0 && myIndexX < rowsColumns && myIndexY > 0 && myIndexY < rowsColumns){
                    myMap[myIndexX][myIndexY] = 'o';
                    enemyMap[myIndexX][myIndexY] = 'o';
                }
            }
        }
        i++;
    }
    
    i = 1;
    while ((myMap[X][Y + i] != '~') && (myMap[X][Y + i] != 'o') && (Y + i < rowsColumns))//checks asending Y
    {
        for (int k = -2; k < 2; k++){//these loops are filling the cells with 'o'
            for (int j = -1; j < 2; j++){
                myIndexX = X + j;//temp battlefield matrix index storage
                myIndexY = Y + k + i;
                
                if (myMap[myIndexX][myIndexY] != 'X' && myIndexX > 0 && myIndexX < rowsColumns && myIndexY > 0 && myIndexY < rowsColumns) {
                    myMap[myIndexX][myIndexY] = 'o';
                    enemyMap[myIndexX][myIndexY] = 'o';
                }
            }		
        }
        i++;
    }
    
    i = 1;
    while ((myMap[X][Y - i] != '~') && (myMap[X][Y - i] != 'o') && (Y - i > 1))//checks descending Y
    {
        for (int k = -2; k < 2; k++){//these loops are filling the cells with 'o'
            for (int j = -1; j < 2; j++){
                myIndexX = X + j;//temp battlefield matrix index storage
                myIndexY = Y + k +1 - i;
                
                if (myMap[myIndexX][myIndexY] != 'X' && myIndexX > 0 && myIndexX < rowsColumns && myIndexY > 0 && myIndexY < rowsColumns){
                    myMap[myIndexX][myIndexY] = 'o';
                    enemyMap[myIndexX][myIndexY] = 'o';
                }
            }
        }
        i++;
    }
}

void changeLayout(){	
    int temp;	
    printf("\bPlease, choose new value (Min 10; Max 20): ");
    fflush(stdin);
    scanf("%d", &temp);
    if (temp >= 10 && temp <= 20) globalrowsColumns = temp;
    else{
        printf("Wroung value - Please enter the number from 10 to 20\n");
        changeLayout();
    }
}

void changeShipsNumber(){	
    int temp1, temp2, temp3;	
    printf("\nNew Battleships number (Not less than 0): ");
    fflush(stdin);
    scanf("%d", &temp1);
    printf("New Submarines number (Not less than 0): ");
    fflush(stdin);
    scanf("%d", &temp2);
    printf("New Patrol boats number (Not less than 0): ");
    fflush(stdin);
    scanf("%d", &temp3);
    if (temp1 >= 0 && temp2 >= 0 && temp3 >= 0){
        globalBattleships = temp1;
        globalSubmarines = temp2;
        globalPartrol_boats = temp3;
    }
    else{
        printf("Wroung value - Please enter the number not less than 0\n");
        changeShipsNumber();
    }
}