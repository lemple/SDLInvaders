#ifndef INVADER_H__
#define INVADER_H__

//enumeration can create multiple data types
//Invade can be either type1 type2 or type3

enum InvaderType{TYPE1,TYPE2,TYPE3};

#define SPRITEWIDTH 35
#define SPRITEHEIGHT 35
#define COLS 11
#define ROWS 5
#define GAP 10

typedef struct
{
  SDL_Rect pos;
  //checks colum to see if invader is active
  int active;
  //used to set animation frame
  int frame;
  int kill;
  //type1 type2 type3 invader
  enum InvaderType type;
//This varible here swaps between if the player is firing or not.
//I use the already made struct to hold the value
  int isFiring;
}Invader;



#endif
