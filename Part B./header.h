#ifndef __HEADER__H
#define __HEADER__H

#include <stdio.h>
#include <string.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <time.h>
#include <pcap.h>

typedef enum {
    TRUE = 1,
    FALSE = 0
} boolean;

typedef struct sockaddr_in sockaddr_in;
typedef struct packet_mreq packet_mreq;
typedef struct sockaddr sockaddr;
typedef struct iphdr iphdr;
typedef struct icmphdr icmphdr;
typedef struct ethhdr ethhdr;

#define PACKET_LEN 65536
#endif