
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  const char* filename = (argc > 1) ? argv[1] : "data.txt";
  FILE* fp;
  char line[256];
  int line_count = 0;

  printf("Writing lines to %s...\n", filename);

  fp = fopen(filename, "w");
  if (fp == NULL) {
    perror("fopen");
    return EXIT_FAILURE;
  }

  fprintf(fp, "Hello, file I/O in C!\n");
  fprintf(fp, "This is another line.\n");

  if (fclose(fp) == EOF) {
    perror("fclose");
    return EXIT_FAILURE;
  }

  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("fopen");
    return EXIT_FAILURE;
  }

  printf("Reading contents:\n");

  while (fgets(line, sizeof(line), fp) != NULL) {
    printf("%s", line);
    line_count++;
  }

  if (ferror(fp)) {
    perror("fgets");
    fclose(fp);
    return EXIT_FAILURE;
  }

  if (fclose(fp) == EOF) {
    perror("fclose");
    return EXIT_FAILURE;
  }

  printf("Lines read: %d\n", line_count);

  return EXIT_SUCCESS;
}
