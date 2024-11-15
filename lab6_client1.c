#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

void main(int argc, char* argv[]){
	int fd, rc;
	char buf[1024];
	struct sockaddr_in sa;
	struct hostent* he;
	SSL_CTX* ctx;
	SSL* ssl;

	SSL_load_error_strings();
	SSL_library_init();
	ctx = SSL_CTX_new(TLS_client_method());
	ssl = SSL_new(ctx);
	he = gethostbyname(argv[1]);
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(atoi(argv[2]));
	memcpy(&sa.sin_addr.s_addr, he->h_addr, he->h_length);
	connect(fd, (struct sockaddr*)&sa, sizeof(sa));
	SSL_set_fd(ssl, fd);
	SSL_connect(ssl);
	SSL_write(ssl, "Hello, Server!", 15);
	rc = SSL_read(ssl, buf, sizeof(buf));
	write(1, buf, rc);
	SSL_shutdown(ssl);
	SSL_free(ssl);
	close(fd);
	SSL_CTX_free(ctx);
	return EXIT_SUCCESS;
}
