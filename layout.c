/*! \file */
#include <stdio.h>
#include <raylib.h>
#include "layout.h"

/**
 * Draw horizontal and vertical lines of the game board to screen
 */
void layoutDrawBoard(void){
	// vertical lines
	for (size_t i = 0; i <= BLOCKS_ACROSS; i++){
		DrawLine(
				BOARD_LEFT_POSITION + (SQUARE * i), 			// x1
				BOARD_TOP_POSITION,					// y1
				BOARD_LEFT_POSITION + (SQUARE * i), 			// x2
				BOARD_TOP_POSITION + (SQUARE * BLOCKS_DOWN),	// y2
				LIGHTGRAY);
	}

	//horizontal lines
	for (size_t j = 0; j <= BLOCKS_DOWN; j++){
		DrawLine(
				BOARD_LEFT_POSITION, 							// x1
				BOARD_TOP_POSITION + (SQUARE * j),				// y1
				BOARD_LEFT_POSITION + (SQUARE * BLOCKS_ACROSS), // x2
				BOARD_TOP_POSITION + (SQUARE * j), 				// y2
				LIGHTGRAY);
	}
}

/**
 Draw horizontal and vertical lines of the next shape board
 */
void layoutDrawNextShapeBoard(void){

	// Next shape - vertical
	for (size_t i = 0; i <= 4; i++){
		DrawLine(
				NEXT_SHAPE_TOP_LEFT_X + (SQUARE * i),
				BOARD_TOP_POSITION,
				NEXT_SHAPE_TOP_LEFT_X + (SQUARE * i),
				BOARD_TOP_POSITION + (SQUARE * 4),
				LIGHTGRAY);
	}

	// horizontal
	for (size_t i = 0; i <= 4; i++){
		DrawLine(
				NEXT_SHAPE_TOP_LEFT_X,
				BOARD_TOP_POSITION + (SQUARE * i),
				NEXT_SHAPE_TOP_LEFT_X + (SQUARE * 4),
				BOARD_TOP_POSITION + (SQUARE * i),
				LIGHTGRAY);
	}

}

/**
 Draw text for the menu options
 */
void layoutMenu(void){	

	DrawText("1. Start", MENU_POS_X, (MENU_POS_Y), MENU_POS_X, RED);
	DrawText("2. Options", MENU_POS_X, (MENU_POS_Y) * 2, MENU_POS_X, GREEN); 
	DrawText("3. Exit", MENU_POS_X, (MENU_POS_Y) * 3, MENU_POS_X, BLUE); 

}

/**
 Function that returns integer representing menu choice 
 @param[in] keyPress 
 * \retval <keyPress> integer representing menu choice
 */
int layoutNavigation(int keyPress){

	static int keyCount = 1;
	
	switch (keyCount){
		case 1: 
			DrawRectangle(0, MENU_POS_Y, WIDTH, MENU_POS_Y, BLACK);
			break;
		case 2:
			DrawRectangle(0, (MENU_POS_Y) * 2, WIDTH, (MENU_POS_Y) ,BLACK);
			break;
		case 3:
			DrawRectangle(0, (MENU_POS_Y) * 3, WIDTH, (MENU_POS_Y),BLACK);
			break;
	}
	
	if (keyPress == 264){
		keyCount++;
	}

	if (keyPress == 265){
		keyCount--;
	}

	if (keyCount < 1){
		keyCount = 3;
	}

	if (keyCount > 3){
		keyCount = 1;
	}

	return keyCount;
}
