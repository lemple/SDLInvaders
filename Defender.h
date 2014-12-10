#ifndef DEFENDER_H
#define DEFENDER_H

#include <SDL.h>

//move defender by sprite width
//create new function for the defneder use switch statement for keypress
// an enumeration for direction to move USE more enums!
enum DIRECTION { LEFT, RIGHT, NONE };

// Defender constants
static const int DEFENDERWIDTH = 30;
static const int DEFENDERHEIGHT = 60;

// Data structure to represent defender
typedef struct
{
  SDL_Rect position;
}Defender;

Defender createDefender(int _x, int _y);

void drawDefender(SDL_Renderer *ren, Defender defender);
void defenderMovement();

#endif // DEFENDER_H
