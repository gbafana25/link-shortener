#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void store_short_link(int length, char *filename) {
	char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
	char *output[length];
	memset(&output, 0, sizeof(output));
	srand(time(NULL));
	for(int i = 0; i < length; i++) {
		int index = (rand() % strlen(alphabet));
		//output[i] = &alphabet[index];
		strncat((char *) &output, (const char * restrict) &alphabet[index], 1);
	}
	FILE *file;	
	file = fopen(filename, "a");
	fprintf(file, "{%s}\n", output);
	fclose(file);
}
