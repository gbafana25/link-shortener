#include "storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void get_short_link(int length, char *filename) {
	char *resp = "HTTP/1.1 200 OK\r\nContent-Length: 10\r\n";
	char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
	char *output[length];
	memset(&output, 0, sizeof(output));
	srand(time(NULL));
	for(int i = 0; i < length; i++) {
		int index = (rand() % strlen(alphabet));
		//output[i] = &alphabet[index];
		strncat((char *) &output, (const char * restrict) &alphabet[index], 1);
	}
	write_short_link(filename, (char *) &output);
}

void write_short_link(char *filename, char *contents) {
	FILE *store;
	store = fopen(filename, "a");
	fprintf(store, "{%s}\n", contents);
	fclose(store);
}

void write_source_link(char *filename, char *contents) {
	FILE *store;
	store = fopen(filename, "a");
	fprintf(store, "%s", contents);
	fclose(store);
}
