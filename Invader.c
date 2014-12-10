#include "Invader.h"

// Local globals
static int gameWidth = -1;
static int gameHeight = -1;

void initializeInvaders(Invader invaders[ROWS][COLS], int _width, int _height)
{
  gameWidth = _width;
  gameHeight = _height;

  SDL_Rect pos;
  pos.w=SPRITEWIDTH;
  pos.h=SPRITEHEIGHT;
  int ypos=GAP;

  //configures data structure to start
  for(int r=0; r<ROWS; ++r)
  {
    int xpos=GAP;
    for(int c=0; c<COLS; ++c)
    {
      pos.x=xpos+SPRITEWIDTH;
      pos.y=ypos+SPRITEHEIGHT;
      xpos+=(GAP+SPRITEWIDTH);
      invaders[r][c].pos=pos;
      invaders[r][c].active=1;
      invaders[r][c].frame=0;
      if(r==0)
        invaders[r][c].type=TYPE1;
      else if(r==1 || r==2)
        invaders[r][c].type=TYPE2;
      else
        invaders[r][c].type=TYPE3;

    }
    ypos+=(GAP+SPRITEHEIGHT);
  }
}

void updateInvaders(Invader invaders[ROWS][COLS])
{
  if(gameWidth == -1 || gameHeight == -1)
  {
    printf("Width and height not initialised in Invader data structure\n");
    return;
  }

  // assert(gameWidth != -1 && gameHeight != -1);

  enum DIR{FWD,BWD};
  static int DIRECTION=FWD;
  int yinc=0;
  //so invaders can change direction when they hit the end of the screen
  if(invaders[0][COLS].pos.x>=(gameWidth-SPRITEWIDTH)-(COLS*(SPRITEWIDTH+GAP)))
  {
    DIRECTION=BWD;
    yinc=GAP;

  }
  //for unit detection you need to alter the direction change when a colum is killed
  //need to wrtie funciton for which row is active
  else if(invaders[0][0].pos.x<=SPRITEWIDTH)
  {
    DIRECTION=FWD;
    yinc=GAP;

  }

  for(int r=0; r<ROWS; ++r)
  {
    for(int c=0; c<COLS; ++c)
    {
      if(DIRECTION==FWD)
        invaders[r][c].pos.x+=1;
      else
        invaders[r][c].pos.x-=1;
      invaders[r][c].pos.y+=yinc;

    }
  }
}


//loads in textures for multiple may need array
void drawInvaders(SDL_Renderer *ren, SDL_Texture *tex, Invader invaders[ROWS][COLS])
{
  SDL_Rect SrcR;
  SrcR.x=0;
  SrcR.y=0;
  SrcR.w=88;
  SrcR.h=64;

  for(int r=0; r<ROWS; ++r)
  {
    for(int c=0; c<COLS; ++c)
    {
      //more likely to switch the invader type to input diffrent texture
      switch(invaders[r][c].type)
      {
      //sets the color of the diffrent sprites can import own sprite for diffrent image
      case TYPE1 : SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); break;
      case TYPE2 : SDL_SetRenderDrawColor(ren, 0, 255, 0, 255); break;
      case TYPE3 : SDL_SetRenderDrawColor(ren, 0, 0, 255, 255); break;
      }
      SDL_RenderFillRect(ren,&invaders[r][c].pos);
      //copy 1 texture of someszie to another position
      SDL_RenderCopy(ren, tex,&SrcR,&invaders[r][c].pos);


    }
  }
}
