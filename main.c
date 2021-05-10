#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "storage.h"

#define PORT 8080
#define BUF_SIZE 512
#define SHURL_SIZE 10 // shortened url size

void getinputurl(char *data, char *filename) {
	printf("got link creation request\n");
	char *start = strstr(data, "create/");
	char *src = strchr(start, '/');
	char *end = strchr(src, ' ');
	char fin[end-(src+1)];
	strncpy(fin, src+1, end-(src+1));
	FILE *recs;
	recs = fopen(filename, "a");
	fprintf(recs, "%s{", (const char *) &fin);
	fclose(recs);

}

int main() {
	int srv, client;
	char *data[BUF_SIZE];
	struct sockaddr_in srvaddr, clientaddr;
	char *create_request = "GET /create/";
	//char *rtmsg = "got link";

	srv = socket(AF_INET, SOCK_STREAM, 0);
	if(srv == -1) {
		perror("failed to create socket");
		exit(1);
	}

	int opt = 1;
	setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
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
				getinputurl((char *) &data, "records.txt");
				//puts(source_url);
				store_short_link(SHURL_SIZE, "records.txt");
				//printf(test);
				close(client);
			}
			//puts((const char *) &data);
		}
	}
}
