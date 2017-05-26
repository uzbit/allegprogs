#ifndef _ETREE_H
#define _ETREE_H

#ifndef NULL
#define NULL 0
#endif

template<class itype>
class etree
{
   public:
   etree();
   etree(etree<itype> &b);
   ~etree();

   int size(){return mysize;}
   void insert(const itype &data);
   itype find(const itype &data);
   void remove(const itype &data);
   void remove_all();
   void traverse(int order,void (*process)(const itype &data));
   etree<itype>& operator = (const etree<itype> &b);

   private:
   struct node
   {
      itype data;
      node *left;
      node *right;
   };
   node *mytop;
   int mysize;
   void $destruct(node *node_check);
   void $insert(const itype &data,node *&node_check);
   void $remove(const itype &data,node *node_check,etree<itype> &b);
   void $copy_to(node *node_check,node *node_to);

   void $pre_traverse(node *node_check,void (*process)(const itype &data));
   void $in_traverse(node *node_check,void (*process)(const itype &data));
   void $post_traverse(node *node_check,void (*process)(const itype &data));

   node* $find(const itype &data,node *node_check);
   node* $make_node(const itype &data);
};
template<class itype>
etree<itype>::etree():mytop(0),mysize(0)
{
}
template<class itype>
etree<itype>::etree(etree<itype> &b):mytop(0),mysize(0)
{
   if(b.mytop)
      $copy_to(b.mytop,mytop);
}
template<class itype>
etree<itype>::~etree()
{
   if(mytop)
      $destruct(mytop);
   mytop=NULL;
   mysize=0;
}
template<class itype>
void etree<itype>::insert(const itype &data)
{
   $insert(data,mytop);
}
template<class itype>
itype etree<itype>::find(const itype &data)
{
   node *temp=new node;
   temp=$find(data,mytop);
   if(temp)
      return temp->data;
   return NULL;
}
template<class itype>
void etree<itype>::remove(const itype &data)
{
   etree<itype> temp;
   $remove(data,mytop,temp);
   remove_all();
   $copy_to(temp.mytop,mytop);
}
template<class itype>
void etree<itype>::remove_all()
{
   if(mytop)
      $destruct(mytop);
   mytop=NULL;
   mysize=0;
}
template<class itype>
void etree<itype>::traverse(int order,void (*process)(const itype &data))
{
   if(order<0)
      $pre_traverse(mytop,process);
   else if(order==0)
      $in_traverse(mytop,process);
   else $post_traverse(mytop,process);
}
template<class itype>
etree<itype>& etree<itype>::operator = (const etree<itype> &b)
{
   remove_all();
   $copy_to(b.mytop,mytop);
}
template<class itype>
void etree<itype>::$destruct(node *node_check)
{
   if(node_check->left==NULL && node_check->right==NULL)
      delete node_check;
   else if(node_check->left!=NULL)
      $destruct(node_check->left);
   else if(node_check->right!=NULL)
      $destruct(node_check->right);
}
template<class itype>
void etree<itype>::$insert(const itype &data,node *&node_check)
{
   if(node_check==NULL)
   {
      node_check=$make_node(data);
      if(mysize==0)
         mytop=node_check;
      mysize++;
   }
   else if(data<node_check->data)
      $insert(data,node_check->left);
   else
      $insert(data,node_check->right);
}
template<class itype>
void etree<itype>::$remove(const itype &data,node *node_check,etree<itype> &b)
{
   if(node_check!=NULL)
   {
      if(node_check->data!=data)
         b.insert(node_check->data);
      $remove(data,node_check->left,b);
      $remove(data,node_check->right,b);
   }
}
template<class itype>
void etree<itype>::$copy_to(node *node_check,node *node_to)
{
   if(node_check!=NULL)
   {
      $insert(node_check->data,node_to);
      $copy_to(node_check->left,node_to);
      $copy_to(node_check->right,node_to);
   }
}
template<class itype>
void etree<itype>::$pre_traverse(node *node_check,void (*process)(const itype &data))
{
   if(node_check!=NULL)
   {
      process(node_check->data);
      $pre_traverse(node_check->left,process);
      $pre_traverse(node_check->right,process);
   }
}
template<class itype>
void etree<itype>::$in_traverse(node *node_check,void (*process)(const itype &data))
{
   if(node_check!=NULL)
   {
      $in_traverse(node_check->left,process);
      process(node_check->data);
      $in_traverse(node_check->right,process);
   }
}
template<class itype>
void etree<itype>::$post_traverse(node *node_check,void (*process)(const itype &data))
{
   if(node_check!=NULL)
   {
      $pre_traverse(node_check->left,process);
      $pre_traverse(node_check->right,process);
      process(node_check->data);
   }
}
template<class itype>
etree<itype>::node* etree<itype>::$find(const itype &data,node *node_check)
{
   if(node_check!=NULL)
   {
      if(data==node_check->data)
         return node_check;
      node *temp=new node;
      temp=$find(data,node_check->left);
      if(temp)
         return temp;
      temp=$find(data,node_check->right);
      if(temp)
         return temp;
   }
   return NULL;
}
template<class itype>
etree<itype>::node* etree<itype>::$make_node(const itype &data)
{
   node *new_node=new node;
   new_node->data=data;
   new_node->left=NULL;
   new_node->right=NULL;
   return new_node;
}


#endif
