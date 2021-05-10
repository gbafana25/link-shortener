#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *store_short_link(int length, char *filename) {
	char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
	char *output[length];
	srand(time(NULL));
	for(int i = 0; i < length; i++) {
		int index = (rand() % strlen(alphabet));
		//output[i] = &alphabet[index];
		strncat((char *) &output, &alphabet[index], 1);
	}
	FILE *file;	
	file = fopen(filename, "a");
	fprintf(file, "%s}\n", output);
	fclose(file);
}
