#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h> // Include the time.h library for date and time functions

#define MAX_PATIENTS 100
#define MAX_DOCTORS 10
#define MAX_RECEPTIONISTS 5

#define FILENAME_PATIENTS "patients.txt"
#define FILENAME_DOCTORS "doctors.txt"
#define FILENAME_RECEPTIONISTS "receptionists.txt"

#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_CYAN "\033[36m"

#define SCREEN_WIDTH 160 // Adjust this for your console width

// Admin credentials
char adminUsername[] = "admin";
char adminPassword[] = "admin123";

//Structure for Patient
struct Patient {
    int id;
    char date[15];      // Field for date
    char name[50];
    int age;
    char disease[50];
    char address[100];
    char mobile[15];
    char gender[10];    // Added gender field
    char username[20];
    char password[20];
};

//Structure for Doctor
struct Doctor {
    int id;
    char name[50];
    char speciality[50];
    char username[20];
    char password[20];
};

//Structure for Receptionist
struct Receptionist {
	int id;
    char name[50];    
    char username[50];
    char password[50];
};

//Arrays to store Patients, Doctors & Receptionists
struct Patient patients[MAX_PATIENTS];
struct Doctor doctors[MAX_DOCTORS];
struct Receptionist receptionists[MAX_RECEPTIONISTS];

int patientCount = 0, doctorCount = 0, receptionistCount = 0;

//Function Prototypes

//Dataload Function
void loadData();//1

//Center Align Function
void printCentered(const char *text);//2
void printCenteredInput(const char *text);//3

//Patient Functions
void addPatient();//4
void displayPatients();//5
void editPatient();//6
void deletePatient();//7
void patientLogin();//8
void displayRecordsByUsername(const char *username);//9
void savePatients();//10

//Doctors Function
void addDoctor();//11
void displayDoctors();//12
void editDoctor();//13
void deleteDoctor();//14
void doctorLogin();//15
void doctorActions(int doctorIndex);//16
void displayDoctorProfile(int doctorIndex);//17
void saveDoctors();//18

//Receptionist Function
void addReceptionist();//19
void displayReceptionists();//20
void editReceptionist();//21
void deleteReceptionist();//22
void saveReceptionists();//23

//Login Function
int login();//24 (Admin Login)
int receptionistLogin();//25

//Menu Function
void patientMenu();//26
void doctorMenu();//27
void manageReceptionists();//28 (Manages Only Patients)
void adminMenu();//29
void receptionistMenu();//30

//Main Function
int main() {
    // Load existing patient data
    loadData();

    int choice;
	
    while (1) {
        system("cls");
        printf("\n");
        printCentered("------------------------------------------------------------------------");
        printCentered("|                      HOSPITAL MANAGEMENT SYSTEM                      |");
        printCentered("------------------------------------------------------------------------");
        printCentered("|                         1. Admin Login                               |");
        printCentered("|                         2. Receptionist Login                        |");
        printCentered("|                         3. Patient Login                             |");
        printCentered("|                         4. Doctor Login                              |");
        printCentered("|                         5. Exit                                      |");
        printCentered("------------------------------------------------------------------------");

        // Center-aligned prompt for choice
        printCenteredInput("Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (login()) {
                    adminMenu(); // Admin management
                }
                break;
            case 2:
                if (receptionistLogin()) {
                    receptionistMenu(); // Receptionist management
                }
                break;
            case 3:
                patientLogin();
                break;
            case 4:
                doctorLogin();
                break;
            case 5:
            	printf("\n");
                printCentered(COLOR_GREEN"Exiting Program. Goodbye!\n"COLOR_RESET);
                return 0;
            default:
            	printf("\n");
                printCentered(COLOR_RED"Invalid Option. Please Try Again.\n"COLOR_RESET);
        }
    }

    return 0;
}

// 1. Function to load all data (Patients, Doctors, Receptionists)
void loadData() {
	
// Load Patients Data
FILE *file = fopen(FILENAME_PATIENTS, "r");
    if (file == NULL) {
        printCentered(COLOR_RED"No existing patient data found.\n"COLOR_RESET);
        return;
    }

    while (fscanf(file, "%d\n%[^\n]\n%d\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n",
                  &patients[patientCount].id,
                  patients[patientCount].name,
                  &patients[patientCount].age,
                  patients[patientCount].gender, // Load gender
                  patients[patientCount].disease,
                  patients[patientCount].address,
                  patients[patientCount].mobile,
                  patients[patientCount].username,
                  patients[patientCount].password,
                  patients[patientCount].date) == 10) { // Expect exactly 10 fields
        		  patientCount++;
    }

    fclose(file);
//    printf("Loaded %d patient(s) from file.\n", patientCount);
    
// Load Doctors Data
file = fopen(FILENAME_DOCTORS, "r");
if (file != NULL) {
    while (fscanf(file, "%d %[^\n] %[^\n] %[^\n] %[^\n]",
            &doctors[doctorCount].id,
            doctors[doctorCount].name,
            doctors[doctorCount].speciality,
            doctors[doctorCount].username,   // Read the Username
            doctors[doctorCount].password)   // Read the Password
            != EOF) {
        
        doctorCount++;
        if (doctorCount >= MAX_DOCTORS) {
            printCentered(COLOR_RED"\nMaximum Doctor Limit Reached While Loading.\n"COLOR_RESET);
            break;
        }
    }
    fclose(file);
}
    
file = fopen(FILENAME_RECEPTIONISTS, "r");
    if (file != NULL) {
        while (fscanf(file, "%d\n", &receptionists[receptionistCount].id) != EOF) {
            // Read name, username, and password
            fgets(receptionists[receptionistCount].name, sizeof(receptionists[receptionistCount].name), file);
            fgets(receptionists[receptionistCount].username, sizeof(receptionists[receptionistCount].username), file);
            fgets(receptionists[receptionistCount].password, sizeof(receptionists[receptionistCount].password), file);

            // Remove trailing newlines
            receptionists[receptionistCount].name[strcspn(receptionists[receptionistCount].name, "\n")] = '\0';
            receptionists[receptionistCount].username[strcspn(receptionists[receptionistCount].username, "\n")] = '\0';
            receptionists[receptionistCount].password[strcspn(receptionists[receptionistCount].password, "\n")] = '\0';

            receptionistCount++;
            if (receptionistCount >= MAX_RECEPTIONISTS) {
                printCentered(COLOR_RED"\nMaximum Receptionist Limit Reached While Loading.\n"COLOR_RESET);
                break;
            }
        }
        fclose(file);
    } else {
        printCentered(COLOR_RED"Error opening file for reading.\n"COLOR_RESET);
    }
}

// 2. Printf Data Are Align Center
void printCentered(const char* text) {
    int width = SCREEN_WIDTH;  // The width of the console (adjust as needed for your screen size)
    int padding = (width - strlen(text)) / 2;  // Calculate how much space to add before the text
	int i;
    // Print spaces for padding
    for (i = 0; i < padding; i++) {
        printf(" ");
    }

    // Print the centered text
    printf("%s\n", text);
}

// 3. Scanf Data Are Align Center
void printCenteredInput(const char *text) {
	int i;
    int padding = (SCREEN_WIDTH - strlen(text)) / 2;
    for (i = 0; i < padding; i++) 
	printf(" ");
    printf("%s", text); // No newline to keep `scanf` aligned
}

//Patients Function

// 4. Add Patient Function
void addPatient() {
    system("cls"); // Clear the screen
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                            Patients List                             |");
    printCentered("========================================================================");
    printf("\n");
    displayPatients();
    printf("\n");

    if (patientCount >= MAX_PATIENTS) {
        printCentered(COLOR_RED"Cannot Add More Patients. Maximum Limit Reached.\n"COLOR_RESET);
        return;
    }

    struct Patient newPatient;

    // Auto-generate Patient ID
    newPatient.id = patientCount + 1;
	
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                           Add New Patient                            |");
    printCentered("========================================================================");
	printf("\n");
	
    char buffer[100];
    sprintf(buffer, "New Patient ID: %d\n", newPatient.id);
    printCentered(buffer);

    // Validate Patient Name
    int validName = 0;
    while (!validName) {
        printCenteredInput("Enter Patient Name: ");
        scanf(" %[^\n]", newPatient.name);
		int i;
        validName = 1;
        for (i = 0; newPatient.name[i] != '\0'; i++) {
            if (!isalpha(newPatient.name[i]) && newPatient.name[i] != ' ') {
                validName = 0;
                printCentered(COLOR_RED"Name must contain only alphabetic characters and spaces.\n"COLOR_RESET);
                break;
            }
        }

        if (strlen(newPatient.name) == 0) {
            validName = 0;
            printCentered(COLOR_RED"Name cannot be empty.\n"COLOR_RESET);
        }
    }

    // Validate Age
    int validAge = 0;
    while (!validAge) {
        printCenteredInput("Enter Patient Age: ");
        if (scanf("%d", &newPatient.age) != 1 || newPatient.age <= 0) {
            printCentered(COLOR_RED"Invalid input. Age must be a positive integer.\n"COLOR_RESET);
            while (getchar() != '\n');
        } else {
            validAge = 1;
        }
    }

    // Validate Disease
    int validDisease = 0;
    while (!validDisease) {
        printCenteredInput("Enter Patient Disease: ");
        scanf(" %[^\n]", newPatient.disease);
		int i;
        validDisease = 1;
        for (i = 0; newPatient.disease[i] != '\0'; i++) {
            if (!isalpha(newPatient.disease[i]) && newPatient.disease[i] != ' ') {
                validDisease = 0;
                printCentered(COLOR_RED"Disease can only contain letters and spaces.\n"COLOR_RESET);
                break;
            }
        }
    }

    // Validate Address
    printCenteredInput("Enter Patient Address: ");
    scanf(" %[^\n]", newPatient.address);

    // Validate Mobile Number
    int validMobile = 0;
    while (!validMobile) {
        printCenteredInput("Enter Patient Mobile (10 digits): ");
        scanf("%s", newPatient.mobile);

        if (strlen(newPatient.mobile) == 10 && strspn(newPatient.mobile, "0123456789") == 10) {
            validMobile = 1;
        } else {
            printCentered(COLOR_RED"Mobile number must be exactly 10 digits.\n"COLOR_RESET);
        }
    }

    // Validate Gender
    int validGender = 0;
    while (!validGender) {
        printCenteredInput("Enter Gender (Male/Female): ");
        scanf("%s", newPatient.gender);

        if (strcmp(newPatient.gender, "Male") == 0 || strcmp(newPatient.gender, "Female") == 0) {
            validGender = 1;
        } else {
            printCentered(COLOR_RED"Invalid Gender. Please enter either 'Male' or 'Female'.\n"COLOR_RESET);
        }
    }

    // Set Username
    printCenteredInput("Set Username: ");
    scanf("%s", newPatient.username);

    // Set Password (hidden)
    char ch;
    int pos = 0;
    char password[20] = {0};
    printCenteredInput("Set Password: ");
    while (1) {
        ch = getch();
        if (ch == '\r') {
            password[pos] = '\0';
            break;
        } else if (ch == '\b' && pos > 0) {
            printf("\b \b");
            pos--;
        } else if (pos < sizeof(password) - 1) {
            password[pos++] = ch;
            printf("*");
        }
    }
    printf("\n");
    strcpy(newPatient.password, password);

    // Automatically add the current date
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(newPatient.date, sizeof(newPatient.date), "%d/%m/%Y", tm_info);

    patients[patientCount++] = newPatient;
    savePatients();

    printCentered(COLOR_GREEN"\nPatient Added Successfully.\n"COLOR_RESET);
}

// 5. Display Patients Function
void displayPatients() {
    if (patientCount == 0) {
        printCentered(COLOR_RED"No Patients To Display.\n"COLOR_RESET);
        return;
    }
char row[200];
    // Print header with border design, center aligned
    printCentered("=======================================================================================================================================");
    sprintf(row, "| %-5s | %-25s | %-5s | %-10s | %-20s | %-15s | %-15s | %-15s |", 
                  "ID", "Name", "Age", "Gender", "Disease", "Mobile", "Date", "Username");
                  printCentered(row);
    printCentered("=======================================================================================================================================");

    int i;
    for (i = 0; i < patientCount; i++) {
        // Construct the row dynamically before passing to printCentered
        
        sprintf(row, "| %-5d | %-25s | %-5d | %-10s | %-20s | %-15s | %-15s | %-15s |", 
                patients[i].id, 
                patients[i].name, 
                patients[i].age, 
                patients[i].gender, 
                patients[i].disease, 
                patients[i].mobile, 
                patients[i].date, 
                patients[i].username);

        // Pass the formatted row string to printCentered to ensure the row is centered
        printCentered(row);  // Now passing the row (formatted string) to printCentered
    }

    printCentered("=======================================================================================================================================\n");
}

// 6. Edit Patient Functions
void editPatient() {
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                             Patients List                             |");
    printCentered("========================================================================");
    printf("\n");
    displayPatients();
    printf("\n");
    
    int id, found = 0, i;

    printf("\n");
    printCentered("========================================================================");
    printCentered("|                             Edit Patient                             |");
    printCentered("========================================================================");
    printf("\n");

    printCenteredInput("Enter Patient ID To Edit: ");
    scanf("%d", &id);

    for (i = 0; i < patientCount; i++) {
        if (patients[i].id == id) {
            found = 1;
            char buffer[100];
            sprintf(buffer, "Editing Details For Patient ID %d", id);
            printCentered(buffer);

            char temp[100]; // Temporary buffer for user input

            // Name
            sprintf(buffer, "Enter New Name (or press Enter to keep '%s'): ", patients[i].name);
            printCenteredInput(buffer);
            getchar(); // Clear newline character from previous input
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(patients[i].name, temp);  // Update the name
            }

            // Age
            sprintf(buffer, "Enter New Age (or press Enter to keep '%d'): ", patients[i].age);
            printCenteredInput(buffer);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                int age = atoi(temp); // Convert to integer
                if (age > 0) { // Validate positive integer
                    patients[i].age = age;
                } else {
                    printCentered(COLOR_RED"Invalid age input. Keeping the old value.\n"COLOR_RESET);
                }
            }

            // Disease
            sprintf(buffer, "Enter New Disease (or press Enter to keep '%s'): ", patients[i].disease);
            printCenteredInput(buffer);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(patients[i].disease, temp);  // Update the disease
            }

            // Address
            sprintf(buffer, "Enter New Address (or press Enter to keep '%s'): ", patients[i].address);
            printCenteredInput(buffer);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(patients[i].address, temp);  // Update the address
            }

            // Mobile
            sprintf(buffer, "Enter New Mobile No. (or press Enter to keep '%s'): ", patients[i].mobile);
            printCenteredInput(buffer);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                if (strlen(temp) == 10 && strspn(temp, "0123456789") == 10) {
                    strcpy(patients[i].mobile, temp); // Update mobile
                } else {
                    printCentered(COLOR_RED"Invalid mobile number input. Keeping the old value.\n"COLOR_RESET);
                }
            }

            // Username
            sprintf(buffer, "Enter New Username (or press Enter to keep '%s'): ", patients[i].username);
            printCenteredInput(buffer);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(patients[i].username, temp);  // Update the username
            }

            // Gender (Validation: Male or Female)
            int validGender = 0;
            while (!validGender) {
                sprintf(buffer, "Enter New Gender (or press Enter to keep '%s'): ", patients[i].gender);
                printCenteredInput(buffer);
                fgets(temp, sizeof(temp), stdin);
                if (temp[0] != '\n') { // Check if the user entered something
                    temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                    if (strcmp(temp, "Male") == 0 || strcmp(temp, "Female") == 0) {
                        strcpy(patients[i].gender, temp); // Update gender
                        validGender = 1;
                    } else {
                        printCentered(COLOR_RED"Invalid gender. Please enter 'Male' or 'Female'.\n"COLOR_RESET);
                    }
                } else {
                    validGender = 1; // User chose to keep the original gender
                }
            }

            // Password (hidden input)
            printCenteredInput("Set New Password (hidden, or press Enter to keep current password): ");
            char ch;
            int pos = 0;
            char newPassword[20] = ""; // Temporary buffer for password
            while (1) {
                ch = getch(); // Get character without displaying it
                if (ch == '\r') { // Enter key pressed
                    newPassword[pos] = '\0'; // Null-terminate the password
                    break;
                } else if (ch == '\b' && pos > 0) { // Backspace pressed
                    printf("\b \b"); // Remove character from console
                    pos--;
                } else if (pos < sizeof(newPassword) - 1) { // Normal character
                    newPassword[pos++] = ch;
                    printf("*"); // Display asterisk
                }
            }
            printf("\n");

            if (strlen(newPassword) > 0) { // Update password only if a new one was entered
                strcpy(patients[i].password, newPassword);
            }

            savePatients();
            printCentered(COLOR_GREEN"Patient Details Updated Successfully.\n"COLOR_RESET);
            break;
        }
    }

    if (!found) {
        char error[50];
        sprintf(error, "Patient with ID %d not found.", id);
        printCentered(COLOR_RED);
        printCentered(error);
        printCentered(COLOR_RESET);
    }
}

// 7. Delete Patient Function
void deletePatient(){
	
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                            Patients List                             |");
    printCentered("========================================================================");
    printf("\n");
    displayPatients();
    printf("\n");
    
	int id, i, j, found=0;

    printf("\n");
    printCentered("========================================================================");
	printCentered("|                            Delete Patient                            |");
    printCentered("========================================================================");
    printf("\n");
    
	// Prompt for patient ID to delete
    printCenteredInput("Enter Patient ID To Delete: ");
    scanf("%d", &id);

    for (i = 0; i < patientCount; i++) {
        if (patients[i].id == id) {
            found = 1;
            for (j = i; j < patientCount - 1; j++) {
                patients[j] = patients[j + 1]; // Shift left
            }
            patientCount--;
            savePatients();
            printCentered(COLOR_GREEN"Patient Deleted Successfully.\n"COLOR_RESET);
            break;
        }
    }

    if (!found) {
        char errorMessage[50];
        sprintf(errorMessage, "Patient With ID %d Not Found.", id);
        printCentered(COLOR_RED);
        printCentered(errorMessage);
        printCentered(COLOR_RESET);
    }
}

// 8. Patient Login
void patientLogin() {
    system("cls"); // Clear the screen

    char username[20], password[20];
    char ch;
    int i = 0;
    
    printf("\n");
    printCentered("========================================================================");
    printCentered("|                            Patient Login                             |");
    printCentered("========================================================================");
    printf("\n");

    // Input for username
    printCenteredInput("Enter Username: ");
    scanf("%s", username);
    getchar(); // Clear the newline character left by scanf

    // Input for password with hidden characters (using getch)
    printCenteredInput("Enter Password: ");
    while ((ch = getch()) != '\r') { // Enter key to stop
        if (ch == '\b' && i > 0) { // Handle backspace
            printf("\b \b");
            i--;
        } else if (ch != '\b') {
            password[i++] = ch;
            printf("*"); // Display asterisk for each character
        }
    }
    password[i] = '\0'; // Null terminate the password string
    printf("\n"); // Move to the next line after the password input

    // Display all records with the same username
    displayRecordsByUsername(username);
}

// 9. Display All Records with the Same Username
void displayRecordsByUsername(const char *username) {
    int found = 0, i;

    // Loop through patients to find the matching username
    for (i = 0; i < patientCount; i++) {
        if (strcmp(patients[i].username, username) == 0) {
            found = 1;
            char row[200];
            sprintf(row, COLOR_GREEN, "\nLogin Successful! Welcome, %s.\n"COLOR_RESET, patients[i].name);
            sprintf(row, COLOR_GREEN, "\nPatient Records for Name: %s\n", patients[i].name);
            printCentered("=======================================================================================================================================");
            sprintf(row, "| %-5s | %-25s | %-5s | %-10s | %-20s | %-15s | %-15s | %-15s |",
                   "ID", "Name", "Age", "Gender", "Disease", "Mobile", "Date", "Username");
            		printCentered(row);
			printCentered("=======================================================================================================================================");

            // Print the patient's data in the formatted table
            sprintf(row, "| %-5d | %-25s | %-5d | %-10s | %-20s | %-15s | %-15s | %-15s |",
                   patients[i].id,
                   patients[i].name,
                   patients[i].age,
                   patients[i].gender,
                   patients[i].disease,
                   patients[i].mobile,
                   patients[i].date,
                   patients[i].username);
                   printCentered(row);
        }
    }

    if (!found) {
        printCentered(COLOR_RED"\nInvalid Username or Password. Please try again.\n"COLOR_RESET);
    } else {
        printCentered("=======================================================================================================================================\n");
    }

    // Pause for the user to read the result
    printCentered("\nPress Enter to continue...");
    getchar();
}

// 10. Function To Save Patient Data
void savePatients() {
    FILE *file = fopen(FILENAME_PATIENTS, "w");
    if (file == NULL) {
        printCentered(COLOR_RED"Error: Unable to save patients.\n"COLOR_RESET);
        return;
    }

    int i;
    for (i = 0; i < patientCount; i++) {
        fprintf(file, "%d\n%s\n%d\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
                patients[i].id,
                patients[i].name,
                patients[i].age,
                patients[i].gender,     // Save gender
                patients[i].disease,
                patients[i].address,
                patients[i].mobile,
                patients[i].username,
                patients[i].password,
                patients[i].date);
    }

    fclose(file);
    printCentered(COLOR_GREEN"Patients data saved successfully.\n"COLOR_RESET);
}

// 11. Add Doctor Function
void addDoctor() {
	
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                             Doctor List                              |");
    printCentered("========================================================================");
    printf("\n");
    displayDoctors();
    printf("\n");
    
    printf("\n");
    printCentered("========================================================================");
    printCentered("|                              Add Doctor                              |");
    printCentered("========================================================================");
    printf("\n");

    if (doctorCount >= MAX_DOCTORS) {
        printCentered(COLOR_RED"Cannot Add More Doctors. Maximum Limit Reached.\n"COLOR_RESET);
        return;
    }

    struct Doctor newDoctor;

    // Auto-generate a unique Doctor ID
    char buffer[50];
    sprintf(buffer, "New Doctor ID: %d", doctorCount + 1);
    printCentered(buffer);
    newDoctor.id = doctorCount + 1;

    // Validate Doctor Name
    int validName = 0, i;
    while (!validName) {
        printCenteredInput("Enter Doctor Name: ");
        scanf(" %[^\n]", newDoctor.name);

        validName = 1; // Assume the name is valid
        for (i = 0; newDoctor.name[i] != '\0'; i++) {
            if (!isalpha(newDoctor.name[i]) && newDoctor.name[i] != ' ' && newDoctor.name[i] != '.') {
                printCentered(COLOR_RED"Doctor name should only contain letters and spaces.\n"COLOR_RESET);
                validName = 0; // Invalid character found
                break;
            }
        }
    }

    // Validate Doctor Specialty
    int validSpecialty = 0;
    while (!validSpecialty) {
        printCenteredInput("Enter Doctor Specialty: ");
        scanf(" %[^\n]", newDoctor.speciality);

        validSpecialty = 1; // Assume the specialty is valid
        for (i = 0; newDoctor.speciality[i] != '\0'; i++) {
            if (!isalpha(newDoctor.speciality[i]) && newDoctor.speciality[i] != ' ' && newDoctor.speciality[i] != '.') {
                printCentered(COLOR_RED"Doctor specialty should only contain letters, spaces, and periods.\n"COLOR_RESET);
                validSpecialty = 0; // Invalid character found
                break;
            }
        }
    }

    // Validate Username
    printCenteredInput("Set a Username: ");
    scanf(" %[^\n]", newDoctor.username);

    // Validate and Hide Password Input
    printCenteredInput("Set a Password: ");
    char ch;
    int pos = 0;
    while (1) {
        ch = getch(); // Get character without displaying it
        if (ch == '\r') { // Enter key pressed
            newDoctor.password[pos] = '\0'; // Null-terminate password
            break;
        } else if (ch == '\b' && pos > 0) { // Backspace pressed
            printf("\b \b");
            pos--;
        } else if (pos < sizeof(newDoctor.password) - 1) {
            newDoctor.password[pos++] = ch;
            printf("*"); // Display asterisk
        }
    }
    printf("\n");

    // Save Doctor and Increment Count
    doctors[doctorCount++] = newDoctor;
    saveDoctors();
    printCentered(COLOR_GREEN"Doctor Added Successfully.\n"COLOR_RESET);
}

// 12. Display Doctor Function
void displayDoctors() {
//    system("cls");
    int i;
    char row[200];
    if (doctorCount == 0) {
        printf("\nNo Doctors To Display.\n");
        return;
    }

    // Display table headers with proper column widths
    printCentered("=================================================================================================================");
    sprintf(row, "| %-5s | %-25s | %-50s | %-20s |", "ID", "Name", "Specialty", "Username");
    printCentered(row);
    printCentered("=================================================================================================================");

    // Display each doctor's details
    for (i = 0; i < doctorCount; i++) {
        sprintf(row, "| %-5d | %-25s | %-50s | %-20s |",
               doctors[i].id,
               doctors[i].name,
               doctors[i].speciality,
               doctors[i].username); // Display Username
               printCentered(row);
    }

    // Footer for the table
    printCentered("=================================================================================================================\n");
}

// 13. Edit Doctor Function
void editDoctor() {
    int id, i, found = 0;
	
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                             Doctor List                              |");
    printCentered("========================================================================");
    printf("\n");
    displayDoctors();
    printf("\n");
    
    printf("\n");
    printCentered("========================================================================");
    printCentered("|                             Edit Doctor                              |");
    printCentered("========================================================================");
    printf("\n");
    
     printCenteredInput("Enter Doctor ID To Edit: ");
    scanf("%d", &id);

    for (i = 0; i < doctorCount; i++) {
        if (doctors[i].id == id) {
            found = 1;

            char buffer[100];
            sprintf(buffer, "Editing Details For Doctor ID %d", id);
            printCentered(buffer);

            char temp[100]; // Temporary buffer for user input

            // Name
            sprintf(buffer, "Enter Doctor's New Name (or press Enter to keep '%s'): ", doctors[i].name);
            printCenteredInput(buffer);
            getchar(); // Clear newline character from previous input
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(doctors[i].name, temp);    // Update the name
            }

            // Specialty
            sprintf(buffer, "Enter Doctor's New Specialty (or press Enter to keep '%s'): ", doctors[i].speciality);
            printCenteredInput(buffer);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(doctors[i].speciality, temp); // Update the specialty
            }

            // Username
            sprintf(buffer, "Enter Doctor's New Username (or press Enter to keep '%s'): ", doctors[i].username);
            printCenteredInput(buffer);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') {
                temp[strcspn(temp, "\n")] = '\0';
                strcpy(doctors[i].username, temp);
            }

            // Password (Hidden Input)
            printCenteredInput("Set New Password (hidden, or press Enter to keep current password): ");
            int pos = 0;
            char newPassword[20] = ""; // Temporary buffer for password
            while (1) {
                char ch = getch();
                if (ch == '\r') { // Enter key pressed
                    newPassword[pos] = '\0';
                    break;
                } else if (ch == '\b' && pos > 0) { // Backspace pressed
                    printf("\b \b");
                    pos--;
                } else if (pos < sizeof(newPassword) - 1) {
                    newPassword[pos++] = ch;
                    printf("*");
                }
            }
            printf("\n");

            if (strlen(newPassword) > 0) { // Update password only if a new one is entered
                strcpy(doctors[i].password, newPassword);
            }

            saveDoctors();
            printCentered(COLOR_GREEN"Doctor Details Updated Successfully.\n"COLOR_RESET);
            break;
        }
    }

    if (!found) {
        char errorBuffer[100];
        sprintf(errorBuffer, "Doctor With ID %d Not Found.", id);
        printCentered(COLOR_RED);
        printCentered(errorBuffer);
        printCentered(COLOR_RESET);
    }
}

// 14. Delete Doctor Function
void deleteDoctor(){
	
	int id, i, j, found=0;
	
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                             Doctor List                              |");
    printCentered("========================================================================");
    printf("\n");
    displayDoctors();
    printf("\n");
    
    printf("\n");
    printCentered("========================================================================");
    printCentered("|                            Delete Doctor                             |");
    printCentered("========================================================================");
    printf("\n");
    
	printCenteredInput("Enter Doctor ID To Delete: ");
    scanf("%d", &id);

    for (i = 0; i < doctorCount; i++) {
        if (doctors[i].id == id) {
            found = 1;
            for (j = i; j < doctorCount - 1; j++) {
                doctors[j] = doctors[j + 1]; // Shift Left
            }
            doctorCount--;
            saveDoctors();
            printCentered(COLOR_GREEN"Doctor Deleted Successfully.\n"COLOR_RESET);
            break;
        }
    }

    if (!found) {
        char errorMessage[100];
        sprintf(errorMessage, "Doctor With ID %d Not Found.", id);
        printCentered(COLOR_RED);
        printCentered(errorMessage);
        printCentered(COLOR_RESET);
    }
}

// 15. Doctor Login
void doctorLogin() {
//	system("cls");
    char inputUsername[20], inputPassword[20];
    int found = 0;
    
    printf("\n");
    printCentered("========================================================================");
    printCentered("|                             Doctor Login                             |");
    printCentered("========================================================================");
    printf("\n");
    
    // Input for username
    printCenteredInput("Enter Username: ");
    scanf("%s", inputUsername);

    // Input for password with hidden characters
    printCenteredInput("Enter Password: ");
    char ch;
    int pos = 0;
    while (1) {
        ch = getch(); // Get character without displaying it
        if (ch == '\r') { // Enter key pressed
            inputPassword[pos] = '\0'; // Null-terminate the password
            break;
        } else if (ch == '\b' && pos > 0) { // Backspace pressed
            printf("\b \b"); // Remove character from console
            pos--;
        } else if (pos < sizeof(inputPassword) - 1) { // Normal character
            inputPassword[pos++] = ch;
            printf("*"); // Display asterisk
        }
    }
    printf("\n");

    // Check credentials
    int i;
    for (i = 0; i < doctorCount; i++) {
        if (strcmp(doctors[i].username, inputUsername) == 0 &&
            strcmp(doctors[i].password, inputPassword) == 0) {
            found = 1;

            char successMessage[100];
            sprintf(successMessage, "Login Successful. Welcome, %s", doctors[i].name);
            printCentered(COLOR_GREEN);
            printCentered(successMessage);
            printCentered(COLOR_RESET);

            doctorActions(i); // Redirect to doctor-specific actions
            break;
        }
    }

    // Handle invalid login
    if (!found) {
        printCentered(COLOR_RED);
        printCentered("Invalid Username or Password. Please try again.");
        printCentered(COLOR_RESET);
    }
}

// 16. Doctor Menu
void doctorActions(int doctorIndex) {
    system("cls");
    int choice;

    // Print the header with design and centered
    do {
        printCentered("========================================================================");
    	printCentered("|                             DOCTOR MENU                              |");
        printCentered("========================================================================");
        printCentered("|                         1. View Patient Records                      |");
        printCentered("|                         2. View Profile                              |");
        printCentered("|                         3. Logout                                    |");
        printCentered("========================================================================");

        // Center-aligned prompt for choice
        printCenteredInput("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayPatients(); // Or filter patients by doctor if needed
                break;
            case 2:
                displayDoctorProfile(doctorIndex);
                break;
            case 3:
                printCentered("Logging out...");
                return;  // Exit the loop and log out
            default:
                printCentered(COLOR_RED"Invalid choice. Please try again.\n"COLOR_RESET);
        }
    } while (choice != 3);
}

// 17. Doctor Profile
void displayDoctorProfile(int doctorIndex) {
	char row[200];
	
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                            Doctor Profile                            |");
    printCentered("========================================================================");
    printf("\n");
    
    printCentered("=================================================================================================================");
    sprintf(row, "| %-5s | %-25s | %-50s | %-20s |",
           "ID", "Name", "Speciality", "Username");
           printCentered(row);
    printCentered("=================================================================================================================");

    // Print the doctor's information
    sprintf(row, "| %-5d | %-25s | %-50s | %-20s |",
           doctors[doctorIndex].id,
           doctors[doctorIndex].name,
           doctors[doctorIndex].speciality,
           doctors[doctorIndex].username);
           printCentered(row);

    printCentered("=================================================================================================================\n");
}

// 18. Function To Save Doctor Data
void saveDoctors() {
    FILE *file = fopen(FILENAME_DOCTORS, "w");
    if (file == NULL) {
        printCentered(COLOR_RED"\nError Opening File To Save Doctors!\n"COLOR_RESET);
        return;
    }

    int i;
    for (i = 0; i < doctorCount; i++) {
        // Save Doctor ID, Name, Specialty, Username, and Password
        fprintf(file, "%d\n%s\n%s\n%s\n%s\n\n",  // Include username and password
                doctors[i].id,
                doctors[i].name,
                doctors[i].speciality,
                doctors[i].username,
                doctors[i].password);  // Saving the password
    }

    fclose(file);
    printCentered(COLOR_GREEN"\nDoctor Data Saved To File.\n"COLOR_RESET);
}

// 19. Add Receptionist Function(Admin Only)
void addReceptionist() {
	
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                          Receptionists List                          |");
    printCentered("========================================================================");
    printf("\n");
    displayReceptionists();
    printf("\n");
	
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                           Add Receptionist                           |");
    printCentered("========================================================================");
    printf("\n");
    
    if (receptionistCount >= MAX_RECEPTIONISTS) {
        printCentered(COLOR_RED"Cannot Add More Receptionists. Maximum Limit Reached."COLOR_RESET);
        return;
    }

    struct Receptionist newReceptionist;

    // Auto-generate a unique Receptionist ID
    char buffer[50];
    newReceptionist.id = receptionistCount + 1;
    sprintf(buffer, "New Receptionist ID: %d", newReceptionist.id);
    printCentered(buffer);

    // Validate Receptionist Name
    int validName = 0, i;
    while (!validName) {
        printCenteredInput("Enter Receptionist Name: ");
        scanf(" %[^\n]", newReceptionist.name); // Read input with spaces

        validName = 1; // Assume the name is valid
        for (i = 0; newReceptionist.name[i] != '\0'; i++) {
            if (!isalpha(newReceptionist.name[i]) && newReceptionist.name[i] != ' ') {
                validName = 0; // Invalid character found
                printCentered(COLOR_RED"Name must contain only alphabetic characters and spaces."COLOR_RESET);
                break;
            }
        }
    }

    // Validate Username
    printCenteredInput("Enter Receptionist Username: ");
    scanf("%s", newReceptionist.username);

    // Validate Password (hidden input)
    printCenteredInput("Enter Receptionist Password: ");
    int j = 0;
    char ch;
    while ((ch = getch()) != '\r') { // '\r' is the Enter key
        if (ch == '\b' && j > 0) { // Handle backspace
            printf("\b \b");
            j--;
        } else if (j < sizeof(newReceptionist.password) - 1 && ch != '\b') {
            newReceptionist.password[j++] = ch;
            printf("*"); // Print '*' for each character entered
        }
    }
    newReceptionist.password[j] = '\0'; // Null-terminate the password
    printf("\n");

    // Save Receptionist and Increment Count
    receptionists[receptionistCount++] = newReceptionist;
    saveReceptionists();
    printCentered(COLOR_GREEN"Receptionist Added Successfully."COLOR_RESET);
}

// 20. Display Receptionist Function(Admin Only)
void displayReceptionists() {
    int i;
    char row[200];

    if (receptionistCount == 0) {
        printCentered(COLOR_RED"No Receptionists To Display.\n"COLOR_RESET);
        return;
    }

    // Print the table header with correct formatting
    printCentered("========================================================================================================================");
    sprintf(row, "| %-10s | %-50s | %-50s |", "ID", "Name", "Username");
    printCentered(row);
    printCentered("========================================================================================================================");

    // Loop through each receptionist and display their details
    for (i = 0; i < receptionistCount; i++) {
        sprintf(row, "| %-10d | %-50s | %-50s |", 
                receptionists[i].id, 
                receptionists[i].name, 
                receptionists[i].username);
        printCentered(row);
    }

    // Footer for the table
    printCentered("========================================================================================================================");
    printf("\n");
}

// 21. Edit Receptionist Function (Admin Only)
void editReceptionists() {
        int id, found = 0, i;
        char row[200];
        
        if (receptionistCount == 0) {
        printf("\nNo Receptionists To Edit.\n");
        return;
    }
    
    printf("\n");
    printCentered("========================================================================");
    printCentered("|                          Receptionists List                          |");
    printCentered("========================================================================");
    printf("\n");
    displayReceptionists();
    printf("\n");
	
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                          Edit Receptionist                           |");
    printCentered("========================================================================");
    printf("\n");
    
 printCenteredInput("Enter Receptionist ID To Edit: ");
    scanf("%d", &id);
    
    for (i = 0; i < receptionistCount; i++) {
        if (receptionists[i].id == id) {
            found = 1;
            char temp[100]; // Temporary buffer for user input
            sprintf(row, "Editing Details For Receptionist ID %d\n", id);
            printCentered(row);

            // Name
            sprintf(row, "Enter New Name (or press Enter to keep '%s'): ", receptionists[i].name);
            getchar(); // Clear newline character from previous input
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(receptionists[i].name, temp); // Update name
                printCentered(row);
            }

            // Username
            sprintf(row, "Enter New Username (or press Enter to keep '%s'): ", receptionists[i].username);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(receptionists[i].username, temp); // Update username
                printCentered(row);
            }

            // Password (hidden input)
            printCenteredInput("Enter New Password (or press Enter to keep existing Password): ");
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(receptionists[i].password, temp); // Update password
            }

            saveReceptionists();
            printCentered(COLOR_GREEN"Receptionist Details Updated Successfully.\n"COLOR_RESET);
            break;
        }
    }

    if (!found) {
        sprintf(row, COLOR_RED"Receptionist With ID %d Not Found.\n"COLOR_RESET, id);
        printCentered(row);
    }
}

// 22. Delete Receptionist Function(Admin Only)
void deleteReceptionist(){
	char row[200];
	if(receptionistCount == 0){
		printCentered(COLOR_RED"No Receptionists To Delete.\n"COLOR_RESET);
		return;
	}
	
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                          Receptionists List                          |");
    printCentered("========================================================================");
    printf("\n");
    displayReceptionists();
    printf("\n");
	
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                         Delete Receptionist                          |");
    printCentered("========================================================================");
    printf("\n");
	
	int id;
    printCenteredInput("Enter Receptionist ID To Delete: ");
    scanf("%d", &id);

    int i, j, found = 0;
    for (i = 0; i < receptionistCount; i++) {
        if (receptionists[i].id == id) {
            found = 1;
            for (j = i; j < receptionistCount - 1; j++) {
                receptionists[j] = receptionists[j + 1]; // Shift Left
            }
            receptionistCount--;
            saveReceptionists();
            printCentered(COLOR_GREEN"\nReceptionist Deleted Successfully.\n"COLOR_RESET);
            break;
        }
    }

    if (!found) {
        sprintf(row, COLOR_RED"\nReceptionist With ID %d Not Found.\n"COLOR_RESET, id);
        printCentered(row);
    }
}

// 23. Function To Save Receptionist Data
void saveReceptionists() {
	int i;
    FILE *file = fopen(FILENAME_RECEPTIONISTS, "w");
    if (file == NULL) {
        printCentered(COLOR_RED"\nError Opening File To Save Receptionists!\n"COLOR_RESET);
        return;
    }

    for (i = 0; i < receptionistCount; i++) {
        fprintf(file, "%d\n%s\n%s\n%s\n\n",
                receptionists[i].id,
                receptionists[i].name,
                receptionists[i].username,
                receptionists[i].password);
    }
    fclose(file);
    printCentered(COLOR_GREEN"\nReceptionist Data Saved To File.\n"COLOR_RESET);
}

// 24. Login Function For Admin
int login() {
    system("cls");
    char username[20], password[20];
	
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                             Admin Login                              |");
    printCentered("========================================================================");
    printf("\n");
    
    // Centered input prompt for Admin Username
    printCenteredInput("Enter Admin Username: ");
    scanf("%s", username);

    // Centered input prompt for Admin Password
    printCenteredInput("Enter Admin Password: ");
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r') { // '\r' is Enter key
        if (ch == '\b' && i > 0) { // Handle backspace
            printf("\b \b");
            i--;
        } else if (i < sizeof(password) - 1 && ch != '\b') {
            password[i++] = ch;
            printf("*"); // Display asterisk for each character
        }
    }
    password[i] = '\0'; // Null-terminate the password
    printf("\n");

    // Check if username and password match
    if (strcmp(username, adminUsername) == 0 && strcmp(password, adminPassword) == 0) {
        printCentered(COLOR_GREEN"\nAdmin Login Successful!\n"COLOR_RESET);
        return 1;
    } else {
        printCentered(COLOR_RED"\nInvalid Username or Password. Please try again.\n"COLOR_RESET);
        return 0;
    }
}

// 25. Login Function For Receptionist
int receptionistLogin() {
    system("cls");
    char username[20], password[20];
    int i;
	
	printf("\n");
    printCentered("========================================================================");
    printCentered("|                          Receptionist Login                          |");
    printCentered("========================================================================");
    printf("\n");
    
    // Centered input prompt for Receptionist Username
    printCenteredInput("Enter Receptionist Username: ");
    scanf("%s", username); // Use %s to read without spaces

    // Centered input prompt for Receptionist Password
    printCenteredInput("Enter Receptionist Password: ");
    int j = 0;
    char ch;
    while ((ch = getch()) != '\r') { // '\r' is the Enter key
        if (ch == '\b' && j > 0) { // Handle backspace
            printf("\b \b");
            j--;
        } else if (j < sizeof(password) - 1 && ch != '\b') {
            password[j++] = ch;
            printf("*"); // Display asterisk
        }
    }
    password[j] = '\0'; // Null-terminate the password
    printf("\n");

    // Check if username and password match
    for (i = 0; i < receptionistCount; i++) {
        if (strcmp(receptionists[i].username, username) == 0 && strcmp(receptionists[i].password, password) == 0) {
            printCentered(COLOR_GREEN"\nReceptionist Login Successful!\n"COLOR_RESET);
            return 1; // Login successful
        }
    }

    printCentered(COLOR_RED"\nInvalid Username or Password. Please try again.\n"COLOR_RESET);
    return 0; // Login failed
}

// 26. Display Patient Menu
void patientMenu() {
    int choice;
    do {
//        system("cls");

        // Display the menu with borders and center-alignment        
        printf("\n");
        printCentered("========================================================================");
        printCentered("|                             PATIENT MENU                             |");
        printCentered("========================================================================");
        printCentered("|                         1. Add Patient                               |");
        printCentered("|                         2. Display Patients                          |");
        printCentered("|                         3. Edit Patient                              |");
        printCentered("|                         4. Delete Patient                            |");
        printCentered("|                         5. Go Back                                   |");
        printCentered("========================================================================");
        printf("\n");

        // Center-aligned prompt for choice
        printCenteredInput("Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPatient(); // Calls the function to add a new patient
                break;
            case 2:
                displayPatients(); // Calls the function to display all patients
                break;
            case 3:
                editPatient(); // Calls the function to edit a patient's details
                break;
            case 4:
                deletePatient(); // Calls the function to delete a patient
                break;
            case 5:
                printCentered(COLOR_GREEN"\nReturning To Previous Menu...\n"COLOR_RESET);
                break;
            default:
                printCentered(COLOR_RED"\nInvalid Choice! Please Try Again.\n"COLOR_RESET);
        }
    } while (choice != 5); // Repeat until the user chooses to go back
}

// 27. Display Doctor Menu
void doctorMenu() {
    int choice;
    do {
//        system("cls");

		printf("\n");
		printCentered("========================================================================");
        printCentered("|                             DOCTOR MENU                              |");
		printCentered("========================================================================");
        printCentered("|                         1. Add Doctor                                |");
        printCentered("|                         2. Display Doctors                           |");
        printCentered("|                         3. Edit Doctor                               |");
        printCentered("|                         4. Delete Doctor                             |");
        printCentered("|                         5. Go Back                                   |");
		printCentered("========================================================================");
        printf("\n");

        // Center-aligned prompt for choice
        printCenteredInput("Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addDoctor();        // Calls the function to add a new doctor
                break;
            case 2:
                displayDoctors();   // Calls the function to display all doctors
                break;
            case 3:
                editDoctor();       // Calls the function to edit a doctor's details
                break;
            case 4:
                deleteDoctor();     // Calls the function to delete a doctor
                break;
            case 5:
                printCentered(COLOR_GREEN"\nReturning To Previous Menu...\n"COLOR_RESET);
                break;
            default:
                printCentered(COLOR_RED"\nInvalid Choice! Please Try Again.\n"COLOR_RESET);
        }
    } while (choice != 5);  // Repeat until the user chooses to go back
}

// 28. Display Receptionist Menu For Manage Only Patients
void receptionistMenu() {
    int choice;
    do {
//        system("cls");

		printf("\n");
		printCentered("========================================================================");
        printCentered("|                          RECEPTIONIST MENU                           |");
		printCentered("========================================================================");
        printCentered("|                         1. Add Patient                               |");
        printCentered("|                         2. Display Patients                          |");
        printCentered("|                         3. Edit Patient                              |");
        printCentered("|                         4. Delete Patient                            |");
        printCentered("|                         5. Go Back                                   |");
		printCentered("========================================================================");
        printf("\n");

        // Center-aligned prompt for choice
        printCenteredInput("Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPatient();        // Calls the function to add a new patient
                break;
            case 2:
                displayPatients();   // Calls the function to display all patients
                break;
            case 3:
                editPatient();       // Calls the function to edit a patient's details
                break;
            case 4:
                deletePatient();     // Calls the function to delete a patient
                break;
            case 5:
                printCentered(COLOR_GREEN"\nReceptionist Successfully Logout...\n"COLOR_RESET);
                break;
            default:
                printCentered(COLOR_RED"\nInvalid Choice! Please Try Again.\n"COLOR_RESET);
        }
    } while (choice != 5);  // Repeat until the user chooses to logout
}


// 29. Display Receptionist Menu Manages All Data
void adminMenu() {
    int choice;
    do {
//        system("cls");  // Clear the screen

		printf("\n");
		printCentered("========================================================================");
        printCentered("|                              ADMIN MENU                              |");
		printCentered("========================================================================");
        printCentered("|                         1. Manage Patients                           |");
        printCentered("|                         2. Manage Doctors                            |");
        printCentered("|                         3. Manage Receptionists                      |");
        printCentered("|                         4. Logout                                    |");
		printCentered("========================================================================");
        printf("\n");

        // Center-aligned prompt for choice
        printCenteredInput("Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                patientMenu();          // Calls the function to manage patients
                break;
            case 2:
                doctorMenu();           // Calls the function to manage doctors
                break;
            case 3:
                manageReceptionists();  // Calls the function to manage receptionists
                break;
            case 4:
                printCentered(COLOR_GREEN"\nAdmin Successfully Logged Out...\n"COLOR_RESET);
                break;
            default:
                printCentered(COLOR_RED"\nInvalid Choice! Please Try Again.\n"COLOR_RESET);
        }
    } while (choice != 4);  // Repeat until the user chooses to logout
}


// 30. Display Receptionist Menu Manage By Admin
void manageReceptionists() {
    int choice;
    do {
        system("cls");  // Clear the screen

		printf("\n");
		printCentered("========================================================================");
        printCentered("|                         MANAGE RECEPTIONISTS                         |");
		printCentered("========================================================================");
        printCentered("|                         1. Add Receptionist                          |");
        printCentered("|                         2. Display Receptionists                     |");
        printCentered("|                         3. Edit Receptionist                         |");
        printCentered("|                         4. Delete Receptionist                       |");
        printCentered("|                         5. Go Back                                   |");
		printCentered("========================================================================");
        printf("\n");

        // Center-aligned prompt for choice
        printCenteredInput("Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addReceptionist();  // Calls the function to add a new receptionist
                break;
            case 2:
                displayReceptionists();  // Calls the function to display all receptionists
                break;
            case 3:
                editReceptionists();  // Calls the function to edit a receptionist's details
                break;
            case 4:
                deleteReceptionist();  // Calls the function to delete a receptionist
                break;
            case 5:
                printCentered(COLOR_GREEN"\nReturning To Previous Menu...\n"COLOR_RESET);
                break;
            default:
                printCentered(COLOR_RED"\nInvalid Choice! Please Try Again.\n"COLOR_RESET);
        }
    } while (choice != 5);  // Repeat until the user chooses to go back
}
