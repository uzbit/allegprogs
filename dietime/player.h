#ifndef _PLAYER_H
#define _PLAYER_H
#include"../estring/estring.h"
#include<stdlib.h>

struct PLAYER
{
   int x,y,angle,power,health,points;
   int missiles[3];
   estring name;
   PLAYER();
   void init(PLAYER *p,int numplayers);
   int nummissiles(int index){return missiles[index];}
};
PLAYER::PLAYER()
{
   x=0;y=0;angle=0;power=0;health=0;points=0;
   for(int i=0;i<3;i++)
      missiles[i]=0;
}
void PLAYER::init(PLAYER *p,int numplayers)
{
   y=0;
   health=100;angle=0;power=0;
   missiles[0]=200;
   missiles[1]=10;
   missiles[2]=5;
   int pos[8]={0,100,200,300,400,500,600,700};
   int temp=(rand()%8);
   bool check;
   do{
      check=1;
      for(int i=0;i<numplayers;i++)
         if(pos[temp]==p[i].x)
         {
            check=0;
            break;
         }
      if(!check)
         temp=(rand()%8);
   }while(!check);
   x=pos[temp];
}

#endif

