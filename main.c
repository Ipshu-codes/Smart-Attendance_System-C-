#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structure definition for student */
struct student {
    int id;
    char name[50];
    int total_classes;
    int attended_classes;
};

/* Global array to store students */
struct student s[100];
int count = 0;   // Number of students

/* Function to save data into file */
void saveToFile() {
    FILE *fp = fopen("attendance.dat", "wb");   // Open file in write binary mode
    
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, fp);  // Save number of students
    fwrite(s, sizeof(struct student), count, fp);  // Save student data

    fclose(fp);   // Close file
}

/* Function to load data from file */
void loadFromFile() {
    FILE *fp = fopen("attendance.dat", "rb");   // Open file in read binary mode
    
    if (fp != NULL) {
        fread(&count, sizeof(int), 1, fp);  // Read number of students
        fread(s, sizeof(struct student), count, fp);  // Read student data
        fclose(fp);
    }
}

/* Function to add a new student */
void addStudent() {
    printf("\nEnter ID: ");
    scanf("%d", &s[count].id);

    printf("Enter Name: ");
    scanf(" %[^\n]", s[count].name);   // Read full name with spaces

    // Initialize attendance values
    s[count].total_classes = 0;
    s[count].attended_classes = 0;

    count++;   // Increase student count

    saveToFile();   // Save updated data
    printf("Student Added Successfully!\n");
}

/* Function to mark attendance for all students */
void markAttendance() {
    int i, present;

    for (i = 0; i < count; i++) {
        printf("\nMark attendance for %s (1=Present, 0=Absent): ", s[i].name);
        scanf("%d", &present);

        s[i].total_classes++;   // Increase total classes

        if (present == 1)
            s[i].attended_classes++;   // Increase attended count if present
    }

    saveToFile();   // Save updated data
    printf("\nAttendance Marked Successfully!\n");
}

/* Function to view attendance of a specific student */
void viewAttendance() {
    int id, i, found = 0;

    printf("\nEnter Student ID: ");
    scanf("%d", &id);

    for (i = 0; i < count; i++) {
        if (s[i].id == id) {
            float percentage = 0;

            if (s[i].total_classes != 0)
                percentage = (s[i].attended_classes * 100.0) / s[i].total_classes;

            printf("\nName: %s", s[i].name);
            printf("\nTotal Classes: %d", s[i].total_classes);
            printf("\nAttended Classes: %d", s[i].attended_classes);
            printf("\nAttendance Percentage: %.2f%%\n", percentage);

            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student Not Found!\n");
}

/* Function to display students with attendance below 75% */
void lowAttendance() {
    int i;

    printf("\nStudents Below 75%% Attendance:\n");

    for (i = 0; i < count; i++) {
        if (s[i].total_classes != 0) {
            float percentage = (s[i].attended_classes * 100.0) / s[i].total_classes;

            if (percentage < 75) {
                printf("%s (%.2f%%)\n", s[i].name, percentage);
            }
        }
    }
}

/* Main function - Entry point of program */
int main() {
    int choice;

    loadFromFile();   // Load previous data when program starts

    while (1) {   // Infinite loop until user exits
        printf("\n\n--- Smart Attendance System ---\n");
        printf("1. Add Student\n");
        printf("2. Mark Attendance\n");
        printf("3. View Attendance\n");
        printf("4. Show Low Attendance\n");
        printf("5. Exit\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: 
                addStudent();
                break;

            case 2: 
                markAttendance();
                break;

            case 3: 
                viewAttendance();
                break;

            case 4: 
                lowAttendance();
                break;

            case 5: 
                printf("Exiting Program...\n");
                exit(0);

            default: 
                printf("Invalid Choice!\n");
        }
    }

    return 0;
}
