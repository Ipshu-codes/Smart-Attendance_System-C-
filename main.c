#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structure for student */
struct student {
    char id[20];          // Roll number
    char name[50];        // Name
    int total_classes;
    int attended_classes;
};

/* Global array of students */
struct student s[100];
int count = 0; // number of students

/* Save students to file */
void saveToFile() {
    FILE *fp = fopen("attendance.dat", "wb");
    if(!fp) { printf("Error opening file!\n"); return; }

    fwrite(&count, sizeof(int), 1, fp);
    fwrite(s, sizeof(struct student), count, fp);
    fclose(fp);
}

/* Load students from file */
void loadFromFile() {
    FILE *fp = fopen("attendance.dat", "rb");
    if(fp) {
        fread(&count, sizeof(int), 1, fp);
        fread(s, sizeof(struct student), count, fp);
        fclose(fp);
    }
}

/* Add student */
void addStudent() {
    char id_input[20], name_input[50];
    int exists = 0;

    printf("\nEnter Roll Number: ");
    scanf("%s", id_input);

    // Check duplicate ID
    for(int i=0; i<count; i++) {
        if(strcmp(s[i].id, id_input) == 0) {
            exists = 1;
            break;
        }
    }
    if(exists) { printf("Roll number already exists! Enter unique ID.\n"); return; }

    printf("Enter Name: ");
    scanf(" %[^\n]", name_input);

    // Check duplicate Name
    exists = 0;
    for(int i=0; i<count; i++) {
        if(strcmp(s[i].name, name_input) == 0) {
            exists = 1;
            break;
        }
    }
    if(exists) { printf("Name already exists! Enter a different name.\n"); return; }

    // Save new student
    strcpy(s[count].id, id_input);
    strcpy(s[count].name, name_input);
    s[count].total_classes = 0;
    s[count].attended_classes = 0;
    count++;

    saveToFile();
    printf("Student added successfully!\n");
}

/* Mark attendance */
void markAttendance() {
    int present;
    for(int i=0; i<count; i++) {
        int valid = 0;
        while(!valid) {
            printf("\nMark attendance for %s (1=Present, 0=Absent): ", s[i].name);
            if(scanf("%d", &present)==1 && (present==0 || present==1)) {
                valid = 1;
                s[i].total_classes++;
                if(present==1) s[i].attended_classes++;
            } else {
                printf("Invalid input! Enter 1 for Present or 0 for Absent.\n");
                while(getchar() != '\n'); // clear input buffer
            }
        }
    }
    saveToFile();
    printf("\nAttendance marked successfully!\n");
}

/* View student attendance */
void viewAttendance() {
    char id_input[20];
    int found = 0;
    printf("\nEnter Student Roll Number: ");
    scanf("%s", id_input);

    for(int i=0; i<count; i++) {
        if(strcmp(s[i].id, id_input)==0) {
            found = 1;
            float perc = 0;
            if(s[i].total_classes != 0)
                perc = (s[i].attended_classes * 100.0)/s[i].total_classes;

            printf("\nName: %s\nTotal Classes: %d\nAttended: %d\nAttendance: %.2f%%\n",
                   s[i].name, s[i].total_classes, s[i].attended_classes, perc);
            break;
        }
    }

    if(!found) printf("Student not found! Please enter a valid roll number.\n");
}

/* Show students below 75% attendance */
void lowAttendance() {
    int flag = 0;
    printf("\nStudents below 75%% attendance:\n");
    for(int i=0; i<count; i++) {
        if(s[i].total_classes != 0) {
            float perc = (s[i].attended_classes * 100.0)/s[i].total_classes;
            if(perc < 75) {
                printf("%s (%s) - %.2f%%\n", s[i].name, s[i].id, perc);
                flag = 1;
            }
        }
    }
    if(!flag) printf("All students have attendance >= 75%%\n");
}

/* Main function */
int main() {
    int choice;
    loadFromFile();

    while(1) {
        printf("\n\n--- Smart Attendance System ---\n");
        printf("1. Add Student\n");
        printf("2. Mark Attendance\n");
        printf("3. View Attendance\n");
        printf("4. Show Low Attendance\n");
        printf("5. Exit\n");

        int valid = 0;
        while(!valid) {
            printf("Enter choice: ");
            if(scanf("%d", &choice)==1 && choice>=1 && choice<=5) {
                valid = 1;
            } else {
                printf("Invalid choice! Enter a number 1-5.\n");
                while(getchar()!='\n');
            }
        }

        switch(choice) {
            case 1: addStudent(); break;
            case 2: markAttendance(); break;
            case 3: viewAttendance(); break;
            case 4: lowAttendance(); break;
            case 5: printf("Exiting program...\n"); exit(0);
        }
    }

    return 0;
}