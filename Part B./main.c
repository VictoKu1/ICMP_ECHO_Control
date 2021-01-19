#include "header.h"

//* RUN THROUGH LINUX TERMINAL AS: sudo ./main .

int main() {
    char buffer[PACKET_LEN];
    packet_mreq mr;
    sockaddr saddres;
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
    printf("_________________________________________________________________________\n");
    printf("ICMP sniffing process initiated please wait till it will catch something:\n");
    printf("_________________________________________________________________________\n");
    int packetCounter = 0;

    // Getting captured packets
    while (TRUE) {
        bzero(buffer, PACKET_LEN); //* Setting all the cells to 0.
        int data_size = recvfrom(sock, buffer, PACKET_LEN, 0, &saddres, (socklen_t * )
        sizeof(saddres)); //* Receives information from socket .
        if (data_size == 0) {
            goto clear;
        }
        ip = (iphdr *) (buffer + sizeof(ethhdr)); //*IP header .
        if (((unsigned int) ip->protocol) == 1) {
            icmp = (icmphdr *) (buffer + sizeof(iphdr) + sizeof(ethhdr)); //*ICMP header .
            memset(&source, 0, sizeof(source));
            source.sin_addr.s_addr = ip->saddr;
            memset(&dest, 0, sizeof(dest));
            dest.sin_addr.s_addr = ip->daddr;
            time_t t;
            printf("Packet Number: %d\n", packetCounter);
            time(&t);
            printf("Time: %s", ctime(&t));
            printf("\n");
            printf("Source IP: %s\n", inet_ntoa(source.sin_addr));
            printf("Destination IP: %s\n", inet_ntoa(dest.sin_addr));
            printf("Type: %d\n", ((unsigned int) (icmp->type)));
            printf("Code: %d\n", ((unsigned int) (icmp->code)));
            printf("_______________________________________________________________________\n");
            packetCounter++;
        }
    }
    clear:
    close(sock);
    return 0;
}