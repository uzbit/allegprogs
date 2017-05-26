#include<allegro.h>
#include<iostream>
//#include<conio.h>
//#include<dos.h>
#include<stdio.h>
#include<time.h>
#include"missile.h"
#include"player.h"
#include"../esorts/esorts.h"

#define SW 800
#define SH 600

BITMAP *buffer,*ground,*back,*tank[2];
PALETTE palette;
int numplayers,turn=0;

void ingraph();
void menu(PLAYER *&p);
bool handle_players(PLAYER *p,MISSILE &m);
void handleio(PLAYER &p,MISSILE &m);
bool handle_hits(PLAYER *p,MISSILE &m);
void move_tanks(PLAYER *p);
void make_ground();
void draw_hit(int x,int y,int radius);
void draw_all(PLAYER *p,MISSILE &m);
void destroy_bmps()
{
   for(int i=0;i<2;i++)
      destroy_bitmap(tank[i]);
   destroy_bitmap(back);
   destroy_bitmap(buffer);
   destroy_bitmap(ground);
}

volatile int ticks = 0;
void ticker()
{
	ticks++;
}
END_OF_FUNCTION(ticker)

const int updates_per_second = 30;

int main(int argc,char **argv)
{
   bool temphit=0;
   PLAYER *players;
   MISSILE missile;
   menu(players);
   ingraph();
   while(!keypressed())
   {
      draw_sprite(buffer,back,0,0);
      textout_centre_ex(buffer,font,"Version 1.01",SW/2,SH/2+200,255,0);
      acquire_screen();
      blit(buffer,screen,0,0,0,0,SW,SH);
      release_screen();
      clear(buffer);
   }
   for(int i=0;i<numplayers;i++)
      players[i].init(players,numplayers);
   make_ground();
   move_tanks(players);
   turn=rand()%numplayers;
   while(!key[KEY_ESC])
   {
	  while(ticks == 0)
	  {
			rest(1);//rest until a full tick has passed
	  }
	
	  while(ticks > 0)
	  {
			int old_ticks = ticks;
 	
      if(!handle_players(players,missile))
      {
         while(!keypressed())
         {
            draw_sprite(buffer,back,0,0);
            acquire_screen();
            blit(buffer,screen,0,0,0,0,SW,SH);
            release_screen();
            clear(buffer);
         }
         clear(ground);
         make_ground();
         for(int i=0;i<numplayers;i++)
            players[i].init(players,numplayers);
         move_tanks(players);
         turn=rand()%numplayers;
      }
      if(missile.is)
      {
         missile.process();
         temphit=handle_hits(players,missile);
         if(temphit)
         {
            turn++;
            move_tanks(players);
            clear_keybuf();
            if(turn==numplayers)
               turn=0;
         }
      }
	  
	  ticks--;
	  if(old_ticks <= ticks)//logic is taking too long: abort and draw a frame
		 break; 
		
	  }
      draw_all(players,missile);
	  clear_keybuf();
   }
   destroy_bmps();

   return 0;
}
END_OF_MAIN();

void ingraph()
{
   allegro_init();
   install_keyboard();
   install_timer();

   LOCK_VARIABLE(ticks);
   LOCK_FUNCTION(ticker);
   install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));
 
   if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, SW,SH,0,0) != 0)
       {printf("GFX_MODE not set\n"); exit(0);}
   //generate_332_palette(palette);
   
   set_color_depth(8);

   buffer=create_bitmap(SW,SH);
   ground=create_bitmap(SW,SH);
   tank[0]=load_bmp("./bmps/tank.bmp",palette);
   tank[1]=load_bmp("./bmps/turret.bmp",palette);
   back=load_bmp("./bmps/dietime.bmp",palette);

   if (back==NULL)
        printf("back not set\n");
   if (tank[0]==NULL)
        printf("tank not set\n");
   if (buffer==NULL)
        printf("buffer not set\n");
//   get_palette(pal);

   set_palette(palette);
   
   clear(buffer);
   clear(ground);
   clear_keybuf();
}

void menu(PLAYER *&p)
{
   //clrscr();
   char temp;
   int compplayers;
  /* do{
      cout<<"Would you like to have computer players? (y/n): ";
      cin>>temp;
   }while((temp!='n' && temp!='y') && (temp!='N' && temp!='Y'));
   switch(temp)
   {
      case 'n':
      case 'N':break;
      case 'y':
      case 'Y':
      do{
         cout<<"Type in the number of total players (up to 6): ";
         cin>>numplayers;
      }while(numplayers<=1 || numplayers>6);
      do{
         cout<<"Type in the number of computer players (up to 6): ";
         cin>>compplayers;
      }while(compplayers<=1 || compplayers>6);
      do{
         cout<<"Type in the number of computer players (up to 6): ";
         cin>>compplayers;
      }while(compplayers<=1 || compplayers>6);


      break;

   }*/
   do{
      cout<<"Type in the number of human players (up to 6): ";
      cin>>numplayers;
   }while(numplayers<=1 || numplayers>6);
   cin.ignore(80,'\n');
   p=new PLAYER[numplayers];
   for(int i=0;i<numplayers;i++)
   {
      do{
         cout<<"Type in the name for player "<<i+1<<": ";
         getline(cin,p[i].name);
      }while(p[i].name.length()>8);
   }
}

bool handle_players(PLAYER *p,MISSILE &m)
{
   int temp=0;
   if(p[turn].health>0)
      handleio(p[turn],m);
   else
      turn++;
   if(turn==numplayers)
      turn=0;
   for(int i=0;i<numplayers;i++)
      if(p[i].health<=0)
         temp++;
   if(numplayers-temp==1)
   {
      for(int i=0;i<numplayers;i++)
         if(p[i].health>0)
         {
            p[i].points+=(numplayers-1)*10;
            draw_all(p,m);
            textprintf_centre_ex(screen,font,SW/2,SH/2,255,0,"%s WINS!!",p[i].name.string());
            rest(100);
            clear_keybuf();
            while(!keypressed());
            clear_keybuf();
            return 0;
         }
   }
   else if(numplayers-temp==0)
   {
      draw_all(p,m);
      textprintf_centre_ex(screen,font,SW/2,SH/2,255,0,"TIE FOR FIRST!!");
      rest(100);
      clear_keybuf();
      while(!keypressed());
      clear_keybuf();
      return 0;
   }
   return 1;
}
void handleio(PLAYER &p,MISSILE &m)
{
   if(!m.is)
   {
      if(p.power+1<=25)
         if(key[KEY_UP])
            p.power++;
      if(p.power-1>=0)
         if(key[KEY_DOWN])
            p.power--;
      if(p.angle-1>=0)
         if(key[KEY_RIGHT])
            p.angle--;
      if(p.angle+1<=180)
         if(key[KEY_LEFT])
            p.angle++;
      
      if(key[KEY_1] && p.nummissiles(0)>0)
         m.type=1;
      if(key[KEY_2] && p.nummissiles(1)>0)
         m.type=2;
      if(key[KEY_3] && p.nummissiles(2)>0)
         m.type=3;
      if(key[KEY_SPACE])
         m.init(p);
   }
}
bool handle_hits(PLAYER *p,MISSILE &m)
{
    int groundpix=getpixel(ground,(int)m.x,(int)m.y);;
    float deduction;
    bool hit=0;
    for(int i=0;i<numplayers;i++)
    {
       if(p[i].health>0 && m.y>200)
       {
          if((m.x>p[i].x && m.x<p[i].x+tank[0]->w && m.y>p[i].y && m.y<p[i].y+tank[0]->h) || groundpix!=0)
             hit=1;
          if(hit)
          {
              draw_all(p,m);
              draw_hit((int)m.x,(int)m.y,(int)(m.type*10)-1);
              for(int c=0;c<numplayers;c++)
              {
                 deduction=0;
                 for(int w=0;w<tank[0]->w;w++)
                    for(int h=0;h<tank[0]->h;h++)
                       if(getpixel(ground,p[c].x+w,p[c].y+h)==255)
                          deduction++;

                 deduction=(deduction/(tank[0]->w*tank[0]->h));
                 deduction*=100;
                 p[c].health-=(int)deduction;
                 if(c!=turn)
                    p[turn].points+=(int)deduction;
                 else p[turn].points-=(int)deduction;
              }
              m.is=0;
              break;
          }
       }
    }
    if(hit)
    {
       int temp[60][200];
       for(float y=m.y-(m.type*10);y<=m.y+(m.type*10);y++)
          for(float x=m.x-(m.type*10);x<=m.x+(m.type*10);x++)
             if(getpixel(ground,(int)x,(int)y)==255)
                putpixel(ground,(int)x,(int)y,0);
       for(int x=0;x<60;x++)
          for(int y=0;y<200;y++)
             temp[x][y]=getpixel(ground,x+((int)m.x-(m.type*10)),y+400);
       for(int c=0;c<60;c++)
          quick(temp[c],0,199);
       for(int x2=0;x2<60;x2++)
          for(int y2=0;y2<200;y2++)
             putpixel(ground,x2+((int)m.x-(m.type*10)),y2+400,temp[x2][y2]);
       m.type=1;
    }
    return hit;
}
void move_tanks(PLAYER *p)
{
   int pix;
   int check=0;
   for(int i=0;i<numplayers;i++)
      for(int v=0;v<600;v++)
      {
         for(int c=0;c<tank[0]->w;c++)
         {
            pix=getpixel(ground,p[i].x+c,v+(tank[0]->h-10));
            if(pix==makecol(139,121,97))
            {
               p[i].y=v;
               check=1;
               break;
            }
         }
         if(check)
         {
            check=0;
            break;
         }
      }
}
void make_ground()
{
   int oldy=(-rand()%150)+550,y;
   for(int x=0;x<=830;x+=30)
   {
      y=(-rand()%100)+550;
      line(ground,x,y,x-30,oldy,makecol(139,121,97));
      oldy=y;
   }
   floodfill(ground,1,599,makecol(139,121,97));
}
void draw_hit(int x,int y,int radius)
{
   for(int i=0;i<radius;i++)
      circle(screen,x,y,radius-i,makecol(255,(i*13),10));
   circlefill(ground,x,y,radius,255);
}
void draw_all(PLAYER *p,MISSILE &m)
{
   blit(ground,buffer,0,0,0,0,SW,SH);
   if(m.is)
      circlefill(buffer,(int)m.x,(int)m.y,m.type,255);
   for(int i=0;i<numplayers;i++)
   {
      if(p[i].health>0)
      {
         textprintf_ex(buffer,font,p[i].x,p[i].y-10,255,0,"%s",p[i].name.string());
         if(p[i].angle<=90)
         {
            draw_sprite(buffer,tank[0],p[i].x,p[i].y);
            pivot_sprite(buffer,tank[1],p[i].x+20,p[i].y+8,0,0,-itofix((256*p[i].angle)/360));
         }
         else
         {
            draw_sprite_h_flip(buffer,tank[0],p[i].x,p[i].y);
            pivot_sprite(buffer,tank[1],p[i].x+15,p[i].y+8,0,0,-itofix((256*p[i].angle)/360));
         }
      }
      textprintf_ex(buffer,font,425,i*10,255,0,"%s:  %d HEALTH  %d POINTS",p[i].name.string(),p[i].health,p[i].points);
   }
   textprintf_ex(buffer,font,0,0,255,0,"PLAYER: %s   WEAPON TYPE: %d   NUM LEFT: %d",p[turn].name.string(),m.type,p[turn].nummissiles(m.type-1));
   textprintf_ex(buffer,font,0,10,255,0,"ANGLE: %d   POWER: %d",p[turn].angle,p[turn].power);
   line(buffer,0,(numplayers+1)*10,800,(numplayers+1)*10,255);
   line(buffer,400,0,400,(numplayers+1)*10,255);
   acquire_screen();
   blit(buffer,screen,0,0,0,0,SW,SH);
   release_screen();
   clear(buffer);
}


