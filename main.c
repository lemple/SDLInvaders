#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "Invader.h"
#include "Defender.h"
//move defender by sprite width
//create new function for the defneder use switch statement for keypress


// include the map for the maze.
// the width of the screen taking into account the maze and block
#define WIDTH 800
// the height of the screen taking into account the maze and block
#define HEIGHT 600
#define ANIMATIONLENGTH 50

/* an enumeration for direction to move USE more enums!
#define BOOL char
#define TRUE 1
#define FALSE 0
*/


enum DIRECTION{LEFT,RIGHT,NONE};


void initializePlayer(Defender* player);
void initializeInvaders(Invader invaders[ROWS][COLS]);
void updateInvaders(Invader invaders[ROWS][COLS]);

void ProjectileShot(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect *projectile, Defender* defender);

void drawInvaders(SDL_Renderer *ren, SDL_Texture *tex, Invader invaders[ROWS][COLS]);
void drawDefender(SDL_Renderer *ren, SDL_Rect defender, SDL_Texture *tex);
void defenderMovement(SDL_Rect *defender);

void Collision(Invader invaders[ROWS][COLS], SDL_Rect *projectile, Defender* defender);

int main()
{
//  SDL_Rect defender;
//  defender.h = SPRITEHEIGHT;
//  defender.w = 60;
//  defender.x = (WIDTH-defender.w)/2;
//  defender.y = HEIGHT-defender.h;

  Defender defender;
  initializePlayer(&defender);

  SDL_Rect projectile;
  projectile.w = 10;
  projectile.x = WIDTH/2 - projectile.w/2;
  projectile.h = 20;
  projectile.y = HEIGHT-40;



  Invader invaders[ROWS][COLS];
  initializeInvaders(invaders);
  // initialise SDL and check that it worked otherwise exit
  // see here for details http://wiki.libsdl.org/CategoryAPI
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
  {
    printf("%s\n",SDL_GetError());
    return EXIT_FAILURE;
  }
  // we are now going to create an SDL window

  SDL_Window *win = 0;
  win = SDL_CreateWindow("Invaders", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (win == 0)
  {
    printf("%s\n",SDL_GetError());
    return EXIT_FAILURE;
  }
  // the renderer is the core element we need to draw, each call to SDL for drawing will need the
  // renderer pointer
  SDL_Renderer *ren = 0;
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  // check to see if this worked
  if (ren == 0)
  {
    printf("%s\n",SDL_GetError() );
    return EXIT_FAILURE;
  }

  // this will set the background colour to white.
  // however we will overdraw all of this so only for reference
  SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

//  // SDL image is an abstraction for all images
  SDL_Surface *image;
//  // we are going to use the extension SDL_image library to load a png
  image=IMG_Load("invaderSpriteSheet.png");
  if(!image)
  {
    printf("IMG_Load: %s\n", IMG_GetError());
    return EXIT_FAILURE;
  }


    //SDL_Rect defender;


//  // SDL texture converts the image to a texture suitable for SDL rendering  / blitting
//  // once we have the texture it will be store in hardware and we don't need the image data anymore

  SDL_Texture *tex = 0;
  tex = SDL_CreateTextureFromSurface(ren, image);
  // free the image
  SDL_FreeSurface(image);

  int quit=0;
  // now we are going to loop forever, process the keys then draw

  while (quit !=1)
  {
    SDL_Event event;
    // grab the SDL even (this will be keys etc)
    while (SDL_PollEvent(&event))
    {
      // look for the x of the window being clicked and exit
      if (event.type == SDL_QUIT)
        quit = 1;


      // check for a key down
      if (event.type == SDL_KEYDOWN)
      {
        switch (event.key.keysym.sym)
        {
        // if we have an escape quit
        case SDLK_ESCAPE : quit=1; break;
/*
        case SDLK_a : defender.x--; break;

        case SDLK_d : defender.x++; break;
*/

       }

    }
  }
  //if more textures then more textures need to be added
  // now we clear the screen (will use the clear colour set previously)
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);
  ProjectileShot(ren, tex, &projectile, &defender);
  Collision(invaders, &projectile, &defender);

  updateInvaders(invaders);
  drawInvaders(ren,tex,invaders);

  drawDefender(ren, defender.pos, tex);
  defenderMovement(&(defender.pos));


  // Up until now everything was drawn behind the scenes.
  // This will show the new, red contents of the window.
  SDL_RenderPresent(ren);

  }

  SDL_Quit();
  return 0;
}

void initializePlayer(Defender* player)
{

  player->pos.h = SPRITEHEIGHT;
  player->pos.w = 60;
  player->pos.x = (WIDTH-player->pos.w)/2;
  player->pos.y = HEIGHT-player->pos.h;

  player->isFiring = 0;
}

void initializeInvaders(Invader invaders[ROWS][COLS])
{
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
      invaders[r][c].pos = pos;
      invaders[r][c].active = 1;
      invaders[r][c].isFiring = 0;
      invaders[r][c].frame = 0;
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

//loads in textures for multiple may need array
void drawInvaders(SDL_Renderer *ren, SDL_Texture *tex, Invader invaders[ROWS][COLS])
{
  static int frameTime = 0;
  //static int invaderDead = 0;
  static int rowCount = 0;

    /*
     Retrives images fomr the sprite sheet
     and places them dependent on the row/coloum
     of the invaders
     */

  SDL_Rect SrcRT1;
  SrcRT1.x=0;
  SrcRT1.y=0;
  SrcRT1.w=100;
  SrcRT1.h=80;

  SDL_Rect SrcRT2;
  SrcRT2.x=90;
  SrcRT2.y=0;
  SrcRT2.w=100;
  SrcRT2.h=80;

  SDL_Rect SrcRT3;
  SrcRT3.x=185;
  SrcRT3.y=0;
  SrcRT3.w=100;
  SrcRT3.h=80;
//incremeting the frame time so its equal to ANIMATIONLENGTH
  frameTime++;
  for(int r=0; r<ROWS; ++r)
  {
    for(int c=0; c<COLS; ++c)
    {
       //Provides a half second delay inbetween sprite animations
      if(frameTime == ANIMATIONLENGTH)
      {
        //assigns the invaders rows and colums to diffrent animations
        //and allows them to be sotred on either frame 0 or 1
        invaders[r][c].frame = (invaders[r][c].frame == 0 ? 1 : 0);
        rowCount++;
        //checks every invader on the screen then sets the value to 0 to restart
        if (rowCount == 55)
        {
          frameTime = 0;
          rowCount = 0;
        }
      }
      //runs the animaton from the spritesheet if frame 0
      if(invaders[r][c].frame == 0)
      {
        SrcRT1.x=0;
        SrcRT2.x=0;
        SrcRT3.x=0;
      }
      //runs animation for frame 1
      else
      {
        SrcRT1.x=90;
        SrcRT2.x=0;
        SrcRT3.x=185;
      }
      if(invaders[r][c].active == 1)
      {




        switch(invaders[r][c].type)
        {
        //sets the color of the diffrent sprites can import own sprite for diffrent image
        case TYPE1 : SDL_RenderCopy(ren, tex, &SrcRT3, &invaders[r][c].pos); break;
        case TYPE2 : SDL_RenderCopy(ren, tex, &SrcRT2, &invaders[r][c].pos); break;
        case TYPE3 : SDL_RenderCopy(ren, tex, &SrcRT1, &invaders[r][c].pos); break;
        }
      }
      //more likely to switch the invader type to input diffrent texture

     /* SDL_RenderFillRect(ren,&invaders[r][c].pos);
      //copy 1 texture of someszie to another position
      SDL_RenderCopy(ren, tex,&SrcRT1,&invaders[r][c].pos);
     */

    }
  }
}

void updateInvaders(Invader invaders[ROWS][COLS])
{
  enum DIR{FWD,BWD};
  static int DIRECTION=FWD;
  int yinc=0;
  //activecolum left
  static int activeCL = 0;
  //active colum right
  static int activeCR = COLS-1;
  int halt = 0;


  for(int c = COLS-1; c >= 0; c--)
  {
      for(int r = 0; r < ROWS; r++)
      {
        if(invaders[r][c].active)
        {
            activeCR = c;
            halt = 1;
            break;
        }
      }
      if(halt)
      {
         break;
      }
  }
halt = 0;

for(int r = 0; r < ROWS; r++)
{
  for(int c =  0; c < COLS; c++)
  {
    if(invaders[r][c].active)
    {
      activeCL = c;
      halt = 1;
      break;
    }
  }
  if(halt)
  {
    break;
  }
}
  //so invaders can change direction when they hit the end of the screen
  if(invaders[0][activeCR].pos.x>=WIDTH-SPRITEWIDTH)//(WIDTH-SPRITEWIDTH)-(COLS*(SPRITEWIDTH+GAP)))
  {
      // printf("%d\n", activeCR);
    DIRECTION=BWD;
    yinc=GAP;

  }
  //for unit detection you need to alter the direction change when a colum is killed
  //need to wrtie funciton for which row is active
  else if(invaders[0][activeCL].pos.x<=SPRITEWIDTH/2)
  {
    DIRECTION=FWD;
    yinc=GAP;
  }

  for(int r=0; r<ROWS; ++r)
  {
    for(int c=0; c<COLS; ++c)
    {
      if(DIRECTION==FWD)
        invaders[r][c].pos.x+=5/2;
      else
        invaders[r][c].pos.x-=5/2;
      invaders[r][c].pos.y+=yinc;

    }
  }

}

void ProjectileShot(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect *projectile, Defender* defender)
{
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    SDL_Rect projectileSprite;
    projectileSprite.x = 325;
    projectileSprite.y = 252;
    projectileSprite.w = 30;
    projectileSprite.h = 60;

    if(keystate[SDL_SCANCODE_SPACE] && (defender->isFiring == 0))
    {
        defender->isFiring = 1;
        projectile->x = defender->pos.x +23;
    }

    if(defender->isFiring == 1)
    {
      projectile->y -=10;

      SDL_RenderFillRect(ren, projectile);
      SDL_RenderCopy(ren, tex, &projectileSprite, projectile);

      if(projectile->y <= 0)
      {
          defender->isFiring = 0;
          //printf("BOOM DESTORYED"); /* http://desktopwallpapers.biz/wp-content/uploads/2014/08/Genius-For-Downloads.jpg */
          projectile->y = HEIGHT-40;
      }
    }
}




void drawDefender(SDL_Renderer *ren, SDL_Rect defender, SDL_Texture *tex)
{
    SDL_Rect defenderSprite;
    defenderSprite.x = 100;
    defenderSprite.y = 425;
    defenderSprite.h = 40;
    defenderSprite.w = 60;

    SDL_RenderFillRect(ren, &defender);
    SDL_RenderCopy(ren, tex, &defenderSprite, &defender);



/*
  SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
  SDL_RenderFillRect(ren,&defender);
*/

}

void defenderMovement(SDL_Rect *defender)
{
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    int mRight = 0;
    int mLeft = 0;

    if(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A])
    {
        mLeft = 1;
        if(defender -> x <= 7)
        {
            mLeft = 0;
        }
    }
    if(keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D])
    {
        mRight = 1;
        if(defender -> x >= 750)
        {
            mRight = 0;
        }
    }
    if(mLeft == 1)
    {
       // printf("Right pressed \n");
        defender -> x -=6;
    }
    if(mRight == 1)
    {
      //  printf("Left pressed \n");
        defender -> x +=6;
    }


}

void Collision(Invader invaders[ROWS][COLS], SDL_Rect *projectile, Defender* defender)
{
  for(int c = 0; c< COLS;  c++)
  {
    for(int r = 0; r < ROWS; r++)
    {
      if(SDL_HasIntersection(&invaders[r][c].pos, projectile) && invaders[r][c].active)
      {
        invaders[r][c].active = 0;
        defender->isFiring = 0;
        projectile->y = HEIGHT-40;
      }
    }
  }
}




