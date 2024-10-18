#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


void main() {
	socklen_t sl;
	int sfd, cfd, L=2, limit=0, rc;
	char buf[256];
	struct sockaddr_in saddr, caddr;

	void childend(int signo) { wait(NULL); printf("CHILDEND\n");limit--;}	
	
	signal(SIGCHLD, childend);

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(1234);
	sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	listen(sfd, 10);

	while(1) {
		while(limit>=L){
			sleep(1);
		}
		sl = sizeof(caddr);
		limit++;
		cfd = accept(sfd, (struct sockaddr*)&caddr, &sl);
		if (!fork()) {
			printf("new connection from %s:%d\n", inet_ntoa((struct in_addr)caddr.sin_addr), ntohs(caddr.sin_port));
    
			rc = read(cfd, buf, sizeof(buf));
			write(1, buf, rc);
			printf("\n");
		    
		        if((strncmp(buf, "123456", 10))==0){
		        	write(cfd, "Y X\n", 5);
		        }
		        else{
		      		if((strncmp(buf, "654321", 10))==0){
					write(cfd, "X Y\n", 5);
			      	}
			      	else{
					write(cfd, "ERROR\n", 7);
		      		}
			}
			sleep(5);
			close(sfd);
			close(cfd);
			exit(0);
		}
		//close(cfd);
	}
}
