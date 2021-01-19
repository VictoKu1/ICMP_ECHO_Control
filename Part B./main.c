#include <stdio.h>
#include <string.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
//* RUN THROUGH LINUX TERMINAL AS: sudo ./main .

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

#define MTU 1024

int main() {
    int PACKET_LEN = 65536;
    char buffer[PACKET_LEN];
    packet_mreq mr;
    sockaddr saddr;
    sockaddr_in source, dest;
    iphdr *ip;
    icmphdr *icmp;
    // Create the raw socket
    int sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock == -1) {
        goto clear;
    }

    // Turn on the promiscuous mode.
    mr.mr_type = PACKET_MR_PROMISC;
    setsockopt(sock, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr));
    unsigned int type;
    unsigned int code;
    printf("_________________________________________________________________________\n");
    printf("ICMP sniffing process initiated please wait till it will catch something:\n");
    printf("_________________________________________________________________________\n");
    // Getting captured packets
    while (TRUE) {
        bzero(buffer, PACKET_LEN);
        int data_size = recvfrom(sock, buffer, PACKET_LEN, 0, &saddr, (socklen_t*)sizeof(saddr));
        if (data_size == 0) {
            goto clear;
        }
        ip = (iphdr *) (buffer + sizeof(ethhdr));
        if (ip->protocol == 1) {
            icmp = (icmphdr *) (buffer + sizeof(ethhdr) + sizeof(iphdr));
            memset(&source, 0, sizeof(source));
            source.sin_addr.s_addr = ip->saddr;
            memset(&dest, 0, sizeof(dest));
            dest.sin_addr.s_addr = ip->daddr;
            printf("Source IP:%s\nDesination IP:%s\nType:%d\nCode:%d\n", inet_ntoa(source.sin_addr), inet_ntoa(dest.sin_addr),(unsigned int) icmp->type, (unsigned int) icmp->code);
            printf("_______________________________________________________________________\n");
        }
    }
    clear:
    close(sock);
    return 0;
}