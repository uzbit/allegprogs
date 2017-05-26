#ifndef _MISSILE_H
#define _MISSILE_H
#include<math.h>
#include"player.h"

#define GRAVITY .5

struct MISSILE
{
   float x,y,dx,dy;
   int type;
   bool is;
   MISSILE();
   void init(PLAYER &p);
   void process();
};
MISSILE::MISSILE()
{
   is=0;x=0;y=0;dx=0;dy=0;type=1;
}
void MISSILE::init(PLAYER &p)
{
   float rad=(M_PI/180)*p.angle;
   x=p.x+17+(cos(rad)*24);
   y=p.y+8-(sin(rad)*24);
   dx=p.power*cos(rad);
   dy=-(p.power*sin(rad));
   p.missiles[type-1]--;
   is=1;
}
void MISSILE::process()
{
   dy+=GRAVITY;
   if(x+dx<=0)
      x=800;
   if(x+dx>800)
      x=0;
   if(y+dy>=600)
      is=0;
   x+=dx;
   y+=dy;
}




#endif

