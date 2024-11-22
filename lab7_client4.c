#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void main(int argc, char* argv[]){
	int fd, rc;
	socklen_t sl;
	char buf[10];
	
	int on = 1;
	fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	
	struct sockaddr_in sa;
	struct hostent* he;
	he = gethostbyname(argv[1]);
	sa.sin_family = AF_INET;
	memcpy(&sa.sin_addr.s_addr, he->h_addr, he->h_length);
	sa.sin_port = htons(atoi(argv[2]));
	
	//write(sfd, argv[3], (strlen(argv[3])));
  	//rc = read(sfd, buf, sizeof(buf));
  	//write(1, buf, rc);
	
	sendto(fd, argv[3], (strlen(argv[3])), 0, (struct sockaddr*)&sa, sizeof(sa));
	sl = sizeof(sa);
	sa.sin_port = htons(1235);
	sa.sin_addr.s_addr = INADDR_BROADCAST;
	rc = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&sa, &sl);
	write(1, buf, rc);
	close(fd);
}
