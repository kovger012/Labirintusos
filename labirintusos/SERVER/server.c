#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/init.h>
#include <sys/socket.h>

#define BUFLEN

#define PORT 88

int main()
{
	int playerPointer = 0;
	struct sockaddr_in players[4]; 
	struct sockaddr_in si_me,si_other;
	
	int s, i, slen = sizeof(si_other), recv_len;
	char buf[BUFLEN];
	
	//create a UDP Socket
	if((s=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1)
	{
			printf("Failed to create UDP socket\n");
			exit(1);
	}
	
	//zero out the structure
	memset((char*)&si_me,0,sizeof(si_me));
	
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//bind socket to port
	if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
	{
		printf("Failed to bind UDP socket\n");
		exit(1);
	}
	
	//Listening for data
	while(1)
	{
		printf("Waiting for data...\n");
		fflush(stdout);
		
		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
		{
			printf("Failed to receive data...");
			continue;
		}
		
		//Process received data
			//Register new player
			if(strcmp(buf,"START") && playerPointer < 4)
			{
				players[playerPointer] = si_other;
				playerPointer++;
			}
		
	}
	return 0;
}