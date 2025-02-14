/*! \file shape.c */
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "time.h"
#include "shape.h"
#include "layout.h"

#define DEBUG

/** ADT of a number of game boards  */
typedef struct gameBoardType {
	int shape_board[BLOCKS_DOWN][BLOCKS_ACROSS]; /**< Contains the active shape */ 
	int blocked_board[BLOCKS_DOWN][BLOCKS_ACROSS]; /**< Contains occupied squares */
	int temp_board[BLOCKS_DOWN][BLOCKS_ACROSS]; /**< Used to copy the entire board temporarily */
	bool hasShapeBeingChosen; /**< Decides whether a new shape is required */
	bool GameOver; /**< Is game finished? */
} gameBoardType;

typedef struct shape_details{
	int shape_number;
	int full_details[4][4];
	int coordinates[4];
	int rotation;
	Color colour;
} shape_details;

typedef struct shape{
	shape_details current;
	shape_details next;

	Vector2 next_board_shape[4];
	
	int fall_counter;
	int *ptrToArray; //ptr to shape_board used for placement of active shape 
} shape;

shape Shape;

/**
 * Allocate space for a pointer to type gameBoard and initialise
 * some values.
 * \retval <gameBoard_ptr> Returns a pointer of type gameBoard */
gameBoard ShapeInitBoard(void){
	gameBoard gameBoard_ptr = malloc(sizeof(struct gameBoardType));

	if (gameBoard_ptr == NULL){
		EXIT_FAILURE;
	} else {

		for (size_t i = 0; i < BLOCKS_DOWN; i++){
			for (size_t j = 0; j < BLOCKS_ACROSS; j++){
				gameBoard_ptr->shape_board[i][j] = 0;
				gameBoard_ptr->blocked_board[i][j] = 0;
				gameBoard_ptr->temp_board[i][j] = 0;
			}
		}

		// first next shape.. random after
		Shape.next.shape_number = 0;
		Shape.next.rotation = 0;
		Shape.next.colour = getColorDetails(Shape.next.shape_number);

		gameBoard_ptr->hasShapeBeingChosen = false;		
	}
	return gameBoard_ptr;
}

/**
 * \returns Number used in the generation of next random shape
 * */
int getNumber(void){
	return rand() % 7;//need to change when working
}

/**
 * This function allocates details to the global variable Shape a variable 
 * of type shape_details
 * @param[in] gameBoard Check if a shape has been chosen*/
void generateNextAndActiveShape(gameBoard gameBoard_ptr){
	srand(time(NULL));	

	// if shape at bottom or new game
	if (gameBoard_ptr->hasShapeBeingChosen == false){

		Shape.fall_counter = 0; 	// apply fall counter to all shapes 

		//current shape - always copied from next
		Shape.current.shape_number = Shape.next.shape_number;	
		Shape.current.rotation = Shape.next.rotation;

		transferShapeDetails(
				Shape.current.coordinates, 
				Shape.current.full_details,
				Shape.current.shape_number,
				Shape.current.rotation);
		Shape.current.colour = Shape.next.colour;

		//debug
		//printf("%d\n",Shape.current.shape_number);

		//next shape - next always randomly generated apart from first one
		Shape.next.shape_number = getNumber();	
		//Shape.next.rotation = rand() % 4;
		Shape.next.rotation = 0;

		transferShapeDetails(
				Shape.next.coordinates, 
				Shape.next.full_details,
				Shape.next.shape_number,
				Shape.next.rotation);

		Shape.next.colour = getColorDetails(Shape.next.shape_number);
		
		//debug
		//printf("%d\n",Shape.next.shape_number);

	}

	putActiveShapeOnBoard(gameBoard_ptr);

	gameBoard_ptr->hasShapeBeingChosen = true;
}

/**
 * Transfer shape details (coordinates etc) to supplied arguments
 * @param[out] array Where are we sending the current shape array data?
 * @param[out] array_full Where are we sending the full shape data (each rotation)?
 * @param[out] shape The shape that is in use
 * @param[out] rotation Used to iterate onto next rotation when Up arrow is pressed 
 * */
void transferShapeDetails(int array[], int array_full[][4], int shape, int rotation){

	//starting from [0][4] these are the amounts that ptr should traverse
	int shapeO[4][4] = {{4,5,14,15},{4,5,14,15},{4,5,14,15},{4,5,14,15}};
	int shapeI[4][4] = {{6,16,26,36},{24,25,26,27},{6,16,26,36},{6,16,26,36}};
	int shapeJ[4][4] = {{4,5,6,16},{5,15,25,24},{14,24,25,26},{15,16,25,35}};
	int shapeL[4][4] = {{5,15,25,26},{14,15,16,24}, {4,5,15,25},{6,14,15,16}};
	int shapeZ[4][4] = {{15,16,26,27},{6,16,15,25},{15,16,26,27},{6,16,15,25}};
	int shapeT[4][4] = {{14,15,16,25},{5,14,15,25},{5,14,15,16},{5,15,16,25}};
	int shapeS[4][4] = {{14,15,24,23},{14,24,25,35},{14,15,24,23},{14,24,25,35}};

	switch(shape){
		case 0:
			for (size_t i = 0; i < 4; i++){
				array[i] = shapeO[rotation][i] + Shape.fall_counter; 
			}
			for (size_t i = 0; i < 4; i++){
				for (size_t j = 0; j < 4; j++){
					array_full[i][j] = shapeO[i][j];
				}
			}
			Shape.next_board_shape[0].x = NEXT_SHAPE_X_CALC(1);
			Shape.next_board_shape[0].y = NEXT_SHAPE_Y_CALC(1);
			Shape.next_board_shape[1].x = NEXT_SHAPE_X_CALC(2);
			Shape.next_board_shape[1].y = NEXT_SHAPE_Y_CALC(1);
			Shape.next_board_shape[2].x = NEXT_SHAPE_X_CALC(1);
			Shape.next_board_shape[2].y = NEXT_SHAPE_Y_CALC(2);
			Shape.next_board_shape[3].x = NEXT_SHAPE_X_CALC(2);
			Shape.next_board_shape[3].y = NEXT_SHAPE_Y_CALC(2);
			break;
		case 1:
			for (size_t i = 0; i < 4; i++){
				array[i] = shapeI[rotation][i] + Shape.fall_counter;
			}
			for (size_t i = 0; i < 4; i++){
				for (size_t j = 0; j < 4; j++){
					array_full[i][j] = shapeI[i][j];
				}
			}
			Shape.next_board_shape[0].x = NEXT_SHAPE_X_CALC(2);
			Shape.next_board_shape[0].y = NEXT_SHAPE_Y_CALC(0);
			Shape.next_board_shape[1].x = NEXT_SHAPE_X_CALC(2);
			Shape.next_board_shape[1].y = NEXT_SHAPE_Y_CALC(1);
			Shape.next_board_shape[2].x = NEXT_SHAPE_X_CALC(2);
			Shape.next_board_shape[2].y = NEXT_SHAPE_Y_CALC(2);
			Shape.next_board_shape[3].x = NEXT_SHAPE_X_CALC(2);
			Shape.next_board_shape[3].y = NEXT_SHAPE_Y_CALC(3);
			break;
		case 2:
			for (size_t i = 0; i < 4; i++){
				array[i] = shapeJ[rotation][i] + Shape.fall_counter;
			}
			for (size_t i = 0; i < 4; i++){
				for (size_t j = 0; j < 4; j++){
					array_full[i][j] = shapeJ[i][j];
				}
			}
			Shape.next_board_shape[0].x = NEXT_SHAPE_X_CALC(1);
			Shape.next_board_shape[0].y = NEXT_SHAPE_Y_CALC(1);
			Shape.next_board_shape[1].x = NEXT_SHAPE_X_CALC(2);
			Shape.next_board_shape[1].y = NEXT_SHAPE_Y_CALC(1);
			Shape.next_board_shape[2].x = NEXT_SHAPE_X_CALC(3);
			Shape.next_board_shape[2].y = NEXT_SHAPE_Y_CALC(1);
			Shape.next_board_shape[3].x = NEXT_SHAPE_X_CALC(3);
			Shape.next_board_shape[3].y = NEXT_SHAPE_Y_CALC(2);
			break;
		case 3:
			for (size_t i = 0; i < 4; i++){
				array[i] = shapeL[rotation][i] + Shape.fall_counter;
			}
			for (size_t i = 0; i < 4; i++){
				for (size_t j = 0; j < 4; j++){
					array_full[i][j] = shapeL[i][j];
				}
			}
			Shape.next_board_shape[0].x = NEXT_SHAPE_X_CALC(1);
			Shape.next_board_shape[0].y = NEXT_SHAPE_Y_CALC(1);
			Shape.next_board_shape[1].x = NEXT_SHAPE_X_CALC(1);
			Shape.next_board_shape[1].y = NEXT_SHAPE_Y_CALC(2);
			Shape.next_board_shape[2].x = NEXT_SHAPE_X_CALC(1);
			Shape.next_board_shape[2].y = NEXT_SHAPE_Y_CALC(3);
			Shape.next_board_shape[3].x = NEXT_SHAPE_X_CALC(2);
			Shape.next_board_shape[3].y = NEXT_SHAPE_Y_CALC(3);
			break;
		case 4:
			for (size_t i = 0; i < 4; i++){
				array[i] = shapeZ[rotation][i] + Shape.fall_counter;
			}
			for (size_t i = 0; i < 4; i++){
				for (size_t j = 0; j < 4; j++){
					array_full[i][j] = shapeZ[i][j];
				}
			}
			Shape.next_board_shape[0].x = NEXT_SHAPE_X_CALC(0);
			Shape.next_board_shape[0].y = NEXT_SHAPE_Y_CALC(1);
			Shape.next_board_shape[1].x = NEXT_SHAPE_X_CALC(1);
			Shape.next_board_shape[1].y = NEXT_SHAPE_Y_CALC(1);
			Shape.next_board_shape[2].x = NEXT_SHAPE_X_CALC(1);
			Shape.next_board_shape[2].y = NEXT_SHAPE_Y_CALC(2);
			Shape.next_board_shape[3].x = NEXT_SHAPE_X_CALC(2);
			Shape.next_board_shape[3].y = NEXT_SHAPE_Y_CALC(2);
			break;
		case 5:
			for (size_t i = 0; i < 4; i++){
				array[i] = shapeT[rotation][i] + Shape.fall_counter;
			}
			for (size_t i = 0; i < 4; i++){
				for (size_t j = 0; j < 4; j++){
					array_full[i][j] = shapeT[i][j];
				}
			}
			Shape.next_board_shape[0].x = NEXT_SHAPE_X_CALC(1);
			Shape.next_board_shape[0].y = NEXT_SHAPE_Y_CALC(1);
			Shape.next_board_shape[1].x = NEXT_SHAPE_X_CALC(2);
			Shape.next_board_shape[1].y = NEXT_SHAPE_Y_CALC(1);
			Shape.next_board_shape[2].x = NEXT_SHAPE_X_CALC(3);
			Shape.next_board_shape[2].y = NEXT_SHAPE_Y_CALC(1);
			Shape.next_board_shape[3].x = NEXT_SHAPE_X_CALC(2);
			Shape.next_board_shape[3].y = NEXT_SHAPE_Y_CALC(2);
			break;
		case 6:
			for (size_t i = 0; i < 4; i++){
				array[i] = shapeS[rotation][i] + Shape.fall_counter;
			}
			for (size_t i = 0; i < 4; i++){
				for (size_t j = 0; j < 4; j++){
					array_full[i][j] = shapeS[i][j];
				}
			}
			Shape.next_board_shape[0].x = NEXT_SHAPE_X_CALC(2);
			Shape.next_board_shape[0].y = NEXT_SHAPE_Y_CALC(1);
			Shape.next_board_shape[1].x = NEXT_SHAPE_X_CALC(3);
			Shape.next_board_shape[1].y = NEXT_SHAPE_Y_CALC(1);
			Shape.next_board_shape[2].x = NEXT_SHAPE_X_CALC(2);
			Shape.next_board_shape[2].y = NEXT_SHAPE_Y_CALC(2);
			Shape.next_board_shape[3].x = NEXT_SHAPE_X_CALC(1);
			Shape.next_board_shape[3].y = NEXT_SHAPE_Y_CALC(2);
			break;
	}
}

/**
 * Select the appropriate colour for the next shape
 * @param[in] shape An empty value that requires populating
 * \retval <list[shape]> Returns colour value
 * */
Color getColorDetails(int shape){
	Color list[7] = {ORANGE, GREEN, BLUE, DARKBLUE, YELLOW, VIOLET, RED}; 
	return list[shape];
}

/**
 *Function that moves pointer a specified distance four times using 
 Shape.current.coordinates to place a '1' on the shape_board to signal a
 moving block
 @param[out] gameBoard_ptr Accesses four elements of the member shape_board
 */
void putActiveShapeOnBoard(gameBoard gameBoard_ptr){
	Shape.ptrToArray = gameBoard_ptr->shape_board[0];

	for (size_t i = 0; i < 4; i++){
		*(Shape.ptrToArray + Shape.current.coordinates[i]) = 1;
	
	}
}

/**
 Function that moves a pointer a specified distance four times using 
 Shape.current.coordinates to place a '1' on the blocked_board to signal an occupied
 space. Takes moving shape off the board and converts it to a blocked shape. 
 Signals that a current shape is no longer in existence. 
 @param[out] gameBoard_ptr Accesses four elements of the member blocked_board
 */
void putOnBlockedBoard(gameBoard gameBoard_ptr){

	Shape.ptrToArray = gameBoard_ptr->blocked_board[0];

	for (size_t i = 0; i < 4; i++){
		*(Shape.ptrToArray + Shape.current.coordinates[i]) = 1;
	
	}

	gameBoard_ptr->hasShapeBeingChosen = false;	
}

/**
 Clear shape from current frame and reset array values for next placement. 
 */
void clearScreen(gameBoard gameBoard_ptr){
	for (size_t i = 0; i < BLOCKS_DOWN; i++){
		for (size_t j = 0; j < BLOCKS_ACROSS; j ++){
			gameBoard_ptr->shape_board[i][j] = 0;
		}
	}
}

/** 
 * Draw active shape onto raylib coordinates using the function
 * DrawRectangle
 * */
void drawActiveShape(gameBoard gameBoard_ptr){
	for (size_t i = 0; i < BLOCKS_DOWN; i++){
		for (size_t j = 0; j < BLOCKS_ACROSS; j ++){
			if (gameBoard_ptr->shape_board[i][j] == 1){
				DrawRectangle(
					BOARD_LEFT_POSITION + (SQUARE * j), BOARD_TOP_POSITION + (SQUARE * i),
					SQUARE, SQUARE,
					Shape.current.colour);
			}
		}
	}
}

/**
 Draw blocked squares to screen
 @param[in] gameBoard_ptr Used for obtaining blocked values from 2D array.
 */
void drawBlockedShapes(gameBoard gameBoard_ptr){
	for (size_t i = 0; i < BLOCKS_DOWN; i++){
		for (size_t j = 0; j < BLOCKS_ACROSS; j ++){

			if (gameBoard_ptr->blocked_board[i][j] == 1){
				DrawRectangle(
					BOARD_LEFT_POSITION + (SQUARE * j), BOARD_TOP_POSITION + (SQUARE * i),
					SQUARE, SQUARE,
					GRAY);
			}
		}
	}
}
/**
 Draws next shape to the left of game board
 @param[in] gameBoard_ptr
 */
void drawNextShape(gameBoard gameBoard_ptr){
	for (size_t i = 0; i < 4; i++){
		DrawRectangle(
				Shape.next_board_shape[i].x,
				Shape.next_board_shape[i].y,
				SQUARE, SQUARE, 
				Shape.next.colour);
	}

	DrawText("Next Shape", 
			NEXT_SHAPE_TOP_LEFT_X - (6 * SQUARE), 
			BOARD_TOP_POSITION + SQUARE, 
			20, LIGHTGRAY);
}

/** 
 * Draw score to screen
 * */
void drawScore(int score){
	DrawText(TextFormat("Score: %d", score), SQUARE, 200, SQUARE, LIGHTGRAY);
}

/**
 Allows shape to move by moving to the next row down
 */
void FallActiveShape(gameBoard gameBoard_ptr){
	for (size_t i = 0; i < 4; i++){
		Shape.current.coordinates[i] += 10;
	}

	// update fall data for all shapes (used for rotation)
	for (size_t i = 0; i < 4; i++){
		for (size_t j = 0; j < 4; j++){
			Shape.current.full_details[i][j] += 10;
		}
	}

	Shape.fall_counter += 10;
	
	//printf("%d \n", Shape.fall_counter);
}

/**
 Checks the current shape position for its shape positions
 returns true if at edge
 */
bool ShapeIsInLeftColumn(void){
	int test;
	for (size_t i = 0; i < 4; i++){
		test = Shape.current.coordinates[i] % 10;
		//printf("%d ",Shape.current.coordinates[i]);
		if (test == 0){
			return true;
		}
	}
	return false;
}
/**
 Checks the current shape position for its shape positions
 returns true if at edge
 */
bool ShapeIsInRightColumn(void){

	for (size_t i = 0; i < 4; i++){
		for (size_t j = 9; j <=199; j += 10){
			if (Shape.current.coordinates[i] == j){
				return true;
			}
		}
	}
	return false;
}
/**
 Checks if any square of the moving shape is one the bottom row
 @param[in] gameBoard_ptr
 */
bool IsAtFloor(gameBoard gameBoard_ptr){
	for (size_t i = 0; i < 4; i++){
		if(Shape.current.coordinates[i] > 189){

			return true;
		}
	}
	return false;
}
/**
 Function that checks position of shape. If there is a blocked shape underneath
 return true.
 @param[in] gameBoard_ptr
 */
bool IsOnTop(gameBoard gameBoard_ptr){
	for (size_t i = 0; i < BLOCKS_DOWN; i++){
		for (size_t j = 0; j < BLOCKS_ACROSS; j ++){
			if(gameBoard_ptr->shape_board[i][j] == 1){
				if (gameBoard_ptr->blocked_board[i+1][j] == 1){
					return true;
				}
			}
		}
	}
	return false;

}
/**
 Function to check if there is a block to left side
 of active shape
 */
bool IsOnSide(gameBoard gameBoard_ptr){
	for (size_t i = 0; i < BLOCKS_DOWN; i++){
		for (size_t j = 0; j < BLOCKS_ACROSS; j ++){
			if(gameBoard_ptr->shape_board[i][j] == 1){
				if (gameBoard_ptr->blocked_board[i][j-1] == 1 ||
						gameBoard_ptr->blocked_board[i][j+1] == 1){
					return true;
				}
			}
		}
	}
	return false;
}

/**
 If a row is full of blocked tiles, add that row number to array
 @param[in] gameBoard_ptr
 @param[out] array array containing 4 elements set to -1
 @param[out] pointer to variable score
 */
int IsRowFull(gameBoard gameBoard_ptr){
	int row = 0;

	for (size_t i = 0; i < BLOCKS_DOWN; i++){
		for (size_t j = 0; j < BLOCKS_ACROSS; j ++){
			if (gameBoard_ptr->blocked_board[i][j] == 1){
				row++;
				if (row == 10){
					return i;
				}
			}
		}
		row = 0;
	}

#ifdef DEBUG
#endif
	return -1;
}

/**
 Cycle through array and for each element that isn't -1,
 remove that row from the board
 @param[in] gameBoard_ptr
 @param[in] array
 */
bool destroyRow(gameBoard gameBoard_ptr, int row){

	if (row == -1){
		return false;
	}

	for (size_t i = 0 ; i < BLOCKS_ACROSS; i++){ // make the square 0
			gameBoard_ptr->blocked_board[row][i] = 0;
	}

#ifdef DEBUG
	printf("destroyed %d \n", row);
#endif
	
	FallBlocked(gameBoard_ptr, row);

/*  for each row above the vanished row, if a block occupies a space, drop down 
 until it either finds the floor or a blcoked square under neath it*/
	
	return true;	
}

/**
 private function to drop blocks from 0 to row down */
static bool FallBlocked(gameBoard gameBoard_ptr, int i){	
	for (size_t a = 0; a <= i; a++){
		for (size_t b = 0; b < BLOCKS_ACROSS; b++){
			if (a > 0){
				gameBoard_ptr->temp_board[a][b]	= gameBoard_ptr->blocked_board[a-1][b];
			}
		}
	}

	for (size_t a = 0; a <= i; a++){
		for (size_t b = 0; b < BLOCKS_ACROSS; b++){
			gameBoard_ptr->blocked_board[a][b] = 
				gameBoard_ptr->temp_board[a][b];
		}
	}
}


/**
 Move object left
 */
void MoveLeft(void){
	if (IsKeyPressed(KEY_LEFT)){
		for (size_t i = 0; i < 4; i++){
			for (size_t j = 0; j < 4; j++){
				Shape.current.coordinates[i] -= 1;
				Shape.current.full_details[i][j] -=1;
			}
		}
	}
}
/**
 Move object right
 */
void MoveRight(void){
	if (IsKeyPressed(KEY_RIGHT)){
		for (size_t i = 0; i < 4; i++){
			for (size_t j = 0; j < 4; j++){
				Shape.current.coordinates[i] += 1;
				Shape.current.full_details[i][j] +=1;
			}
		}
	}
}
/** Changes current rotation value of moving shape by altering global variable member
 * Shape.current
 * */
void MoveUp(void){
	if (IsKeyPressed(KEY_UP)){
		Shape.current.rotation++;
		if (Shape.current.rotation > 3){	
			Shape.current.rotation = 0;
		}
	}

	for (size_t i = 0; i < 4; i++){
		Shape.current.coordinates[i] = 
			Shape.current.full_details[Shape.current.rotation][i];
	}
}
/** Not yet used */
bool MoveDown(gameBoard gameBoard_ptr){
	if (IsKeyPressed(KEY_DOWN)){
		return true;
		
	}
	return false;
}
/**
 * Deallocate memory
 **/
void freeBoard(gameBoard gameBoard_ptr){
	free(gameBoard_ptr);
}
