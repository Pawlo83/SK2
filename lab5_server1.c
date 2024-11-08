#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void main(int argc, char* argv[]){
  int cfd, i, rc, bufsize;
  char buf[256];
  
int _read(int sfd, char *buf, int bufsize) {
	int i, rc = 0;
	do {
	i = read(sfd, buf, bufsize);
	if (i < 0) return i;
	bufsize -= i;
	buf += i;
	rc += i;
	//printf("%d\n",buf-1);
	//printf("%d\n",buf);
	} while (*(buf-1)!='\n');
	return rc;
}
  
  socklen_t sl;
  struct sockaddr_in saddr, caddr;
  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = INADDR_ANY;
  saddr.sin_port = htons(1234);
  int sfd, on = 1;
  sfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on,
  sizeof(on));
  bind(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
  listen(sfd, 10);
  while(1) {
    sl = sizeof(caddr);
    cfd = accept(sfd, (struct sockaddr*) &caddr, &sl);
    printf("new connection from %s:%d\n", inet_ntoa((struct in_addr)caddr.sin_addr), ntohs(caddr.sin_port));
    
    i=0;
    /*while(rc[i]=='\n'){
  	rc[i] = read(sfd, buf, sizeof(buf));
  	printf("%c\n",rc[i]);
  	i++;
    }*/
    bufsize = sizeof(buf);
    _read(cfd,&buf,bufsize);
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
    write(sfd, "\n", 1);
    //write(cfd, "Hello World!\n", 13);
    close(cfd);
  }
}
