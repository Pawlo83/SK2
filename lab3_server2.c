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
	struct cln {
		int cfd;
		struct sockaddr_in caddr;
	};
	void* cthread(void* arg) {
		struct cln* c = (struct cln*)arg;
		printf("[%lu] new connection from: %s:%d\n",
		(unsigned long int)pthread_self(),
		inet_ntoa((struct in_addr)c->caddr.sin_addr),
		ntohs(c->caddr.sin_port));
		write(c->cfd, "Hello World!\n", 13);
		sleep(1000);
		close(c->cfd);
		free(c);
		return 0;
	}
	pthread_t tid; socklen_t sl; int sfd;
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(1234);
	sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	listen(sfd, 10);
	
	while(1) {
		struct cln* c = malloc(sizeof(struct cln));
		sl = sizeof(c->caddr);
		c->cfd = accept(sfd, (struct sockaddr*)&c->caddr, &sl);
		pthread_create(&tid, NULL, cthread, c);
		pthread_detach(tid);
	}
}
