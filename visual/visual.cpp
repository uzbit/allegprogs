#include<allegro.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
//#include<dos.h>
#include<unistd.h>
#include<time.h>

using namespace std;

#define SW 640
#define SH 480

//global variables
BITMAP *buffer,*mouse;
PALETTE mpalette;
int type,sorted,SIZE=10;
float delay_time;
unsigned long int ifs;

struct data
{
   int dat,color;
   int x;
   const data& operator=(const data &rhs);
};

void swap(data &num1,data &num2)
{
   data temp=num1;
   num1=num2;
   num2=temp;
}


volatile int ticks = 0;
void ticker()
{
	ticks++;
}
END_OF_FUNCTION(ticker)

const int updates_per_second = 30;

void ingraph();
void fill(data array[]);
void print(data array[]);
void menu();
void save();
void quick(data array[], int left,int right);
void select(data array[],int length);
void insert(data array[]);
void shell(data array[]);
void bubble(data array[]);
void merge(data array[], int first, int last);
void merge_data(data array[], int first, int mid, int last);

int main()
{
   data *array;
   ingraph();
   do{
     
	  	
	  delay_time=0;sorted=10;type=10;
      ifs=0;
      menu();
      array=new data[SIZE];
      fill(array);
      if(!sorted)
         for(int v=0;v<SIZE;v++)
            swap(array[v],array[(rand()%SIZE)]);
      else if(sorted==2)
         for(int i=0;i<SIZE/2;i++)
            swap(array[i],array[(SIZE-i)-1]);
      switch(type)
      {
         case 1: select(array,SIZE);break;
         case 2: quick(array,0,SIZE);break;
         case 3: insert(array);break;
         case 4: shell(array);break;
         case 5: bubble(array);break;
         case 6: merge(array,0,SIZE);break;
         default:break;
      }
     // save();
	  delete [] array;
	  
   }while(type && !key[KEY_ESC]);
   //while(!key[KEY_ESC]);
   return 0;
}
END_OF_MAIN();

void ingraph()
{
   if (allegro_init() != 0)
        exit(0);
        
   install_mouse();
   install_keyboard();
   install_timer();
   
   
//   LOCK_VARIABLE(ticks);
 //  LOCK_FUNCTION(ticker);
 //  install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));
 

   set_color_depth(8);
   set_gfx_mode(GFX_QUARTZ_WINDOW,SW,SH,0,0);

   buffer=create_bitmap(SW,SH);
   clear(buffer);
   mouse=load_bmp("./mouse.bmp",mpalette);
   acquire_screen();
}
void fill(data array[])
{
   for(int i=0;i<SIZE;i++)
   {
      array[i].dat=i+10;
      array[i].color=i+32;
      array[i].x=i*10;
   }
}
void print(data array[])
{
   char strtime[30];
   sprintf(strtime,"NUMBER OF IFS: %d",ifs);
   for(int i=0;i<SIZE;i++)
      rectfill(buffer,array[i].x,0,array[i].x+5,array[i].dat*4,array[i].color);
   switch(type)
   {
      case 1: textout_ex(buffer,font,"METHOD: select",0,350,255,0);break;
      case 2: textout_ex(buffer,font,"METHOD: quick",0,350,255,0);break;
      case 3: textout_ex(buffer,font,"METHOD: insert",0,350,255,0);break;
      case 4: textout_ex(buffer,font,"METHOD: shell",0,350,255,0);break;
      case 5: textout_ex(buffer,font,"METHOD: bubble",0,350,255,0);break;
      case 6: textout_ex(buffer,font,"METHOD: merge",0,350,255,0);break;
      default:break;
   }
   textout_ex(buffer,font,strtime,0,380,255,0);
   blit(buffer,screen,0,0,0,0,SW,SH);
   clear(buffer);
   sleep((int)delay_time);
}
/*
void save()
{
   ofstream fout;
   fout.open("sort.dat",ios::app);
   fout<<"SIZE OF ARRAY: "<<SIZE<<'\n';
   switch(type)
   {
      case 1: fout<<"METHOD: select"<<'\n';break;
      case 2: fout<<"METHOD: quick"<<'\n';break;
      case 3: fout<<"METHOD: insert"<<'\n';break;
      case 4: fout<<"METHOD: shell"<<'\n';break;
      case 5: fout<<"METHOD: bubble"<<'\n';break;
      case 6: fout<<"METHOD: merge"<<'\n';break;
      default:break;
   }
   switch(sorted)
   {
      case 1: fout<<"DATA: sorted inc."<<'\n';break;
      case 2: fout<<"DATA: sorted dec."<<'\n';break;
      case 0: fout<<"DATA: random"<<'\n';break;
      default:break;
   }
   fout<<"NUMBER OF IFS: "<<ifs<<"\n\n";
   fout.close();
}*/
void quick(data array[], int left, int right)
{
   if(right <= left)
      return;
   data pivot=array[left];
   int lo=left, hi=right;

   while(lo<hi)
   {
		while((pivot.dat<array[hi].dat) && (lo<hi))
			hi--;
        ifs++;
	    if(hi!=lo)
		{
            array[lo] = array[hi];
			lo++;
		}

		while((pivot.dat > array[lo].dat) && (lo<hi))
			lo++;
        ifs++;
		if(hi!=lo)
		{

			array[hi]=array[lo];
			hi--;
		}
        print(array);
	}
	array[hi]=pivot;

    quick(array,left,hi-1);
    quick(array,hi+1,right);
}
void select(data array[],int length)
{
   int min=0;
   for(int i=0;i<length-1;i++)
   {
      min=i;
      for(int v=i;v<length;v++)
      {
         ifs++;
         if(array[v].dat<array[min].dat)
            min=v;
      }
      ifs++;
      if(min!=i)
         swap(array[min],array[i]);
      print(array);
   }
}
void insert(data array[])
{
   int j,i;
   data temp;
   bool looking;
   for(i=1;i<SIZE;i++)
   {
      temp=array[i];
      j=i-1;
      looking=1;
      while(j>=0 && looking)
      {
         ifs++;
         if(temp.dat<array[j].dat)
         {
            array[j+1]=array[j];
            j--;
         }
         else looking=0;
      }
      array[j+1]=temp;
      print(array);
   }
}
void shell(data array[])
{
   int k=SIZE/2,v;
   data temp;
   bool looking;
   while(k > 0)
   {
      for(int i=k;i<SIZE;i++)
      {
         temp=array[i];
         v=i-k;
         looking=1;
         while(v >= 0 && looking)
         {
            ifs++;
            if(temp.dat < array[v].dat)
            {
               array[v+k]=array[v];
               v=v-k;
            }
            else looking=0;
            print(array);
         }
         array[v+k]=temp;
         print(array);
      }
      k=k/2;
   }
}
void bubble(data array[])
{
   bool swap_made=1;
   int i=0;

   do{
      swap_made=0;
      for(int v=0;v<SIZE-i;v++)
      {
         ifs++;
         if(array[v].dat > array[v+1].dat)
         {
            swap(array[v],array[v+1]);
            swap_made=1;
         }
         print(array);
      }
      i++;
   }while(i<SIZE-1 && swap_made);
}
void merge_data(data array[], int first, int mid, int last)
{
   int I,J,K;
   data temp[(last-first)+1];
   I = first;  J = mid+1;  K = 0;
   while ((I <= mid) && (J <= last))
   {
      ifs++;
      if (array[I].dat <= array[J].dat)
      {
	    temp[K] = array[I];
	    I++;
      }
      else
      {
	    temp[K] = array[J];
	    J++;
      }
      K++;
      print(array);
   }
   while (I <= mid)
   {
      temp[K] = array[I];
      I++;
      K++;
      print(array);
   }
   while (J <= last)
   {
      temp[K] = array[J];
      J++;
      K++;
      print(array);
   }
   for (K = 0,J = first; J <= last; K++,J++)
   {
      array[J] = temp[K];
      print(array);
   }
}

void merge(data array[], int first, int last)
{
   int mid;
   ifs++;
   if (first < last)
   {
      mid = (first + last) / 2;
      merge(array,first,mid);
      merge(array,mid+1,last);
      merge_data(array,first,mid,last);
   }
}
const data& data::operator=(const data &rhs)
{
   color=rhs.color;
   dat=rhs.dat;
   return *this;
}
void menu()
{
   bool start=0;
   char delaytime[40];
   char sizeofdata[40];

   while(!start)
   {
      switch(type)
      {
         case 1: textout_ex(buffer,font,"type: select",100,0,255,0);break;
         case 2: textout_ex(buffer,font,"type: quick",100,0,255,0);break;
         case 3: textout_ex(buffer,font,"type: insert",100,0,255,0);break;
         case 4: textout_ex(buffer,font,"type: shell",100,0,255,0);break;
         case 5: textout_ex(buffer,font,"type: bubble",100,0,255,0);break;
         case 6: textout_ex(buffer,font,"type: merge",100,0,255,0);break;
         default:break;
      }
      sprintf(delaytime,"delay: %f seconds",(delay_time/1000));
      textout_ex(buffer,font,delaytime,100,240,255,0);
      sprintf(sizeofdata,"size: %d",(SIZE));
      textout_ex(buffer,font,sizeofdata,100,160,255,0);
      switch(sorted)
      {
         case 1: textout_ex(buffer,font,"choice: sorted inc.",100,320,255,0);break;
         case 2: textout_ex(buffer,font,"choice: sorted dec.",100,320,255,0);break;
         case 0: textout_ex(buffer,font,"choice: random",100,320,255,0);break;
         default:break;
      }
      textout_ex(buffer,font,"METHOD:",10,0,255,0);
      textout_ex(buffer,font,"select",10,20,255,0);
      textout_ex(buffer,font,"quick",10,40,255,0);
      textout_ex(buffer,font,"insert",10,60,255,0);
      textout_ex(buffer,font,"shell",10,80,255,0);
      textout_ex(buffer,font,"bubble",10,100,255,0);
      textout_ex(buffer,font,"merge",10,120,255,0);

      textout_ex(buffer,font,"SIZE:",10,160,255,0);
      textout_ex(buffer,font,"increase",10,180,255,0);
      textout_ex(buffer,font,"decrease",10,200,255,0);

      textout_ex(buffer,font,"DELAY:",10,240,255,0);
      textout_ex(buffer,font,"increase",10,260,255,0);
      textout_ex(buffer,font,"decrease",10,280,255,0);

      textout_ex(buffer,font,"DATA:",10,320,255,0);
      textout_ex(buffer,font,"sorted inc.",10,340,255,0);
      textout_ex(buffer,font,"sorted dec.",10,360,255,0);
      textout_ex(buffer,font,"random",10,380,255,0);
      textout_ex(buffer,font,"START",400,240,40,0);
      textout_ex(buffer,font,"EXIT",10,460,255,0);
      if(mouse_b&1)
      {
         if(mouse_x>=10 && mouse_x<=90 && mouse_y>=10 && mouse_y <=30)
            type=1;
         if(mouse_x>=10 && mouse_x<=90 && mouse_y>=30 && mouse_y <=50)
            type=2;
         if(mouse_x>=10 && mouse_x<=90 && mouse_y>=50 && mouse_y <=70)
            type=3;
         if(mouse_x>=10 && mouse_x<=90 && mouse_y>=70 && mouse_y <=90)
            type=4;
         if(mouse_x>=10 && mouse_x<=90 && mouse_y>=90 && mouse_y <=110)
            type=5;
         if(mouse_x>=10 && mouse_x<=90 && mouse_y>=110 && mouse_y <=130)
            type=6;

         if(SIZE+1 <=60 && mouse_x>=10 && mouse_x<=90 && mouse_y>=170 && mouse_y <=190)
            SIZE+=1;
         if(SIZE-1 >= 10 && mouse_x>=10 && mouse_x<=90 && mouse_y>=190 && mouse_y <=210)
            SIZE-=1;
         
         if(mouse_x>=10 && mouse_x<=90 && mouse_y>=250 && mouse_y <=270)
            delay_time+=10;
         if(delay_time-10 >= 0 && mouse_x>=10 && mouse_x<=90 && mouse_y>=270 && mouse_y <=290)
            delay_time-=10;
         if(mouse_x>=10 && mouse_x<=90 && mouse_y>=330 && mouse_y <=350)
            sorted=1;
         if(mouse_x>=10 && mouse_x<=90 && mouse_y>=350 && mouse_y <=370)
            sorted=2;
         if(mouse_x>=10 && mouse_x<=90 && mouse_y>=370 && mouse_y <=390)
            sorted=0;
         if(mouse_x>=10 && mouse_x<=90 && mouse_y>=450 && mouse_y <=470)
            exit(1);
         if(type!=10 && mouse_x>=400 && mouse_x<=460 && mouse_y>=230 && mouse_y <=250)
            start=1;
         
      }
      draw_sprite(buffer,mouse,mouse_x,mouse_y);
      blit(buffer,screen,0,0,0,0,SW,SH);
      clear(buffer);
   }
   clear_keybuf();
}

