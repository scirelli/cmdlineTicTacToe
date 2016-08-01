//
//  main.cpp
//  TicTacToe
//
//  Created by Cirelli, Stephen on 7/31/16.
//  Copyright © 2016 Cirelli, Stephen. All rights reserved.
//
//  Currently only supports 3x3 board

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BOARD_WIDTH 3
#define BOARD_HEIGHT 3

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
bool makeMove(int move, char piece, char *board);
int cpuMove(char board[][BOARD_HEIGHT]);

int main(int argc, const char * argv[]) {
    char board[BOARD_WIDTH][BOARD_HEIGHT],
         userChar = '\0';
    
    clearBoard(board);
    printBoardToStdout(board);
    
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
                printf("\n\nCPU's turn...\n");
                sleep(2);
                if(!makeMove(cpuMove(board), cpuPiece, *board)){
                    printf("Game Over");
                    keepPlaying = false;
                }
            }else{
                printf("Invalid move! Try again. %c\n\n", userResponse);
            }
        }else if( userResponse == ESCAPE_CHAR ){
            keepPlaying = false;
        }else if( userResponse == 'p' ){
            printBoardToStdout(board);
        }else{
            printf("Unrecognized character: '%c'", userResponse);
        }
    }
}

bool makeMove(char move, char piece, char *board){
    return makeMove((move - '0')-1, piece, board);
}

//i = (BOARD_WIDTH*y) + x
bool makeMove(int move, char piece, char *board){
    if( move > 8 || move < 0 ) return false;
    char *pos = board+move;
    
    if(*pos == BLANK_SQUARE){
        *pos = piece;
        return true;
    }

    return false;
}

int cpuMove(char board[][BOARD_HEIGHT]){
    return 1;
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
    printf("\n\nYour chose: %c", userPiece);

    fgetc(stdin);
    return userPiece;
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
    int i=0;

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

void drawMoveRow( char rowBuffer[], int rowNumber ){
    int i=0;

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
    int i;

    for(i=0; i<BOARD_HEIGHT-1; i++) {
        drawMoveRow(rowBuffer, i);
        printf("%s", rowBuffer);
        drawTopBoarder(rowBuffer);
        printf("%s", rowBuffer);
    }
    drawMoveRow(rowBuffer, i);
    printf("%s", rowBuffer);
}
