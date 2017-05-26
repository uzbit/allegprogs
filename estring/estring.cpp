#include<string.h>
#include<iostream>
#include<istream>
#include<ostream>
#include<stdlib.h>

#define MAX_LEN 257

estring::estring()
{
   myLength=0;
   myString=NULL;
}
estring::estring(const estring &string1)
{
   myLength=string1.length();
   myString=new char[myLength+1];
   strcpy(myString,string1.string());
}
estring::estring(const char *string1)
{
   myLength=strlen(string1);
   myString=new char [myLength+1];
   strcpy(myString,string1);
}
estring::~estring()
{
   delete myString;
   myLength=0;
}
void estring::new_length(int len)
{
   char temp[myLength+1];
   memcpy(temp,myString,sizeof(myString));
   temp[myLength]='\0';
   delete [] myString;
   myString=new char[len+1];
   memcpy(myString,temp,sizeof(temp));
   myLength=len;
}
int estring::find(const char ch)
{
   for (int i=0;i<(myLength+1);i++)
      if(myString[i]==ch)
	 return i;
   return -1;
}

//OPERATORS/////////////////////////////////////////////////////
estring& estring::operator = (const estring &string1)
{
   if(myString != string1.string())
   {
      myLength=string1.length();
      delete [] myString;
      myString=new char[myLength+1];
      strcpy(myString,string1.string());
   }
   return *this;
}
estring& estring::operator = (const  char *string1)
{
   if(myString != string1)
   {
      myLength=strlen(string1);
      delete [] myString;
      myString=new char[myLength+1];
      strcpy(myString,string1);
   }
   return *this;
}
estring& estring::operator += (const estring &string1)
{
   int len=myLength+string1.length();
   myLength=len;
   strcat(myString,string1.string());
   return *this;
}
estring& estring::operator += (const char *string1)
{
   int len=strlen(string1)+myLength;
   myLength=len;
   strcat(myString,string1);
   return *this;
}
estring& estring::operator += (const char ch)
{
   char tempch[2]={ch,'\0'};
   myLength+=1;
   strcat(myString,tempch);
   return *this;
}
char& estring::operator [] (int index)
{
   if(index<myLength)
      return myString[index];
}
bool operator < ( const estring &string1, const estring &string2)
{
   return stricmp(string1.string(),string2.string())<0;
}
bool operator < (const  estring &string1, const char *string2)
{
   return stricmp(string1.string(),string2)<0;
}

bool operator > ( const estring &string1, const estring &string2)
{
   return stricmp(string1.string(),string2.string())>0;
}
bool operator > ( const estring &string1, const char *string2)
{
   return stricmp(string1.string(),string2)>0;
}

bool operator == ( const estring &string1, const estring &string2)
{
   return stricmp(string1.string(),string2.string())==0;
}
bool operator == ( const estring &string1, const char *string2)
{
   return stricmp(string1.string(),string2)==0;
}
bool operator != (const  estring &string1, const char *string2)
{
   return !(stricmp(string1.string(),string2)==0);
}
bool operator != (const  estring &string1, const estring &string2)
{
   return !(stricmp(string1.string(),string2.string())==0);
}

istream& operator >> (istream &is,estring &string1)
{
   char tempstring[MAX_LEN];
   is>>tempstring;
   string1=tempstring;
   return is;
}
ostream& operator << (ostream &os, estring &string1)
{
   return os<<string1.string();
}

istream& getline(istream &is,estring &string1)
{
   char tempString[MAX_LEN];
   is.getline(tempString,MAX_LEN,'\n');
   string1=tempString;
   return is;
}
