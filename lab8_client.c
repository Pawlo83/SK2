#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
//server lab5

int main(int argc, char* argv[])
{
    WSADATA wd;
    SOCKET fd;
    struct sockaddr_in sa;
    struct hostent* he;
    char buf[1024];
    int i, rc, bufsize;

    int _read(int sfd, char *buf, int bufsize) {
        int i, rc = 0;
        do {
        i = recv(sfd, buf, bufsize, 0);
        if (i < 0) return i;
        bufsize -= i;
        buf += i;
        rc += i;
        } while (*(buf-1)!='\n');
        return rc;
    }

    WSAStartup(MAKEWORD(2, 2), &wd);
    he = gethostbyname(argv[1]);
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sa.sin_family = AF_INET;
    memcpy(&sa.sin_addr.s_addr, he->h_addr, he->h_length);
    sa.sin_port = htons(atoi(argv[2]));
    connect(fd, (struct sockaddr*)&sa, sizeof(sa));

    send(fd, argv[3], (strlen(argv[3])), 0);
    send(fd, "\n", 1, 0);

    bufsize = sizeof(buf);
    rc=_read(fd,&buf,bufsize);
    write(1, buf, rc);

    //recv(fd, buf, sizeof(buf), 0);

    //printf("%s:%s -- %s\n", argv[1], argv[2], buf);


    closesocket(fd);
    WSACleanup();

}
