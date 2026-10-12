
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_STUDENTS 100
#define NAME_SIZE 50
#define FILE_NAME "students.txt"

typedef struct Student {
    char name[NAME_SIZE];
    int id;
    float gpa;
} Student;

/* Reads one line and removes its trailing newline. */
static int read_line(char *buffer, size_t size)
{
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }

    size_t len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* Discard characters beyond the buffer. */
        }
    }

    return 1;
}

static int parse_int(const char *text, int *value)
{
    char *end;
    long result;

    errno = 0;
    result = strtol(text, &end, 10);

    while (*end == ' ' || *end == '\t') {
        end++;
    }

    if (text == end || *end != '\0' ||
        errno == ERANGE || result < 1 || result > 2147483647L) {
        return 0;
    }

    *value = (int)result;
    return 1;
}

static int parse_gpa(const char *text, float *value)
{
    char *end;
    float result;

    errno = 0;
    result = strtof(text, &end);

    while (*end == ' ' || *end == '\t') {
        end++;
    }

    if (text == end || *end != '\0' || errno == ERANGE ||
        result < 0.0f || result > 4.0f) {
        return 0;
    }

    *value = result;
    return 1;
}

int load_students(Student arr[], int max)
{
    FILE *fp = fopen(FILE_NAME, "r");
    char line[256];
    int count = 0;

    if (fp == NULL) {
        return 0;
    }

    while (count < max && fgets(line, sizeof(line), fp) != NULL) {
        Student s = {0};
        char extra;

        if (sscanf(line, " %49[^,],%d,%f %c",
                   s.name, &s.id, &s.gpa, &extra) != 3) {
            continue;
        }

        if (s.name[0] == '\0' ||
            strchr(s.name, ',') != NULL ||
            s.gpa < 0.0f || s.gpa > 4.0f) {
            continue;
        }

        size_t len = strlen(s.name);
        while (len > 0 &&
               (s.name[len - 1] == ' ' ||
                s.name[len - 1] == '\t')) {
            s.name[--len] = '\0';
        }

        if (s.name[0] == '\0') {
            continue;
        }

        arr[count++] = s;
    }

    fclose(fp);
    return count;
}

int save_students(const Student arr[], int count)
{
    FILE *fp = fopen(FILE_NAME, "w");

    if (fp == NULL) {
        perror("Error opening students file");
        return -1;
    }

    for (int i = 0; i < count; i++) {
        if (fprintf(fp, "%s,%d,%.2f\n",
                    arr[i].name, arr[i].id, arr[i].gpa) < 0) {
            fclose(fp);
            return -1;
        }
    }

    if (fclose(fp) == EOF) {
        return -1;
    }

    return 0;
}

void add_student(Student arr[], int *count)
{
    char name[256];
    char input[128];
    Student s = {0};

    if (*count >= MAX_STUDENTS) {
        printf("Cannot add more students (limit reached).\n");
        return;
    }

    printf("Enter name: ");
    if (!read_line(name, sizeof(name))) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    if (name[0] == '\0' || strchr(name, ',') != NULL ||
        strlen(name) >= NAME_SIZE) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    printf("Enter ID: ");
    if (!read_line(input, sizeof(input)) ||
        !parse_int(input, &s.id)) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    printf("Enter GPA: ");
    if (!read_line(input, sizeof(input)) ||
        !parse_gpa(input, &s.gpa)) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    strcpy(s.name, name);
    arr[*count] = s;
    (*count)++;

    printf("Student added successfully!\n");
}

void list_students(const Student arr[], int count)
{
    if (count == 0) {
        printf("No students found.\n");
        return;
    }

    printf("%-5s %-20s %s\n", "ID", "Name", "GPA");

    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %.2f\n",
               arr[i].id, arr[i].name, arr[i].gpa);
    }
}

int main(void)
{
    Student students[MAX_STUDENTS];
    int count = load_students(students, MAX_STUDENTS);
    char input[64];
    int choice;

    printf("Loaded %d student(s) from %s\n", count, FILE_NAME);

    for (;;) {
        printf("\n=== Student Management System ===\n");
        printf("1. List students\n");
        printf("2. Add student\n");
        printf("3. Save and Exit\n");
        printf("Select an option: ");

        if (!read_line(input, sizeof(input))) {
            break;
        }

        if (!parse_int(input, &choice)) {
            printf("Invalid input.\n");
            continue;
        }

        switch (choice) {
        case 1:
            list_students(students, count);
            break;
        case 2:
            add_student(students, &count);
            break;
        case 3:
            goto save_and_exit;
        default:
            printf("Invalid option.\n");
            break;
        }
    }

save_and_exit:
    if (save_students(students, count) != 0) {
        fprintf(stderr, "Error saving data.\n");
        return 1;
    }

    printf("Data saved. Goodbye!\n");
    return 0;
}
