#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <pcap.h>
#include <stdlib.h>

//* RUN THROUGH LINUX TERMINAL AS: sudo ./main .

typedef enum {
    TRUE = 1,
    FALSE = 0
} boolean;

#define MTU 1024

int main() {
    int PACKET_LEN = 65536;
    char buffer[PACKET_LEN];
    struct packet_mreq mr;

    // Create the raw socket
    int sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock == -1) {
        goto clear;
    }

    // Turn on the promiscuous mode.
    mr.mr_type = PACKET_MR_PROMISC;
    setsockopt(sock, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr));

    // Getting captured packets
    while (TRUE) {
        bzero(buffer, PACKET_LEN);
        int data_size = recvfrom(sock, buffer, PACKET_LEN, 0, NULL, NULL);
        if (data_size == -1) {
            goto clear;
        }
    }
    clear:
    close(sock);
    return 0;
}