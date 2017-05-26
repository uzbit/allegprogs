#ifndef _ESORTS_H
#define _ESORTS_H

template<class itype>
void _swap(itype &data1,itype &data2)
{
   itype temp=data1;
   data1=data2;
   data2=temp;
}
template<class itype>
void quick(itype *array, int left,int right);
template<class itype>
void select(itype *array,int length);
template<class itype>
void insert(itype *array,int length);
template<class itype>
void shell(itype *array,int length);
template<class itype>
void bubble(itype *array,int length);

template<class itype>
void quick(itype *array, int left,int right)
{
   int mleft=left,mright=right;
   itype piv=array[(left+right)/2];
   do{
      while(array[mleft]<piv)
         mleft++;
      while(array[mright]>piv)
         mright--;
      if(mleft<=mright)
      {
         _swap(array[mleft],array[mright]);
         mleft++;
         mright--;
      }
   }while(mleft<=mright);
   if(left < mright)
      quick(array, left, mright);
   if(mleft < right)
      quick(array, mleft, right);
}
template<class itype>
void select(itype *array,int length)
{
   int min;
   for(int i=0;i<length-1;i++)
   {
      min=i;
      for(int v=i;v<length;v++)
         if(array[v]<array[i])
            min=v;
      if(min!=i)
         _swap(array[min],array[i]);
   }
}
template<class itype>
void insert(itype *array,int length)
{
   int j,i;
   bool looking;
   itype temp;
   for(i=1;i<length;i++)
   {
      j=i-1;
      looking=1;
      temp=array[i];
      while(j>=0 && looking)
      {
         if(temp<array[j])
         {
            array[j+1]=array[j];
            j--;
         }
         else looking=0;
      }
      array[j+1]=temp;
   }
}
template<class itype>
void shell(itype *array,int length)
{
   int k=length/2,v;
   bool looking;
   itype temp;
   while(k > 0)
   {
      for(int i=k;i<length;i++)
      {
         v=i-k;
         looking=1;
         temp=array[i];
         while(v >= 0 && looking)
         {
            if(temp < array[v])
            {
               array[v+k]=array[v];
               v=v-k;
            }
            else looking=0;
         }
         array[v+k]=temp;
      }
      k=k/2;
   }
}
template<class itype>
void bubble(itype *array,int length)
{
   int i=0;
   bool swap_made;
   do{
      swap_made=0;
      for(int v=0;v<length-i;v++)
      {
         if(array[v] > array[v+1])
         {
            _swap(array[v],array[v+1]);
            swap_made=1;
         }
      }
      i++;
   }while(i<length-1 && swap_made);
}

#endif
