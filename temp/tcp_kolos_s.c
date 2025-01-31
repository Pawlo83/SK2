#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    int cfd, rc, sfd, on;
    char buf[256];
    struct sockaddr_in saddr, caddr;
    socklen_t sl;

    sfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(1234);
    saddr.sin_addr.s_addr = INADDR_ANY;

    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on,
    sizeof(on));
    bind(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
    listen(sfd, 10);
    while(1) {
        sl = sizeof(caddr);
        cfd = accept(sfd, (struct sockaddr*) &caddr, &sl);
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
        close(cfd);
    }
}
