
#include <stdio.h>

typedef struct Student {
    char name[50];
    int age;
    float gpa;
} Student;

int save_student(const Student *s, const char *filename)
{
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        return -1;
    }

    if (fprintf(fp, "%s %d %.2f\n",
                s->name, s->age, s->gpa) < 0) {
        fclose(fp);
        return -1;
    }

    if (fclose(fp) == EOF) {
        return -1;
    }

    return 0;
}

int load_student(const char *filename, Student *out)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        return -1;
    }

    if (fscanf(fp, "%49s %d %f",
               out->name, &out->age, &out->gpa) != 3) {
        fclose(fp);
        return -1;
    }

    if (fclose(fp) == EOF) {
        return -1;
    }

    return 0;
}

int main(void)
{
    Student original = {"Alice", 21, 3.75f};
    Student loaded = {0};

    printf("Saving student to file...\n");

    if (save_student(&original, "student.txt") != 0) {
        fprintf(stderr, "Error saving student.\n");
        return 1;
    }

    printf("Loading student from file...\n");

    if (load_student("student.txt", &loaded) != 0) {
        fprintf(stderr, "Error loading student.\n");
        return 1;
    }

    printf("Loaded student: %s, %d, GPA %.2f\n",
           loaded.name, loaded.age, loaded.gpa);

    return 0;
}
