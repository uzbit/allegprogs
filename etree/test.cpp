#include<conio.h>
#include<iostream.h>
#include<stdio.h>
#include<stdlib.h>
#include"etree.h"
void print(const int &a)
{cout<<a<<endl;}

void main(int argc,char **argv)
{
   clrscr();
   int num=0;
   num=strtol(argv[1],NULL,10);
   etree <int> tree;
   if(num)
   {
      for(int i=0;i<num;i++)
         tree.insert(rand()%num);
      printf("done inserting %d\n",num);
      tree.remove_all();
      printf("done removing %d\n",num);
      tree.traverse(0,print);
      getch();
   }
   else
   {
      cout<<"Type in the number of ints: ";
      cin>>num;
      for(int i=0;i<num;i++)
         tree.insert(rand()%num);
      do{
         cout<<"Type in the number to find (0 to quit): ";
         cin>>num;
         cout<<tree.find(num)<<endl;
      }while(num);
   }
}
