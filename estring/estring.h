#ifndef _ESTRING_H
#define _ESTRING_H

#include<string.h>
#include<iostream>
#include<istream>
#include<ostream>

using namespace std;
//typedef int bool;

class estring
{
   public:
   estring();
   estring(const estring &string1);
   estring(const char *string1);
   ~estring();

   int length()const {return myLength;}
   void new_length(int len);
   char* string()const {return myString;}
   char* makeCaps()const {return strupr(myString);}
   char* makeLower()const {return strlwr(myString);}

   int find(const char ch);

   estring& operator = ( const estring &string1);
   estring& operator = ( const char *string1);
   estring& operator += ( const estring &string1);
   estring& operator += ( const char *string1);
   estring& operator += ( const char ch);

   char& operator [] (int index);

   private:
   //THE STRING
   char *myString;
   int myLength;
};
istream& getline(istream &is,estring &string1);

bool operator < ( const estring &string1, const estring &string2);
bool operator < ( const estring &string1, const char *string2);

bool operator > ( const estring &string1, const estring &string2);
bool operator > ( const estring &string1, const char *string2);

bool operator == ( const estring &string1, const estring &string2);
bool operator == ( const estring &string1, const char *string2);

bool operator != ( const estring &string1, const estring &string2);
bool operator != ( const estring &string1, const char *string2);

istream& operator >> (istream &is,estring &string1);
ostream& operator << (ostream &os, estring &string1);

#include"estring.cpp"
#endif
