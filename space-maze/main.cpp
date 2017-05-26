//COMMENTS!!!
//T.J. McCormack session 3
//This is a maze game where you fly in a little space
//ship and try to get to the end of the maze with out
//getting dead
#include<allegro5/allegro.h>
#include<iostream>
#include<stdio.h>
#include<unistd.h>
//#include<conio.h>
#include<fstream>
#include"../estring/estring.h"
#include"game.h"
#include"room.h"

#define WALL_LEN 30
#define NUM_STARS 100
#define SW 640
#define SH 480
//Easy way of calling draw_sprite
#define TL(x,y) draw_sprite(buffer,bmps[0],x,y)
#define RL(x,y) draw_sprite(buffer,bmps[1],x,y)
#define BL(x,y) draw_sprite(buffer,bmps[2],x,y)
#define LL(x,y) draw_sprite(buffer,bmps[3],x,y)
#define ES(x,y) draw_sprite(buffer,bmps[4],x+2,y+2)
#define BS(x,y) draw_sprite(buffer,bmps[5],x+2,y+2)
#define LS(x,y) draw_sprite(buffer,bmps[6],x+2,y+2)
#define HS(x,y) draw_sprite(buffer,bmps[7],x+2,y+2)
#define BMS(x,y) draw_sprite(buffer,bmps[8],x+2,y+2)
#define SS(x,y) draw_sprite(buffer,bmps[9],x+2,y+2)
#define AS(x,y) draw_sprite(buffer,bmps[11],x+2,y+2)
#define BAS(x,y) draw_sprite(buffer,bmps[15],x+2,y+2)
#define WS(x,y) draw_sprite(buffer,bmps[16],x+2,y+2)
//ALLEGRO stuff
BITMAP *buffer,*bmps[17],*instruct,*intro;
PALETTE pal;
//For atmosphere
struct STAR
{
   int x,y,dy,dx;
   void init();
};
//INITGRAPH
//this function initializes the allegro graphics mode
//and loads the bitmaps into memory
void ingraph();
//INTRO
//this function does the intro screen
void introduction(STAR *stars);
//STARTLEVEL
//this function initializes
//all of the structs
//called in main function
void startlevel(ROOM **&array,PLAYER &p,BALIEN *&aliens,WHOLE *&wormholes,STAR *stars,estring &level,int &numaliens,int &numwormholes);
//LOAD
//this function loads the information from the file level
//into the array array
void load(ROOM **array,estring &level,int &numaliens,int &numwormholes);
//DRAW_ALL
//this is pretty obvious- it draws all of the info onto the screen
void draw_all(ROOM **array,PLAYER &p,BALIEN *aliens,WHOLE *wormholes,STAR *stars,int numaliens,int numwormholes);
//DRAW_ROOMS
//this function draws all of the rooms status into the
//buffer
void draw_rooms(ROOM **array,PLAYER &p);
//DRAW_PLAYER
//this function draws all of the other
//information that the player has such as
//lasers and bombs ect.
void draw_player(PLAYER &p);
//DRAW_OTHERS
//draws all of the aliens and wormholes in
//their other stuff
void draw_others(ROOM **array,BALIEN *aliens,WHOLE *wormholes,int numaliens,int numwormholes);
void draw_stars(STAR *stars, int dir);

volatile int ticks = 0;
void ticker()
{
	ticks++;
}
END_OF_FUNCTION(ticker)


void main(int argc,char **argv)
{
   estring level="./maps/map1.map";
   int numlevel=1,count=0;
   int numaliens=0,numwormholes=0;
   char temp[20];
   ROOM **array = NULL;
   PLAYER player1;
   BALIEN *aliens = NULL;
   WHOLE *wormholes = NULL;
   STAR stars[NUM_STARS];
   startlevel(array,player1,aliens,wormholes,stars,level,numaliens,numwormholes);
   ingraph();
   introduction(stars);
   while(!key[KEY_ESC])
   {
      while(ticks == 0)
	  {
			rest(1);//rest until a full tick has passed
	  }
	
	  while(ticks > 0)
	  {
		 int old_ticks = ticks;
 	
	     if (rand()%2==0)
			for(int i=0;i<numaliens;i++)
			   aliens[i].process(array,player1,count);
	  	else
			for(int i=numaliens-1;i>=0;i--)
			   aliens[i].process(array,player1,count);
	  	
		 player1.process(array,aliens,wormholes,numaliens,numwormholes);
      
		 ticks--;
		 if(old_ticks <= ticks)//logic is taking too long: abort and draw a frame
			break; 

		 count++;
	  }
	  
	  if (key[KEY_R])
		 startlevel(array,player1,aliens,wormholes,stars,level,numaliens,numwormholes);
	    
      draw_all(array,player1,aliens,wormholes,stars,numaliens,numwormholes);
	  
	  if((player1.dir==5) || (key[KEY_Z] && key[KEY_C] && key[KEY_X]))
      {
        numlevel++;
        sprintf(temp,"./maps/map%d.map",numlevel);
        temp[19]='\0';
        level=temp;
        startlevel(array,player1,aliens,wormholes,stars,level,numaliens,numwormholes);
        clear_keybuf();
      }
      if(player1.dir==6)
      {
         introduction(stars);
         level="./maps/map1.map";
         numlevel=1;
         startlevel(array,player1,aliens,wormholes,stars,level,numaliens,numwormholes);
      }
   }
   for(int i=0;i<17;i++)
      destroy_bitmap(bmps[i]);
   destroy_bitmap(intro);
   destroy_bitmap(instruct);

   delete [] wormholes;
   delete [] aliens;
   for(int i=0;i<SW/WALL_LEN;i++)
      delete [] array[i];
   delete [] array;
}
END_OF_MAIN();

void STAR::init()
{
   x=rand()%SW;
   y=rand()%SH;
   dy=(rand()%4)+1;
   dx=(rand()%4)+1;
}

void introduction(STAR *stars)
{
   int choice=0;
   int count = 0, dir=0;
   while(!key[KEY_ESC])
   {
      if (count % 50 == 0)
		 dir = rand()%4;
	  draw_stars(stars, dir);
	  
	  draw_sprite(buffer,intro,0,0);
      circlefill(buffer,200,(choice*60)+200,5,255);
      if(key[KEY_UP] && choice-1>=0)
         choice--;
      if(key[KEY_DOWN] && choice+1<=2)
         choice++;
      if(key[KEY_ENTER])
      {
         if(choice==0)
            return;
         if(choice==1)
         {
            clear_keybuf();
            clear(screen);
            while(!keypressed())
               draw_sprite(screen,instruct,0,0);
            choice=0;
            clear_keybuf();
         }
         if(choice==2)
            exit(0);
		 
	  }
      blit(buffer,screen,0,0,0,0,SW,SH);
      clear(buffer);
      rest(50);
	  count++;
   }
}
void startlevel(ROOM **&array,PLAYER &p,BALIEN *&aliens,WHOLE *&wormholes,STAR *stars,estring &level,int &numaliens,int &numwormholes)
{
   
   numaliens=0;
   numwormholes=0;
   if (array == NULL)
   {
      array=new ROOM*[SW/WALL_LEN];
      for(int i=0;i<SW/WALL_LEN;i++)
         array[i]=new ROOM[(SH/WALL_LEN)-2];
   }
   load(array,level,numaliens,numwormholes);
   
   if (aliens != NULL)
	  delete [] aliens;
   if(numaliens>0)
      aliens=new BALIEN[numaliens];
   if (wormholes != NULL)
	  delete [] wormholes;
   if(numwormholes>1)
      wormholes=new WHOLE[numwormholes];
   p.init(array);
   if(numaliens>0)
      for(int i=0;i<numaliens;i++)
         aliens[i].init(array);
   if(numwormholes>1)
      for(int i=0;i<numwormholes;i++)
         wormholes[i].init(array);
   for(int i=0;i<NUM_STARS;i++)
      stars[i].init();
}
void load(ROOM **array,estring &level,int &numaliens,int &numwormholes)
{
   if(access(level.string(),0))
      exit(0);
   ifstream fin;
   fin.open(level.string());
   for(int y=0;y<(SH/WALL_LEN)-2;y++)
      for(int x=0;x<SW/WALL_LEN;x++)
      {
         fin>>array[x][y].bits;
         if((array[x][y].bits&BEG)==BEG)
            continue;
         if((array[x][y].bits&BIGALIEN)==BIGALIEN)
            numaliens++;
         if((array[x][y].bits&WORMHOLE)==WORMHOLE)
            numwormholes++;
         array[x][y].bits|=HIDE;
      }
   fin.close();
}
void draw_stars(STAR *stars, int dir)
{
   int dx , dy;
   switch(dir){
	  case PLAYER_UP:
		 dx=0;dy=1;
		
		 for(int i=0;i<NUM_STARS;i++)
		 if(stars[i].y+stars[i].dy>=SH)
		 {
		    stars[i].x=rand()%SW;
		    stars[i].dy=(rand()%4)+1;
		    stars[i].y=0;
		 }
		 break;
	  case PLAYER_DOWN:
		 dx=0;dy=-1;
		 for(int i=0;i<NUM_STARS;i++)
		 if(stars[i].y-stars[i].dy<=0)
		 {
		    stars[i].x=rand()%SW;
		    stars[i].dy=(rand()%4)+1;
		    stars[i].y=SH;
		 }
		 break;
	  case PLAYER_LEFT:
		 dx=-1;dy=0;
		 for(int i=0;i<NUM_STARS;i++)
		 if(stars[i].x-stars[i].dx<=0)
		 {
		    stars[i].x=SW;
		    stars[i].dx=(rand()%4)+1;
		    stars[i].y=rand()%SH;
		 }
		 break;
	  case PLAYER_RIGHT:
		 dx=1;dy=0;
		 for(int i=0;i<NUM_STARS;i++)
		 if(stars[i].x+stars[i].dx>=SW)
		 {
		    stars[i].x=0;
		    stars[i].dx=(rand()%4)+1;
		    stars[i].y=rand()%SH;
		 }
		 break;	 		 
	  default:
		 break;
   }
   for(int i=0;i<NUM_STARS;i++)
   {
      stars[i].x+=dx*stars[i].dx;
	  stars[i].y+=dy*stars[i].dy;
	  
      putpixel(buffer,stars[i].x,stars[i].y,255);
   }
}

void draw_all(ROOM **array,PLAYER &p,BALIEN *aliens,WHOLE *wormholes,STAR *stars,int numaliens,int numwormholes)
{
   draw_stars(stars, p.dir);
   draw_others(array,aliens,wormholes,numaliens,numwormholes);
   draw_player(p);
   draw_rooms(array,p);
   line(buffer,0,422,640,422,255);
   textprintf(buffer,font,0,427,255,"HEALTH: %d",p.numhealth);
   textprintf(buffer,font,100,427,255,"LASERS: %d",p.numlasers);
   textprintf(buffer,font,200,427,255,"BOMBS: %d",p.numbombs);
   textprintf(buffer,font,300,427,255,"LIVES: %d",p.numlives);
   textout(buffer,font,"SPACE: LASERS",0,445,255);
   textout(buffer,font,"B: BOMBS",0,455,255);
   textout(buffer,font,"G: GO",0,465,255);
   textout(buffer,font,"R: RESET",SW-70,427,255);
   
   line(buffer,0,440,640,440,255);
   line(buffer,0,422,640,422,255);
   blit(buffer,screen,0,0,0,0,SW,SH);
   clear(buffer);
   rest(25);
}
void draw_rooms(ROOM **array,PLAYER &p)
{
   for(int y=0;y<(SH/WALL_LEN)-2;y++)
      for(int x=0;x<SW/WALL_LEN;x++)
      {
         if((array[x][y].bits&HIDE)==0)
         {
            if((array[x][y].bits&T)==T)
              TL(x*WALL_LEN,y*WALL_LEN);
            if((array[x][y].bits&R)==R)
              RL(x*WALL_LEN,y*WALL_LEN);
            if((array[x][y].bits&B)==B)
              BL(x*WALL_LEN,y*WALL_LEN);
            if((array[x][y].bits&L)==L)
              LL(x*WALL_LEN,y*WALL_LEN);
            if((array[x][y].bits&END)==END)
              ES(x*WALL_LEN,y*WALL_LEN);
            if((array[x][y].bits&BEG)==BEG)
              BS(x*WALL_LEN,y*WALL_LEN);
            if((array[x][y].bits&HEALTH)==HEALTH)
              HS(x*WALL_LEN,y*WALL_LEN);
            if((array[x][y].bits&LASER)==LASER)
              LS(x*WALL_LEN,y*WALL_LEN);
            if((array[x][y].bits&BOMB)==BOMB)
              BMS(x*WALL_LEN,y*WALL_LEN);
            if((array[x][y].bits&NEWLIFE)==NEWLIFE)
              SS(x*WALL_LEN,y*WALL_LEN);
            if((array[x][y].bits&ALIEN)==ALIEN)
              AS(x*WALL_LEN,y*WALL_LEN);
            if((array[x][y].bits&WORMHOLE)==WORMHOLE)
              WS(x*WALL_LEN,y*WALL_LEN);
         }
      }
}
void draw_player(PLAYER &p)
{
   if(p.dir==5)
   {
      textout_centre(screen,font,"Level Cleared!!",SW/2,SH/2,255);
      rest(500);
      clear_keybuf();
      while(!keypressed());
      return;
   }
   if(p.dir==6)
   {
      textout_centre(screen,font,"Game Over!!",SW/2,SH/2,255);
      rest(500);
	  clear_keybuf();
      while(!keypressed());
      return;
   }
   else if(p.dir==0)
      draw_sprite(buffer,bmps[9],(p.x*WALL_LEN)+2,(p.y*WALL_LEN)+2);
   else if(p.dir==2)
      draw_sprite_v_flip(buffer,bmps[9],(p.x*WALL_LEN)+2,(p.y*WALL_LEN)+2);
   else if(p.dir==3)
      draw_sprite(buffer,bmps[10],(p.x*WALL_LEN)+2,(p.y*WALL_LEN)+2);
   else if(p.dir==1)
      draw_sprite_h_flip(buffer,bmps[10],(p.x*WALL_LEN)+2,(p.y*WALL_LEN)+2);

   if(p.las.is)
   {
      if(p.las.dir==0)
         draw_sprite(buffer,bmps[12],(p.las.x*WALL_LEN)+2,(p.las.y*WALL_LEN)+2);
      else if(p.las.dir==2)
         draw_sprite_v_flip(buffer,bmps[12],(p.las.x*WALL_LEN)+2,(p.las.y*WALL_LEN)+2);
      else if(p.las.dir==1)
         draw_sprite(buffer,bmps[13],(p.las.x*WALL_LEN)+2,(p.las.y*WALL_LEN)+2);
      else if(p.las.dir==3)
         draw_sprite_h_flip(buffer,bmps[13],(p.las.x*WALL_LEN)+2,(p.las.y*WALL_LEN)+2);
   }
   if(p.las.was)
      draw_sprite(buffer,bmps[14],(p.las.x*WALL_LEN)+2,(p.las.y*WALL_LEN)+2);
   if(p.bmb.was)
      draw_sprite(buffer,bmps[14],(p.bmb.x*WALL_LEN)+2,(p.bmb.y*WALL_LEN)+2);

   if(p.bmb.is)
   {
      draw_sprite(buffer,bmps[8],(p.bmb.x*WALL_LEN)+2,(p.bmb.y*WALL_LEN)+2);
      textprintf(buffer,font,(p.bmb.x*WALL_LEN)+10,(p.bmb.y*WALL_LEN)+10,255,"%d",(int)(p.bmb.time+0.5));
   }
}
void draw_others(ROOM **array,BALIEN *aliens,WHOLE *wormholes,int numaliens,int numwormholes)
{
   if(numaliens>0)
      for(int i=0;i<numaliens;i++)
         if(aliens[i].numhealth>0)
            if((array[aliens[i].x][aliens[i].y].bits&HIDE)==0)
               BAS(aliens[i].x*WALL_LEN,aliens[i].y*WALL_LEN);
   if(numwormholes>1)
      for(int i=0;i<numwormholes;i++)
           if((array[wormholes[i].x][wormholes[i].y].bits&HIDE)==0)
               WS(wormholes[i].x*WALL_LEN,wormholes[i].y*WALL_LEN);
}

void ingraph()
{
   allegro_init();
   install_timer();
   
   LOCK_VARIABLE(ticks);
   LOCK_FUNCTION(ticker);
   install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));
 
   
   install_keyboard();
   set_color_depth(8);
   set_gfx_mode(GFX_AUTODETECT,SW,SH,0,0);
   buffer=create_bitmap(SW,SH);
   bmps[0]=load_bmp("./bmps/top.bmp",pal);
   bmps[1]=load_bmp("./bmps/right.bmp",pal);
   bmps[2]=load_bmp("./bmps/bottom.bmp",pal);
   bmps[3]=load_bmp("./bmps/left.bmp",pal);
   bmps[4]=load_bmp("./bmps/end.bmp",pal);
   bmps[5]=load_bmp("./bmps/beg.bmp",pal);
   bmps[6]=load_bmp("./bmps/10l.bmp",pal);
   bmps[7]=load_bmp("./bmps/10h.bmp",pal);
   bmps[8]=load_bmp("./bmps/bomb.bmp",pal);
   bmps[9]=load_bmp("./bmps/ship.bmp",pal);
   bmps[10]=load_bmp("./bmps/shipl.bmp",pal);
   bmps[11]=load_bmp("./bmps/alien.bmp",pal);
   bmps[12]=load_bmp("./bmps/laser.bmp",pal);
   bmps[13]=load_bmp("./bmps/laserl.bmp",pal);
   bmps[14]=load_bmp("./bmps/explode.bmp",pal);
   bmps[15]=load_bmp("./bmps/bigalien.bmp",pal);
   bmps[16]=load_bmp("./bmps/wormhole.bmp",pal);
   intro=load_bmp("./bmps/intro.bmp",pal);
   instruct=load_bmp("./bmps/instruct.bmp",pal);
   set_palette(pal);
   clear(buffer);
}
