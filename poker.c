// WORK IN PROGRESS

#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h>
#include <time.h>
#include <sys/socket.h> 
#include <netdb.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <math.h>

void get_rank(unsigned char card, char* buffer) 
{
	switch (card & 15) 
	{
		case 0:
			strncpy(buffer, "A", 1);
			break;
		case 10:
			strncpy(buffer, "J", 1);
			break;
		case 11:
			strncpy(buffer, "Q", 1);
			break;
		case 12:
			strncpy(buffer, "K", 1);
			break;
		default:
			char digits[3];

			sprintf(digits, "%d", (card & 15) + 1);

			if (card & 15 < 9) 
			{
				strncpy(buffer, digits, 1);
			}
			else 
			{
				strncpy(buffer, digits, 2);
			}
			break;
	}
}

void get_rank_upside_down(unsigned char card, char* buffer, int* len) 
{
	memset(buffer, 0, 8);
	
	const char* ace = "\U00002C6F\0";
	const char* two = "\u218A\0";
	const char* three = "\U0000218B\0";
	const char* four = "+\0";
	const char* five = "\U000003B6\0";
	const char* six = "9\0";
	const char* seven = "L\0";
	const char* eight = "8\0";
	const char* nine = "6\0";
	const char* ten = "0I\0";
	const char* jack = "\U0000017F\0";
	const char* queen = "\U0000038C\0";
	const char* king = "\U0000A7B0\0";

	switch (card & 15) 
	{
		case 0:
			strcat(buffer, ace);
			*len = 2;
			break;
		case 1:
			strcat(buffer, two);
			*len = 2;
			break;
		case 2:
			strcat(buffer, three);
			*len = 2;
			break;
		case 3:
			strcat(buffer, four);
			*len = 2;
			break;
		case 4:
			strcat(buffer, five);
			*len = 2;
			break;
		case 5:
			strcat(buffer, six);
			*len = 2;
			break;
		case 6:
			strcat(buffer, seven);
			*len = 2;
			break;
		case 7:
			strcat(buffer, eight);
			*len = 2;
			break;
		case 8:
			strcat(buffer, nine);
			*len = 2;
			break;
		case 9:
			strcat(buffer, ten);
			*len = 3;
			break;
		case 10:
			strcat(buffer, jack);
			*len = 2;
			break;
		case 11:
			strcat(buffer,  queen);
			*len = 2;
			break;
		case 12:
			strcat(buffer,  king);
			*len = 2;
			break;
	}
}

void get_suit(unsigned char card, char* buffer) 
{
	memset(buffer, 0, 8);

	const char* spade = "\U00002660\0";
	const char* heart = "\U00002665\0"; 
	const char* diamond = "\U00002666\0";
	const char* club = "\U00002663\0"; 

	switch (card & 48) 
	{
		case 0:
			strncpy(buffer, spade, 4);
			break;
		case 16:
			strncpy(buffer, heart, 4);
			break;
		case 32:
			strncpy(buffer, diamond, 4);
			break;
		case 48:
			strncpy(buffer, club, 4);
			break;
	}
}

void get_suit_upside_down(unsigned char card, char* buffer) 
{
	memset(buffer, 0, 8);

	const char* spade = "\U00002665\0";
	const char* heart = " \U00002660\0";
	const char* diamond = "\U00002666\0";
	const char* club = "\U000025CF\0";

	switch (card & 48) 
	{
		case 0:
			strcat(buffer, spade);
			break;
		case 16:
			strcat(buffer, heart);
			break;
		case 32:
			strcat(buffer, diamond);
			break;
		case 48:
			strcat(buffer, club);
			break;
	}	
}

void display_card(unsigned char card, char* buffer) 
{
	memset(buffer, 32, 141);

	buffer[0] = 32;

	memset(buffer + 1, 95, 11);
	
	buffer[12] = 32;
	buffer[13] = 10;
	buffer[14] = 124;

	for (int line = 1; line < 10; line++) 
	{
		buffer[line * 14] = 124;
		buffer[line * 14 + 12] = 124;
		buffer[line * 14 + 13] = 10;
	}

	memset(buffer + 127, 95, 11);
	
	buffer[140] = 0;

	char suit[5];
	char rank[10];
	char upside_down_rank[10];
	int len;

	get_suit(card, suit);
	get_rank(card, rank);
	get_rank_upside_down(card, upside_down_rank, &len);

	printf("%.30s", buffer);
	printf("%s", rank);
	printf("%.*s", 45 - len + 2, buffer + 31 + len - 2);
	printf("%s", suit);
	printf("%.*s", 45 - len + 2, buffer + 77);
	printf("%s", upside_down_rank);
	printf("%s\n", buffer + 123);
}

void host() 
{
	int socket_fd;

	struct addrinfo hints;
	struct addrinfo *result, *rp;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0; // don't really need to this, line 299

	int success = getaddrinfo(NULL, "3500", &hints, &result);

	if (success != 0) 
	{
		printf("%s\n", gai_strerror(success));
		exit(-1);
	}

	printf("%d\n", success);

	char ip_buffer[16];

	for (rp = result; rp != NULL; rp = rp->ai_next) 
	{
		memset(ip_buffer, 0, 16);
		inet_ntop(AF_INET, rp->ai_addr->sa_data, ip_buffer, 16);	

		printf("IP: %s\n", ip_buffer);
		
		socket_fd = socket(AF_INET, SOCK_STREAM, 0);

		int yes = 1;

		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) 
		{
			printf("?: %d\n", errno);
			close(socket_fd);
			exit(1);
		}

		int bind_success = bind(socket_fd, rp->ai_addr, rp->ai_addrlen);

		if (bind_success == -1) 
		{
			printf("%d\n", errno);
			printf("?\n");
			close(socket_fd);
			exit(1);
		}
		else 
		{
			break;
		}
	}

	freeaddrinfo(result);

	int number_of_clients = -1;

	char input_buffer[3];

	while (number_of_clients < 1) 
	{
		printf("Number of clients: ");

		char* read_success = fgets(input_buffer, sizeof(input_buffer), stdin);

		if (read_success == NULL) 
		{
			printf("Not an valid number.\n");
		}
		else 
		{
			number_of_clients = atoi(read_success);
		}
	}

	int* csfds = malloc(sizeof(int) * number_of_clients);

	listen(socket_fd, 10);

	int sum = 0;

	for (int i = 0; i < number_of_clients; i++)
	{
		csfds[i] = accept(socket_fd, 0, 0);

		int num;

		recv(csfds[i], &num, sizeof(num), 0);

		sum += num;
	}

	printf("%d\n", sum);

	int avg = sum / number_of_clients;

	srandom(avg);

	unsigned char cards[52];
	char suit[5];
	char card_buffer[141];

	memset(cards, 0, 52);

	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 13; j++) 
		{
			cards[i * 13 + j] = i << 4 | j;

			unsigned char card = cards[i * 13 + j];
		}
	}

	// Fisher-Yates algorithm
	for (int i = 51; i != 0; i--) 
	{
		double num = (double)random() / (double)RAND_MAX;
		double normalised_num = num * i;
		int index = floor(normalised_num);
		unsigned char _card = cards[i];

		// There's probably a better way of swapping like using XOR but I'm too lazy and the compiler probably got me
		cards[i] = cards[index];
		cards[index] = _card;
	}

	/*for (int i = 0; i < 52; i++) 
	{
		unsigned char card = cards[i];

		display_card(card, card_buffer);
	}*/

	for (int i = 0; i < number_of_clients; i++) 
	{
		close(csfds[i]);
	}
	
	free(csfds);

	close(socket_fd);
}

int main(int argc, char** argv) 
{
	if (argc != 2) 
	{
		printf("Usage: %s <type>\n", argv[0]);
		return -1;
	}

	int type = atoi(argv[1]);

	host();
	
	return 0;
}
