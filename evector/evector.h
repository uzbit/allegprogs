#include<iostream.h>
#include<stdlib.h>
#include<conio.h>

#ifndef _VECTOR_H
#define _VECTOR_H

template<class itype>
class evector
{
   public:
   evector();
   evector(int size);
   ~evector();

   int size(){return mysize;}
   itype& operator[](int index);
   void resize(int newsize);

   protected:
   int mysize;
   itype *mydata;
};
template<class itype>
evector<itype>::evector()
:mysize(0), mydata(0)
{
}
template<class itype>
evector<itype>::evector(int size)
:mysize(0), mydata(0)
{
   mysize=size;
   mydata=new itype[mysize];
}
template<class itype>
evector<itype>::~evector()
{
   delete mydata;
   mysize=0;
}
template<class itype>
itype& evector<itype>::operator[](int index)
{
   if(index>=mysize || index<0)
   {
      cout<<"ERROR IN INDEX: "<<index<<" MAX INDEX: "<<mysize-1<<'\n';
      getch();
      exit(1);
   }
   return mydata[index];
}
template<class itype>
void evector<itype>::resize(int newsize)
{
   itype *temp=new itype[mysize];
   for(int i=0;i<mysize;i++)
      temp[i]=mydata[i];
   delete [] mydata;
   mydata=new itype[newsize];
   for(int v=0;v<mysize;v++)
      mydata[v]=temp[v];
   mysize=newsize;
   delete temp;
}
#endif
