/*  
	Max Conroy, Michael Johnson & Keith M. Weiner
	C Compiler
	April, 2015
*/

#include <stdio.h>
#include <stdlib.h>
#define MAXSTR 300
#include "G1P4.h"

#define ScanEOF 13

int Token;

//********************************************************************************************************************
        // Main
//********************************************************************************************************************

int main(int argc, char *argv[])
{
	if(Init() == 1)
	{

		getChar();//get first character
		do{
			lexer();//determine what that character is
			SystemGoal();
		}while(Token != ScanEOF);//do this until end of file
		 //closing files
		WrapUp();
	}

 
	system("PAUSE");
	return 0;
}

