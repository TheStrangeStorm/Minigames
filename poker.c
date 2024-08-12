// WORK IN PROGRESS

#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>

int get_rank(unsigned char card) 
{
	return card & 15;
}

void get_rank_upside_down(unsigned char card, char* buffer, int* len) 
{
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
			*len = 2;
			break;
		case 10:
			strcat(buffer,  jack);
			*len = 3;
			break;
		case 11:
			strcat(buffer,  queen);
			*len = 3;
			break;
		case 12:
			strcat(buffer,  king);
			*len = 3;
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

	char rank[9];
	int len;

	get_rank_upside_down(9, rank, &len);

	printf(
		" ___________ \n"
		"|           |\n"
		"| 2         |\n"
		"|           |\n"
		"|           |\n"
		"|     A     |\n"
		"|           |\n"
		"|           |\n"
		"|         %s |\n"
		"|___________|\n"
	, rank);

	printf("%s\n", buffer);
}

void host(int sfd) 
{
	listen(sfd, 10);

	int number_of_clients;

	while (number_of_clients < 0) 
	{
		printf("Number of clients: ");

		scanf("%d", &number_of_clients);
	}

	unsigned char cards[52];
	char suit[5];

	memset(cards, 0, 52);

	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 13; j++) 
		{
			cards[i * 13 + j] = i << 4 | j;

			unsigned char card = cards[i * 13 + j];

			get_suit(card, suit);

			printf("%d %s\n", get_rank(card), suit);
		}
	}

	char card_buffer[141];

	display_card(10, card_buffer);

	close(sfd);
}

int main(int argc, char** argv) 
{
	if (argc != 2) 
	{
		printf("Usage: %s <type>\n", argv[0]);
		return -1;
	}

	int type = atoi(argv[1]);

	struct sockaddr_in my_add = {
		AF_INET,
		0x50c3,
		0,

	};

	char host_name[256];

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_fd == -1) 
	{
		printf("huh couldn't even create a socket: %d\n", errno);
		return -1;
	}

	if (bind(socket_fd, (const struct sockaddr*)&my_add, sizeof(my_add)) == -1 && errno != 98) 
	{
		printf("couldn't bind, errno: %d\n", errno);
		return -1;
	}

	struct sockaddr result = {0};
	int len = sizeof(result);

	getsockname(socket_fd, &result, &len); 

	char ip_buffer[256];

	inet_ntop(AF_INET, &result.sa_data, ip_buffer, 256);

	printf("Your IP address: %s\n", ip_buffer);

	host(socket_fd);

	/*int used_socket_fd;

	switch (type) 
	{
		case 0:
			listen(socket_fd, 10);
			used_socket_fd = accept(socket_fd, 0, 0);

			if (used_socket_fd == -1) 
			{
				printf("couldnt accept incoming connection, errno: %d\n", errno);
				return -1;
			}

			break;
		case 1:
			if (connect(socket_fd, (const struct sockaddr*)&my_add, sizeof(my_add))) 
			{
				printf("couldnt connect, errno: %d\n", errno);
				return -1;
			}
			used_socket_fd = socket_fd;
			break;
		default:
			printf("type a valid number dumb dumb\n");
			return -1;
	}

	struct pollfd fds[2] = {
		{
			0,
			POLLIN,
			0
		},
		{
			used_socket_fd,
			POLLIN,
			0 
		}
	};

	char buffer[64];
	char peer_buffer[64];

	int poll_result;

	while (poll(fds, 2, 60000) != 0)
	{
		memset(buffer, 0, 64);

		poll_result = poll(fds, 2, 10000);

		if (fds[0].revents & POLLIN) 
		{
			int bytes_read = read(0, buffer, 63);

			if (bytes_read <= 1) continue;

			buffer[bytes_read - 1] = 0;
			send(used_socket_fd, buffer, 63, 0);
		}
		else if  (fds[1].revents & POLLIN) 
		{
			recv(used_socket_fd, peer_buffer, 63, 0);

			if (peer_buffer[0] == 46) continue;

			printf("%s\n", peer_buffer);
		}

		if (strncmp(buffer, "quit", 4) == 0 || strncmp(peer_buffer, "quit", 4) == 0) 
		{
			printf("frrrrr\n");
			break;
		}
	}

	if (!poll_result) 
	{
		memset(buffer, 0, 64);

		const char quit_msg[] = "quit\n";

		strcat(buffer, quit_msg);

		send(used_socket_fd, buffer, 64, 0);
	}

	close(used_socket_fd);
	close(socket_fd);*/
	
	return 0;
}
