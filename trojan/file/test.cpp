#include<iostream.h>
#include<stdio.h>
#include<conio.h>
#include"fileio.h"

void main()
{
   FILEIO file;

   file.newfile("vato.ss\0",2);
   cout<<file.handle();
   //file.open("e:\\programmin\\trojan\\file\\test.cpp\0",2);
   //cout<<file.read_byte();
   //file.close();
}
