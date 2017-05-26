#ifndef _GAME_H
#define _GAME_H
#include<time.h>
#include<allegro.h>
#include"room.h"

#define WALL_LEN 30
#define SW 640
#define SH 480
#define BMB_TIME 3
#define BA_MOVE_SPEED 15

//up = 0
//down = 2
//right = 1
//left = 3
#define PLAYER_UP 		0
#define PLAYER_DOWN		2
#define PLAYER_LEFT		1
#define PLAYER_RIGHT	3


const int updates_per_second = 20;

struct LASERS
{
   int x,y,dir;
   bool is,was;
};
struct BOMBS
{
   int x,y,start;
   float time;
   bool is,was;
};
struct WHOLE;
struct BALIEN;

class PLAYER
{
   public:
   int numhealth,numlasers,numbombs,numlives,dir,hit,x,y;
   LASERS las;
   BOMBS bmb;

   PLAYER();
   //INIT
   //this function initializes the player
   //according to array
   void init(ROOM **array);
   //PROCESS
   //this function process the player input and output
   void process(ROOM **array,BALIEN *aliens,WHOLE *wormholes,int numaliens,int numwormholes);
   //PROCESS_LASERS
   //this function processes the lasers
   void process_lasers(ROOM **array);
   //PROCESS_BOMBS
   //process the bombs according to array
   void process_bombs(ROOM **array);

   protected:
   int begx,begy,canwarp;
   //$ACTIONS
   //this fucntion handles the io of the keyboard and
   //such
   void $actions(ROOM **array);
};
struct WHOLE
{
   int x,y;
   void init(ROOM **array);
};
struct BALIEN
{
   int x,y,numhealth;
   void init(ROOM **array);
   void process(ROOM **array,PLAYER &p,int count);
   protected:
   void $runaround(ROOM **array,PLAYER &p);
};
void WHOLE::init(ROOM **array)
{
   for(int cy=0;cy<(SH/WALL_LEN)-2;cy++)
      for(int cx=0;cx<SW/WALL_LEN;cx++)
      {
         if((array[cx][cy].bits&WORMHOLE)==WORMHOLE)
         {
            array[cx][cy].bits^=WORMHOLE;
            x=cx;
            y=cy;
            return;
         }
      }
}
void BALIEN::init(ROOM **array)
{
   for(int cy=0;cy<(SH/WALL_LEN)-2;cy++)
      for(int cx=0;cx<SW/WALL_LEN;cx++)
      {
         if((array[cx][cy].bits&BIGALIEN)==BIGALIEN)
         {
            array[cx][cy].bits^=BIGALIEN;
            x=cx;
            y=cy;
			numhealth=100;
            return;
         }
      }
}
void BALIEN::process(ROOM **array,PLAYER &p,int count)
{
   if(count%BA_MOVE_SPEED==0)
      $runaround(array,p);
   if(p.las.is && (p.las.x==x && p.las.y==y) && numhealth > 0)
   {
	 p.las.is=0;
     numhealth-=25;
   }
   //check if bmb hits alien
   if(p.bmb.was && p.bmb.time==0 && (p.bmb.x==x && p.bmb.y==y))
      numhealth-=75;
   if(p.bmb.was && p.bmb.time==0 && ((p.bmb.x+1==x && p.bmb.y==y) || (p.bmb.x-1==x && p.bmb.y==y) || (p.bmb.x==x && p.bmb.y+1==y) || (p.bmb.x==x && p.bmb.y-1==y)))
      numhealth-=50;
}
void BALIEN::$runaround(ROOM **array,PLAYER &p)
{
   if(y>p.y && y-1>=0 && (array[x][y].bits&T)!=T)
      y--;
   if(y<p.y && y+1<(SH/WALL_LEN-2) && (array[x][y].bits&B)!=B)
      y++;
   if(x>p.x && x-1>=0 && (array[x][y].bits&L)!=L)
      x--;
   if(x<p.x && x+1<(SW/WALL_LEN) && (array[x][y].bits&R)!=R)
      x++;
}
PLAYER::PLAYER()
{
  begx=0;begy=0;canwarp=1;hit=0;x=0;y=0;numhealth=0;numlasers=0;numbombs=0;numlives=0;dir=0;
  las.is=0;las.x=0;las.y=0;las.dir=0;las.was=0;
  bmb.is=0;bmb.x=0;bmb.y=0;bmb.start=0;bmb.was=0;
}
void PLAYER::init(ROOM **array)
{
   for(int cy=0;cy<(SH/WALL_LEN)-2;cy++)
      for(int cx=0;cx<SW/WALL_LEN;cx++)
      {
         if((array[cx][cy].bits&BEG)==BEG)
         {
            begx=cx;
            begy=cy;
         }
      }
   x=begx;y=begy;
   numhealth=100;numlasers=15;numbombs=1;numlives=3;
   dir=0;
}

void PLAYER::process(ROOM **array,BALIEN *aliens,WHOLE *wormholes,int numaliens,int numwormholes)
{
   $actions(array);
   process_lasers(array);
   process_bombs(array);
   if((array[x][y].bits&HEALTH)==HEALTH)
      numhealth+=10;
   if((array[x][y].bits&LASER)==LASER)
      numlasers+=10;
   if((array[x][y].bits&BOMB)==BOMB)
      numbombs++;
   if((array[x][y].bits&NEWLIFE)==NEWLIFE)
      numlives++;
   if((array[x][y].bits&END)==END)
      dir=5;
   if((array[x][y].bits&ALIEN)==ALIEN)
   {
      if(numhealth-20<=0)
      {
         numlives--;
         x=begx;
         y=begy;
         numhealth=100;numlasers=10;numbombs=0;
         dir=0;
      }
      else numhealth-=20;
   }
   if(!hit && numaliens>0)
   {
      for(int i=0;i<numaliens;i++)
		 if (aliens[i].numhealth > 0)
         if(x==aliens[i].x && y==aliens[i].y)
         {
            hit=1;
            if(numhealth-10<=0)
            {
               numlives--;
               x=begx;
               y=begy;
               numhealth=100;numlasers=15;numbombs=1;
               dir=0;
            }
            else numhealth-=10;
         }
   }
   if(numwormholes>1 && canwarp)
   {
      for(int i=0;i<numwormholes;i++)
      {
         if(x==wormholes[i].x && y==wormholes[i].y)
         {
            if(i==numwormholes-1)
            {
               x=wormholes[0].x;
               y=wormholes[0].y;
               array[x][y].defog();
               canwarp=0;
               return;
            }
            else
            {
               x=wormholes[i+1].x;
               y=wormholes[i+1].y;
               array[x][y].defog();
               canwarp=0;
               return;
            }
         }
      }
   }
   if(numlives==0)
      dir=6;
   array[x][y].clear(0);

   //for defoging
   switch (dir){
	  case 0:
		 if(y-2>=0)
		 {
			array[x][y-2].defog();
			array[x][y-1].defog();
		 }else if(y-1 >=0)
			array[x][y-1].defog();
		 break;
	  case 1:
		 if(x+2<(SW/WALL_LEN))
		 {
			array[x+2][y].defog();
			array[x+1][y].defog();
		 }else if(x+1 <(SW/WALL_LEN))
			array[x+1][y].defog();
		 break;
	  case 2:
		 if(y+2<(SH/WALL_LEN)-2)
		 {
			array[x][y+2].defog();
			array[x][y+1].defog();
		 }else if(y+1 <(SH/WALL_LEN)-2)
			array[x][y+1].defog();
		 break;
	  case 3:
	     if(x-2>=0)
		 {
			array[x-2][y].defog();
			array[x-1][y].defog();
		 }else if(x-1 >=0)
			array[x-1][y].defog();
		 break;
	  default:
		 break;
   }
   /*
   if(dir==0 && y-2>=0)
   {
      array[x][y-2].defog();
      array[x][y-1].defog();
   }else if(dir == 0 && y-1 >=0)
	  array[x][y-1].defog();
   
   if(dir==1 && x+2<(SW/WALL_LEN))
   {
      array[x+2][y].defog();
      array[x+1][y].defog();
   }else if(dir == 1 && x+1 <(SW/WALL_LEN))
	  array[x+1][y].defog();
   
   if(dir==2 && y+2<(SH/WALL_LEN)-2)
   {
      array[x][y+2].defog();
      array[x][y+1].defog();
   }else if(dir == 2 && y+1 <(SH/WALL_LEN)-2)
	  array[x][y+1].defog();
   
   if(dir==3 && x-2>=0)
   {
      array[x-2][y].defog();
      array[x-1][y].defog();
   }else if(dir == 3 && x-1 >=0)
	  array[x-1][y].defog();
   */

}
void PLAYER::process_lasers(ROOM **array)
{
   las.was=0;
   if(las.is)
   {
      if((array[las.x][las.y].bits&ALIEN)==ALIEN)
      {
         las.is=0;
         las.was=1;
         array[las.x][las.y].bits^=ALIEN;
      }
      else if((array[las.x][las.y].bits&BIGALIEN)==BIGALIEN)
         las.is=0;

      else
      {
         if(las.dir==0)
         {
            if(las.y-1<0 || (array[las.x][las.y].bits&T)==T)
               las.is=0;
            else las.y--;
         }
         else if(las.dir==2)
         {
            if(las.y+1>=(SH/WALL_LEN)-2 || (array[las.x][las.y].bits&B)==B)
               las.is=0;
            else las.y++;
         }
         else if(las.dir==1)
         {
            if(las.x+1>=(SW/WALL_LEN) || (array[las.x][las.y].bits&R)==R)
               las.is=0;
            else las.x++;
         }
         else if(las.dir==3)
         {
            if(las.x-1<0 || (array[las.x][las.y].bits&L)==L)
               las.is=0;
            else las.x--;
         }
      }
   }
   else if(key[KEY_SPACE])
   {
      if(!las.is && numlasers>0)
      {
         if(dir==0 && (array[x][y].bits&T)!=T)
         {
            las.x=x;
            las.y=y-1;
         }
         else if(dir==2 && (array[x][y].bits&B)!=B)
         {
            las.x=x;
            las.y=y+1;
         }
         else if(dir==1 && (array[x][y].bits&R)!=R)
         {
            las.x=x+1;
            las.y=y;
         }
         else if(dir==3 && (array[x][y].bits&L)!=L)
         {
            las.x=x-1;
            las.y=y;
         }
         else
         {
            numlasers--;
            return;
         }
         las.dir=dir;
         las.is=1;
         numlasers--;
      }
   }
}
void PLAYER::process_bombs(ROOM **array)
{
   bool temp=0;
   bmb.was=0;
   if(bmb.is)
   {
      bmb.time = BMB_TIME-((clock()-bmb.start)/(BPS_TO_TIMER(updates_per_second)+0.0));
      if(((clock()-bmb.start)/(BPS_TO_TIMER(updates_per_second)+0.0))>=BMB_TIME)
      {
         if((bmb.x==x && bmb.y==y))
         {
             if(numhealth-50<=0)
             {
                numlives--;
                x=begx;
                y=begy;
                numhealth=100;numlasers=10;numbombs=0;
                dir=0;
             }
             else numhealth-=50;
         }
         if(bmb.x==x-1 && bmb.y==y)
            temp=1;
         if(bmb.x==x+1 && bmb.y==y)
            temp=1;
         if(bmb.x==x && bmb.y==y+1)
            temp=1;
         if(bmb.x==x && bmb.y==y-1)
            temp=1;
         if(temp)
         {
            if(numhealth-20<=0)
            {
               numlives--;
               x=begx;
               y=begy;
               numhealth=100;numlasers=10;numbombs=0;
               dir=0;
            }
            else numhealth-=20;
         }

         array[bmb.x][bmb.y].clear(1);
         if(bmb.y-1>=0 && (array[bmb.x][bmb.y-1].bits&ALIEN)==ALIEN)
            array[bmb.x][bmb.y-1].clear(0);
         if((bmb.y+1)*WALL_LEN<SH-60 && (array[bmb.x][bmb.y+1].bits&ALIEN)==ALIEN)
            array[bmb.x][bmb.y+1].clear(0);
         if((bmb.x+1)*WALL_LEN<SW-30 && (array[bmb.x+1][bmb.y].bits&ALIEN)==ALIEN)
            array[bmb.x+1][bmb.y].clear(0);
         if(bmb.x-1>=0 && (array[bmb.x-1][bmb.y].bits&ALIEN)==ALIEN)
            array[bmb.x-1][bmb.y].clear(0);
         bmb.is=0;
         bmb.was=1;
      }
   }
   else if(key[KEY_B] && numbombs>0)
   {
      bmb.x=x;
      bmb.y=y;
      bmb.start=clock();
      bmb.time=5;
      bmb.is=1;
      numbombs--;
   }
}
void PLAYER::$actions(ROOM **array)
{
   if(key[KEY_UP])
      dir=PLAYER_UP;

   else if(key[KEY_DOWN])
      dir=PLAYER_DOWN;

   else if(key[KEY_RIGHT])
      dir=PLAYER_LEFT;

   else if(key[KEY_LEFT])
      dir=PLAYER_RIGHT;

   if(key[KEY_G])
   {
      if(dir==0 && y-1>=0 && (array[x][y].bits&T)!=T)
      {y--;canwarp=1;hit=0;}
      if(dir==2 && (y+1)*WALL_LEN<SH-60 && (array[x][y].bits&B)!=B)
      {y++;canwarp=1;hit=0;}
      if(dir==1 && (x+1)*WALL_LEN<SW-30 && (array[x][y].bits&R)!=R)
      {x++;canwarp=1;hit=0;}
      if(dir==3 && x-1>=0 && (array[x][y].bits&L)!=L)
      {x--;canwarp=1;hit=0;}
   }
}
#endif
