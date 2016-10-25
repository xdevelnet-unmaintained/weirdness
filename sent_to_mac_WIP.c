#define _BSD_SOURCE

#include <stdio.h>
#include <iso646.h>
#include <string.h>
#include <stdint.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ether.h>
#include <stdlib.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEFAULT_MAXIMUM_ALLOWED_WINDOW_SIZE 5840
#define MAX_DATA_LEN 1000
#define DEFAULT_PACKED_IT 54321
#define DEFAULT_TTL 255

struct pseudo_header {
	uint32_t source_address;
	uint32_t dest_address;
	uint8_t placeholder;
	uint8_t protocol;
	uint16_t tcp_length;
};

typedef uint8_t octet;

// don't want to dazzle my eyes with these "sizeof(struct blablabla)" over whole file. Feel free to remove if u don't like it.
const size_t eth_header_len = sizeof(struct ether_header);
const size_t ip_header_len = sizeof(struct iphdr);
const size_t tcp_header_len = sizeof(struct tcphdr);
const size_t ptcp_header_len = sizeof(struct pseudo_header);

static unsigned short int csum(unsigned short int *ptr, int nbytes) { // proudly copypasted from public GPL source. Probably shitty, but IDC
	register long int sum;
	unsigned short int oddbyte;
	register short int answer;

	sum = 0;
	while(nbytes > 1) {
		sum += *ptr++;
		nbytes -= 2;
	}
	if (nbytes == 1) {
		oddbyte = 0;
		*((unsigned char *) &oddbyte) = *(unsigned char *) ptr;
		sum += oddbyte;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum = sum + (sum >> 16);
	answer = (short) ~sum;

	return(answer);
}

/*
 * |----------------------------------------------------------------|
 * |              |             |              |                    |
 * |  ETH HEADER  |  IP HEADER  |  TCP HEADER  |        DATA        |
 * |              |             |              |                    |
 * |----------------------------------------------------------------|
 *
 *  Some part of IP HEADER right before TCP HEADER will be used to store pseudo header structure.
 *  Yes, I wanna avoid redundant memmove(), memcpy(), malloc(), and other time-cost things.
 *
 *  http://www.tcpipguide.com/free/t_TCPChecksumCalculationandtheTCPPseudoHeader-2.htm
 */

void compose_tcp(void *mem, size_t datalength, uint16_t source_port, uint16_t destination_port, uint32_t source_ip, uint32_t dest_ip) {
	struct pseudo_header *psh = mem + eth_header_len + ip_header_len - ptcp_header_len;
	psh->source_address = source_ip;
	psh->dest_address = dest_ip;
	psh->placeholder = 0;
	psh->protocol = IPPROTO_TCP;
	psh->tcp_length = htons((uint16_t) (tcp_header_len + datalength));

	struct tcphdr *tcph = mem + eth_header_len + ip_header_len;
	tcph->source = htons(source_port);
	tcph->dest = htons(destination_port);
	tcph->seq = 0;
	tcph->ack_seq = 0;
	tcph->doff = 5;
	tcph->fin=0;
	tcph->syn=1;
	tcph->rst=0;
	tcph->psh=0;
	tcph->ack=0;
	tcph->urg=0;
	tcph->window = htons(DEFAULT_MAXIMUM_ALLOWED_WINDOW_SIZE);
	tcph->check = csum((unsigned short *) psh, (int) (ptcp_header_len + tcp_header_len + datalength));
	tcph->urg_ptr = 0;
}

void compose_ip(void *mem, size_t datalength, uint32_t source_ip, uint32_t dest_ip) { // data from ip PoV
	struct iphdr *iph = mem + eth_header_len;
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = (uint16_t) (ip_header_len + ptcp_header_len + datalength);
	iph->id = htonl(DEFAULT_PACKED_IT);
	iph->frag_off = 0;
	iph->ttl = DEFAULT_TTL;
	iph->protocol = IPPROTO_TCP;
	iph->check = 0; //Set to 0 before calculating checksum
	iph->saddr = source_ip;
	iph->daddr = dest_ip;
	iph->check = csum((unsigned short *) iph, iph->tot_len);
}

static inline uint32_t get_ip_from_if(int fd, const char *interface) {
	struct ifreq ifr = { .ifr_addr.sa_family = AF_INET };
	strncpy(ifr.ifr_name, interface, IFNAMSIZ-1);
	ioctl(fd, SIOCGIFADDR, &ifr);
	return ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr;
}

void compose_eth_frame(void *mem, size_t length) {
}

static inline int prepare_temp_socket() {
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if ((sockfd) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	return sockfd;
}
/*
int main() {
	int sockfd = prepare_temp_socket();
	uint32_t source_ip = get_ip_from_if(sockfd, "eth0");
	uint32_t destination_ip = inet_addr("192.168.1.1");
	close(sockfd);

	octet buffer[eth_header_len + ip_header_len + tcp_header_len + MAX_DATA_LEN] = {};

	size_t data_length = 5;

	compose_tcp(buffer, data_length, 1234, 9999, source_ip, destination_ip);
	compose_ip(buffer, data_length, source_ip, destination_ip);

	return printf("pseudo: %zu, tcphdr: %zu\n", sizeof(struct pseudo_header), sizeof(struct tcphdr));
}*/
