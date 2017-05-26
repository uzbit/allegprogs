//T.J. McCormack session 3
//MAPEDIT is a tool which allows you to create maps for
//the maze game.
//With this program you can load a previously saved map or
//create a completely new map

#include<allegro.h>
#include<fstream.h>
#include<iostream.h>
#include<unistd.h>
//#include<conio.h>
#include"../room.h"
#include"../../estring/estring.h"

//DEFINES USED FOR CONSTANTS
#define WALL_LEN 30
#define SW 640
#define SH 480
//FUNCTIONS WICH DRAW THE GIVEN BITMAPS AT THE
//X Y COORDS
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
#define AS(x,y) draw_sprite(buffer,bmps[10],x+2,y+2)
#define BAS(x,y) draw_sprite(buffer,bmps[11],x+2,y+2)
#define WS(x,y) draw_sprite(buffer,bmps[12],x+2,y+2)

//GLOBAL ALLEGRO STUFF
BITMAP *buffer,*bmps[13];
PALETTE pal;

//ingraph() :
//initializes allegro and sets the graphics for use
//this function also loads the different bitmaps into the
//array of bitmaps: bmps[14]
void ingraph();
//make_map(ROOM **array):
//this is the function that actually handles the io
//durring the map making process.  The two dimentional
//array of rooms is updated as the user is editing
void make_map(ROOM **array);
//estring load(ROOM **array):
//load prompts the user to type in a file name, then if
//the file name exists in the same directory it will open
//it and load the info, else it will enter the make_map function
//and allow the user to edit
estring load(ROOM **array);
//save(ROOM **array,estring &file):
//this function saves the ROOM array into a file
//with the given file name file
void save(ROOM **array,estring &file);
//void draw(ROOM **array,int posx,int posy):
//this function prints out all of the interface stuff
//as well as the array data
//called from make_map
void draw(ROOM **array,int posx,int posy);
//void draw_room(ROOM **array):
//this function checks each ROOM in the given array
//and prints the bitmaps on the buffer accordingly
//called by draw
void draw_room(ROOM **array);

void main()
{
   ROOM **array;
   estring file;

   array=new ROOM*[SW/WALL_LEN];
   for(int i=0;i<SW/WALL_LEN;i++)
      array[i]=new ROOM[(SH/WALL_LEN)-2];

   file=load(array);
   ingraph();
   make_map(array);
   save(array,file);
   for(int i=0;i<12;i++)
      destroy_bitmap(bmps[i]);
}
END_OF_MAIN();

void draw_room(ROOM **array)
{
   for(int y=0;y<(SH/WALL_LEN)-2;y++)
      for(int x=0;x<SW/WALL_LEN;x++)
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
         if((array[x][y].bits&BIGALIEN)==BIGALIEN)
           BAS(x*WALL_LEN,y*WALL_LEN);
         if((array[x][y].bits&WORMHOLE)==WORMHOLE)
           WS(x*WALL_LEN,y*WALL_LEN);
      }
}
void draw(ROOM **array,int posx,int posy)
{
   int temproom=0;
   draw_room(array);
   circlefill(buffer,posx+WALL_LEN/2,posy+WALL_LEN/2,5,255);

   textout(buffer,font,"C CLEAR",0,425,255);
   textout(buffer,font,"1 TOP",100,425,255);
   textout(buffer,font,"2 RIGHT",200,425,255);
   textout(buffer,font,"3 BOTTOM",300,425,255);
   textout(buffer,font,"4 LEFT",400,425,255);
   textout(buffer,font,"E END",500,425,255);
   textout(buffer,font,"SB BEG.",0,445,255);
   textout(buffer,font,"H +10 HEA.",100,445,255);
   textout(buffer,font,"L +10 LAS.",200,445,255);
   textout(buffer,font,"B BOMB",300,445,255);
   textout(buffer,font,"N NEW LIFE",400,445,255);
   textout(buffer,font,"A ALIEN",500,445,255);
   textout(buffer,font,"SA BIGALIEN",0,465,255);
   textout(buffer,font,"W WORMHOLE",100,465,255);


   line(buffer,98,422,98,480,255);
   line(buffer,198,422,198,480,255);
   line(buffer,298,422,298,480,255);
   line(buffer,398,422,398,480,255);
   line(buffer,498,422,498,480,255);
   line(buffer,0,422,640,422,255);
   blit(buffer,screen,0,0,0,0,SW,SH);
   clear(buffer);
}
void make_map(ROOM **array)
{
   int x=0,y=0;
   int hasbeg=0,hasend=0,wormholes=0;
   for(int y=0;y<(SH/WALL_LEN)-2;y++)
      for(int x=0;x<SW/WALL_LEN;x++)
      {
         if((array[x][y].bits&END)==END)
            hasend++;
         if((array[x][y].bits&BEG)==BEG)
            hasbeg++;
      }
   while(1)
   {
      if(key[KEY_UP] && y-1>=0)
         y--;
      else if(key[KEY_DOWN] && (y+1)*WALL_LEN<SH-60)
         y++;
      else if(key[KEY_LEFT] && x-1>=0)
         x--;
      else if(key[KEY_RIGHT] && (x+1)*WALL_LEN<SW-30)
         x++;

      if(key[KEY_1] || key[KEY_8_PAD])
         array[x][y].set_walls(T);
      if(key[KEY_2] || key[KEY_6_PAD])
         array[x][y].set_walls(R);
      if(key[KEY_3] || key[KEY_2_PAD])
         array[x][y].set_walls(B);
      if(key[KEY_4] || key[KEY_4_PAD])
         array[x][y].set_walls(L);

      if(key[KEY_E] && hasend==0)
      {
         array[x][y].set_status(END);
         hasend++;
      }
      else if((key[KEY_B] && (key[KEY_RSHIFT]||key[KEY_LSHIFT])) && hasbeg==0)
      {
         array[x][y].set_status(BEG);
         hasbeg++;
      }
      else if(key[KEY_H])
         array[x][y].set_status(HEALTH);
      else if(key[KEY_L])
         array[x][y].set_status(LASER);
      else if(key[KEY_B] && !key[KEY_RSHIFT] && !key[KEY_LSHIFT])
         array[x][y].set_status(BOMB);
      else if(key[KEY_N])
         array[x][y].set_status(NEWLIFE);
      else if(key[KEY_A] && (key[KEY_RSHIFT] || key[KEY_LSHIFT]))
         array[x][y].set_status(BIGALIEN);
      else if(key[KEY_A])
         array[x][y].set_status(ALIEN);
      else if(key[KEY_W])
      {
         array[x][y].set_status(WORMHOLE);
         wormholes++;
      }


      if(key[KEY_C])
      {
         if((array[x][y].bits&END)==END)
            hasend--;
         if((array[x][y].bits&BEG)==BEG)
            hasbeg--;
         if((array[x][y].bits&WORMHOLE)==WORMHOLE)
            wormholes--;
         
         array[x][y].clear(1);
         array[x][y].clear(0);
      }
      draw(array,x*WALL_LEN,y*WALL_LEN);

      if(key[KEY_ESC] && hasbeg==1 && hasend==1 && (wormholes>1 || wormholes==0))
         break;
      rest(75);
   }
   clear_keybuf();
}

void save(ROOM **array,estring &file)
{
   ofstream fout;
   fout.open(file.string());
   for(int y=0;y<(SH/WALL_LEN)-2;y++)
      for(int x=0;x<SW/WALL_LEN;x++)
         fout<<array[x][y].bits<<endl;

   fout.close();
}
estring load(ROOM **array)
{
   estring file;
   int temp;
   //clrscr();
   cout<<"Type in the file name (with .map ext.): ";
   cin>>file;

   if(!access(file.string(),0))
   {
      ifstream fin;
      fin.open(file.string());
      for(int y=0;y<(SH/WALL_LEN)-2;y++)
         for(int x=0;x<SW/WALL_LEN;x++)
            fin>>array[x][y].bits;

      fin.close();
   }
   return file;
}
void ingraph()
{
   allegro_init();
   install_timer();
   install_keyboard();
   set_color_depth(8);
   set_gfx_mode(GFX_AUTODETECT,SW,SH,0,0);
   buffer=create_bitmap(SW,SH);
   bmps[0]=load_bmp("../bmps/top.bmp",pal);
   bmps[1]=load_bmp("../bmps/right.bmp",pal);
   bmps[2]=load_bmp("../bmps/bottom.bmp",pal);
   bmps[3]=load_bmp("../bmps/left.bmp",pal);
   bmps[4]=load_bmp("../bmps/end.bmp",pal);
   bmps[5]=load_bmp("../bmps/beg.bmp",pal);
   bmps[6]=load_bmp("../bmps/10l.bmp",pal);
   bmps[7]=load_bmp("../bmps/10h.bmp",pal);
   bmps[8]=load_bmp("../bmps/bomb.bmp",pal);
   bmps[9]=load_bmp("../bmps/ship.bmp",pal);
   bmps[10]=load_bmp("../bmps/alien.bmp",pal);
   bmps[11]=load_bmp("../bmps/bigalien.bmp",pal);
   bmps[12]=load_bmp("../bmps/wormhole.bmp",pal);
   set_palette(pal);
   clear(buffer);
}
