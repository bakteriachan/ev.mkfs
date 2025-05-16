#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FLAG_IMPLEMENTATION
#include <flag.h>

void usage(FILE *stream, char *program_name) {
  fprintf(stream, "Usage: %s [OPTIONS] filename\n", program_name);
  fprintf(stream, "Options:\n");
  flag_print_options(stream);
}

int main(int argc,char *argv[]) {
  bool *print_help = flag_bool("help", false, "Print this message");
  size_t *size = flag_size("size", 1024, "The size of the disk");
  size_t *block_size = flag_size("bs", 1024, "Size of blocks");

  if(!flag_parse(argc, argv)) {
    usage(stderr, argv[0]);
    exit(1);
  }

  if(*print_help) {
    usage(stdout, argv[1]);
    exit(0);
  }

  int rest_argc = flag_rest_argc();
  if(rest_argc <= 0) {
    usage(stderr, argv[0]);
    exit(1);
  }

  printf("%d\n", (int)*size);
  printf("%d\n", (int)*block_size);
  return 0;
}
