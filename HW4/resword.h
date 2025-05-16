/********************** resword.h ******************/
#ifndef RESWORD_H
#define RESWORD_H

#include <stdlib.h>
#include "sym.h"
#define RESWORDMAX 13

typedef struct {
    char* word;
    int symValue;
} ReservedWord;

ReservedWord reswords[RESWORDMAX] = {
    {"BEGIN",     symBEGIN},
    {"CALL",      symCALL},
    {"CONST",     symCONST},
    {"DO",        symDO},
    {"END",       symEND},
    {"IF",        symIF},
    {"PROCEDURE", symPROCEDURE},
    {"PROGRAM",   symPROGRAM},
    {"READ",      symREAD},
    {"THEN",      symTHEN},
    {"VAR",       symVAR},
    {"WHILE",     symWHILE},
    {"WRITE",     symWRITE}
};

int ResOrIdent(char *s)
{
  for (int i=0; i<RESWORDMAX; i++)
  {
    if (strcmp(s, reswords[i].word)==0) return reswords[i].symValue;
  }
  return symIDENTIFIER;
}

int isResword(int sym)
{
  for (int i=0; i<RESWORDMAX; i++)
  {
    if(sym==reswords[i].symValue)
    {
      return 1;
    }
  }

  return -1;
}

#endif // RESWORD_H