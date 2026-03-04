#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure for patient node
typedef struct Patient {
    char visitDate[20];
    long long patientID;
    char name[100];
    int age;
    char gender[10];
    char teethNumber[10];
    char treatmentName[100];
    char department[100];
    float treatmentValue;
    int billNumber;
    char grade[10];
    struct Patient* left;
    struct Patient* right;
} Patient;

// Convert string to lowercase for case-insensitive comparison
void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower((unsigned char)str[i]);
}

// Create a new patient node
Patient* createPatient(const char* date, long long id, const char* name, int age, const char* gender,
                       const char* teethNumber, const char* treatment, const char* department,
                       float value, int billNumber, const char* grade) {
    Patient* newPatient = (Patient*)malloc(sizeof(Patient));
    strcpy(newPatient->visitDate, date);
    newPatient->patientID = id;
    strcpy(newPatient->name, name);
    newPatient->age = age;
    strcpy(newPatient->gender, gender);
    strcpy(newPatient->teethNumber, teethNumber);
    strcpy(newPatient->treatmentName, treatment);
    strcpy(newPatient->department, department);
    newPatient->treatmentValue = value;
    newPatient->billNumber = billNumber;
    strcpy(newPatient->grade, grade);
    newPatient->left = newPatient->right = NULL;
    return newPatient;
}

// Insert patient into BST by patient ID
Patient* insertPatient(Patient* root, Patient* newPatient) {
    if (root == NULL)
        return newPatient;

    if (newPatient->patientID < root->patientID)
        root->left = insertPatient(root->left, newPatient);
    else if (newPatient->patientID > root->patientID)
        root->right = insertPatient(root->right, newPatient);

    return root;
}

// Case-insensitive name comparison
int compareNames(const char* name1, const char* name2) {
    char temp1[100], temp2[100];
    strcpy(temp1, name1);
    strcpy(temp2, name2);
    toLowerCase(temp1);
    toLowerCase(temp2);
    return strcmp(temp1, temp2);
}

// Search for a patient by name (case-insensitive)
Patient* searchByName(Patient* root, const char* name) {
    if (root == NULL)
        return NULL;

    if (compareNames(root->name, name) == 0)
        return root;

    Patient* found = searchByName(root->left, name);
    if (found)
        return found;

    return searchByName(root->right, name);
}

// Search for a patient by ID
Patient* searchByID(Patient* root, long long id) {
    if (root == NULL || root->patientID == id)
        return root;

    if (id < root->patientID)
        return searchByID(root->left, id);
    else
        return searchByID(root->right, id);
}

// Display all patient data (Inorder Traversal)
void displayPatients(Patient* root) {
    if (root != NULL) {
        displayPatients(root->left);
        printf("\n--- Patient Record ---\n");
        printf("Visit Date: %s\n", root->visitDate);
        printf("Patient ID: %lld\n", root->patientID);
        printf("Name: %s\n", root->name);
        printf("Age: %d\n", root->age);
        printf("Gender: %s\n", root->gender);
        printf("Teeth Number: %s\n", root->teethNumber);
        printf("Treatment Name: %s\n", root->treatmentName);
        printf("Department: %s\n", root->department);
        printf("Treatment Value: %.2f\n", root->treatmentValue);
        printf("Bill Number: %d\n", root->billNumber);
        printf("Grade: %s\n", root->grade);
        displayPatients(root->right);
    }
}

// Free BST memory
void freeTree(Patient* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// Parse line for patient.csv
int parsePatientCSV(char* line, char* date, long long* id, char* name, int* age,
                    char* gender, char* teethNumber, char* treatment,
                    char* department, float* value, int* billNumber, char* grade) {
    char* token;
    int field = 0;

    token = strtok(line, ",");
    while (token != NULL && field <= 11) {
        // Strip quotes
        if (token[0] == '"') {
            memmove(token, token+1, strlen(token));
            if (token[strlen(token)-1] == '"')
                token[strlen(token)-1] = '\0';
        }

        switch (field) {
            case 0: break; // Sno. ignore
            case 1: strcpy(date, token); break;
            case 2: *id = atoll(token); break;
            case 3: strcpy(name, token); break;
            case 4: *age = atoi(token); break;
            case 5: strcpy(gender, token); break;
            case 6: strcpy(teethNumber, token); break;
            case 7: strcpy(treatment, token); break;
            case 8: strcpy(department, token); break;
            case 9: *value = atof(token); break;
            case 10: *billNumber = atoi(token); break;
            case 11: strcpy(grade, token); break;
        }
        token = strtok(NULL, ",");
        field++;
    }
    return (field >= 12);
}

// Parse line for sheet_data.csv
int parseSheetCSV(char* line, char* date, long long* id, char* name, int* age,
                  char* gender, char* teethNumber, char* treatment,
                  char* department, float* value, int* billNumber, char* grade) {
    char* token;
    int field = 0;

    token = strtok(line, ",");
    while (token != NULL && field <= 10) {
        if (token[0] == '"') {
            memmove(token, token+1, strlen(token));
            if (token[strlen(token)-1] == '"')
                token[strlen(token)-1] = '\0';
        }

        switch (field) {
            case 0: strcpy(date, token); break;
            case 1: *id = atoll(token); break;
            case 2: strcpy(name, token); break;
            case 3: *age = atoi(token); break;
            case 4: strcpy(gender, token); break;
            case 5: strcpy(teethNumber, token); break;
            case 6: strcpy(treatment, token); break;
            case 7: strcpy(department, token); break;
            case 8: *value = atof(token); break;
            case 9: *billNumber = atoi(token); break;
            case 10: strcpy(grade, token); break;
        }
        token = strtok(NULL, ",");
        field++;
    }
    return (field >= 11);
}

// Load patients from file with parser selection
Patient* loadPatientsFromFile(const char* filename, Patient* root, int isSheet) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file %s\n", filename);
        return root;
    }

    char line[1024];
    int isFirstLine = 1;

    while (fgets(line, sizeof(line), fp)) {
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }

        line[strcspn(line, "\n")] = '\0';
        char date[20], name[100], gender[10], teethNumber[10];
        char treatment[100], department[100], grade[10];
        long long id;
        int age, billNumber;
        float value;

        int success = isSheet ? parseSheetCSV(line, date, &id, name, &age, gender,
                                              teethNumber, treatment, department,
                                              &value, &billNumber, grade)
                              : parsePatientCSV(line, date, &id, name, &age, gender,
                                                teethNumber, treatment, department,
                                                &value, &billNumber, grade);

        if (success) {
            Patient* newPatient = createPatient(date, id, name, age, gender, teethNumber,
                                                treatment, department, value, billNumber, grade);
            root = insertPatient(root, newPatient);
        }
    }

    fclose(fp);
    printf("Loaded data from %s\n", filename);
    return root;
}

// Save patient to patient.csv
void savePatientToFile(const char* filename, Patient* p) {
    FILE* fp = fopen(filename, "a");
    if (fp) {
        fprintf(fp, "0,%s,%lld,%s,%d,%s,%s,%s,%s,%.2f,%d,%s\n",
                p->visitDate, p->patientID, p->name, p->age, p->gender,
                p->teethNumber, p->treatmentName, p->department,
                p->treatmentValue, p->billNumber, p->grade);
        fclose(fp);
    } else {
        printf("Error writing to file.\n");
    }
}

// Main
int main() {
    Patient* root = NULL;
    int choice;
    const char* filename = "patient.csv";

    // Download Google Sheet CSV
    system("curl -L -o sheet_data.csv \"https://docs.google.com/spreadsheets/d/1Jbgsml5mLTMq6vKo68FYW3_I4dezCBWXDMscoZgcPdE/export?format=csv\"");

    root = loadPatientsFromFile("patient.csv", root, 0); // isSheet = 0
    root = loadPatientsFromFile("sheet_data.csv", root, 1); // isSheet = 1

    do {
        printf("\n=== Dental Clinic Patient Record System ===\n");
        printf("1. Add Patient\n2. Search by Patient ID\n3. Search by Name\n4. Display All Patients\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice); getchar();

        if (choice == 1) {
            long long id;
            int age, billNumber;
            float value;
            char date[20], name[100], gender[10], teeth[10], treatment[100], dept[100], grade[10];

            printf("Enter Visit Date (DD-MM-YYYY): "); fgets(date, 20, stdin); date[strcspn(date, "\n")] = 0;
            printf("Enter Patient ID: "); scanf("%lld", &id); getchar();
            printf("Enter Name: "); fgets(name, 100, stdin); name[strcspn(name, "\n")] = 0;
            printf("Enter Age: "); scanf("%d", &age); getchar();
            printf("Enter Gender: "); fgets(gender, 10, stdin); gender[strcspn(gender, "\n")] = 0;
            printf("Enter Teeth Number: "); fgets(teeth, 10, stdin); teeth[strcspn(teeth, "\n")] = 0;
            printf("Enter Treatment Name: "); fgets(treatment, 100, stdin); treatment[strcspn(treatment, "\n")] = 0;
            printf("Enter Department Name: "); fgets(dept, 100, stdin); dept[strcspn(dept, "\n")] = 0;
            printf("Enter Treatment Value: "); scanf("%f", &value); getchar();
            printf("Enter Bill Number: "); scanf("%d", &billNumber); getchar();
            printf("Enter Grade: "); fgets(grade, 10, stdin); grade[strcspn(grade, "\n")] = 0;

            Patient* p = createPatient(date, id, name, age, gender, teeth, treatment, dept, value, billNumber, grade);
            root = insertPatient(root, p);
            savePatientToFile(filename, p);
            printf("Patient added successfully.\n");

        } else if (choice == 2) {
            long long id;
            printf("Enter Patient ID: "); scanf("%lld", &id);
            Patient* p = searchByID(root, id);
            if (p) printf("Found: %s | ID: %lld | Treatment: %s | Value: %.2f\n", p->name, p->patientID, p->treatmentName, p->treatmentValue);
            else printf("Patient not found.\n");

        } else if (choice == 3) {
            char name[100];
            printf("Enter Name to search: "); fgets(name, 100, stdin); name[strcspn(name, "\n")] = 0;
            Patient* p = searchByName(root, name);
            if (p) printf("Found: %s | ID: %lld | Treatment: %s | Value: %.2f\n", p->name, p->patientID, p->treatmentName, p->treatmentValue);
            else printf("Patient not found.\n");

        } else if (choice == 4) {
            displayPatients(root);
        }

    } while (choice != 5);

    freeTree(root);
    printf("Program terminated.\n");
    return 0;
}
