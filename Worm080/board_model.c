// A simple variant of the game Snake
//
// Used for teaching in classes
//
// Author:
// Franz Regensburger
// Ingolstadt University of Applied Sciences
// (C) 2011
//
// The board model
#include <stdlib.h>
#include "board_model.h"
#include "messages.h"

void cleanupBoard(struct board* aboard){
// For each row free the array of its columns
  for (aboard->cells = 0; aboard->cells < COLS; aboard->cells++) {
    if (aboard->cells == NULL) { break; }
    free(aboard->cells);
  }
  // Free the array of pointers
  free(aboard->cells);
}
// Place an item onto the curses display.
void placeItem(struct board* aboard,int y, int x, enum BoardCodes board_code, chtype symbol, enum ColorPairs color_pair) {

    //  Store item on the display (symbol code)
    move(y, x);                         // Move cursor to (y,x)
    attron(COLOR_PAIR(color_pair));     // Start writing in selected color
    addch(symbol);                      // Store symbol on the virtual display
    attroff(COLOR_PAIR(color_pair));    // Stop writing in selected color
    aboard->cells[y][x] = board_code;
}

// Get the last usable row on the display
int getLastRowOnBoard(struct board* aboard) {
    return aboard->last_row;
}

// Get the last usable column on the display
int getLastColOnBoard(struct board* aboard) {
    return aboard->last_col;
}

int getNumberOfFoodItems(struct board* aboard) {
    return aboard->food_items;
}

enum BoardCodes getContentAt(struct board* aboard, struct pos position) {
    return aboard->cells[position.y][position.x];
}

void setNumberOfFoodItems(struct board* aboard, int n){
    aboard->food_items = n;
}

void decrementNumberOfFoodItems(struct board* aboard){
    aboard->food_items--;
}

enum ResCodes initializeBoard(struct board* aboard) {
  int y;
  //Maximal index of a row, reserve space for message area
  aboard->last_row = LINES-ROWS_RESERVED-1;
  //Maximal index of a column
  aboard->last_col= COLS-1;
  // Check dimensions of the board
  if ( aboard->last_col < MIN_NUMBER_OF_COLS-1 ||aboard->last_row < MIN_NUMBER_OF_ROWS-1) {
     char buf[100];
     sprintf(buf,"Das Fenster ist zu klein: wir brauchen %dx%d",
     MIN_NUMBER_OF_COLS , MIN_NUMBER_OF_ROWS + ROWS_RESERVED);
     showDialog(buf,"Bitte eine Taste druecken");
     return RES_FAILED;
  }
  //Allocate memory for 2-dimensional array of cells
  //Alloc array of rows
  aboard->cells = malloc(COLS*sizeof(int));
  if (aboard->cells==NULL) {
      showDialog("Abbruch: Zu wenig Speicher","Bitte eine Taste druecken");
      exit(RES_FAILED); //No memory -> direct exit
  }
  for (y=0;y<COLS;y++){
    //Allocate array of columns for each y
      aboard->cells[y] = malloc(COLS*sizeof(int));
      if (aboard->cells[y] == NULL){
          showDialog("Abbruch: Zu wenig Speicher","Bitte eine Taste druecken");
          exit(RES_FAILED);
      }
  }
  return RES_OK;
}


enum ResCodes initializeLevel(struct board* aboard) {
    int y, x;// define local variables for loops etc
    // Fill board and screen buffer with empty cells.
    for (y = 0;y <=aboard->last_row; y++) {
        for (x = 0;x <=aboard->last_col ; x++) {
          placeItem(aboard,y,x,BC_FREE_CELL,SYMBOL_FREE_CELL,COLP_FREE_CELL);
        }
    }
    // Draw a line in order to separate the message area
    // Note: we cannot use function placeItem() since the message area
    // is outside the board!
    y = aboard->last_row + 1;
    for (x=0; x <= aboard->last_col ; x++) {
        move(y, x);
        attron(COLOR_PAIR(COLP_BARRIER));
        addch(SYMBOL_BARRIER);
        attroff(COLOR_PAIR(COLP_BARRIER));
    } 
    // Barriers: use a loop
    int offset =4;
    for (y = 12; y < 22; y++) {
        x = (int) offset+ ((aboard->last_col)/3);
        placeItem(aboard,y,x,BC_BARRIER,SYMBOL_BARRIER,COLP_BARRIER);
    }
    for (y =6; y < 17; y++) {
        x = (int)  (2*(aboard->last_col)/3)-offset;
        placeItem(aboard,y,x,BC_BARRIER,SYMBOL_BARRIER,COLP_BARRIER);
    }
    // Food
    // Food 1
    placeItem(aboard, 3, 3,BC_FOOD_1,SYMBOL_FOOD_1,COLP_FOOD_1);
    placeItem(aboard, 12, 22,BC_FOOD_1,SYMBOL_FOOD_1,COLP_FOOD_1);
    // Food 2
    placeItem(aboard, 2, 1,BC_FOOD_2,SYMBOL_FOOD_2,COLP_FOOD_2);
    placeItem(aboard,5, 4,BC_FOOD_2,SYMBOL_FOOD_2,COLP_FOOD_2);
    placeItem(aboard, 19, 12,BC_FOOD_2,SYMBOL_FOOD_2,COLP_FOOD_2);
    placeItem(aboard, 5, 10,BC_FOOD_2,SYMBOL_FOOD_2,COLP_FOOD_2);
    // Food 3
    placeItem(aboard, 2, 2,BC_FOOD_3,SYMBOL_FOOD_3,COLP_FOOD_3);
    placeItem(aboard, 9, 8,BC_FOOD_3,SYMBOL_FOOD_3,COLP_FOOD_3);
    placeItem(aboard, 5, 7,BC_FOOD_3,SYMBOL_FOOD_3,COLP_FOOD_3);
    placeItem(aboard, 9, 7,BC_FOOD_3,SYMBOL_FOOD_3,COLP_FOOD_3);
    // Initialize number of food items
    // Attention: must match number of items placed on the board above
    aboard->food_items = 10;
    return RES_OK;
}
