FILEIO::FILEIO()
{
   myhandle=0;
}
bool FILEIO::newfile(char *filename, int access)
{
   if(access>=0 && access<3 && filename)
   {
      myhandle=0;
      char temp=access;
      //make a file with name=filename
      __asm__ __volatile__
      ("xorl %%eax,%%eax\n
        movb $0x3c,%%ah\n
        movb %2,%%cl\n
        movw %1,%%dx\n
        int $0x21\n
        jc EXISTS\n
        movl %%eax,%0\n
        EXISTS:"
        :"=g" (myhandle)
        :"g" (filename), "g"(temp)
        :"ecx","eax","edx","memory"
      );
      if(myhandle)
         return 1;
   }
   return 0;
}
char FILEIO::read_byte()
{
   char temp;
   __asm__ __volatile__
   ("clc\n
     xorl %%eax,%%eax\n
     movb $0x3f,%%ah\n
     movb $1,%%cl\n
     movl %1,%%ebx\n
     int $0x21\n"
     :"=g" (temp)
     :"g" (myhandle)
     :"eax","edx","memory"
   );
   return temp;
}

/*void FILE::copy(char *file1, char *file2)
{
}*/

bool FILEIO::open(char *filename, int access)
{
   if(access>=0 && access<3 && filename)
   {
      char temp=access;
      __asm__ __volatile__
      ("clc\n
        xorl %%eax,%%eax\n
        movb $0x3d,%%ah\n
        movb %2,%%al\n
        lea %1,%%dx\n
        int $0x21\n
        movl %%eax,%0\n"
        :"=g" (myhandle)
        :"g" (filename),"g" (temp)
        :"eax","edx","memory"
      );
      if(myhandle)
         return 1;
   }
   return 0;
}
bool FILEIO::close()
{
   if(myhandle)
   {
      __asm__ __volatile__
      ("xorl %%eax,%%eax\n
        movb $0x3e,%%ah\n
        movl %0,%%ebx\n
        int $0x21\n"
        :
        :"g" (myhandle)
        :"eax","ebx"
      );
      return 1;
   }
   return 0;
}
