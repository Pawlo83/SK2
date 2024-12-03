#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]){
  int cfd1, cfd2, rc, bufsize, bug;
  socklen_t sl;
  char buf1[100000], buf2[100000];
  struct sockaddr_in saddr, caddr;

  int _read(int sfd, char *buf, int bufsize) {
  	int i, rc = 0;
  	do {
    	i = read(sfd, buf, bufsize);
    	if (i==0){
    		bug=bug+1;
    	}
    	if (i < 0) return i;
    	bufsize -= i;
    	buf += i;
    	rc += i;
  	} while (*(buf-1)!='\n' && bug!=3);
  	return rc;
  }
  
  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = INADDR_ANY;
  saddr.sin_port = htons(12345);
  int sfd, on = 1;
  sfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
  bind(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
  listen(sfd, 10);
  while(1) {
    sl = sizeof(caddr);
    cfd1 = accept(sfd, (struct sockaddr*) &caddr, &sl);
    printf("new connection from %s:%d\n", inet_ntoa((struct in_addr)caddr.sin_addr), ntohs(caddr.sin_port));
    cfd2 = accept(sfd, (struct sockaddr*) &caddr, &sl);
    printf("new connection from %s:%d\n", inet_ntoa((struct in_addr)caddr.sin_addr), ntohs(caddr.sin_port));
	  while(1){
	    bufsize = sizeof(buf1);
	    bug=0;
        rc=_read(cfd1,&buf1,bufsize);
        if(bug!=0){
            printf("ERROR c1: bug>=3");
        	break;
        }
        bug=0;
        rc=_read(cfd2,&buf2,bufsize);
        if(bug!=0){
            printf("ERROR c2: bug>=3");
        	break;
        }	    
        write(cfd1, buf2, rc);
        write(cfd2, buf1, rc);
	  }
    close(cfd1);
    close(cfd2);
  }
}
