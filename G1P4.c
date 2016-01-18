/*
	Max Conroy, Michael Johnson, & Keith M. Weiner
	C Compiler
	April, 2015
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#define MAXSTR 300
#include "G1P4.h"

#define Letter 0
#define Digit 1
#define Symbol 99

#define Begin 0 //    Token Numbers
#define End 1
#define Read  2
#define Write 3
#define Identity 4//ID
#define Integer 5//INT_Literal
#define LeftPar 6 // "("
#define RightPar 7// ")"
#define SemiColon 8// ";"
#define Comma 9//","
#define AssignOp 10// :=
#define Addition 11// +
#define Subtraction 12 // -
#define ScanEOF 13
#define Error 14
#define Nothing 15
#define Syntax 16



/*
1. <program> -> BEGIN <statement list> END
2. <statement list> -> <statement> {<statement list>}
3. <statement> -> ID := <expression>;
4. <statement> -> READ ( <id list> );
5. <statement> -> WRITE ( <expr list> );
6. <id list> -> ID {, ID}
7. <expr list> -> <expression> {, <expression>}
8. <expression> -> <primary> {<add op> <primary>}
9. <primary> -> ( <expression> )
10. <primary> -> ID
11. <primary> -> INTLITERAL
12. <add op> -> PLUSOP
13. <add op> -> MINUSOP
14. <system goal> -> <program> SCANEOF
*/

/* *Variable List **************************************************/
char buffer[256];
int CharClass;
char lexeme [100];
char ListLine[MAXSTR];
char TempLine[MAXSTR];
char errorTemp[MAXSTR];
char IDKTemp[MAXSTR];
char colAr[MAXSTR];
char TempAr[100];
char VarInt[4] = {"int"};
char Name[MAXSTR];
int s, g, tempEr, ChError;
char NextChar;
int check;
int Token;
int LexCount;
int SynCount;
int NextToken;
int comment;
int tempSC;
int NotTok;
char ch;
static int chron = 0;
char SymTab[300];
int tempcounter = 0;
int prT, nxT;
char *subString;
char Copy[MAXSTR];
int fileKill;
char beg[] = {"BEGIN"};//compare strings
char wri[] = {"WRITE"};
char re[] = {"READ"};
char en[] = {"END"};
char Type[10];
FILE *InFile, *OutFile, *ListFile, *fopen(), *temp2, *temp1;
int lexLen;

time_t Current, StartTime, EndTime;

char temp1Name[MAXSTR] = "temp1.txt";
char temp2Name[MAXSTR]= "temp2.txt";
char temp2Rename[MAXSTR]= "OutputFile.txt";
char outfileName[MAXSTR] = "Write.txt";


//OPENING & CLOSING FILEs
/******************************************************The source file***************************************************/

char DoItAllIn(char Name[MAXSTR])
{  
	char Exe[MAXSTR];
	int ExType, exWhile;
	exWhile=0;
	while(exWhile == 0 )//While our flag is 0 we continue to prompt the user to make a file name
	{
		if(Name[0] == '\n')
		{
			exit(0);
		}
		Name[strlen(Name)-1] = '\0';
		
		if((strrchr(Name, '.')) != NULL)//we check if the name entered has an extension
		{
            if(checker(Name) == 0)//checker is function that checks if their is file of that name and returns a zero if there is a file of that name
			{
				
				printf("File found!\n");
				return Name[MAXSTR];//exit out of loop
			}
			else
				printf("File not found please re-enter name1");
		}
		else//if there isn't an extension we add a new extension in Extensions function.
		{
			ExType=1;//flag to determine which extension to use
			strcpy(Exe, ".in");//parameter fopen doesn't permit me to put Extension(...) in the fopen so i give In_Name this value
			strcat(Name, Exe);
            if(checker(Name) == 0)//again check if there is a file of that name then open it
			{
				printf("File found!\n");
				return Name[MAXSTR];
			}
			else
				printf("File not found please re-enter name2\n");//prompt user to re-enter file name				
			
		}
		fgets(Name, MAXSTR, stdin);
	}
	return Name[MAXSTR];
}

/******************************************************************************************************************************************
******************************************************************************************************************************************/

int checker(char Name[MAXSTR])
{
	int chfile;
    chfile=0;
	chfile = access(Name, F_OK);//check if name isn't used
	return chfile;
}

/*******************************************************************************************************************************************/
int Init()
{
	StartTime = time(NULL);
	Current = time(NULL);
	int ANum, f;
	ANum = 0;
	while(ANum==0)
	{
		printf("Please enter name of your source file.\n");
		fgets(Name, sizeof(Name), stdin);
		Name[MAXSTR] = DoItAllIn(Name);
		if(Name[0] == '\n')
		{
			ANum =2;
		}
		ANum=1;
	}
	InFile = fopen(Name, "r");//open files
	OutFile = fopen(outfileName, "w");
	ListFile = fopen("OutPut.LIS", "w");
	fprintf(ListFile, "Current time & date: ");
	fprintf(ListFile, ctime(&StartTime));
	fprintf(ListFile,"\n\n");
	temp2 = fopen(temp2Name, "w");
	temp1 = fopen(temp1Name,"w");
	fprintf(temp1,"#include <stdio.h>\n");
	fprintf(temp1, "#include <stdlib.h>\n\n");
	fprintf(temp1, "int main(int argc, char *argv[])\n");
	fprintf(temp1, "{\n");
	tempSC = 0;
	tempEr = 0;
	s=0;
	ChError = 1;
	f=0;
	for(f=0; f<=300; f++)
	{
		SymTab[f]= '\0';
	}

	return ANum;
}   // end Init()

void WrapUp()
{
	fprintf(ListFile, "%d Lexical Errors.\n", LexCount);//number of lexical errors in the file
	fprintf(ListFile, "%d Syntax  Errors.\n", SynCount);//number of syntax errors
	EndTime = time(NULL);
	fprintf(ListFile, "\nTime took to compile %f seconds\n", difftime(EndTime, StartTime));
	fprintf(ListFile, ctime(&EndTime));
	fprintf(ListFile,"\n\n");
	fprintf(temp2, "system(\"PAUSE\");\n");
	fprintf(temp2, "return 0;\n}");
    fclose(temp2);
	fclose(temp1);
    // Reoopen files to allow append
	temp1 =fopen(temp1Name, "a");
    temp2 =fopen(temp2Name, "r");
	// Append file
	while(fgets(buffer, sizeof(buffer),temp2))
	{
		fprintf(temp1, "%s", buffer);
	}
	// Close Files
	fclose(ListFile);
	fclose(InFile);
	fclose(temp1);
	fclose(temp2);
	fclose(OutFile);
    // Change name of input file
	subString = strchr(Name, '.');
	strncpy(Copy,Name,subString - Name);
	strcat(Copy,".c");
	rename(temp1Name, Copy );  // rename "temp1" file to "OutputFile"*/
    // Delete Temp2
    fileKill = unlink(temp2Name);
    if(fileKill != 0)
    {
        puts("Some kind of file error!");
    }

}   // end WrapUp()



//********************************************************************************************************************
        // SCANNER
//********************************************************************************************************************
int lexer()
{
	
	comment = 0;
	lexLen = 0;
	int k;	
	int u;
	if(Token == Identity)
		strcpy(TempAr, lexeme);
	for(u=0; u<=100; u++)
	{
		lexeme[u] = '\0';
	}

	blank();

	switch(CharClass)//determine what it is
	{
		case Letter:
			addChar();
			getChar();
			while(CharClass == Letter || CharClass == Digit)
			{
				addChar();
				getChar();
			}
			if(strcmp(lexeme, beg)==0)//test to determine if it any designated words
			{
				Token = Begin;
			}
			else if(strcmp(lexeme, en) == 0)
			{
				Token = End;

			}
			else if(strcmp(lexeme, wri) == 0)
			{
				Token = Write;
			}
			else if(strcmp(lexeme, re) == 0)
			{
				Token = Read;
			}
			else						//Anything else is an ID
			{
				Token = Identity;
			}
			break;

		case Digit:		//If it is a Number
		 addChar();
		 getChar();
		 while(CharClass == Digit)
		 {
			addChar();
			getChar();
		 }
		 Token = Integer;
		 break;

		case Symbol:        //if it isnt an digit, letter, or EOF determine if it is an acceptable symbol
			LookUp(NextChar);
			getChar();
			break;

		case EOF:
			Token = ScanEOF;
			lexeme[0] = 'E';
			lexeme[1] = 'O';
			lexeme[2] = 'F';
			lexeme[3] = 0;
			break;
	}

	return NextChar;//return the next character
}
//********************************************************************************************************************
    //  LookUp Function
//********************************************************************************************************************

int LookUp(char ch)
{
	switch(ch)//determine what kind of symbol it is
	{
		case '(':
			addChar();
			Token = LeftPar;
			break;

		case ')':
			addChar();
			Token = RightPar;
			break;

		case '+':
			addChar();
			Token = Addition;  //ADDITION OPERATOR
			break;

		case '-':
			getChar();

			if(NextChar == '-')//determine if the next character is another minus if it is
			{
				while(NextChar != '\n')//skip until new line
				{
					getChar();
				}
				comment = 1;
				break;
			}
			else//if there isn't
			{
				addChar();
				Token = Subtraction;//Subtraction Operator
			}
			break;

		case ';':	//SEMI COLON
			addChar();
			Token = SemiColon;
			tempSC = SemiColon;
			break;

		case ',':   //COMMA
			addChar();
			Token = Comma;
			break;

		case ':':		//colon
			addChar();
			getChar();
			if(NextChar == '=')//if there is an "=" then it is
			{
				addChar();
				Token = AssignOp;// Assignment Operator
			}
			else
			{
				Token = Error;//else we produce an error
				LexCount++;//increment error count
			}

			break;

		default:
			addChar();//Anything else is an error
			Token = Error;
			LexCount++;
			break;
	}
	return Token;	//return the token number
}

//********************************************************************************************************************
    // addChar Function
//********************************************************************************************************************

void addChar()//store
{
	if(lexLen <= 98)
	{
		lexeme[lexLen++] = toupper(NextChar);//convert to upper case to accept lower case
		lexeme[lexLen] = 0;
	}

	else
		printf("Comment"); //to big of line
}


//******************************************************************/
    // getChar Function
//*******************************************************************/
void getChar()//get our next character in the file
{
	int l,t;
	if(Token == Error)//store error
	{
		errorTemp[tempEr] = NextChar;
		Token = Error;
		tempEr++;
	}

	if((NextChar = getc(InFile)) != EOF)//check if it EOF
	{
		ListLine[s] = NextChar;
		s++;

		if(isalpha(NextChar))//check if it is alpha
		{
			NextChar = toupper(NextChar);
			CharClass = Letter;
		}

		else if(isdigit(NextChar))//check if it is a number
		{
			CharClass = Digit;
		}

		else
		{
			CharClass = Symbol;//check for symbol
		}
	}
	else
	{
		CharClass = EOF;
	}
	
	if(NextChar == '\n')//print line number and line along with any errors
	{
		t=0;
		for(t=0; t<=MAXSTR; t++)
		{
			TempLine[t] = ListLine[t];
		}
		fprintf(ListFile, "%d. %s",chron, ListLine);
		t=0;
		if(errorTemp[0] != '\0')
		{
			for(t=0; t<=tempEr-1; t++)
			{
				fprintf(ListFile, "\tError. %c NOT RECOGNIZED AT LINE %d\n", errorTemp[t], chron);
			}
		}
		chron++;
		l=0;
		for(l=0; l<=MAXSTR; l++)
		{
			errorTemp[l] = '\0';
			ListLine[l] = '\0';
		}
		g=0;
		s=0;
		tempEr = 0;
		nxT = 0;
		prT =0;
	}
}

//********************************************************************************************************************
    // blank Function
//********************************************************************************************************************

void blank()//check for whitespace
{
	while(isspace(NextChar))
	{
		getChar();
	}
}

//PARSER
//********************************************************************************************************************
    // printLine Function
//********************************************************************************************************************

void printLine()
{
	int k;
	if(Token != Error)
		fprintf(OutFile,"PROGRAM LINE: %s\n", TempLine);//print line of program in the Output file	

	for(k=0; k<=MAXSTR; k++)//Null arrays
	{
		TempLine[k] = '\0';
	}	
}
//***************************************************************************************************************
    // Match Function
//***************************************************************************************************************

int Match(int compare)//match token 
{
	int num1;
	if(Token == compare)
	{
		fprintf(OutFile, "Matching: %d, Token is: %d, Buffer is: %s\n", compare, Token, lexeme);
		if(Token == Identity|| Token == Addition || Token == Subtraction||Token==Integer && nxT != Read )
			fprintf(temp2, "%s", lexeme);
		num1=1;
	}
	else if(compare == tempSC)//Token wasn't handling semicolon right made a temp specifically for semicolons
	{
		fprintf(OutFile, "Matching: %d, Token is: %d, Buffer is: %s\n", compare, tempSC, lexeme);
		tempSC = 0;
		num1=1;
	}
	else if(Token == Error)
	{
		fprintf(OutFile, "//SYMBOL ERROR RECOGNIZED. %s", lexeme);
		num1=0;
	}
	else//if both these don't match 
	{
		if(comment != 1 && NotTok ==1)
		{
			num1=0;
			SynEr(compare);//specify the syntax error
			SynCount++;//increment Syntax error count			
		}
	}	
	if(compare == Identity)
		Process();
	comment = 0;
	lexer();
	return num1;
}

//******************************************************************/
    // SystemGoal Function
//******************************************************************/
//<system goal> -> <program> SCANEOF
void SystemGoal()
{
	Program();
	Match(ScanEOF);//match end of file
}

//******************************************************************/
    // Program Function
//******************************************************************/
//<program> -> BEGIN <statement list> END
void Program()
{
	Match(Begin);
	printLine();
	StmtList();
	Match(End);
    printLine() ;
}

//******************************************************************/
    // StatementList Function
//******************************************************************/
//<statement list> -> <statement> {<statement list>}
void StmtList()
{
	Statement();
}

/*
	<statement> -> ID := <expression>;
	<statement> -> READ ( <id list> );
	<statement> -> WRITE ( <expr list> );
*/
//******************************************************************/
    // Statement Function
//******************************************************************/
void Statement()//match identity, read, or write
{
	int flag;
	switch(Token)
	{
		case Identity:
			flag=Identity;
			fprintf(temp2, "\t");
			Match(Identity);
			if(Match(AssignOp)==1)
			{
				fprintf(temp2, "=");
			}
			Expression();
			if(Match(SemiColon)==1)
				fprintf(temp2,";\n");
			printLine();
			break;

		case Read:
			nxT = Read;
			Match(Read);
			Match(LeftPar);
			IDList();
			Match(RightPar);
			Match(SemiColon);
			printLine();
			break;

		case Write:
			nxT = Write;
			Match(Write);
			Match(LeftPar);
			Expr_List();//cant do math expressions in write.
			Match(RightPar);
			Match(SemiColon);
			
			printLine();
			break;

		default:
			break;

	}
	while(Token==Identity||Token==Read||Token==Write)
	{
		Statement();
	}
}

//*******************************************************************/
    // IDList Function
//******************************************************************/
//<id list> -> ID {, ID}
void IDList()
{
	Match(Identity);
	fprintf(temp2, "\tscanf(\" %%d\", &%s);\n", TempAr);
	while(Token == Comma){
		Match(Comma);
		IDList();
	}
}

//*****************************************************************/
    // ExprList Function
//*****************************************************************/
//<expr list> -> <expression> {, <expression>}
void Expr_List()
{
	Expression();
	if(Token != Comma && nxT == Write)
		fprintf(temp2, ");\n");
	while(Token == Comma){
		fprintf(temp2,");\n");
		if(Match(Comma)==1)
			prT = Comma;
		Expression();
	}
	
}

//*****************************************************************/
    // Expression Function
//*****************************************************************/
//<expression> -> <primary> {<add op> <primary>}
/*
	<add op> -> PLUSOP
	<add op> -> MINUSOP
*/
void Expression()
{
	Primary();
	while(Token == Addition || Token == Subtraction)
	{
		if(Token == Addition)
		{
			Match(Addition);
		}
		else if(Token == Subtraction)
		{
			Match(Subtraction);
		}
		else
			printf("error @ 719");
		Expression();	
	}		

}

/*
	<primary> -> ( <expression> )
	<primary> -> ID
	<primary> -> INTLITERAL
	
*/
//*****************************************************************/
    // Primary Function
//*****************************************************************/
void Primary()
{	

	switch(Token)
	{
		case Identity:
			if(nxT == Write && Token != AssignOp)
				fprintf(temp2, "\tprintf(\"%%d\",");
			Match(Identity);
			if(prT == Comma && Token != Identity && Token != AssignOp)
				fprintf(temp2,");\n");
			
			if(Token == AssignOp)
				if(Match(AssignOp)==1)
				{
					fprintf(temp2, "=");
				}
			else if(Token == Addition)
			{
				if(Match(Addition)==1)
					fprintf(temp2, "+");
			}
			else if(Token == Subtraction)
			{
				if(Match(Subtraction)==1)
					fprintf(temp2,"-");
			}
			else 
				printf("error");	
			break;

		case Integer:  //not printing integers to .c file
			if(Match(Integer)==1)
				fprintf(temp2, "%s", lexeme);
			break;

		case LeftPar:
			Match(LeftPar);
			Expression();
			Match(RightPar);
			break;
		default:
			break;
	}
}

void SynEr(int SynType)//Tell the user what the program expects
{
	int m;
	switch(SynType)
	{
		case Begin:
			strcpy(Type, "BEGIN");
			break;
		case End:
			strcpy(Type, "END");
			break;
		case Read:
			strcpy(Type, "READ");
			break;
		case Write:
			strcpy(Type, "WRITE");
			break;
		case Identity:
		case Integer:
			fprintf(ListFile, "\tExpecting an Integer or ID at line %d\n", chron-1);
			break;
		case LeftPar:
			strcpy(Type, "(");
			break;
		case RightPar:
			strcpy(Type, ")");
			break;
		case SemiColon:
			strcpy(Type, ";");
			break;
		case Comma:
			strcpy(Type, ",");
			break;
		case AssignOp:
			strcpy(Type, ":=");
			break;
		case Addition:
		case Subtraction:
			fprintf(ListFile, "\tExpecting + or - at line  %d\n", chron-1);
			break;
		case ScanEOF:
			strcpy(Type, "EOF");
			break;
		default:
			fprintf(ListFile, "\tError\n");
			break;
	}
	
	if(Type[0] != '\0')
	{
		fprintf(ListFile, "\tSYNTAX ERROR. Expecting %s at line %d\n", Type, chron-1);//print it out in listfile
	}
	m=0;
	for(m=0; m<=10; m++)//null array
	{
		Type[m] = '\0';
	}
}

//.C
void Process()
{
	int booltable = 0;
	int n;
	if(LookUpSym()==0)//symbol not in table
	{	
		enter();
		Generate();
	}
}

int LookUpSym()
{
	char temp[300]; //temp buffer for symbol lookup
	int w;
	int t;
	w=0;
	t=0;
	for(w=0; w<=300; w++)
	{
		temp[t] = SymTab[w];
		if(SymTab[w] == '^')
		{
			temp[t]='\0';
			if(strcmp(temp,lexeme)==0)
			{
				return 1;//That symbol is there
			}
			else
			
				t=0;
				temp[0] = '\0';
		}
		else
			t++;
	}
	return 0;//Symbol isn't there
	
}

void enter()
{
	int a;
	int d;
	int SymLen;
	if(SymTab[0]=='\0')
	{	
		for(a=0; a<=100; a++)
		{
			SymTab[a] = lexeme[a];
		}
		SymTab[strlen(SymTab)] = '^';
	}
	else 
	{	
		d = 0;
		SymLen = strlen(SymTab);
		for(a=SymLen; a<= strlen(lexeme)+SymLen; a++)
		{
			SymTab[a] = lexeme[d];
			d++;
		}
		SymTab[strlen(SymTab)] = '^';
	}
}

void Generate()
{
	fprintf(temp1, "\t%s %s;\n", VarInt, lexeme);
}
