#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

void translate(int choice, char* buffer)
{

	char* option1 = "ROCK";
	char* option2 = "PAPER";
	char* option3 = "SCISSORS";

	switch (choice) 
	{
		case 0:
			strcat(buffer, option1);

			break;
		case 1:
			strcat(buffer, option2);
			break;
		case 2:
			strcat(buffer, option3);
			break;
	}
}

int mod(int a, int b)
{
	
	if (b < 0) return -mod(-a, -b);

	int r = a % b;

	if (r < 0) r += b;

	return r;
}


int main() 
{
	int run = 1;

	srandom(time(0));

	while (run)
	{

		char buffer[32];

		memset(buffer, 0, 32);

		double num = (double)rand() / (double)RAND_MAX;
		double normalised_num = num * 3;	
		int choice = floor(normalised_num);

		printf("0 - ROCK\n1 - PAPER\n2 - SCISSORS\n\nChoice: ");

		int user_choice;
		int index = 0;

		scanf("%d", &user_choice);

		if (!(0 <= user_choice && user_choice <= 2)) 
		{
			break;
		}

		strcat(buffer, "\n");

		translate(user_choice, buffer);

		char* versus = " VS ";

		strcat(buffer, versus);

		translate(choice, buffer);

		printf("%s\n", buffer);

		int result = mod(user_choice - choice + 1, 3) - 1;

		switch (result) 
		{
			case -1:
				printf("LOST\n\n");
				break;
			case 0:
				printf("DRAW\n\n");
				break;
			case 1:
				printf("WIN\n\n");
				break;
		}
	}

	return 0;
}
