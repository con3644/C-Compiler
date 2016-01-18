/*  
	Max Conroy, Michael Johnson & Keith M. Weiner
	C Compiler
	April, 2015
*/

#include <stdio.h>
#include <stdlib.h>
#ifndef G1P4_H
#define G1P4_H


extern int Token;

/*  Function Prototypes *******************************************/
//file handlers
int checker(char []);
char DoItAllIn(char []);
int Init();
void WrapUp();
//scanner
int lexer();
int LookUp(char);
void getChar();
void addChar();
void blank();
//Grammar/Parser
void printLine();
void SystemGoal();
void Program();
void StmtList();
void Statement();
void Expr_List();
void Expression();
void IDList();
void Primary();
void SynEr(int);
int Match(int);
//.C
int LookUpSym();
void enter();
void Generate();
void Process();
#endif
