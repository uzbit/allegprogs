#ifndef _FILEIO_H
#define _FILEIO_H

//#ifndef __cplusplus
//   extern "C"{
//#endif

class FILEIO
{
   public:

   FILEIO();
   bool newfile(char *filename, int access);
   char read_byte();
   bool copy(char *file1, char *file2);
   bool open(char *filename, int access);
   bool close();
   int handle(){return myhandle;}

   protected:
   int myhandle;
};

#include"fileio.cpp"

//#ifdef __cplusplus
//   }
//#endif

#endif

