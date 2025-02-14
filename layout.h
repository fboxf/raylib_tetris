/*! \file layout.h */
#ifndef LAYOUT_H
#define LAYOUT_H
/**
screen width
 */
#define WIDTH 600 

/**
 screen height*/
#define HEIGHT 600 

/**
 Square size in pixels*/
#define SQUARE 20

/**
 Left side of game board as a ratio of width (starting from 
 left side of screen
 */
#define BOARD_LEFT_POSITION 	WIDTH * 0.5

/**
 Top position of board as ratio of height (starting from 
 top of the screen)
 */
#define BOARD_TOP_POSITION		HEIGHT * 0.1

/**
 How many columns is our gameboard?
 */
#define BLOCKS_ACROSS	10	

/** How many rows is our gameboard 
 */
#define BLOCKS_DOWN		20

/**
 Where is our menu text position (x)?
 */
#define MENU_POS_X WIDTH/20 

/** 
 Where is out menu text position (y)?*/
#define MENU_POS_Y HEIGHT/12

/**
 Text size*/
#define MENU_TXT_SIZE WIDTH/20

/**
 Next shape details. The top left corner of our imaginary
 next shape board (hidden)
 */
#define NEXT_SHAPE_TOP_LEFT_X	BOARD_LEFT_POSITION - (8 * SQUARE)

/**
 Macro returns how many squares across from next shape start point
 as an integer. Used for placement of each square (x)
 */
#define NEXT_SHAPE_X_CALC(x) ((NEXT_SHAPE_TOP_LEFT_X) + (x * SQUARE))

/**
 Macro returns how many squares down from next shape start point
 as an integer. Used for placement of each square (y)
 */
#define NEXT_SHAPE_Y_CALC(x) ((BOARD_TOP_POSITION) + (x * SQUARE) )

void layoutDrawBoard(void);
//void layoutDrawNextShapeBoard(void);
void layoutMenu(void);
int layoutNavigation(int keyPress);

#endif
