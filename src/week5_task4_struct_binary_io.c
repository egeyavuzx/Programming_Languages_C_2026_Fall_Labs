
#include <stdio.h>

typedef struct Student {
  char name[50];
  int age;
  float gpa;
} Student;

int save_student_binary(const Student* s, const char* filename) {
  FILE* fp = fopen(filename, "wb");

  if (fp == NULL) {
    return -1;
  }

  if (fwrite(s, sizeof(Student), 1, fp) != 1) {
    fclose(fp);
    return -1;
  }

  if (fclose(fp) == EOF) {
    return -1;
  }

  return 0;
}

int load_student_binary(const char* filename, Student* out) {
  FILE* fp = fopen(filename, "rb");

  if (fp == NULL) {
    return -1;
  }

  if (fread(out, sizeof(Student), 1, fp) != 1) {
    fclose(fp);
    return -1;
  }

  if (fclose(fp) == EOF) {
    return -1;
  }

  return 0;
}

long file_size(const char* filename) {
  FILE* fp = fopen(filename, "rb");
  long size;

  if (fp == NULL) {
    return -1;
  }

  if (fseek(fp, 0, SEEK_END) != 0) {
    fclose(fp);
    return -1;
  }

  size = ftell(fp);

  if (size < 0) {
    fclose(fp);
    return -1;
  }

  if (fclose(fp) == EOF) {
    return -1;
  }

  return size;
}

int main(void) {
  Student original = {0};
  Student loaded = {0};
  FILE* fp;
  long text_size;
  long binary_size;

  snprintf(original.name, sizeof(original.name), "%s", "Alice");
  original.age = 21;
  original.gpa = 3.75f;

  printf("Saving student in binary format...\n");

  if (save_student_binary(&original, "student.bin") != 0) {
    fprintf(stderr, "Error saving binary file.\n");
    return 1;
  }

  fp = fopen("student.txt", "w");

  if (fp == NULL) {
    perror("Error opening text file");
    return 1;
  }

  if (fprintf(fp, "%s %d %.2f\n", original.name, original.age, original.gpa) <
      0) {
    fprintf(stderr, "Error writing text file.\n");
    fclose(fp);
    return 1;
  }

  if (fclose(fp) == EOF) {
    fprintf(stderr, "Error closing text file.\n");
    return 1;
  }

  printf("Loading student from binary file...\n");

  if (load_student_binary("student.bin", &loaded) != 0) {
    fprintf(stderr, "Error loading binary file.\n");
    return 1;
  }

  printf("Loaded student: %s, %d, GPA %.2f\n", loaded.name, loaded.age,
         loaded.gpa);

  text_size = file_size("student.txt");
  binary_size = file_size("student.bin");

  if (text_size < 0 || binary_size < 0) {
    fprintf(stderr, "Error determining file size.\n");
    return 1;
  }

  printf("Text file size: %ld bytes\n", text_size);
  printf("Binary file size: %ld bytes\n", binary_size);

  return 0;
}
