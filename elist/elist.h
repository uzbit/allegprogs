#ifndef _ELIST_H
#define _ELIST_H

#include<stdlib.h>
#include<conio.h>

//typedef int bool;

#ifndef NULL
#define NULL 0
#endif

template <class itype>
class elist
{
   public:

   elist();
   elist(elist<itype> &l);
   ~elist();

   void next()
   {
      if(!atend())
         mycurrent=mycurrent->next;
   }
   void previous()
   {
      if(!atbeg())
	 mycurrent=mycurrent->previous;
   }
   void last()
   {
      mycurrent=mylast;
   }
   void first()
   {
      mycurrent=myfirst;
   }
  
   int size(){return mysize;}
   bool atend(){return mycurrent==mylast || empty();}
   bool atbeg(){return mycurrent==myfirst || empty();}
   bool empty(){return mysize==0;}
   itype data(){return mycurrent->data;}
   void new_data(const itype &data){memcpy(mycurrent->data,data,sizeof(data));}
   void insert_before(const itype &data);
   void insert_after(const itype &data);
   void sorted_insert(const itype &data, bool (*compare)(const itype &n1,const itype &n2));
   void remove();
   void traverse(void (*process)(itype &n));
   itype& operator [] (int index);
   elist<itype>& operator = (elist<itype> &l);

   protected:
   struct node
   {
      itype data;
      node *next;
      node *previous;
   };
   typedef node *pnode;
   int mysize;
   pnode mycurrent,myfirst,mylast;
   pnode $make_node(const itype &data);
};

template<class itype>
elist<itype>::elist()
:mycurrent(0),myfirst(0),mylast(0),mysize(0)
{
}
template<class itype>
elist<itype>::elist(elist<itype> &l)
:mycurrent(0),myfirst(0),mylast(0),mysize(0)
{
   l.first();
   while(!l.atend())
   {
      insert_after(l.data());
      l.next();
   }
   insert_after(l.data());
   mysize=l.size();
}
template<class itype>
elist<itype>::~elist()
{
   first();
   while(!atend())
   {
      remove();
      next();
   }
   remove();
   mycurrent=NULL;
   myfirst=NULL;
   mysize=0;
}
template<class itype>
void elist<itype>::insert_before(const itype &data)
{
   pnode new_node=$make_node(data);
   if(empty())
   {
      myfirst=new_node;
      mylast=new_node;
   }
   else if(mycurrent==myfirst)
   {
      new_node->next=mycurrent;
      mycurrent->previous=new_node;
      myfirst=new_node;
   }
   else
   {
      new_node->previous=mycurrent->previous;
      new_node->next=mycurrent;
      mycurrent->previous->next=new_node;
      mycurrent->previous=new_node;
   }
   mycurrent=new_node;
   mysize++;
}
template<class itype>
void elist<itype>::insert_after(const itype &data)
{
   pnode new_node=$make_node(data);
   if(empty())
   {
      myfirst=new_node;
      mylast=new_node;
   }
   else if(mycurrent==mylast)
   {
      new_node->previous=mycurrent;
      mycurrent->next=new_node;
      mylast=new_node;
   }
   else
   {
      new_node->next=mycurrent->next;
      new_node->previous=mycurrent;
      mycurrent->next->previous=new_node;
      mycurrent->next=new_node;
   }
   mycurrent=new_node;
   mysize++;
}
template<class itype>
void elist<itype>::sorted_insert(const itype &data,bool (*compare)(const itype &n1,const itype &n2))
{
   if(empty())
   {
      insert_after(data);
      return;
   }
   first();
   while(!atend() && compare(data,mycurrent->data))
      next();
   if(atend() && compare(data,mycurrent->data))
      insert_after(data);
   else insert_before(data);
}
template<class itype>
void elist<itype>::remove()
{
   pnode junk=mycurrent;
   if(mycurrent!=NULL)
   {
      if(mycurrent==myfirst)
         myfirst=mycurrent->next;
      else if(mycurrent==mylast)
         mylast=mycurrent->previous;
      else mycurrent->previous->next=mycurrent->next;
      mycurrent=mycurrent->next;
      delete junk;
      mysize--;
   }
}
template<class itype>
void elist<itype>::traverse(void (*process)( itype &n))
{
   if(!empty())
   {
      first();
      while(!atend())
      {
         process(mycurrent->data);
         next();
      }
      process(mycurrent->data);
   }
}
template<class itype>
itype& elist<itype>::operator [] (int index)
{
   if(index>size() || index<0)
   {
      cout<<"\nERROR IN INDEX "<<index<<" MAX INDEX: "<<mysize-1<<'\n';
      getch();
      exit(1);
   }
   int i=0;
   first();
   while(!atend())
   {
      if(i==index)
         return mycurrent->data;
      next();
      i++;
   }
   if(i==index)
      return mycurrent->data;
}
template<class itype>
elist<itype>& elist<itype>::operator = (elist<itype> &l)
{
   l.first();
   while(!l.atend())
   {
      insert_after(l.data());
      l.next();
   }
   insert_after(l.data());
   mysize=l.size();
   return *this;
}
template<class itype>
elist<itype>::pnode elist<itype>::$make_node(const itype &data)
{
   pnode new_node=new node;
   new_node->data=data;
   new_node->next=NULL;
   new_node->previous=NULL;
   return new_node;
}

#endif
