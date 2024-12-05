#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>

int main(int argc, char *argv[]){	
	int sfd, rc, on;
	socklen_t sl;
	struct sockaddr_in saddr, caddr;
	char buf[256];

	sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //SOCK_STREAM, IPPROTO_TCP
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(1234);
	saddr.sin_addr.s_addr=INADDR_ANY;

	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
	bind(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
	//listen(sfd,10);
	while(1){
		sl=sizeof(caddr);
		//cfd=accept(sfd, (struct sockaddr*) &caddr, &sl);
		//printf("%s:%d", inet_ntoa((struct in_addr)caddr.sin_addr), ntohs(caddr.sin_port));

		//rc=read(cfd,buf,sizeof(buf));
		rc=recvfrom(sfd, buf, sizeof(buf), 0, (struct sockaddr*) &caddr, &sl);
		printf("%s:%d", inet_ntoa((struct in_addr)caddr.sin_addr), ntohs(caddr.sin_port));
		write(1,buf,rc);
		
		//write(cfd,buf,rc);
		sendto(sfd, buf, rc, 0, (struct sockaddr*) &caddr, sl);
	}
}
