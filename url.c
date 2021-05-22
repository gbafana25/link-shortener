#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "storage.h"
#include "url.h"

void parse_url(char *data, char *start_string, char *filename) {
	char *start = strstr(data, start_string);
	char *src = strchr(start, '/');	
	char *end = strchr(src, ' ');
	char fin[end-(src+1)];
	memset(&fin, 0, sizeof(fin));
	strncpy(fin, src+1, end-(src+1));
	write_source_link(filename, fin); 

}
