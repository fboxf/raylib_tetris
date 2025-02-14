/*! \file main.c */
#include <stdio.h>
#include <raylib.h>
#include "shape.h"
#include "layout.h"

/*! \enum Allows user input to change gamestate */
typedef enum {
	MENU, /*!< Tetris remains on menu screen */
	START, /*!< Tetris starts */
	EXIT /*!< Tetris exits */
} gamestate;

int main(void){

	// Init
	const int screenWidth = WIDTH; 
	const int screenHeight = HEIGHT;

	InitWindow(screenWidth, screenHeight, "Tetris");

	SetTargetFPS(30);
		
	int framesCounter = 0; 		
	int row;
	static int score = 0;

	gamestate GameState = MENU;			// start on menu
	gameBoard gameBoard_ptr = NULL; 			// create gameboard ADT
	gameBoard_ptr = ShapeInitBoard();			// initialise values

	// main loop
	while (!WindowShouldClose()){

		if(GameState == MENU){
			layoutNavigation(GetKeyPressed());
			if (IsKeyPressed(KEY_ENTER) && layoutNavigation(GetKeyPressed()) == 1){
				GameState = START;	
			} else {
			}
		}

		BeginDrawing();

		if(GameState == MENU){
			ClearBackground(RAYWHITE);
			layoutMenu();				// draw menu
			//DrawText("Tetris Menu", 20,20,20,LIGHTGRAY);

		} else {						// game start
			ClearBackground(RAYWHITE);
			DrawText("Game", 20,20,20,LIGHTGRAY);

			layoutDrawBoard();

			generateNextAndActiveShape(gameBoard_ptr);

			drawNextShape(gameBoard_ptr);
			drawActiveShape(gameBoard_ptr);

			if(!ShapeIsInLeftColumn() && !IsOnSide(gameBoard_ptr)){
				MoveLeft();
			}
			if(!ShapeIsInRightColumn() && !IsOnSide(gameBoard_ptr)){
				MoveRight();
			}

			MoveUp();

			MoveDown(gameBoard_ptr);

			// move pieces down
			if (framesCounter == 5){

				if(!IsAtFloor(gameBoard_ptr) && !IsOnTop(gameBoard_ptr)){
					FallActiveShape(gameBoard_ptr);
				} else {
					putOnBlockedBoard(gameBoard_ptr);
				}

				row = IsRowFull(gameBoard_ptr);
				destroyRow(gameBoard_ptr, row);
				

				framesCounter = 0;
			}

			drawBlockedShapes(gameBoard_ptr);

			drawScore(score);

			framesCounter++;

			clearScreen(gameBoard_ptr); 	// used for redrawing objects

		}
		EndDrawing();

	}
	CloseWindow();
	
	freeBoard(gameBoard_ptr);
	
	return 0;
}
