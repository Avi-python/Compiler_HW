/************************* plone.c ************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
** �ۭq���Y��
*/
  #include "scanner.h"
  #include "err.h"
  #include "followsym.h"
  #include "idobj.h"
  #include "procobj.h"
/*
** �ۭq�`��
*/
  #define IDLEN      36
  #define BUFSIZE   512
  #define PROCDEPTH  36
/*
** �ۭq�禡�쫬
*/
  int Identifier();
  int Number();
  void IdentifierList();
  int Expression();
  int ExprPr();
  int Term();
  int TermPr();
  int Factor();
  void Condition();
  void WriteStatement();
  void ReadStatement();
  void WhileStatement();
  void IfStatement();
  void CompoundStatement();
  void CallStatement();
  void AssignmentStatement();
  void StatementList();
  void Statement();
  void ProcDeclaration();
  void VarDeclaration();
  void ConstDeclaration();
  void Block();
  void ProgramHead();
  void Program();
/*
** �����ܼ�
*/
  struct symbolTag *token;
  int errorCount = 0;
  struct idobjTag *idobj;
  char outname[IDLEN];
  FILE *outfile;
  int labelCount = 0;
  int level = -1;
  char progname[IDLEN];
  char procname[IDLEN];
  struct procobjTag *procStack[BUFSIZE];
  int procTop = 0;
  struct procobjTag *procobj = NULL;
  char id[IDLEN];
  char buf[BUFSIZE];
/*
** Error()
*/
  void Error(int n)
  {
    int j;
    printf("****");
    for (j=0; j<=token->right; j++) printf(" ");
    printf("^%d  %s\n",n, errmsgs[n]);
    errorCount++;
  }
/*
** checkexist()
*/
  int checkexist()
  {
    idobj=getIdobj(procStack[procTop-1], token->value);
    if (idobj==NULL)
      return 0;
    else
      return 1;
  }
/*
** skip()
*/
  void skip(char follows[], int n)
  {
    if (follows[token->sym]==0)
    {
      Error(n);
      while (follows[token->sym]==0)
        token = nextToken();
    }
  }
/*
** �y�k�W�h#1 <Program>
*/
  void Program()
  {
    ProgramHead();
    Block();
    if (token->sym != symPERIOD) Error(0);
  }
/*
** �y�k�W�h#2 <ProgramHead>
*/
  void ProgramHead()
  {
    struct procobjTag *p;
    if (strcmp(token->value,"PROGRAM")==0)
    {
      token = nextToken();
      if (token->sym == symIDENTIFIER)
      {
        strcpy(procname, token->value);
        p = newProcobj(procname);
        p->next = NULL;
        p->head = NULL;
        p->tail = NULL;
        procpush(p);
        procStack[procTop++] = p;
        // strcpy(progname, token->value);
        // strcpy(outname, token->value);
        // strcat(outname, ".asm");
        // outfile = fopen(outname, "w");
        ++labelCount;
        // sprintf(buf,
        //   ";************** %s ****************\n"
        //   ";\n"
        //   "\tORG\t100H\n"
        //   "\tJMP\t_start%d\n"
        //   "_intstr\tDB\t'     ','$'\n"
        //   "_buf\tTIMES 256 DB ' '\n"
        //   "\tDB 13,10,'$'\n",
        //   outname, labelCount);
        // fprintf(outfile, buf);
        // strcpy(buf, "%include\t\"dispstr.mac\"\n");
        // strcat(buf, "%include\t\"itostr.mac\"\n");
        // strcat(buf, "%include\t\"readstr.mac\"\n");
        // strcat(buf, "%include\t\"strtoi.mac\"\n");
        // strcat(buf, "%include\t\"newline.mac\"\n");
        // fputs(buf, outfile);
        token = nextToken();
        if (token->sym == symSEMI)
          token = nextToken();
        else
          Error(3);
      }
      else
        Error(2);
    }
    else
      Error(1);
  }
/*
** �y�k�W�h#3 <Block>
*/
  void Block()
  {
    ++level;
    if (strcmp(token->value, "CONST")==0)
      ConstDeclaration();
    if (strcmp(token->value, "VAR")==0)
      VarDeclaration();
    // sprintf(buf, "_start%d:\n", labelCount);
    // fprintf(outfile, buf);
    if (strcmp(token->value, "PROCEDURE")==0)
      ProcDeclaration();
    strcpy(procname, procStack[procTop-1]->name);
    CompoundStatement();
    --level;
  }
/*
** �y�k�W�h#4 <ConstDeclaration>
*/
  void ConstDeclaration()
  {
    if (strcmp(token->value, "CONST")==0)
    {
      token = nextToken();
      if (token->sym == symIDENTIFIER)
      {
        varlistadd(procStack[procTop-1], 
          newIdobj(token->value, token->sym, symCONST, 
          level, procStack[procTop-1]->name));
        // strcpy(id, procStack[procTop-1]->name);
        // strcat(id, "_");
        // strcat(id, token->value);
        // sprintf(buf, "%s\tDB\t", id);
        // fprintf(outfile, buf);
        token = nextToken();
      }
      if (token->sym == symEQ)
      {
        token = nextToken();
        // sprintf(buf, "'%s','$'\n", token->value);
        // fprintf(outfile, buf);
        token = nextToken();
        while (token->sym == symCOMMA)
        {
          token = nextToken();
          if (token->sym == symIDENTIFIER)
          {
            varlistadd(procStack[procTop-1], 
              newIdobj(token->value, token->sym, symCONST, 
              level, procStack[procTop-1]->name));
            // sprintf(buf, "%s_%s\tDB\t",
            //   procStack[procTop-1]->name, token->value);
            // fprintf(outfile, buf);
            token = nextToken();
          }
          if (token->sym == symEQ)
          {
            token = nextToken();
            // sprintf(buf, "'%s','$'\n", token->value);
            // fprintf(outfile, buf);
            token = nextToken();
          }
          else
            Error(5);
        }
        if (token->sym == symSEMI)
          token = nextToken();
        else
        {
          Error(6);
          skip(statement, 23);
          if (token->sym == symSEMI)
            token = nextToken();
        }
      }
      else
      {
        Error(5);
        skip(statement, 23);
        if (token->sym == symSEMI)
          token = nextToken();
      }
    }
    else
    {
      Error(4);
    }
  }
/*
** �y�k�W�h#5 <VarDeclaration>
*/
  void VarDeclaration()
  {
    if (strcmp(token->value, "VAR")==0)
    {
      token = nextToken();
      if (token->sym == symIDENTIFIER)
      {
        idobj=newIdobj(token->value,token->sym,
          symVAR, level, procStack[procTop-1]->name);
        varlistadd(procobjTop, idobj);
        // sprintf(buf, "%s_%s\tDW\t0\n",
        //   procStack[procTop-1]->name, token->value);
        // fprintf(outfile, buf);
        token = nextToken();
      }
      while (token->sym == symCOMMA)
      {
        token = nextToken();
        if (token->sym == symIDENTIFIER)
        {
          idobj=newIdobj(token->value,token->sym,symVAR,
                         level, procStack[procTop-1]->name);
          varlistadd(procobjTop, idobj);
        //   sprintf(buf, "%s_%s\tDW\t0\n",
        //     procStack[procTop-1]->name, token->value);
        //   fprintf(outfile, buf);
          token = nextToken();
        }
      }
      if (token->sym == symSEMI)
      {
        token = nextToken();
      }
      else
      {
        Error(6);
        skip(statement, 23);
        if (token->sym == symSEMI)
          token = nextToken();
      }
    }
    else
    {
      Error(7);
    }
  }
/*
** �y�k�W�h#6 <ProcDeclaration>
*/
  void ProcDeclaration()
  {
    int tail;
    char *procid;
    struct procobjTag *p;
    while (strcmp(token->value, "PROCEDURE")==0)
    {
      token = nextToken();
      tail = 0;
      if (token->sym == symIDENTIFIER)
      {
        strcpy(procname, token->value);
        p = newProcobj(procname);
        p->head = NULL;
        p->tail = NULL;
        varlistadd(
          procStack[procTop-1],
          newIdobj(procname, token->sym, symPROCEDURE,
          level, procStack[procTop-1]->name));
        procpush(p);
        procStack[procTop++] = p;
        tail = ++labelCount;
        // sprintf(buf,"\tJMP\t_go%d\n%s:\n"
        //             "\tJMP\t_start%d\n",
        //             tail, token->value, labelCount);
        // fprintf(outfile,buf);
        token = nextToken();
      }
      if (token->sym == symSEMI)
      {
        token = nextToken();
        Block();
        if (token->sym == symSEMI)
        {
        //   sprintf(buf, "\tRET\n_go%d:\n",tail);
        //   fprintf(outfile, buf);
          token = nextToken();
        }
        else
        {
          Error(6);
          skip(statement, 23);
          if (token->sym == symSEMI)
            token = nextToken();
        }
      }
      else
      {
        Error(6);
      }
      --procTop;
    }
  }

/*
** <StatementList>
*/
void StatementList()
{
  if(token->sym == symIF || token->sym == symBEGIN || token->sym == symWHILE
  || token->sym == symREAD || token->sym == symWRITE || token->sym == symCALL
  || token->sym == symIDENTIFIER || token->sym == symSEMI)
  {
    Statement();
    if(token->sym == symSEMI)
    {
      token = nextToken();
    }
    else
    {
      Error(6);
    }
    StatementList();
  }
  else if(token->sym == symEND)
  {
    return;
  }
  else
  {
    Error(32);
    skip(statement, 23);
  }
}

/*
** �y�k�W�h#7 <Statement>
*/
  void Statement()
  {
    if (token->sym == symIF)
      IfStatement();
    else if (token->sym == symBEGIN)
      CompoundStatement();
    else if (token->sym == symWHILE)
      WhileStatement();
    else if (token->sym == symREAD)
      ReadStatement();
    else if (token->sym == symWRITE)
      WriteStatement();
    else if (token->sym == symCALL)
      CallStatement();
    else if (token->sym == symIDENTIFIER)
      AssignmentStatement();
    else if (token->sym == symSEMI) // epsilon production
    {
      return;
    }
    else
    {
      Error(31);
      skip(statement, 23);
    }
  }
/*
** �y�k�W�h#8 <AssignmentStatement>
*/
  void AssignmentStatement()
  {
    char result[BUFSIZE];
    idobj=getIdobj(procStack[procTop-1], token->value);
    // if (idobj != NULL)
    // {
    //   sprintf(result, "%s_%s",
    //     idobj->procname, token->value);    
    // }    
    Identifier();
    if(idobj != NULL && idobj->attr == symCONST)
    {
      Error(29);
    }
    if (token->sym == symBECOMES)
    {
      token = nextToken();
      Expression();
    //   sprintf(buf, "\tPOP\tAX\n"
    //                "\tMOV\t[%s], AX\n", result);
    //   fprintf(outfile, buf);
    }
    else
    {
      Error(8);
      skip(statement, 23);
    }
  }
/*
** �y�k�W�h#9 <CallStatement>
*/
  void CallStatement()
  {
    if (strcmp(token->value, "CALL")==0)
    {
      token = nextToken();
      idobj=getIdobj(procStack[procTop-1], token->value);
    //   if (idobj != NULL)
    //   {
    //     sprintf(buf, "\tCALL\t%s\n", token->value);
    //     fprintf(outfile, buf);
    //   }
      Identifier();
    }
    else
    {
      Error(9);
      skip(statement, 23);
    }
  }
/*
** �y�k�W�h#10 <CompoundStatement>
*/
  void CompoundStatement()
  {
    if (strcmp(token->value,"BEGIN")==0)
    {
      token = nextToken();
      StatementList();
      if (strcmp(token->value,"END")==0)
        token = nextToken();
      else
      {
        Error(11);
        skip(statement, 23);
      }
    }
    else
    {
      Error(10);
      skip(statement, 23);
    }
  }
/*
** �y�k�W�h#11 <IfStatement>
*/
  void IfStatement()
  {
    int head, tail;
    if (strcmp(token->value,"IF")==0)
    {
      token = nextToken();
      Condition();
      head = labelCount;
      tail = ++labelCount;
    //   sprintf(buf, "\tJMP\t_go%d\n"
    //                "_go%d:\n", tail,head);
    //   fprintf(outfile, buf);
      if (strcmp(token->value, "THEN")==0)
      {
        token = nextToken();
        Statement();
        // sprintf(buf, "_go%d:\n", tail);
        // fprintf(outfile, buf);
      }
      else
      {
        Error(13);
        skip(statement, 23);
      }
    }
    else
    {
      Error(12);
      skip(statement, 23);
    }
  }
/*
** �y�k�W�h#12 <WhileStatement>
*/
  void WhileStatement()
  {
    int home, head, tail;
    if (strcmp(token->value,"WHILE")==0)
    {
      token = nextToken();
      home = ++labelCount;
    //   sprintf(buf, "_go%d:\n", home);
    //   fprintf(outfile, buf);
      Condition();
      head = labelCount;
      tail = ++labelCount;
    //   sprintf(buf, "\tJMP\t_go%d\n"
    //                "_go%d:\n", tail, head);
    //   fprintf(outfile, buf);
      if (strcmp(token->value,"DO")==0)
      {
        token = nextToken();
        Statement();
        // sprintf(buf, "\tJMP\t_go%d\n", home);
        // fprintf(outfile, buf);
      }
      else
      {
        Error(15);
        skip(expression, 23);
        Statement();
      }
    //   sprintf(buf, "_go%d:\n", tail);
    //   fprintf(outfile, buf);
    }
    else
    {
      Error(14);
      skip(statement, 23);
    }
  }
/*
** �y�k�W�h#13 <ReadStatement>
*/
  void ReadStatement()
  {
    if (strcmp(token->value,"READ")==0)
    {
      token = nextToken();
      if (token->sym == symLPAREN)
      {
        token = nextToken();
        if (token->sym == symIDENTIFIER)
        {
          idobj=getIdobj(procStack[procTop-1],token->value);
        //   if (idobj != NULL)
        //   {
        //     sprintf(id,"%s_%s",idobj->procname,token->value);
        //     sprintf(buf, "\treadstr\t_buf\n"
        //                  "\tstrtoi\t_buf, '$', %s\n"
        //                  "\tnewline\n", id);
        //     fprintf(outfile, buf);
        //   }
          Identifier();
        }
        while (token->sym == symCOMMA)
        {
          token = nextToken();
          if (token->sym == symIDENTIFIER)
          {
            idobj = getIdobj(procStack[procTop-1],
                             token->value);
            // if (idobj != NULL)
            // {
            //   sprintf(id, "%s_%s",
            //           idobj->procname, token->value);
            //   sprintf(buf, "\treadstr\t_buf\n"
            //                "\tstrtoi\t_buf, '$', %s\n"
            //                "\tnewline\n", id);
            //   fprintf(outfile, buf);
            // }
            Identifier();
          }
        }
        if (token->sym == symRPAREN)
          token = nextToken();
        else
        {
          Error(18);
          skip(statement, 23);
        }
      }
      else
      {
        Error(17);
        skip(statement, 23);
      }
    }
    else
    {
      Error(16);
      skip(statement, 23);
    }
  }
/*
** �y�k�W�h#14 <WriteStatement>
*/
  void WriteStatement()
  {
    if (strcmp(token->value, "WRITE")==0)
    {
      token = nextToken();
      if (token->sym == symLPAREN)
      {
        token = nextToken();
        if (token->sym == symIDENTIFIER)
        {
          idobj=getIdobj(procStack[procTop-1],token->value);
        //   if (idobj != NULL)
        //   {
        //     sprintf(id, "%s_%s",
        //                 idobj->procname, token->value);
        //     if (idobj->attr == symCONST)
        //     {
        //       sprintf(buf,"\tdispstr\t%s\n", id);
        //       fprintf(outfile, buf);
        //     }
        //     else
        //     {
        //       sprintf(buf, "\titostr\t%s, _intstr, '$'\n"
        //         "\tMOV\tDX, _intstr\n"
        //         "\tMOV\tAH, 09H\n"
        //         "\tINT\t21H\n"
        //         "\tnewline\n", id);
        //       fprintf(outfile, buf);
        //     }
        //   }
          Identifier();
        }
        while (token->sym == symCOMMA)
        {
          token = nextToken();
          if (token->sym == symIDENTIFIER)
          {
            idobj = getIdobj(procStack[procTop-1],
                             token->value);
            // if (idobj != NULL)
            // {
            //   sprintf(id, "%s_%s",
            //               idobj->procname, token->value);
            //   if (idobj->attr == symCONST)
            //   {
            //     sprintf(buf,"\tdispstr\t%s\n", id);
            //     fprintf(outfile, buf);
            //   }
            //   else
            //   {
            //     sprintf(buf, "\titostr\t%s, _intstr, '$'\n"
            //       "\tMOV\tDX, _intstr\n"
            //       "\tMOV\tAH, 09H\n"
            //       "\tINT\t21H\n"
            //       "\tnewline\n", id);
            //     fprintf(outfile, buf);
            //   }
            // }
            Identifier();
          }
        }
        if (token->sym == symRPAREN)
          token = nextToken();
        else
        {
          Error(18);
          skip(statement, 23);
        }
      }
      else
      {
        Error(17);
        skip(statement, 23);
      }
    }
    else
    {
      Error(19);
      skip(statement, 23);
    }
  }
/*
** �y�k�W�h#15 <IdentifierList>
*/
  void IdentifierList()
  {
    Identifier();
    while (token->sym == symCOMMA)
    {
      token = nextToken();
      Identifier();
    }
  }
/*
** �y�k�W�h#16 <Condition>
*/
  void Condition()
  {
    Expression();
    if (token->sym == symLESS ||
        token->sym == symLEQ ||
        token->sym == symEQ ||
        token->sym == symNEQ ||
        token->sym == symGREATER ||
        token->sym == symGEQ)
    {
      int operator = token->sym;
      token = nextToken();
      int expr = Expression();
      if(expr == -1)
      {
        skip(condition, 23);
      }
    //   sprintf(buf, "\tPOP\tBX\n"
    //                "\tPOP\tAX\n"
    //                "\tCMP\tAX, BX\n");
    //   fprintf(outfile, buf);
      switch (operator)
      {
        case symEQ:
        //   sprintf(buf,"\tJE\t_go%d\n",(++labelCount));
        //   fprintf(outfile, buf);
          break;
        case symNEQ:
        //   sprintf(buf,"\tJNE\t_go%d\n",(++labelCount));
        //   fprintf(outfile, buf);
          break;
        case symLESS:
        //   sprintf(buf,"\tJL\t_go%d\n",(++labelCount));
        //   fprintf(outfile, buf);
          break;
        case symLEQ:
        //   sprintf(buf,"\tJLE\t_go%d\n",(++labelCount));
        //   fprintf(outfile, buf);
          break;
        case symGREATER:
        //   sprintf(buf,"\tJG\t_go%d\n",(++labelCount));
        //   fprintf(outfile, buf);
          break;
        case symGEQ:
        //   sprintf(buf,"\tJGE\t_go%d\n",(++labelCount));
        //   fprintf(outfile, buf);
          break;
      }
    }
    else
    {
      Error(20);
      skip(condition, 23);
    }
  }
/*
** �y�k�W�h#17 <Expression>
*/
  int Expression()
  {
    int term = Term();
    int expr_pr = ExprPr();
    if(term == 1 && expr_pr == 1) return 1;
    return -1;
  }

/*
** <ExprPr>
*/

int ExprPr()
{
  if(token->sym == symPLUS ||
         token->sym == symMINUS)
  {
    int operator = token->sym;
    token = nextToken();
    int term = Term();
    int expr_pr = ExprPr();
    if(term == 1 && expr_pr == 1) return 1;
    return -1;
  }
  else if(expression[token->sym] == 1)
  {
    return 1;
  }
  else
  {
    Error(28);
    skip(expression, 23);
    return -1;
  }
}

/*
** �y�k�W�h#18 <Term>
*/
int Term()
{
  int factor = Factor();
  int term_pr = TermPr();
  if(factor == 1 && term_pr == 1) return 1;
  return -1;
}

/*
** <TermPr>
*/
int TermPr()
{
  if(token->sym == symMUL ||
         token->sym == symDIV)
  {
    int operator = token->sym;
    token = nextToken();
    int factor = Factor();
    int term_pr = TermPr();
    if(factor == 1 && term_pr == 1) return 1;
    return -1;
  }
  else if(term[token->sym] == 1)
  {
    return 1;
  }
  else
  {
    Error(28);
    skip(term, 23);
    return -1;
  }
}

/*
** �y�k�W�h#19 <Factor>
*/
int Factor()
{
  int operator;
  if(token->sym == symMINUS || token->sym == symPLUS)
  {
    operator = token->sym;
    token = nextToken();
  }
  if(token->sym == symIDENTIFIER)
  {
    idobj = getIdobj(procStack[procTop-1], token->value);
    // if (idobj != NULL)
    // {
    //   sprintf(buf, "\tMOV\tAX, %s_%s\n",
    //           idobj->procname, token->value);
    //   fprintf(outfile, buf);
    // }
    Identifier();
    if(idobj != NULL && idobj->attr == symCONST)
    {
      Error(31);
      skip(factor, 23);
      return -1;
    }
    return 1;
  }
  else if(token->sym == symNUMBER)
  {
    return Number();
  }
  else if(token->sym == symLPAREN)
  {
    token = nextToken();
    int expr = Expression();
    if(expr == 1 && token->sym == symRPAREN)
    {
      token = nextToken();
      return 1;
    }
    else
    {
      Error(18);
      skip(expression, 23);
      return -1;
    }
  }
  else
  {
    Error(27);
    skip(factor, 23);
    return -1;
  }
}
/*
** �ѧO�r�ŰO�B�z
*/
  int Identifier()
  {
    if (token->sym == symIDENTIFIER)
    {
      idobj=getIdobj(procStack[procTop-1],token->value);
      if (idobj == NULL)
      {
        Error(26); // TODO 這邊算是例外狀況
        return -1;
      }
      token = nextToken();
      return 1;
    }
    else
    {
      Error(21);
      return -1;
    }
  }
/*
** �Ʀr�ŰO�B�z
*/
  int Number()
  {
    if (token->sym == symNUMBER)
    {
      token = nextToken();
      return 1;
    }
    else
    {
      Error(22);
      return -1;
    }
  }
/*
****************************** �D�{�� **********************
*/
  int main(int argc, char *argv[])
  {
    FILE *f=fopen(argv[1], "r");
    scanner(f);
    followsyminit();
    token = nextToken();
    Program();
    // fprintf(outfile, "\tMOV\tAX, 4C00H\n"
    //              "\tINT\t21H\n");
    // printf("\n  Plone compile completed. "
    //   "\n  Error count : %d\n", errorCount);
    // if (argc==3)
    // {
    //   printf("\n�{�ǵ��c���|���e�p�U�G\n%s\n",
    //     procobjToString());
    // }
    // fclose(outfile);
    // fclose(f);
    // if (errorCount==0)
    // {
    //   FILE *batchfile;
    //   sprintf(buf, "%s.bat", progname);
    //   batchfile=fopen(buf,"w");
    //   fprintf(batchfile, "nasmw %s.asm -o %s.com\n",
    //           progname, progname);
    //   fprintf(batchfile, "%s.com\n", progname);
    //   fclose(batchfile);
    //  }
    return 0;
  }
