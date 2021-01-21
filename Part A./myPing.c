
/*****************************************************************************/
/*** P.c                                                                   ***/
/***                                                                       ***/
/*** Use the ICMP protocol to request "echo" from destination.             ***/
/*****************************************************************************/

#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define PACKETSIZE 64
#define GOOGLE "8.8.8.8"
struct packet
{
	struct icmphdr hdr;
	char msg[PACKETSIZE - sizeof(struct icmphdr)];
};

int pid = -1;
struct protoent *proto = NULL;

/*--------------------------------------------------------------------*/
/*--- checksum - standard 1s complement checksum                   ---*/
/*--------------------------------------------------------------------*/
unsigned short checksum(void *b, int len)
{
	unsigned short *buf = b;
	unsigned int sum = 0;
	unsigned short result;

	for (sum = 0; len > 1; len -= 2)
		sum += *buf++;
	if (len == 1)
		sum += *(unsigned char *)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

/*--------------------------------------------------------------------*/
/*--- ping - Create message and send it.                           ---*/
/*--------------------------------------------------------------------*/
void ping(struct sockaddr_in *addr)
{
	int i, sd;
	struct packet pckt;
	struct timeval start, end;
	struct sockaddr_in r_addr;
	socklen_t r_addr_size = sizeof(r_addr);

	// Open a raw socket with IMCP protocol
	sd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sd < 0)
	{
		perror("socket");
		return;
	}

	// Initilize the parcket to zero
	bzero(&pckt, sizeof(pckt));

	// Start filling in the required fields such as type of protocol, it's id, the message itself, the sequence number and the checksum)
	pckt.hdr.type = ICMP_ECHO;
	pckt.hdr.un.echo.id = pid;
	for (i = 0; i < sizeof(pckt.msg) - 1; i++)
		pckt.msg[i] = i + '0';

	// Making sure the string ends with '\0'
	pckt.msg[i] = '\0';

	pckt.hdr.un.echo.sequence = 0;
	pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));
	//if ( setsockopt(sd, SOL_IP, IP_TTL, &val, sizeof(val)) != 0)
	//	perror("Set TTL option");
	if (fcntl(sd, F_SETFL, O_NONBLOCK) != 0)
		perror("Request nonblocking I/O");

	// Start tracking time just before sending the packet
	gettimeofday(&start, NULL);
	int wasnt_sent = 1;

	// Attempting to send one packet
	while (wasnt_sent)
	{
		wasnt_sent = sendto(sd, &pckt, sizeof(pckt), 0, (struct sockaddr *)addr, sizeof(*addr));
		if (wasnt_sent < 0)
			perror("sendto");
		else
		{
			printf("***Message was sent***\n");
			wasnt_sent = 0;
		}
	}

	// Attempting to receive the one packet that was sent
	int not_received = 1;
	while (not_received)
	{
		int received = recvfrom(sd, &pckt, sizeof(pckt), 0, (struct sockaddr *)&r_addr, &r_addr_size);
		if (received != -1)
		{
			not_received = 0;
			printf("***Got message!***\n");
		}
	}

	// Saving end time in order to calculate millisecond rtt and microsecond rtt
	gettimeofday(&end, NULL);
	double millisecond_rtt = (end.tv_sec * 1000 - start.tv_sec * 1000) + (end.tv_usec - start.tv_usec) / 1000;
	double microsecond_rtt = end.tv_usec - start.tv_usec;
	printf("the RTT in milliseconds is: %lf\n", millisecond_rtt);
	printf("the RTT in microseconds is: %lf\n", microsecond_rtt);
}

/*--------------------------------------------------------------------*/
/*--- main - look up host and start ping processes.                ---*/
/*--------------------------------------------------------------------*/
int main()
{
	struct hostent *hname;
	struct sockaddr_in addr;
	pid = getpid();
	// Choosing the desired protocol
	proto = getprotobyname("ICMP");
	// Converting the dest ip address to IPv4 binary form (in network byte order)
	hname = gethostbyname(GOOGLE);
	bzero(&addr, sizeof(addr));
	addr.sin_family = hname->h_addrtype;
	addr.sin_port = 0;
	addr.sin_addr.s_addr = *(long *)hname->h_addr;
	ping(&addr);
	return 0;
}

