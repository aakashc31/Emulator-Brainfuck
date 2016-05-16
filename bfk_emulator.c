#include <stdio.h>
#include <stdlib.h>

struct tuple
{
	int key;
	int value;
};

void compile_brfk(char*, int*, int, int, int*, int, struct tuple*, int);
int main(int argc, char** argv)
{
	// If the no. of arguments is not 2, the program won't run.
	if(argc!=2)
	{
		printf("Invalid number of arguments!\n");
		exit(1);
	}

	char* fnm = argv[1]; //filename
	FILE *fp = fopen(fnm,"r"); //file pointer

	// In case the file couldn't be read
	if(fp == NULL)
	{
		printf("Could not open file! Make sure the location of the file is correct\n");
		exit(2);
	}

	//character wise reading
	//keep track of number of chars in each line. How? In an array
	int nol = 1;
	char ch;
	char *prg = (char*)malloc(1000); //allocating space for the program to be stored in memory
	int *lines = (int*)malloc(100); //number of chars in each line.
	struct tuple *braces = (struct tuple*)malloc(100*sizeof(struct tuple)); //array for braces
	int numBraces = 0;
	int index = 0;
	printf("Reading file...\n");
	while((ch=fgetc(fp))!=EOF)
	{
		if(ch=='\n')
		{
			lines[nol-1] = index;
			nol++;
			continue;
		}
		if(ch=='[')
		{
			braces[numBraces].key = index;
			braces[numBraces].value = -1;
			numBraces++;
		}

		else if(ch==']')
		{
			for(int k = numBraces-1;k>=0;k--)
			{
				if(braces[k].value == -1)
				{
					braces[k].value = index;
					break;
				}
			}
		}
		if(ch=='+' || ch=='-' || ch=='<' || ch=='>' || ch=='[' || ch==']' || ch==',' || ch=='.')
		{
			prg[index] = ch;
			index++;
		}

	}

	//if the file has only one line the file won't contain any newline characters
	if(nol==1)
		lines[0] = index;
	// prompting the user
	printf("Lines: %d, Characters: %d read successfully.\nOutput:\n", nol,index);

	int* memory = (int*)malloc(1000); //memory allocated for running the code
	int p = 0; //current pointer
	for(int i =0; i< 1000; i++)
		memory[i] = 0;

	compile_brfk(prg, lines, 0,index, memory, p, braces, numBraces);
	
	// printf("Execution completed!\n");
	return 0;
}

int searchCorrBrace(int key,struct tuple* braces, int numBraces)
{
	for(int i=0;i<numBraces;i++)
	{
		if(braces[i].key == key)
		{
			return braces[i].value;
		}
	}
	return -1;
}

void compile_brfk(char* prg, int* lines, int begin, int length, int* memory, int p, struct tuple* braces, int numBraces)
{
	char inp;
	int next;
	// printf("Executing from %d. Num chars to be executed =  %d\n",begin,length);
	for(int i = begin; i < begin+length; i++)
	{
		char ch = prg[i];
		// printf("Current Character: %c\n", ch);
		switch(ch)
		{
			case '+':
				memory[p]++;
			break;
			
			case '-':
				memory[p]--;
			break;
			
			case '<':
				if(p>=1)
					p--;
			break;
			
			case '>':
				if(p<999)
					p++;
			break;
			
			case '[':
				next = searchCorrBrace(i, braces, numBraces);
				// printf("Pres brace %d Corr brace%d\n", i,next);
				while(memory[p]!=0)
				{
					// printf("current value:%d\n",memory[p] );
					compile_brfk(prg, lines,i+1,next-i-1,memory,p,braces,numBraces);
				}
				i = next;
			break;

			case ',':
				fflush(stdin);
				scanf("%c",&inp);
				memory[p] = inp;
			break;

			case '.':
				printf("%c",memory[p]);
			break;
			default:
				printf("Read error due to %c!\n",ch);
				exit(3);
		}
	}
}