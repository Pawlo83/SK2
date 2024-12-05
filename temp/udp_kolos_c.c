#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>

int main(int argc, char *argv[]){	
	int sfd, rc;
	socklen_t sl;
	struct sockaddr_in saddr;
	char buf[256];

	sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //SOCK_STREAM, IPPROTO_TCP
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(atoi(argv[2]));
	saddr.sin_addr.s_addr=inet_addr(argv[1]);

	//connect(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
  
  //write(sfd, argv[3], strlen(argv[3]));
	sl=sizeof(saddr);
	sendto(sfd,argv[3],strlen(argv[3]),0, (struct sockaddr*) &saddr, sl);

  //rc = read(sfd, buf, sizeof(buf));
	rc=recvfrom(sfd,buf,sizeof(buf), 0, (struct sockaddr*) &saddr, &sl);
	write(1,buf,rc);

  //close(sfd);
}
