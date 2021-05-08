#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define PORT 8080
#define BUF_SIZE 512

int main() {
	int srv, client;
	char *data[BUF_SIZE];
	struct sockaddr_in srvaddr, clientaddr;
	char *create_request = "GET /create/";
	char *input = "?urlenc=";
	//char *rtmsg = "got link";
	
	srv = socket(AF_INET, SOCK_STREAM, 0);
	if(srv == -1) {
		perror("failed to create socket");
		exit(1);
	}


	setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, 0, sizeof(srv));
	memset(&srvaddr, 0, sizeof(srvaddr));	
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(PORT);
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	

	if(bind(srv, (struct sockaddr *) &srvaddr, sizeof(srvaddr)) == -1) {
		perror("failed to bind");
		exit(1);
	}
	
	if(listen(srv, 5) == -1) {
		perror("failed to start listening");
		exit(1);
	}

	memset(&clientaddr, 0, sizeof(clientaddr));
	int srv_size = sizeof(srvaddr);
	while(1) {
		client = accept(srv, (struct sockaddr *) &srvaddr, &srv_size);
		if(client == -1) {
			perror("client failed to connect");
			exit(1);
		} else {
			recv(client, data, BUF_SIZE, 0);
			char *base[1000];
			strncpy((char * restrict) &base, (const char * restrict) &data, strlen(create_request));
			if(strcmp((const char *) &base, create_request) == 0) {
				printf("got link creation request\n");
				if(strstr((const char *) &data, input) != NULL) {
					printf("found input link\n");
					char *path_start = strchr((const char *) &data, ' ');
					//printf(path_start);
					char *var_dec = strchr((const char *) &data, '?');
					char *src_url = strchr(var_dec, '=');
					char *end = strchr(src_url, ' ');
					char src[end-src_url];
					strncpy(src, src_url+1, end-(src_url+1));
					printf(src);
					close(client);
				}
			}
			//puts((const char *) &data);
		}
	}
}
