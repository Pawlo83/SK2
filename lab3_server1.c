#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


void childend(int signo) { wait(NULL); printf("CHILDEND\n");}
void main() {

	socklen_t sl;
	int sfd, cfd, P=2, i;
	struct sockaddr_in saddr, caddr;

	signal(SIGCHLD, childend);

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(1234);
	sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	listen(sfd, 10);

	void run() {
		while(1) {
			sl = sizeof(caddr);
			cfd = accept(sfd, (struct sockaddr*)&caddr, &sl);
			printf("[%d] new connection from: %s:%d\n", getpid(),
			inet_ntoa((struct in_addr)caddr.sin_addr),
			ntohs(caddr.sin_port));
			write(cfd, "Hello World!\n", 13);
			sleep(1000);
			close(cfd);
		}
	}
	
	for (i = 0; i < P; i++){
		if (!fork()){
			run();
		}
	}
	run();
}
