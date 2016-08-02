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

#define ROW_LENGTH BOARD_WIDTH*4+3 // | X | O | X |\n\000 

#define BORDER_HOR_CHAR '-'
#define BORDER_VER_CHAR '|'

#define BLANK_SQUARE ' '

#define PROMPT "--> "

#define HELP_CHAR '?'
#define ESCAPE_CHAR 27
#define PIECE_ONE 'X'
#define PIECE_TWO 'O'

void clearBoard( char board[][BOARD_HEIGHT] );
void drawTopBoarder( char rowBuffer[] );
void drawRow( char rowBuffer[], char row[] );
void printBoardToStdout( char board[][BOARD_HEIGHT] );
void printMoveBoardToStdout();
bool askToPlay();
void playGame(char board[][BOARD_HEIGHT]);
void printHelp();
char getPlayersPiece();
bool makeMove(char move, char piece, char *board);
bool makeMove(unsigned int move, char piece, char *board);
unsigned int cpuMove(char board[][BOARD_HEIGHT], char piece, char playerPiece);
bool isGameOver(const char *board);
bool isWinningBoard(const char *board);
unsigned int randRange(unsigned int min, unsigned int max);

int main(int argc, const char * argv[]) {
    char board[BOARD_WIDTH][BOARD_HEIGHT],
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

        if(userResponse == '?'){
            printHelp();
        }else if(userResponse >= '1' && userResponse <= '9'){
            if( makeMove(userResponse, userPiece, *board) ){
                printBoardToStdout(board);

                if(isWinningBoard(*board)) {
                    printf("YOU WIN!");
                    keepPlaying = false;
                    break;
                }else{
                    printf("\n\nCPU's turn...\n");
                    sleep(2);
                    if(!makeMove(cpuMove(board, cpuPiece, userPiece), cpuPiece, *board)){
                        printf("Game Over\n");
                        keepPlaying = false;
                    }else if(isWinningBoard(*board)) {
                        printf("CPU WINS!");
                        keepPlaying = false;
                        break;
                    }
                }
            }else{
                if(isGameOver(*board)){
                    printf("Game Over\n");
                    keepPlaying = false;
                }else{
                    printf("Invalid move! Try again. %c\n\n", userResponse);
                }
            }
        }else if( userResponse == ESCAPE_CHAR ){
            keepPlaying = false;
        }else if( userResponse == 'p' ){
            printBoardToStdout(board);
        }else{
            printf("Unrecognized character: '%c'\n", userResponse);
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

unsigned int cpuMove(char board[][BOARD_HEIGHT], char cpuPiece, char playerPiece){
    char openSpots[BOARD_WIDTH*BOARD_HEIGHT];
    unsigned char openSpotsPtr = 0,
                  blockableSpotsPtr = 0,
                  square = BLANK_SQUARE;

    //Scan horazontal 
    for(unsigned int y=0, userMoveCount=0, cpuMoveCount=0, oneLastMove=BOARD_WIDTH-1; y<BOARD_HEIGHT; y++){
        for(unsigned int x=0; x<BOARD_WIDTH; x++){
            square = board[x][y];

            //Block or Win
            if(userMoveCount == oneLastMove || cpuMoveCount == oneLastMove){
                return y*BOARD_WIDTH + x;
            }

            if(square == BLANK_SQUARE){
                openSpots[openSpotsPtr++] = y*BOARD_WIDTH + x;
            }else if(square == playerPiece){
                userMoveCount++;
            }else if(square == cpuPiece){
                cpuMoveCount++;
            }
        }
        userMoveCount = 0;
        cpuMoveCount = 0;
    }

    //Scan Vertical
    for(unsigned int x=0, userMoveCount=0, cpuMoveCount=0, oneLastMove=BOARD_HEIGHT-1; x<BOARD_WIDTH; x++){
        for(unsigned int y=0; y<BOARD_HEIGHT; y++){
            square = board[x][y];

            //Block or Win
            if(userMoveCount == oneLastMove || cpuMoveCount == oneLastMove){
                return y*BOARD_WIDTH + x;
            }

            if(square == playerPiece){
                userMoveCount++;
            }else if(square == cpuPiece){
                cpuMoveCount++;
            }
        }
        userMoveCount = 0;
        cpuMoveCount = 0;
    }

    //Scan Left and right angle
    for(unsigned int x=0,pos=0, userMoveCount=0, cpuMoveCount=0, userMoveCount2=0, cpuMoveCount2=0, oneLastMove=BOARD_HEIGHT-1; x<BOARD_WIDTH; x++){
        square = board[x][x];

        if(square == playerPiece){
            userMoveCount++;
        }else if(square == cpuPiece){
            cpuMoveCount++;
        }
        //Block or Win
        if(userMoveCount == oneLastMove || cpuMoveCount == oneLastMove){
            return x*BOARD_WIDTH + x;
        }

        pos = (BOARD_WIDTH-1) - x;
        square = board[pos][x];

        if(square == playerPiece){
            userMoveCount2++;
        }else if(square == cpuPiece){
            cpuMoveCount2++;
        }
        //Block or Win
        if(userMoveCount2 == oneLastMove || cpuMoveCount2 == oneLastMove){
            return x*BOARD_WIDTH + pos;
        }
    }
    
    return openSpots[randRange(0,openSpotsPtr)];
}

unsigned int randRange( unsigned int min, unsigned int max ){
    float r = rand()/RAND_MAX;
    float d = max - min;
    return (unsigned int)(d*r) + min;
}

void printHelp(){
    printf("\n\n=========================================\n");
    printf("Enter %c at any time to bring up this help.\n\n", HELP_CHAR);
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

bool isWinningBoard(const char *board){
    return false;
}

bool isGameOver(const char *board){
    for(int i=0, l=BOARD_WIDTH*BOARD_HEIGHT; i<l; i++){
        if( *(board+i) == BLANK_SQUARE ){
            return false;
        }
    }
    return true;
}

void clearBoard( char board[][BOARD_HEIGHT] ){
    std::memset(board, BLANK_SQUARE, BOARD_WIDTH*BOARD_HEIGHT);
}

void drawTopBoarder( char rowBuffer[] ){
    //Draw top border
    std::memset(rowBuffer, BORDER_HOR_CHAR, ROW_LENGTH-2);
    rowBuffer[ROW_LENGTH-2] = '\n';
    rowBuffer[ROW_LENGTH-1] = '\0';
}

void drawRow( char rowBuffer[], char row[] ){
    unsigned int i=0;

    rowBuffer[0] = ' '; //BORDER_VER_CHAR;
    for(i=0; i<BOARD_WIDTH-1; i++){
        rowBuffer[i*4+1] = ' ';
        rowBuffer[i*4+2] = row[i];
        rowBuffer[i*4+3] = ' ';
        rowBuffer[i*4+4] = BORDER_VER_CHAR;
    }
    rowBuffer[i*4+1] = ' ';
    rowBuffer[i*4+2] = row[i];
    rowBuffer[i*4+3] = ' ';
    rowBuffer[i*4+4] = ' ';

    rowBuffer[ROW_LENGTH-2] = '\n';
    rowBuffer[ROW_LENGTH-1] = '\0';
}

void printBoardToStdout( char board[][BOARD_HEIGHT] ){
    char rowBuffer[ROW_LENGTH];
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

void drawMoveRow( char rowBuffer[], unsigned int rowNumber ){
    unsigned int i=0;

    rowBuffer[0] = ' '; //BORDER_VER_CHAR;
    for(i=0; i<BOARD_WIDTH-1; i++){
        rowBuffer[i*4+1] = ' ';
        rowBuffer[i*4+2] = '0' + (rowNumber*BOARD_WIDTH + i+1);
        rowBuffer[i*4+3] = ' ';
        rowBuffer[i*4+4] = BORDER_VER_CHAR;
    }
    rowBuffer[i*4+1] = ' ';
    rowBuffer[i*4+2] = '0' + (rowNumber*BOARD_WIDTH + i+1);
    rowBuffer[i*4+3] = ' ';
    rowBuffer[i*4+4] = ' ';

    rowBuffer[ROW_LENGTH-2] = '\n';
    rowBuffer[ROW_LENGTH-1] = '\0';
}

void printMoveBoardToStdout(){
    char rowBuffer[ROW_LENGTH];
    unsigned int i;

    for(i=0; i<BOARD_HEIGHT-1; i++) {
        drawMoveRow(rowBuffer, i);
        printf("%s", rowBuffer);
        drawTopBoarder(rowBuffer);
        printf("%s", rowBuffer);
    }
    drawMoveRow(rowBuffer, i);
    printf("%s", rowBuffer);
}
