#include <Defender.h>

Defender createDefender(int _x, int _y)
{
  Defender tmp;
  tmp.position.h = DEFENDERWIDTH;
  tmp.position.w = DEFENDERHEIGHT;
  tmp.position.x = _x;
  tmp.position.y = _y;

  return tmp;
}

void drawDefender(SDL_Renderer *ren, Defender defender)
{
  SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
  SDL_RenderFillRect(ren, &defender.position);
}

void defenderMovement()
{

}
