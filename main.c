#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "ext2.h"

void usage(FILE *stream, char *p_name) {
	fprintf(stream, "Usage: %s device\n", p_name);
}

int main(int argc,char *argv[]) {
	if(argc < 2) {
		usage(stderr, argv[0]);
		exit(1);
	}

	printf("%s\n", argv[1]);

	int fd = open(argv[1], O_RDWR);
	if(fd < 0) {
		int err = errno;
		fprintf(stderr, "Error opening file: %s\n", strerror(err));
		exit(1);
	}

	

	create_ext2_filesystem(fd);

	close(fd);
  
  return 0;
}
