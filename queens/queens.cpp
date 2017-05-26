//#include<conio.h>
#include<iostream.h>
#include<stdlib.h>

int size;
int sols;

bool valid(bool **array,int posx,int posy);
void put_queen(bool **array,int y);
void print(bool **array);

int main()
{
   bool **array;
   //clrscr();
   do{
      sols=0;
      do{
         cout<<"\nType in the size of one side (0 to quit): ";
         cin>>size;
      }while(size<=3 && size);
      if(!size)
         exit(1);
      array=new bool*[size];
      for(int i=0;i<size;i++)
         array[i]=new bool[size];
   
      for(int y=0;y<size;y++)
        for(int x=0;x<size;x++)
           array[x][y]=0;

      put_queen(array,0);
      cout<<"Num Sols: "<<sols;
   }while(1);
   return 0;
}
bool valid(bool **array,int posx,int posy)
{
   //checks verticals
   int i=0;
   for(i=0;i<size;i++)
   {
      if(posy+i<size)
         if(array[posx][posy+i])
			   return 0;
      if(posy-i>=0)
         if(array[posx][posy-i])
			   return 0;
   }
   for(i=0;i<size;i++)
   {
      if(posx+i<size)
         if(array[posx+i][posy])
			   return 0;
      if(posx-i>=0)
         if(array[posx-i][posy])
			   return 0;
   }
   //checks diagonals
   for(i=0;i<size;i++)
   {
      if(posx+i<size && posy+i<size)
         if(array[posx+i][posy+i])
            return 0;
      if(posx-i>=0 && posy-i>=0)
         if(array[posx-i][posy-i])
            return 0;
      if(posx+i<size && posy-i>=0)
         if(array[posx+i][posy-i])
            return 0;
      if(posx-i>=0 && posy+i<size)
         if(array[posx-i][posy+i])
            return 0;
   }
   return 1;
}
void put_queen(bool **array,int y)
{
   for(int x=0;x<size;x++)
   {
     if(valid(array,x,y))
	  {
       array[x][y]=1;
		 if(y==size-1)
       {
          sols++;
		    //print(array);
          //getch();
       }
		 else put_queen(array,y+1);
	  }
     array[x][y]=0;
   }
}
void print(bool **array)
{
   cout<<"\nSolution Number: "<<sols<<'\n';
   for(int y=0;y<size;y++)
      for(int x=0;x<size;x++)
      {
         if(array[x][y])
            cout<<"  ";
         else cout<<" _ ";
         if(x==size-1)
            cout<<'\n';
      }
		cout<<'\n';
}

