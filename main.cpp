//
//  main.cpp
//  TicTacToe
//
//  Created by Cirelli, Stephen on 7/31/16.
//  Copyright © 2016 Cirelli, Stephen. All rights reserved.
//
//  Currently only supports 3x3 board
//  |  |   
//---------
//  |  |
//---------
//  |  |   


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define BOARD_WIDTH 3
#define BOARD_HEIGHT BOARD_WIDTH
#define NUMBER_OF_SQUARES BOARD_WIDTH*BOARD_HEIGHT

#define ROW_LEFT_PADDING_LENGTH 1
#define ROW_RIGHT_PADDING_LENGTH 0
#define ROW_PIECE_SEPARATOR_LENGTH 3 //' | ' 3 characters
#define TOTAL_ROW_PIECE_SEPARATOR_LENGTH BOARD_WIDTH*ROW_PIECE_SEPARATOR_LENGTH
#define ROW_PRINT_LENGTH BOARD_WIDTH + TOTAL_ROW_PIECE_SEPARATOR_LENGTH + ROW_LEFT_PADDING_LENGTH+ROW_RIGHT_PADDING_LENGTH // X | O | X \n\000 

#define BORDER_HOR_CHAR '-'
#define BORDER_VER_CHAR '|'

#define BOARD_LEFT_PADDING_CHAR ' '
#define BOARD_RIGHT_PADDING_CHAR ' '

#define BLANK_SQUARE ' '

#define PROMPT "--> "

#define HELP_CHAR '?'
#define ESCAPE_CHAR 27
#define PIECE_ONE 'X'
#define PIECE_TWO 'O'

void clearBoard( char board[][BOARD_HEIGHT] );
void drawTopBoarder( char rowBuffer[] );
void drawRow( char rowBuffer[], char row[] );
void drawRowLeftPadding( char rowBuffer[], char row[] );
void printBoardToStdout( char board[][BOARD_HEIGHT] );
void printMoveBoardToStdout();
bool askToPlay();
void playGame(char board[][BOARD_HEIGHT]);
void printHelp();
char getPlayersPiece();
bool makeMove(char move, char piece, char *board);
bool makeMove(unsigned int move, char piece, char *board);
unsigned int cpuMove(char board[][BOARD_HEIGHT], char piece, char playerPiece);
bool isWinningBoard(const char *board);
int randRange(unsigned int min, unsigned int max);
int cpuMoveScanHorazontalRows(char board[][BOARD_HEIGHT], char cpuPiece, char playerPiece, unsigned int openSpots[]);
int cpuMoveScanVerticalRows(char board[][BOARD_HEIGHT], char cpuPiece, char playerPiece);
int cpuMovePickEmptySqure(char *board);
bool isTiedBoard(const char *board);
char checkVertialWin(const char board[][BOARD_HEIGHT]);
char checkHorazontalWin(const char board[][BOARD_HEIGHT]);
char checkDiagnalWin(const char board[][BOARD_WIDTH]);

int main(int argc, const char * argv[]) {
    char board[BOARD_HEIGHT][BOARD_WIDTH] = {' ',' ', ' ', ' ',' ', ' ', ' ',' ', ' '},
         userChar = '\0';
    
    clearBoard(board);
    printBoardToStdout(board);

    srand(time(NULL));

    if(askToPlay()){
        playGame(board);
    }else{
        printf("To bad, maybe next time.\n\n");
    }

    return 0;
}

bool askToPlay(){
    char userChar;

    printf("Want to play a game? y/n\n%s", PROMPT);
    userChar = fgetc(stdin);

    if(userChar != 'y' && userChar != 'Y'){
        return false;
    }

    fgetc(stdin);
    return true;
}

void playGame(char board[][BOARD_HEIGHT]){
    char userResponse = '\0',
         userPiece = PIECE_ONE,
         cpuPiece = PIECE_TWO;
    bool keepPlaying = true;

    userPiece = getPlayersPiece();
    if(userPiece == PIECE_ONE){
        cpuPiece = PIECE_TWO;
    }else{
        cpuPiece = PIECE_ONE;
    }

    printHelp();

    while(keepPlaying){
        printBoardToStdout(board);
        printf("\n\n%s", PROMPT);
        userResponse = fgetc(stdin);
        fgetc(stdin);

        if(userResponse == '?' || userResponse == 'h'){
            printHelp();
        }else if(userResponse >= '1' && userResponse <= '9'){
            if( makeMove(userResponse, userPiece, *board) ){
                printBoardToStdout(board);
                if(isWinningBoard(*board) == userPiece){
                    printf("YOU WIN!\n");
                    printf("Game Over\n");
                    keepPlaying = false;
                    break;
                }else if(isTiedBoard(*board) == true){
                    printf("TIED!\n");
                    printf("Game Over\n");
                    keepPlaying = false;
                    break;
                }
                
                printf("\n\nCPU's turn...\n");
                sleep(1);
                makeMove(cpuMove(board, cpuPiece, userPiece), cpuPiece, *board);
                if(isWinningBoard(*board) == cpuPiece){
                    printf("CPU WINS!\n");
                    printf("Game Over\n");
                    keepPlaying = false;
                    break;
                }else if(isTiedBoard(*board) == true){
                    printf("TIED!\n");
                    printf("Game Over\n");
                    keepPlaying = false;
                    break;
                }

            }else{
                printf("Invalid move! Try again. %c\n\n", userResponse);
            }
        }else if( userResponse == ESCAPE_CHAR || userResponse == 'q' || userResponse == 'Q'){
            keepPlaying = false;
        }else if( userResponse == 'p' || userResponse == 'P' ){
            printBoardToStdout(board);
        }else{
            printf("Unrecognized character: '%c'\n", userResponse);
            printHelp();
        }
    }
}

bool makeMove(char move, char piece, char *board){
    return makeMove((unsigned int)((move - '0')-1), piece, board);
}

//i = (BOARD_WIDTH*y) + x
bool makeMove(unsigned int move, char piece, char *board){
    if( move > 8 ) return false;
    char *pos = board+move;
    
    if(*pos == BLANK_SQUARE){
        *pos = piece;
        return true;
    }

    return false;
}

int cpuMoveScanHorazontalRows(char board[][BOARD_WIDTH], char cpuPiece, char playerPiece){
    unsigned int lastOpenSpot   = 0,
                 userMoveCount  = 0,
                 cpuMoveCount   = 0,
                 oneLastMove    = BOARD_WIDTH-1;
    unsigned char square = BLANK_SQUARE;
    
    //Scan all horazontal rows
    for(unsigned int y=0; y<BOARD_HEIGHT; y++){
        //Scan a row
        for(unsigned int x=0; x<BOARD_WIDTH; x++){
            square = board[y][x];

            if(square == BLANK_SQUARE){
                lastOpenSpot = y*BOARD_WIDTH + x;
            }else if(square == playerPiece){
                userMoveCount++;
            }else if(square == cpuPiece){
                cpuMoveCount++;
            }
        }
        //Block or Win
        if((userMoveCount == oneLastMove && cpuMoveCount == 0) || (cpuMoveCount == oneLastMove && userMoveCount == 0)){
            return lastOpenSpot;
        }

        userMoveCount = 0;
        cpuMoveCount = 0;
    }

    return -1;
}

int cpuMoveScanVerticalRows(char board[][BOARD_WIDTH], char cpuPiece, char playerPiece){
    unsigned int lastOpenSpot   = 0,
                 userMoveCount  = 0,
                 cpuMoveCount   = 0,
                 oneLastMove    = BOARD_HEIGHT-1;
    unsigned char square = BLANK_SQUARE;
    
    //Scan Vertical
    for(unsigned int x=0; x<BOARD_WIDTH; x++){
        for(unsigned int y=0; y<BOARD_HEIGHT; y++){
            square = board[y][x];

            if(square == BLANK_SQUARE){
                lastOpenSpot = y*BOARD_WIDTH + x;
            }else if(square == playerPiece){
                userMoveCount++;
            }else if(square == cpuPiece){
                cpuMoveCount++;
            }
        }
        //Block or Win
        if((userMoveCount == oneLastMove && cpuMoveCount == 0) || (cpuMoveCount == oneLastMove && userMoveCount == 0)){
            return lastOpenSpot;
        }

        userMoveCount = 0;
        cpuMoveCount = 0;
    }

    return -1;
}

int cpuMoveScanDiagnalRows(char board[][BOARD_WIDTH], char cpuPiece, char playerPiece){
    unsigned int userMoveCount  = 0,
                 cpuMoveCount   = 0,
                 userMoveCount2 = 0,
                 cpuMoveCount2  = 0,
                 lastOpenSpot   = 0,
                 lastOpenSpot2  = 0,
                 oneLastMove    = oneLastMove = BOARD_HEIGHT-1;
    unsigned char square = BLANK_SQUARE;

    //Scan Left and right angle
    for(unsigned int x=0, pos=0; x<BOARD_WIDTH; x++){
        //Scan top left to bottom right
        square = board[x][x];
        
        if(square == BLANK_SQUARE){
            lastOpenSpot = x*BOARD_WIDTH + x;
        }else if(square == playerPiece){
            userMoveCount++;
        }else if(square == cpuPiece){
            cpuMoveCount++;
        }
        
        //Scan top right to bottom left
        pos = (BOARD_WIDTH-1) - x;
        square = board[x][pos];

        if(square == BLANK_SQUARE){
            lastOpenSpot2 = x*BOARD_WIDTH + pos;
        }else if(square == playerPiece){
            userMoveCount2++;
        }else if(square == cpuPiece){
            cpuMoveCount2++;
        }
    }
    //Block or Win top left to bottome right
    if((userMoveCount == oneLastMove && cpuMoveCount == 0) || (cpuMoveCount == oneLastMove && userMoveCount == 0)){
        return lastOpenSpot;
    }
    //Block or Win top right to bottom left
    if((userMoveCount2 == oneLastMove && cpuMoveCount2 == 0) || (cpuMoveCount2 == oneLastMove && userMoveCount2 == 0)){
        return lastOpenSpot2;
    }

    return -1;
}

int cpuMovePickEmptySqure(char *board){
    unsigned int openSpots[BOARD_WIDTH*BOARD_HEIGHT],
                 openSpotsPtr = 0;
    unsigned char square = BLANK_SQUARE;

    for(unsigned int i=0, length=BOARD_HEIGHT*BOARD_WIDTH; i<length; i++){
        square = board[i];
        if(square == BLANK_SQUARE){
            openSpots[openSpotsPtr++] = i;
        }
    }
    
    if(openSpotsPtr == 0){
        return 0;
    }

    openSpotsPtr = (unsigned int)randRange(0,openSpotsPtr);
    return openSpots[openSpotsPtr];
}

unsigned int cpuMove(char board[][BOARD_WIDTH], char cpuPiece, char playerPiece){
    unsigned int openSpots[BOARD_WIDTH*BOARD_HEIGHT];
    int move = -1;
    unsigned char square = BLANK_SQUARE;

    move = cpuMoveScanHorazontalRows(board, cpuPiece, playerPiece);
    if(move >= 0){
        return (unsigned int)move;
    }

    move = cpuMoveScanVerticalRows(board, cpuPiece, playerPiece);
    if(move >= 0){
        return (unsigned int)move;
    }

    move = cpuMoveScanDiagnalRows(board, cpuPiece, playerPiece);
    if(move >= 0){
        return (unsigned int)move;
    }
    
    move = cpuMovePickEmptySqure(*board);

    return (unsigned int)move;
}

int randRange( unsigned int min, unsigned int max ){
    float r = (float)rand()/(float)RAND_MAX;
    float d = (float)((max+1) - min);
    return (int)(d*r) + min;
}

void printHelp(){
    printf("\n\n=========================================\n");
    printf("Enter %c at any time to bring up this help.\n", HELP_CHAR);
    printf("Type q to quit.\n\n");
    printf("Enter a corresponding number for the square,\nwhere you want to place your piece\n\n");
    printMoveBoardToStdout();
    printf("\n\n");
}

char getPlayersPiece(){
    char userPiece = PIECE_ONE;

    printf("Which piece would you like to be? X/O\n%s", PROMPT);
    userPiece = fgetc(stdin);
    
    switch(userPiece){
        case 'o':
        case 'O':
        case 'x':
        case 'X':
            break;
        default:
            userPiece = PIECE_ONE;
    }
    printf("\n\nYou chose: %c", userPiece);

    fgetc(stdin);
    return userPiece;
}

char checkHorazontalWin(const char board[][BOARD_WIDTH]){
    unsigned char firstSquare = BLANK_SQUARE,
                  nextSquare = BLANK_SQUARE;
    bool winingRow = true;
    
    //Scan all horazontal rows
    for(unsigned int y=0; y<BOARD_HEIGHT; y++){
        //Scan a row
        firstSquare = board[y][0];
        for(unsigned int x=1; x<BOARD_WIDTH; x++){
            nextSquare = board[y][x];
            if(nextSquare == BLANK_SQUARE || nextSquare != firstSquare) {
                winingRow = false;
                break;
            }
        }
        if(winingRow == true){
            return firstSquare;
        }
        winingRow = true;
    }

    return BLANK_SQUARE;
}

char checkVertialWin(const char board[][BOARD_WIDTH]){
    unsigned char firstSquare = BLANK_SQUARE,
                  nextSquare = BLANK_SQUARE;
    bool winingRow = true;
    
    //Scan all horazontal rows
    for(unsigned int x=0; x<BOARD_WIDTH; x++){
        //Scan a row
        firstSquare = board[0][x];
        for(unsigned int y=1; y<BOARD_HEIGHT; y++){
            nextSquare = board[y][x];
            if(nextSquare == BLANK_SQUARE || nextSquare != firstSquare) {
                winingRow = false;
                break;
            }
        }
        if(winingRow == true){
            return firstSquare;
        }
        winingRow = true;
    }

    return BLANK_SQUARE;
}

char checkDiagnalWin(const char board[][BOARD_WIDTH]){
    unsigned char firstSquare = BLANK_SQUARE,
                  nextSquare = BLANK_SQUARE;
    bool winingRow = true;
    
    //Scan all horazontal rows
    for(unsigned int x=0; x<BOARD_WIDTH; x++){
        //Scan a row
        firstSquare = board[0][x];
        for(unsigned int y=1; y<BOARD_HEIGHT; y++){
            nextSquare = board[y][x];
            if(nextSquare == BLANK_SQUARE || nextSquare != firstSquare) {
                winingRow = false;
                break;
            }
        }
        if(winingRow == true){
            return firstSquare;
        }
        winingRow = true;
    }

    return BLANK_SQUARE;
}

char isWinningBoard(const char board[][BOARD_WIDTH]){
    checkHorazontalWin(board);
    checkVertialWin(board);
    checkDiagnalWin(board);

    return false;
}

bool isTiedBoard(const char *board){
    unsigned char square = BLANK_SQUARE;

    for(unsigned int i=0, length=BOARD_HEIGHT*BOARD_WIDTH; i<length; i++){
        square = board[i];
        if(square == BLANK_SQUARE){
            return false;
        }
    }
    
    return true;
}

void clearBoard( char board[][BOARD_WIDTH] ){
    std::memset(board, BLANK_SQUARE, BOARD_WIDTH*BOARD_HEIGHT);
}

void drawTopBoarder( char rowBuffer[] ){
    //Draw top border
    std::memset(rowBuffer, BORDER_HOR_CHAR, ROW_PRINT_LENGTH-2);
    rowBuffer[ROW_PRINT_LENGTH-2] = '\n';
    rowBuffer[ROW_PRINT_LENGTH-1] = '\0';
}

void drawRowLeftPadding( char rowBuffer[], char row[] ){
    int i=0;

    for(i=0; i<BOARD_WIDTH && i<ROW_LEFT_PADDING_LENGTH; i++){
        rowBuffer[i] = BOARD_LEFT_PADDING_CHAR;
    }
}

void drawRow( char rowBuffer[], char row[] ){
    unsigned int i = 0,
        offset = ROW_LEFT_PADDING_LENGTH;

    drawRowLeftPadding(rowBuffer, row);

    // X | O | X \n\0
    for(i=0; i<BOARD_WIDTH-1 && (offset+ROW_PIECE_SEPARATOR_LENGTH)<ROW_PRINT_LENGTH; i++){
        rowBuffer[offset++] = row[i];
        rowBuffer[offset++] = ' ';
        rowBuffer[offset++] = BORDER_VER_CHAR;
        rowBuffer[offset++] = ' ';
    }
    if(offset+ROW_PIECE_SEPARATOR_LENGTH < ROW_PRINT_LENGTH){
        rowBuffer[offset++] = row[i];
        rowBuffer[offset++] = ' ';
        rowBuffer[offset++] = '\n';
        rowBuffer[offset] = '\0';
    }
}

void printBoardToStdout( char board[][BOARD_WIDTH] ){
    char rowBuffer[ROW_PRINT_LENGTH];
    int i=0;

    for(i=0; i<BOARD_HEIGHT-1; i++) {
        drawRow(rowBuffer, board[i]);
        printf("%s", rowBuffer);
        drawTopBoarder(rowBuffer);
        printf("%s", rowBuffer);
    }
    drawRow(rowBuffer, board[i]);
    printf("%s", rowBuffer);
}

void printMoveBoardToStdout(){
    char board[BOARD_HEIGHT][BOARD_WIDTH];
    unsigned int i;
    
   for(i=0; i<BOARD_WIDTH*BOARD_HEIGHT; i++){
       ((char *)board)[i] = '0' + (i+1);
   }

   printBoardToStdout(board);
}
