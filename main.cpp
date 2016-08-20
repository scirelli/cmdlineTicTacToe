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
bool isGameOver(const char *board);
bool isWinningBoard(const char *board);
unsigned int randRange(unsigned int min, unsigned int max);

int main(int argc, const char * argv[]) {
    char board[BOARD_WIDTH][BOARD_HEIGHT] = {'X','O', 'X', 'X','O', 'X', 'X','O', 'X'},
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

unsigned int cpuMove(char board[][BOARD_HEIGHT], char cpuPiece, char playerPiece){
    unsigned int openSpots[BOARD_WIDTH*BOARD_HEIGHT],
                 openSpotsPtr = 0,
                 blockableSpotsPtr = 0;
    unsigned char square = BLANK_SQUARE;

    //Scan horazontal 
    for(unsigned int y=0, userMoveCount=0, cpuMoveCount=0, oneLastMove=BOARD_WIDTH-1; y<BOARD_HEIGHT; y++){
        for(unsigned int x=0; x<BOARD_WIDTH; x++){
            square = board[y][x];

            //Block or Win
            if(userMoveCount == oneLastMove || cpuMoveCount == oneLastMove){
                if(x < BOARD_WIDTH-1){
                    return y*BOARD_WIDTH + (x+1);
                }else{
                    return openSpots[openSpotsPtr-1];
                }
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
    for(unsigned int x=0, userMoveCount=0, cpuMoveCount=0, oneLastMove=BOARD_HEIGHT-1, openSpot=0; x<BOARD_WIDTH; x++){
        for(unsigned int y=0; y<BOARD_HEIGHT; y++){
            square = board[y][x];

            //Block or Win
            if(userMoveCount == oneLastMove || cpuMoveCount == oneLastMove){
                if(x < BOARD_HEIGHT-1){
                    return (y+1)*BOARD_WIDTH + x;
                }else{
                    return openSpot;
                }
            }

            if(square == BLANK_SQUARE){
                openSpot = y*BOARD_WIDTH + x;
            }else if(square == playerPiece){
                userMoveCount++;
            }else if(square == cpuPiece){
                cpuMoveCount++;
            }
        }
        userMoveCount = 0;
        cpuMoveCount = 0;
    }

    //Scan Left and right angle
    for(unsigned int x=0,pos=0, userMoveCount=0, cpuMoveCount=0, userMoveCount2=0, cpuMoveCount2=0, oneLastMove=BOARD_HEIGHT-1, openSpot=0, openSpot2=0; x<BOARD_WIDTH; x++){
        //Scan top left to bottom right
        square = board[x][x];
        
        if(square == BLANK_SQUARE){
            openSpot = x*BOARD_WIDTH + x;
        }else if(square == playerPiece){
            userMoveCount++;
        }else if(square == cpuPiece){
            cpuMoveCount++;
        }
        //Block or Win
        if(userMoveCount == oneLastMove || cpuMoveCount == oneLastMove){
            //TODO: figure out how to handle ending open square or open square some where along the way.
            return openSpot;
        }
        
        //Scan top right to bottom left
        pos = (BOARD_WIDTH-1) - x;
        square = board[x][pos];

        if(square == BLANK_SQUARE){
            openSpot2 = x*BOARD_WIDTH + pos;
        }else if(square == playerPiece){
            userMoveCount2++;
        }else if(square == cpuPiece){
            cpuMoveCount2++;
        }
        //Block or Win
        if(userMoveCount2 == oneLastMove || cpuMoveCount2 == oneLastMove){
            return openSpot2;
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

void printBoardToStdout( char board[][BOARD_HEIGHT] ){
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
    char board[BOARD_WIDTH][BOARD_HEIGHT];
    unsigned int i;
    
   for(i=0; i<BOARD_WIDTH*BOARD_HEIGHT; i++){
       ((char *)board)[i] = '0' + (i+1);
   }

   printBoardToStdout(board);
}
