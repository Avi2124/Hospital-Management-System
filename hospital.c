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

//Patient Functions
void addPatient();//2
void displayPatients();//3
void editPatient();//4
void deletePatient();//5
void patientLogin();//6
void displayRecordsByUsername(const char *username);//7
void savePatients();//8

//Doctors Function
void addDoctor();//9
void displayDoctors();//10
void editDoctor();//11
void deleteDoctor();//12
void doctorLogin();//13
void doctorActions(int doctorIndex);//14
void displayDoctorProfile(int doctorIndex);//15
void saveDoctors();//16

//Receptionist Function
void addReceptionist();//17
void displayReceptionists();//18
void editReceptionist();//19
void deleteReceptionist();//20
void saveReceptionists();//21

//Login Function
int login();//22 (Admin Login)
int receptionistLogin();//23

//Menu Function
void patientMenu();//24
void doctorMenu();//25
void manageReceptionists();//26 (Manages Only Patients)
void adminMenu();//27
void receptionistMenu();//28

//Main Function
int main() {
	system("cls");
    // Load existing patient data
    loadData();

    int choice;

    while (1) {
        printf("==================================\n");
        printf("=== Hospital Management System ===\n");
        printf("==================================\n\n");
        printf("1. Admin Login\n");
        printf("2. Receptionist Login\n");
        printf("3. Patient Login\n");
        printf("4. Doctor Login\n");
        printf("5. Exit\n");
        printf("==================================\n\n");
        printf("Enter Your Choice: ");
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
                printf(COLOR_GREEN"\nExiting Program. Goodbye!\n"COLOR_RESET);
                return;
            default:
                printf(COLOR_RED"\nInvalid Option. Please Try Again.\n"COLOR_RESET);
        }
    }

    return 0;
}

// 1. Function to load all data (Patients, Doctors, Receptionists)

void loadData() {
	
// Load Patients Data
FILE *file = fopen(FILENAME_PATIENTS, "r");
    if (file == NULL) {
        printf(COLOR_RED"No existing patient data found.\n"COLOR_RESET);
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
            printf(COLOR_RED"\nMaximum Doctor Limit Reached While Loading.\n"COLOR_RESET);
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
                printf(COLOR_RED"\nMaximum Receptionist Limit Reached While Loading.\n"COLOR_RESET);
                break;
            }
        }
        fclose(file);
    } else {
        printf(COLOR_RED"Error opening file for reading.\n"COLOR_RESET);
    }
}

//Patients Function

// 2. Add Patient Function
void addPatient() {
    printf("=====================\n");
    printf("=== Patients List ===\n");
    printf("=====================\n\n");
    displayPatients();
    printf("\n");
    if (patientCount >= MAX_PATIENTS) {
        printf(COLOR_RED"\nCannot Add More Patients. Maximum Limit Reached.\n"COLOR_RESET);
        return;
    }
    
    struct Patient newPatient;

    // Auto-generate Patient ID
    newPatient.id = patientCount + 1;

    printf("=======================\n");
    printf("=== Add New Patient ===\n");
    printf("=======================\n\n");
    
    printf("New Patient ID: %d\n", newPatient.id);

    // Validate Patient Name
    int validName = 0, i;
    while (!validName) {
        printf("Enter Patient Name: ");
        scanf(" %[^\n]", newPatient.name); // Allow spaces in name

        validName = 1;  // Assume name is valid
        for (i = 0; newPatient.name[i] != '\0'; i++) {
            if (!isalpha(newPatient.name[i]) && newPatient.name[i] != ' ') {
                validName = 0;  // Found a non-alphabetic character
                printf(COLOR_RED"Name must contain only alphabetic characters and spaces.\n"COLOR_RESET);
                break;
            }
        }

        // Check for empty name
        if (strlen(newPatient.name) == 0) {
            validName = 0;
            printf(COLOR_RED"Name cannot be empty.\n"COLOR_RESET);
        }
    }

    // Validate Age (greater than 0 and numeric)
    int validAge = 0;
    while (!validAge) {
        printf("Enter Patient Age: ");
        if (scanf("%d", &newPatient.age) != 1 || newPatient.age <= 0) {
            printf(COLOR_RED"Invalid input. Age must be a positive integer.\n"COLOR_RESET);
            while (getchar() != '\n'); // Clear invalid input from buffer
        } else {
            validAge = 1; // Age is valid
        }
    }

    // Validate Disease
    int validDisease = 0;
    while (!validDisease) {
        printf("Enter Patient Disease: ");
        scanf(" %[^\n]", newPatient.disease); // Read disease allowing spaces

        validDisease = 1;  // Assume input is valid
        for (i = 0; newPatient.disease[i] != '\0'; i++) {
            if (!isalpha(newPatient.disease[i]) && newPatient.disease[i] != ' ') {
                validDisease = 0;  // Found an invalid character
                printf(COLOR_RED"Disease can only contain letters and spaces.\n"COLOR_RESET);
                break;
            }
        }
    }

    // Validate Address
    printf("Enter Patient Address: ");
    scanf(" %[^\n]", newPatient.address); // Allow space in address

    // Validate Mobile Number
    int validMobile = 0;
    while (!validMobile) {
        printf("Enter Patient Mobile (10 digits): ");
        scanf("%s", newPatient.mobile); // Read mobile number as a string

        // Check if mobile number is exactly 10 digits and contains only digits
        if (strlen(newPatient.mobile) == 10) {
            validMobile = 1;  // Assume valid mobile number
            for (i = 0; i < 10; i++) {
                if (!isdigit(newPatient.mobile[i])) {
                    validMobile = 0;  // Found a non-digit character
                    printf(COLOR_RED"Mobile number should contain only digits.\n"COLOR_RESET);
                    break;
                }
            }
        } else {
            printf(COLOR_RED"Mobile number must be exactly 10 digits.\n"COLOR_RESET);
        }
    }

    // Validate Gender
	int validGender = 0;
	while (!validGender) {
    	printf("Enter Gender (Male/Female): ");
    	scanf("%s", newPatient.gender); // Read gender

    	// Check if gender is either Male or Female
    	if (strcmp(newPatient.gender, "Male") == 0 || strcmp(newPatient.gender, "Female") == 0) {
        validGender = 1;  // Gender is valid
    	} else {
        	printf(COLOR_RED"Invalid Gender. Please enter either 'Male' or 'Female'.\n"COLOR_RESET);
    }
}

    // Set Username
    printf("Set Username: ");
    scanf("%s", newPatient.username);

    // Set Password (hidden)
    char ch;
    int pos = 0;
    printf("Set Password: ");
    while (1) {
        ch = getch(); // Get character without displaying it
        if (ch == '\r') { // Enter key pressed
            newPatient.password[pos] = '\0'; // Null-terminate the password
            break;
        } else if (ch == '\b' && pos > 0) { // Backspace pressed
            printf("\b \b"); // Remove character from console
            pos--;
        } else if (pos < sizeof(newPatient.password) - 1) { // Normal character
            newPatient.password[pos++] = ch;
            printf("*"); // Display asterisk
        }
    }
    printf("\n");

    // Automatically add the current date
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(newPatient.date, sizeof(newPatient.date), "%d/%m/%Y", tm_info);

    patients[patientCount++] = newPatient;
    savePatients();

    printf(COLOR_GREEN"\nPatient Added Successfully.\n"COLOR_RESET);
}

// 3. Display Patients Function
void displayPatients() {
    if (patientCount == 0) {
        printf(COLOR_RED"\nNo Patients To Display.\n"COLOR_RESET);
        return;
    }

    // Updated header with expanded column widths
    printf("\n");
    printf("=======================================================================================================================================\n");
    printf("| %-5s | %-25s | %-5s | %-10s | %-20s | %-15s | %-15s | %-15s |\n",
           "ID", "Name", "Age", "Gender", "Disease", "Mobile", "Date", "Username");
    printf("=======================================================================================================================================\n");

    int i;
    for (i = 0; i < patientCount; i++) {
        // Updated row format with wider columns
        printf("| %-5d | %-25s | %-5d | %-10s | %-20s | %-15s | %-15s | %-15s |\n",
               patients[i].id,
               patients[i].name,
               patients[i].age,
               patients[i].gender,
               patients[i].disease,
               patients[i].mobile,
               patients[i].date,
               patients[i].username);
    }

    printf("=======================================================================================================================================\n");
}

// 4. Edit Patient Functions

void editPatient() {
    displayPatients();
    int id, found = 0, i;
    printf("====================\n");
    printf("=== Edit Patient ===\n");
    printf("====================\n\n");
    printf("Enter Patient ID To Edit: ");
    scanf("%d", &id);

    for (i = 0; i < patientCount; i++) {
        if (patients[i].id == id) {
            found = 1;
            printf("\nEditing Details For Patient ID %d\n", id);

            char temp[100]; // Temporary buffer for user input

            // Name
            printf("Enter New Name (or press Enter to keep '%s'): ", patients[i].name);
            getchar(); // Clear newline character from previous input
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(patients[i].name, temp);  // Update the name
            }

            // Age
            printf("Enter New Age (or press Enter to keep '%d'): ", patients[i].age);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                int age = atoi(temp); // Convert to integer
                if (age > 0) { // Validate positive integer
                    patients[i].age = age;
                } else {
                    printf(COLOR_RED"Invalid age input. Keeping the old value.\n"COLOR_RESET);
                }
            }

            // Disease
            printf("Enter New Disease (or press Enter to keep '%s'): ", patients[i].disease);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(patients[i].disease, temp);  // Update the disease
            }

            // Address
            printf("Enter New Address (or press Enter to keep '%s'): ", patients[i].address);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(patients[i].address, temp);  // Update the address
            }

            // Mobile
            printf("Enter New Mobile No. (or press Enter to keep '%s'): ", patients[i].mobile);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                if (strlen(temp) == 10 && strspn(temp, "0123456789") == 10) {
                    strcpy(patients[i].mobile, temp); // Update mobile
                } else {
                    printf(COLOR_RED"Invalid mobile number input. Keeping the old value.\n"COLOR_RESET);
                }
            }

            // Username
            printf("Enter New Username (or press Enter to keep '%s'): ", patients[i].username);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(patients[i].username, temp);  // Update the username
            }

            // Gender (Validation: Male or Female)
            int validGender = 0;
            while (!validGender) {
                printf("Enter New Gender (or press Enter to keep '%s'): ", patients[i].gender);
                fgets(temp, sizeof(temp), stdin);
                if (temp[0] != '\n') { // Check if the user entered something
                    temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                    if (strcmp(temp, "Male") == 0 || strcmp(temp, "Female") == 0) {
                        strcpy(patients[i].gender, temp); // Update gender
                        validGender = 1;
                    } else {
                        printf(COLOR_RED"Invalid gender. Please enter 'Male' or 'Female'.\n"COLOR_RESET);
                    }
                } else {
                    validGender = 1; // User chose to keep the original gender
                }
            }

            // Password (hidden input)
            printf("Set New Password (hidden, or press Enter to keep current password): ");
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
            printf(COLOR_GREEN"\nPatient Details Updated Successfully.\n"COLOR_RESET);
            break;
        }
    }

    if (!found) {
        printf(COLOR_RED"\nPatient with ID %d not found.\n"COLOR_RESET , id);
    }
}

// 5. Delete Patient Function
void deletePatient(){
	
	displayPatients();
	int id, i, j, found=0;
	printf("======================\n");
	printf("=== Delete Patient ===\n");
	printf("======================\n\n");
	printf("Enter Patient ID To Delete: ");
	scanf("%d", &id);
	
	for(i=0; i<patientCount; i++){
		if(patients[i].id == id){
			found = 1;
			for(j=i; j<patientCount; j++){
				patients[j] = patients[j+1]; // Shift Left
			}
			patientCount--;
			savePatients();
			printf(COLOR_GREEN"\nPatient Deleted Successfully.\n"COLOR_RESET);
			break;
		}
	}
	
	if(!found){
		printf(COLOR_RED"\nPatient With ID %d Not Found.\n"COLOR_RESET, id);
	}
}

// 6. Patient Login
void patientLogin() {
//	system("cls");
    char username[20], password[20];
    char ch;
    int i = 0;

    printf("=====================\n");
    printf("=== Patient Login ===\n");
    printf("=====================\n\n");

    // Input for username
    printf("Enter Username: ");
    scanf("%s", username);
    
    // Input for password with hidden characters
    printf("Enter Password: ");
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

    // Display all records with the same username
    displayRecordsByUsername(username);
}

// 7. Display All Records with the Same Username
void displayRecordsByUsername(const char *username) {
    int found = 0, i;

    for (i = 0; i < patientCount; i++) {
        if (strcmp(patients[i].username, username) == 0) {
            found = 1;
            printf(COLOR_GREEN"\nLogin Successful! Welcome, %s.\n"COLOR_RESET, patients[i].name);
            printf("\nPatient Records for Name: %s\n", patients[i].name);
    		printf("=======================================================================================================================================\n");
    		printf("| %-5s | %-25s | %-5s | %-10s | %-20s | %-15s | %-15s | %-15s |\n",
           			"ID", "Name", "Age", "Gender", "Disease", "Mobile", "Date", "Username");
    		printf("=======================================================================================================================================\n");

			printf("| %-5d | %-25s | %-5d | %-10s | %-20s | %-15s | %-15s | %-15s |\n",
                   patients[i].id,
                   patients[i].name,
                   patients[i].age,
                   patients[i].gender,
                   patients[i].disease,
                   patients[i].mobile,
                   patients[i].date,
                   patients[i].username);
        }
    }

    if (!found) {
        printf(COLOR_RED"\nInvalid Username or Password. Please try again.\n"COLOR_RESET);
    } else {
        printf("=======================================================================================================================================\n");
    }
}

// 8. Function To Save Patient Data
void savePatients() {
    FILE *file = fopen(FILENAME_PATIENTS, "w");
    if (file == NULL) {
        printf(COLOR_RED"Error: Unable to save patients.\n"COLOR_RESET);
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
    printf(COLOR_GREEN"Patients data saved successfully.\n"COLOR_RESET);
}

// 9. Add Doctor Function
void addDoctor() {
    printf("==================\n");
    printf("=== Add Doctor ===\n");
    printf("==================\n\n");
    displayDoctors();

    if (doctorCount >= MAX_DOCTORS) {
        printf(COLOR_RED"\nCannot Add More Doctors. Maximum Limit Reached.\n"COLOR_RESET);
        return;
    }

    struct Doctor newDoctor;

    // Auto-generate a unique Doctor ID
    newDoctor.id = doctorCount + 1;
    printf("New Doctor ID: %d\n", newDoctor.id);

    // Validate Doctor Name
    int validName = 0, i;
    while (!validName) {
        printf("Enter Doctor Name: ");
        scanf(" %[^\n]", newDoctor.name);

        validName = 1; // Assume the name is valid
        for (i = 0; newDoctor.name[i] != '\0'; i++) {
            if (!isalpha(newDoctor.name[i]) && newDoctor.name[i] != ' ' && newDoctor.name[i] != '.') {
                printf(COLOR_RED"Doctor name should only contain letters and spaces.\n"COLOR_RESET);
                validName = 0; // Invalid character found
                break;
            }
        }
    }

    // Validate Doctor Specialty
    int validSpecialty = 0;
    while (!validSpecialty) {
        printf("Enter Doctor Specialty: ");
        scanf(" %[^\n]", newDoctor.speciality);

        validSpecialty = 1; // Assume the specialty is valid
        for (i = 0; newDoctor.speciality[i] != '\0'; i++) {
            if (!isalpha(newDoctor.speciality[i]) && newDoctor.speciality[i] != ' ' && newDoctor.speciality[i] != '.') {
                printf(COLOR_RED"Doctor specialty should only contain letters, spaces, and periods.\n"COLOR_RESET);
                validSpecialty = 0; // Invalid character found
                break;
            }
        }
    }

    // Validate Username
    printf("Set a Username: ");
    scanf(" %[^\n]", newDoctor.username);

    // Validate and Hide Password Input
    printf("Set a Password: ");
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
    printf(COLOR_GREEN"\nDoctor Added Successfully.\n"COLOR_RESET);
}

// 10. Display Doctor Function
void displayDoctors() {
//    system("cls");
    int i;
    if (doctorCount == 0) {
        printf("\nNo Doctors To Display.\n");
        return;
    }

    // Display table headers with proper column widths
    printf("=================================================================================================================\n");
    printf("| %-5s | %-25s | %-50s | %-20s |\n", "ID", "Name", "Specialty", "Username");
    printf("=================================================================================================================\n");

    // Display each doctor's details
    for (i = 0; i < doctorCount; i++) {
        printf("| %-5d | %-25s | %-50s | %-20s |\n",
               doctors[i].id,
               doctors[i].name,
               doctors[i].speciality,
               doctors[i].username); // Display Username
    }

    // Footer for the table
    printf("=================================================================================================================\n");
}

// 11. Edit Doctor Function
void editDoctor() {
    displayDoctors();
    int id, i, found = 0;
    printf("===================\n");
    printf("=== Edit Doctor ===\n");
    printf("===================\n\n");
    printf("Enter Doctor ID To Edit: ");
    scanf("%d", &id);

    for (i = 0; i < doctorCount; i++) {
        if (doctors[i].id == id) {
            found = 1;
            printf("Editing Details For Doctor ID %d\n", id);

            char temp[100]; // Temporary buffer for user input

            // Name
            printf("Enter Doctor's New Name (or press Enter to keep '%s'): ", doctors[i].name);
            getchar(); // Clear newline character from previous input
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(doctors[i].name, temp);    // Update the name
            }

            // Specialty
            printf("Enter Doctor's New Specialty (or press Enter to keep '%s'): ", doctors[i].speciality);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(doctors[i].speciality, temp); // Update the specialty
            }

            // Username
            printf("Enter Doctor's New Username (or press Enter to keep '%s'): ", doctors[i].username);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') {
                temp[strcspn(temp, "\n")] = '\0';
                strcpy(doctors[i].username, temp);
            }

            // Password (Hidden Input)
            printf("Set New Password (hidden, or press Enter to keep current password): ");
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
            printf(COLOR_GREEN"\nDoctor Details Updated Successfully.\n"COLOR_RESET);
            break;
        }
    }

    if (!found) {
        printf(COLOR_RED"\nDoctor With ID %d Not Found.\n"COLOR_RESET, id);
    }
}

// 12. Delete Doctor Function
void deleteDoctor(){
	
	displayDoctors();
	int id, i, j, found=0;
	printf("=====================\n");
	printf("=== Delete Doctor ===\n");
	printf("=====================\n\n");
	printf("Enter Doctor ID To Delete: ");
	scanf("%d", &id);
	
	for(i=0; i<doctorCount; i++){
		if(doctors[i].id == id){
			found=1;
			for(j=i; j<doctorCount-1; j++){
				doctors[j] = doctors[j+1];  //Shift Left
			}
			doctorCount--;
			saveDoctors();
			printf(COLOR_GREEN"\nDoctor Deleted Successfully.\n"COLOR_RESET);
			break;
		}
	}
	
	if(!found){
		printf(COLOR_RED"\nDoctor With ID %d Not Found.\n"COLOR_RESET, id);
	}
}

// 13. Doctor Login
void doctorLogin() {
//	system("cls");
    char inputUsername[20], inputPassword[20];
    int found = 0;
	printf("====================\n");
    printf("=== Doctor Login ===\n");
    printf("====================\n\n");
    printf("Enter Username: ");
    scanf("%s", inputUsername);
    printf("Enter Password: ");

    // Hide password input
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
int i;
    for (i = 0; i < doctorCount; i++) {
        if (strcmp(doctors[i].username, inputUsername) == 0 &&
            strcmp(doctors[i].password, inputPassword) == 0) {
            found = 1;
            printf(COLOR_GREEN"\nLogin Successful. Welcome, %s\n"COLOR_RESET, doctors[i].name);
            doctorActions(i); // Redirect to doctor-specific actions
            break;
        }
    }

    if (!found) {
        printf(COLOR_RED"\nInvalid Username or Password. Please try again.\n"COLOR_RESET);
    }
}

// 14. Doctor Menu
void doctorActions(int doctorIndex) {
	system("cls");
    int choice;
    do {
    	printf("===================\n");
        printf("=== Doctor Menu ===\n");
        printf("===================\n\n");
        printf("1. View Patient Records\n");
        printf("2. View Profile\n");
        printf("3. Logout\n");
        printf("===================\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayPatients(); // Or filter patients by doctor
                break;
            case 2:
                displayDoctorProfile(doctorIndex);
                break;
            case 3:
                printf("Logout\n");
                return;
            default:
                printf(COLOR_RED"Invalid choice. Please try again.\n"COLOR_RESET);
        }
    } while (choice != 3);
}

// 15. Doctor Profile
void displayDoctorProfile(int doctorIndex) {
    // Printing the doctor's profile in a structured format
    printf("\nDoctor Profile\n");
    printf("=================================================================================================================\n");
    printf("| %-5s | %-25s | %-50s | %-20s |\n",
           "ID", "Name", "Speciality", "Username");
    printf("=================================================================================================================\n");

    // Print the doctor's information
    printf("| %-5d | %-25s | %-50s | %-20s |\n",
           doctors[doctorIndex].id,
           doctors[doctorIndex].name,
           doctors[doctorIndex].speciality,
           doctors[doctorIndex].username);

    printf("=================================================================================================================\n");
}

// 16. Function To Save Doctor Data
void saveDoctors() {
    FILE *file = fopen(FILENAME_DOCTORS, "w");
    if (file == NULL) {
        printf(COLOR_RED"\nError Opening File To Save Doctors!\n"COLOR_RESET);
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
    printf(COLOR_GREEN"\nDoctor Data Saved To File.\n"COLOR_RESET);
}


// 17. Add Receptionist Function(Admin Only)

void addReceptionist() {
	printf("======================\n");
    printf("=== Add Receptionist ===\n");
    printf("======================\n\n");
    displayReceptionists();
    if (receptionistCount >= MAX_RECEPTIONISTS) {
        printf(COLOR_RED"\nCannot Add More Receptionists. Maximum Limit Reached.\n"COLOR_RESET);
        return;
    }

    struct Receptionist newReceptionist;

    // Auto-generate a unique Receptionist ID
    newReceptionist.id = receptionistCount + 1;
    printf("New Receptionist ID: %d\n", newReceptionist.id);

    // Validate Receptionist Name
    int validName = 0, i;
    while (!validName) {
        printf("Enter Receptionist Name: ");
        scanf(" %[^\n]", newReceptionist.name); // Read input with spaces

        validName = 1; // Assume the name is valid
        for (i = 0; newReceptionist.name[i] != '\0'; i++) {
            if (!isalpha(newReceptionist.name[i]) && newReceptionist.name[i] != ' ') {
                validName = 0; // Invalid character found
                printf(COLOR_RED"Name must contain only alphabetic characters and spaces.\n"COLOR_RESET);
                break;
            }
        }
    }

    // Validate Username
    printf("Enter Receptionist User Name: ");
    scanf("%s", newReceptionist.username);

    // Validate Password
    printf("Enter Receptionist Password: ");
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
    printf(COLOR_GREEN"\nReceptionist Added Successfully.\n"COLOR_RESET);
}

// 18. Display Receptionist Function(Admin Only)
void displayReceptionists() {
	int i;
    if (receptionistCount == 0) {
        printf("\nNo Receptionists To Display.\n");
        return;
    }

    // Display table headers
    printf("========================================================================================================================\n");
    printf("| %-10s | %-50s | %-50s |\n", "ID", "Name", "Username");
    printf("========================================================================================================================\n");

    // Display each receptionist's details
    for (i = 0; i < receptionistCount; i++) {
        printf("| %-10d | %-50s | %-50s |\n",
               receptionists[i].id,
               receptionists[i].name,
               receptionists[i].username);
    }

    // Footer for the table
    printf("========================================================================================================================\n");
}

// 19. Edit Receptionist Function (Admin Only)
void editReceptionists() {
    displayReceptionists();
    if (receptionistCount == 0) {
        printf("\nNo Receptionists To Edit.\n");
        return;
    }

    int id, found = 0, i;
    printf("=========================\n");
    printf("=== Edit Receptionist ===\n");
    printf("=========================\n\n");
    printf("Enter Receptionist ID To Edit: ");
    scanf("%d", &id);

    for (i = 0; i < receptionistCount; i++) {
        if (receptionists[i].id == id) {
            found = 1;
            printf("Editing Details For Receptionist ID %d\n", id);

            char temp[100]; // Temporary buffer for user input

            // Name
            printf("Enter New Name (or press Enter to keep '%s'): ", receptionists[i].name);
            getchar(); // Clear newline character from previous input
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(receptionists[i].name, temp); // Update name
            }

            // Username
            printf("Enter New Username (or press Enter to keep '%s'): ", receptionists[i].username);
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(receptionists[i].username, temp); // Update username
            }

            // Password
            printf("Enter New Password (or press Enter to keep existing Password): ");
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n') { // Check if the user entered something
                temp[strcspn(temp, "\n")] = '\0'; // Remove trailing newline
                strcpy(receptionists[i].password, temp); // Update password
            }

            saveReceptionists();
            printf(COLOR_GREEN"\nReceptionist Details Updated Successfully.\n"COLOR_RESET);
            break;
        }
    }

    if (!found) {
        printf(COLOR_RED"\nReceptionist With ID %d Not Found.\n"COLOR_RESET, id);
    }
}

// 20. Delete Receptionist Function(Admin Only)
void deleteReceptionist(){
	displayReceptionists();
	if(receptionistCount == 0){
		printf("\nNo receptionists To Delete.\n");
		return;
	}
	
	int id;
	printf("===========================\n");
	printf("=== Delete Receptionist ===\n");
	printf("===========================\n\n");
	printf("Enter Receptionist ID To Delete: ");
	scanf("%d", &id);
	
	int i, j, found=0;
	for(i=0; i<receptionistCount; i++){
		if(receptionists[i].id == id){
			found=1;
			for(j=i; j<receptionistCount-1; j++){
				receptionists[j] = receptionists[j+1]; //Shift Left
			}
			receptionistCount--;
			saveReceptionists();
			printf(COLOR_GREEN"\nReceptionists Deleted Successfully.\n"COLOR_RESET);
			break;
		}
	}
	
	if(!found){
		printf(COLOR_RED"\nReceptionist With ID %d Not Found.\n"COLOR_RESET, id);
	}
}

// 21. Function To Save Receptionist Data
void saveReceptionists() {
	int i;
    FILE *file = fopen(FILENAME_RECEPTIONISTS, "w");
    if (file == NULL) {
        printf(COLOR_RED"\nError Opening File To Save Receptionists!\n"COLOR_RESET);
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
    printf(COLOR_GREEN"\nReceptionist Data Saved To File.\n"COLOR_RESET);
}

// 22. Login Function For Admin
int login() {
//    system("cls");
    char username[20], password[20];
    printf("===================\n");
    printf("=== Admin Login ===\n");
    printf("===================\n\n");
    printf("Enter Admin Username: ");
    scanf("%s", username);

    printf("Enter Admin Password: ");
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r') { // '\r' is Enter key
        if (ch == '\b' && i > 0) { // Handle backspace
            printf("\b \b");
            i--;
        } else if (i < sizeof(password) - 1 && ch != '\b') {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0'; // Null-terminate the password
    printf("\n");

    if (strcmp(username, adminUsername) == 0 && strcmp(password, adminPassword) == 0) {
        printf(COLOR_GREEN"\nAdmin Login Successful!\n"COLOR_RESET);
        return 1;
    } else {
        printf(COLOR_RED"\nInvalid Username or Password. Please try again.\n"COLOR_RESET);
        return 0;
    }
}

// 23. Login Function For Receptionist
int receptionistLogin() {
//    system("cls");
    char username[20], password[20];
    int i;

    printf("==========================\n");
    printf("=== Receptionist Login ===\n");
    printf("==========================\n\n");
    printf("Enter Receptionist Username: ");
    scanf("%s", username); // Use %s to read without spaces

    printf("Enter Receptionist Password: ");
    int j = 0;
    char ch;
    while ((ch = getch()) != '\r') { // '\r' is the Enter key
        if (ch == '\b' && j > 0) { // Handle backspace
            printf("\b \b");
            j--;
        } else if (j < sizeof(password) - 1 && ch != '\b') {
            password[j++] = ch;
            printf("*");
        }
    }
    password[j] = '\0'; // Null-terminate the password
    printf("\n");

    for (i = 0; i < receptionistCount; i++) {
        if (strcmp(receptionists[i].username, username) == 0 && strcmp(receptionists[i].password, password) == 0) {
            printf(COLOR_GREEN"\nReceptionist Login Successful!\n"COLOR_RESET);
            return 1; // Login successful
        }
    }

    printf(COLOR_RED"\nInvalid Username or Password. Please try again.\n"COLOR_RESET);
    return 0; // Login failed
}

// 24. Display Patient Menu
void patientMenu(){
	system("cls");
	int choice;
	do{
		printf("====================\n");
		printf("=== Patient Menu ===\n");
		printf("====================\n\n");
		printf("1. Add Patient\n");
		printf("2. Display Patients\n");
		printf("3. Edit Patient\n");
		printf("4. Delete Patient\n");
		printf("5. Go Back\n");
		printf("====================\n\n");
		printf("Enter Your Choice: ");
		scanf("%d", &choice);
		
		switch(choice){
			case 1:
				addPatient();		// Calls the function to add a new patient
				break;
			case 2:
				displayPatients();	// Calls the function to display all patients
				break;
			case 3:
				editPatient();		// Calls the function to edit a patient's details
				break;
			case 4:
				deletePatient();	// Calls the function to delete a patient
				break;
			case 5:
				printf(COLOR_GREEN"\nReturning To Previous Menu...\n "COLOR_RESET);
				break;
			default:
				printf(COLOR_RED"\nInvalid Choice! Please Try Again.\n"COLOR_RESET);
		}
	}while(choice != 5);			// Repeat until the user chooses to go back
}

// 25. Display Doctor Menu
void doctorMenu(){
	system("cls");
	int choice;
	do{
		printf("===================\n");
		printf("=== Doctor Menu ===\n");
		printf("===================\n\n");
        printf("1. Add Doctor\n");
        printf("2. Display Doctors\n");
        printf("3. Edit Doctor\n");
        printf("4. Delete Doctor\n");
        printf("5. Go Back\n");
        printf("===================\n\n");
        printf("Enter Your Choice: ");
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
                printf(COLOR_GREEN"\nReturning To Previous Menu...\n"COLOR_RESET);
                break;
            default:
                printf(COLOR_RED"\nInvalid Choice! Please Try Again.\n"COLOR_RESET);
        }
    } while (choice != 5);  		// Repeat until the user chooses to go back
}

// 26. Display Receptionist Menu For Manage Only Patients
void receptionistMenu() {
	system("cls");
    int choice;
    do {
    	printf("=========================\n");
        printf("=== Receptionist Menu ===\n");
        printf("=========================\n\n");
        printf("1. Add Patient\n");
        printf("2. Display Patients\n");
        printf("3. Edit Patient\n");
        printf("4. Delete Patient\n");
        printf("5. Logout\n");
        printf("=========================\n\n");
        printf("Enter Your Choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addPatient();
                break;
            case 2:
                displayPatients();
                break;
            case 3:
                editPatient();
                break;
            case 4:
                deletePatient();
                break;
            case 5:
                printf(COLOR_GREEN"\nReceptionist Successfully Logout...\n"COLOR_RESET);
                break;
            default:
                printf(COLOR_RED"\nInvalid Choice! Please Try Again.\n"COLOR_RESET);
        }
    } while (choice != 5);
}

// 27. Display Receptionist Menu Manages All Data
void adminMenu() {
	system("cls");
    int choice;
    do {
    	printf("==================\n");
        printf("=== Admin Menu ===\n");
        printf("==================\n\n");
        printf("1. Manage Patients\n");
        printf("2. Manage Doctors\n");
        printf("3. Manage Receptionists\n");
        printf("4. Logout\n");
        printf("==================\n\n");
        printf("Enter Your Choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                patientMenu(); 			// Patients management
                break;
            case 2:
                doctorMenu(); 			// Doctors management
                break;
            case 3:
                manageReceptionists(); // Receptionist management
                break;
            case 4:
                printf(COLOR_GREEN"\nAdmin Successfully Logout...\n"COLOR_RESET);
                break;
            default:
                printf(COLOR_RED"\nInvalid Choice! Please Try Again.\n"COLOR_RESET);
        }
    } while (choice != 4);
}

// 2. Display Receptionist Menu Manage By Admin
void manageReceptionists() {
    system("cls");
    int choice;
    do {
    	printf("============================\n");
        printf("=== Manage Receptionists ===\n");
        printf("============================\n\n");
        printf("1. Add Receptionist\n");
        printf("2. Display Receptionists\n");
        printf("3. Edit Receptionists\n");
        printf("4. Delete Receptionist\n");
        printf("5. Go Back\n");
        printf("============================\n\n");
        printf("Enter Your Choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addReceptionist();
                break;
            case 2:
                displayReceptionists();
                break;
            case 3:
                editReceptionists();
                break;
            case 4:
                deleteReceptionist();
                break;
            case 5:
                printf(COLOR_GREEN"\nReturning To Previous Menu...\n"COLOR_RESET);
                break;
            default:
                printf(COLOR_RED"\nInvalid Choice! Please Try Again.\n"COLOR_RESET);
        }
    } while (choice != 5);
}
