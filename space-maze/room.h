#ifndef _ROOM_H
#define _ROOM_H

#define T (1<<0)
#define R (1<<1)
#define B (1<<2)
#define L (1<<3)

#define HEALTH    (1<<4)
#define LASER     (1<<5)
#define BOMB      (1<<6)
#define NEWLIFE   (1<<7)
#define BEG       (1<<8)
#define END       (1<<9)
#define ALIEN     (1<<10)
#define BIGALIEN  (1<<11)
#define WORMHOLE  (1<<12)
#define HIDE      (1<<13)

class ROOM
{
   public:
   //THE BITS
   unsigned int bits;

   ROOM();
   //CLEAR
   //this function clears the bits
   //according to the nib part
   //1 for walls
   //0 for status
   void clear(bool nib);
   //SET_WALLS
   //sets the walls bits to the combination
   //input
   void set_walls(int wall);
   //SET_STATUS
   //sets the status bits to the combination
   //input
   void set_status(int status);
   //DEFOG
   //clears the
   void defog();
};

ROOM::ROOM()
{
   bits=0;
}
void ROOM::clear(bool nib)
{
   if(nib)
   {
      bits|=15;
      bits^=15;
   }
   else
   {
          //268435455=(2^28)-1 other
          //4 bits are for walls
      bits|=268435455*16;
      bits^=268435455*16;
   }
}
void ROOM::set_walls(int wall)
{
   if(wall<=15)
      bits|=wall;
}
void ROOM::set_status(int status)
{
   bits|=status;
}
void ROOM::defog()
{
   if((bits&HIDE)==HIDE)
      bits^=HIDE;
}

#endif

