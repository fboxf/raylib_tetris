/*! shape.h */
#ifndef SHAPE_H
#define SHAPE_H

typedef struct gameBoardType *gameBoard;
gameBoard ShapeInitBoard(void);

int getNumber(void); 
void generateNextAndActiveShape(gameBoard gameBoard_ptr);
void transferShapeDetails(int array[], int array_full[][4], int shape, int rotation);
Color getColorDetails(int shape);

void putActiveShapeOnBoard(gameBoard gameBoard_ptr);

void putOnBlockedBoard(gameBoard gameBoard_ptr);

void drawActiveShape(gameBoard gameBoard_ptr);
void drawBlockedShapes(gameBoard gameBoard_ptr);
void drawNextShape(gameBoard gameBoard_ptr);

void drawScore(int score);

void clearScreen(gameBoard gameBoard_ptr);
void FallActiveShape(gameBoard gameBoard_ptr);

bool ShapeIsInLeftColumn(void);
bool ShapeIsInRightColumn(void);
bool IsAtFloor(gameBoard gameBoard_ptr);
bool IsOnTop(gameBoard gameBoard_ptr);
bool IsOnSide(gameBoard gameBoard_ptr);

int IsRowFull(gameBoard gameBoard_ptr);
bool destroyRow(gameBoard gameBoard_ptr, int row);
static bool FallBlocked(gameBoard gameBoard_ptr, int i);

void MoveLeft(void);
void MoveRight(void);
void MoveUp(void);
bool MoveDown(gameBoard gameBoard_ptr);

void freeBoard(gameBoard gameBoard_ptr);

#endif


