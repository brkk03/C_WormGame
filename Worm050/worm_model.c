// A simple variant of the game Snake
//
// Used for teaching in classes
//
// Author:
// Franz Regensburger
// Ingolstadt University of Applied Sciences
// (C) 2011
//
// The worm model

#include <curses.h>
#include "worm.h"
#include "board_model.h"
#include "worm_model.h"

// Initialize the worm
extern enum ResCodes initializeWorm(struct worm* aworm,int len_max,struct pos headpos, enum WormHeading dir, enum ColorPairs color) {
    // Initialize indices
    aworm->maxindex = len_max - 1;
    aworm->headindex = 0;

    for (int i = 0; i <= aworm->maxindex; i++) {
        aworm->wormpos[i].y = UNUSED_POS_ELEM;
        aworm->wormpos[i].x = UNUSED_POS_ELEM;
    }

    // Initialize position of worms head
    aworm->wormpos[aworm->headindex] = headpos;

    // Initialize the heading of the worm
    setWormHeading(aworm, dir);

    // Initialze color of the worm
    aworm->wcolor = color;

    return RES_OK;
}

// Show the worms's elements on the display
// Simple version
extern void showWorm(struct worm* aworm) {
    // Due to our encoding we just need to show the head element
    // All other elements are already displayed
    placeItem(
            aworm->wormpos[aworm->headindex].y,
            aworm->wormpos[aworm->headindex].x,
            SYMBOL_WORM_INNER_ELEMENT,
            aworm->wcolor);
}

extern void cleanWormTail(struct worm* aworm) {
    int tailindex = (aworm->headindex + 1) % (aworm->maxindex + 1);
    if (aworm->wormpos[tailindex].y != UNUSED_POS_ELEM) {
        placeItem(aworm->wormpos[tailindex].y,
                  aworm->wormpos[tailindex].x,
                  SYMBOL_FREE_CELL,
                  COLP_FREE_CELL);
    }
}

extern void moveWorm(struct worm* aworm, enum GameStates* agame_state) {
    // Compute and store new head position according to current heading.
    struct pos headpos;
    headpos.y = aworm->wormpos[aworm->headindex].y + aworm->dy;
    headpos.x = aworm->wormpos[aworm->headindex].x + aworm->dx;

    // Check if we would leave the display if we move the worm's head according
    // to worm's last direction.
    // We are not allowed to leave the display's window.
    if (headpos.x < 0) {
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else if (headpos.x > getLastCol()) { 
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else if (headpos.y < 0) {  
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else if (headpos.y > getLastRow()) {
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else {
        // We will stay within bounds.
        if (isInUseByWorm(aworm, headpos)) {
            *agame_state = WORM_CROSSING;
        }
    }

    if (*agame_state == WORM_GAME_ONGOING) {
       aworm->headindex = (aworm->headindex + 1) % (aworm->maxindex + 1);
       aworm->wormpos[aworm->headindex] = headpos;
    }
}
extern void setWormHeading(struct worm* aworm, enum WormHeading dir) {
  switch(dir) {
        case WORM_UP :// User wants up
            aworm->dx=0;
            aworm->dy=-1;
            break;
        case WORM_DOWN:// User wants down
            aworm->dx=0;
            aworm->dy=1;
            break;
        case WORM_LEFT:// User wants left
            aworm->dx=-1;
            aworm->dy=0;
            break;
        case WORM_RIGHT:// User wants right
            aworm->dx=1;
            aworm->dy=0;
            break;
   }
}
 

extern bool isInUseByWorm(struct worm* aworm, struct pos new_pos) {
    int i;
    bool collision = false;

    i = aworm->headindex;

    do {
        if (aworm->wormpos[i].y == new_pos.y &&
            aworm->wormpos[i].x == new_pos.x) {
            collision = true;
            break;
        }
        i = (i + aworm->maxindex) % (aworm->maxindex + 1);
    } while (i != aworm->headindex && aworm->wormpos[i].y != UNUSED_POS_ELEM);

    return collision;
}
struct pos getWormHeadPos(struct worm* aworm){
  return aworm->wormpos[aworm->headindex];
}
