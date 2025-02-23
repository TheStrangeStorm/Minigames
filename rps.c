#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>

int mod(int a, int b)
{
	if (b < 0) return -mod(-a, -b);

	int r = a % b;

	if (r < 0) r += b;

	return r;
}


int main() 
{
	const char* options[] = {"ROCK", "PAPER", "SCISSORS"};
	const char* results[] = {"LOST", "DRAW", "WIN"};

	char buffer[3];

	int run = 1;

	srandom(time(0));

	while (run)
	{
		double num = (double)rand() / (double)RAND_MAX;
		double normalised_num = num * 3;	
		int choice = floor(normalised_num);

		printf(
			"0 - ROCK\n"
			"1 - PAPER\n"
			"2 - SCISSORS\n\n"

			"Choice: "
		);

		char user_choice;
		int index = 0;

		fgets(buffer, 3, stdin);
		user_choice = strtol(buffer, NULL, 0);

		if (!(0 <= user_choice && user_choice <= 2)) break;

		int result = mod(user_choice - choice + 1, 3);

		printf("%s VS %s\n\n%s\n\n", options[user_choice], options[choice], results[result]);
	}

	return 0;
}
