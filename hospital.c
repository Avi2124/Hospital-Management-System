#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_PATIENTS 100
#define MAX_DOCTORS 10
#define MAX_RECEPTIONISTS 5

#define FILENAME_PATIENTS "patients.txt"
#define FILENAME_DOCTORS "doctors.txt"
#define FILENAME_RECEPTIONISTS "receptionists.txt"

// Admin credentials
char adminUsername[] = "admin";
char adminPassword[] = "admin123";

//Structure for Patient
struct Patient {
    int id;
    char name[50];
    int age;
    char disease[50];
    char address[100]; 
    char mobile[15];   
};

//Structure for Doctor
struct Doctor {
    int id;
    char name[50];
    char speciality[50];
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
void savePatients();//6

//Doctors Function

void addDoctor();//7
void displayDoctors();//8
void editDoctor();//9
void deleteDoctor();//10
void saveDoctors();//11

//Receptionist Function
void addReceptionist();//12
void displayReceptionists();//13
void editReceptionist();//14
void deleteReceptionist();//15
void saveReceptionists();//16


//Login Function
int login();//17 (Admin Login)
int receptionistLogin();//18

//Menu Function
void patientMenu();//19
void doctorMenu();//20
void manageReceptionists();//21 (Manages Only Patients)
void adminMenu();//22
void receptionistMenu();//23

//Main Function
int main(){
	loadData();
	int choice;
	do{
		system("cls");
		printf("----------------------------------");
		printf("\n=== Hospital Management System ===\n");
		printf("----------------------------------\n\n");
        printf("1. Admin Login\n");
        printf("2. Receptionist Login\n");
        printf("3. Exit\n\n");
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
                printf("\nExiting The System. Goodbye!\n");
                break;
            default:
                printf("\nInvalid Choice! Please Try Again.\n");
        }
    } while (choice != 3);

    return 0;
}

// 1. Function to load all data (Patients, Doctors, Receptionists)

void loadData() {
	
// Load Patients Data
FILE *file = fopen(FILENAME_PATIENTS, "r");
    if (file != NULL) {
        while(fscanf(file, "%d %[^\n] %d %[^\n] %[^\n] %[^\n]",
			&patients[patientCount].id, 
            patients[patientCount].name, 
            &patients[patientCount].age, 
            patients[patientCount].disease,
            patients[patientCount].address, 
            patients[patientCount].mobile) != EOF) {  //EOF=End Of File
            patientCount++;
            if (patientCount >= MAX_PATIENTS) {
                printf("\nMaximum Patient Limit Reached While Loading.\n");
                break;
            }
        }
        fclose(file);
    }
    
//Load Doctors Data
file = fopen(FILENAME_DOCTORS, "r");
    if (file != NULL) {
        	while (fscanf(file, "%d %[^\n] %[^\n]",
			&doctors[doctorCount].id,
			doctors[doctorCount].name,
			doctors[doctorCount].speciality) != EOF) {
            doctorCount++;
            if (doctorCount >= MAX_DOCTORS) {
                printf("\nMaximum Doctor Limit Reached While Loading.\n");
                break;
            }
        }
        fclose(file);
    }
    
//Load Rceptionists Data
file = fopen(FILENAME_RECEPTIONISTS, "r");
    if (file != NULL) {
			while (fscanf(file, "%d %[^\n] %[^\n] %[^\n]",
			&receptionists[receptionistCount].id,
			receptionists[receptionistCount].name, 
			receptionists[receptionistCount].username,
			receptionists[receptionistCount].password) != EOF) {
            receptionistCount++;
            if (receptionistCount >= MAX_RECEPTIONISTS) {
                printf("\nMaximum Receptionist Limit Reached While Loading.\n");
                break;
            }
        }
        fclose(file);
    }
}

//Patients Function

// 2. Add Patient Function
void addPatient() {
	system("cls");
    if (patientCount >= MAX_PATIENTS) {
        printf("\nCannot Add More Patients. Maximum Limit Reached.\n");
        return;
    }
	
	struct Patient newPatient;
	int isUnique = 0, i;
	
	while (!isUnique) {
        printf("-----------------------");
        printf("\n=== Add New Patient ===\n");
        printf("-----------------------\n\n");
        printf("Enter Patient ID: ");
        scanf("%d", &newPatient.id);

        // Check if ID already exists
        isUnique = 1;  // Assume the ID is unique
        for (i = 0; i < patientCount; i++) {
            if (patients[i].id == newPatient.id) {
                printf("\nID Already Exists. Please Enter a Unique Patient ID.\n");
                isUnique = 0;  // Set to false and ask for a new ID
                break;
            }
        }
    }
    
    int validName = 0;
    while (!validName) {
        printf("Enter Patient Name: ");
        scanf(" %[^\n]", newPatient.name); // Allow spaces in name

        validName = 1;  // Assume name is valid
        for (i = 0; newPatient.name[i] != '\0'; i++) {
            if (!isalpha(newPatient.name[i]) && newPatient.name[i] != ' ') {
                validName = 0;  // Found a non-alphabetic character
                printf("Name must contain only alphabetic characters and spaces.\n");
                break;
            }
        }

        // Check for empty name
        if (strlen(newPatient.name) == 0) {
            validName = 0;
            printf("Name cannot be empty.\n");
        }
    }

    // Validate Age (greater than 0 and numeric)
    int validAge = 0;
    while (!validAge) {
        printf("Enter Patient Age: ");
        if (scanf("%d", &newPatient.age) != 1 || newPatient.age <= 0) {
            printf("Invalid input. Age must be a positive integer.\n");
            while (getchar() != '\n'); // Clear invalid input from buffer
        } else {
            validAge = 1; // Age is valid
        }
    }
    
    // Validate that Disease contains only alphabetic characters and spaces
    int validDisease = 0;
    while (!validDisease) {
        printf("Enter Patient Disease: ");
        scanf(" %[^\n]", newPatient.disease); // Read disease allowing spaces

        validDisease = 1;  // Assume input is valid
        for (i = 0; newPatient.disease[i] != '\0'; i++) {
            if (!isalpha(newPatient.disease[i]) && newPatient.disease[i] != ' ') {
                validDisease = 0;  // Found an invalid character
                printf("Disease can only contain letters and spaces.\n");
                break;
            }
        }
    }
    
    printf("Enter Patient Address: ");
    scanf(" %[^\n]", newPatient.address); // Allow space in address
    
     // Validate Mobile Number for exactly 10 digits and numeric
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
                    printf("Mobile number should contain only digits.\n");
                    break;
                }
            }
        } else {
            printf("Mobile number must be exactly 10 digits.\n");
        }
    }
	
	patients[patientCount++] = newPatient;
    savePatients();
	printf("\nPatient Added Successfully.\n");
}

// 3. Display Patients Function
void displayPatients() {
	system("cls");
    if (patientCount == 0) {
        printf("\nNo Patients To Display.\n");
        return;
    }
    printf("------------------------");
    printf("\n=== List of Patients ===\n");
    printf("------------------------\n\n");
    int i;
    for (i = 0; i < patientCount; i++) {
    	
        printf("ID: %d | Name: %s | Age: %d | Disease: %s | Address: %s | Mobile: %s\n\n", 
               patients[i].id, 
               patients[i].name, 
               patients[i].age, 
               patients[i].disease,
               patients[i].address,
               patients[i].mobile);
    }
}

// 4. Edit Patient Functions

void editPatient() {
	system("cls");
	displayPatients();
    int id, found = 0, i;
    printf("--------------------");
    printf("\n=== Edit Patient ===\n");
    printf("--------------------\n\n");
    printf("Enter Patient ID To Edit: ");
    scanf("%d", &id);
    
    for (i = 0; i < patientCount; i++) {
        if (patients[i].id == id) {
            found = 1;
            printf("Editing Details For Patient ID %d\n", id);
            
            // Validate that the name contains only alphabetic characters and spaces
            int validName = 0, j;
            while (!validName) {
                printf("Enter New Name: ");
                scanf(" %[^\n]", patients[i].name);
                validName = 1;  // Assume the name is valid
                for (j = 0; patients[i].name[j] != '\0'; j++) {
                    if (!isalpha(patients[i].name[j]) && patients[i].name[j] != ' ') {
                        validName = 0;  // Found an invalid character
                        printf("Name can only contain letters and spaces.\n");
                        break;
                    }
                }
            }
            
            // Validate that age is a positive integer greater than 0
            int validAge = 0;
            while (!validAge) {
                printf("Enter Patient's New Age: ");
                if (scanf("%d", &patients[i].age) != 1 || patients[i].age <= 0) {
                    printf("Age must be a positive integer. Please try again.\n");
                    while (getchar() != '\n');  // Clear the input buffer
                } else {
                    validAge = 1;  // Valid age entered
                }
            }
            getchar(); // Consume newline character after age input
            
            // Disease validation: Only alphabetic characters allowed
            int validDisease = 0;
            while (!validDisease) {
                printf("Enter Patient's New Disease: ");
                scanf(" %[^\n]", patients[i].disease);
                
                validDisease = 1; // Assume valid input
                for (j = 0; j < strlen(patients[i].disease); j++) {
                    if (!isalpha(patients[i].disease[j]) && patients[i].disease[j] != ' ') {
                        validDisease = 0;
                        printf("Disease name must contain only alphabetic characters and spaces.\n");
                        break;
                    }
                }
            }
            
            printf("Enter Patient's New Address: ");
            scanf(" %[^\n]", patients[i].address);
            
            // Mobile number validation: only digits and exactly 10 characters
            int validMobile = 0;
            while (!validMobile) {
                printf("Enter Patient's New Mobile No. (10 digits): ");
                scanf("%s", patients[i].mobile);

                // Check if mobile number is exactly 10 digits and contains only digits
                if (strlen(patients[i].mobile) == 10) {
                    validMobile = 1;  // Assume valid mobile number
                    for (j = 0; j < 10; j++) {
                        if (!isdigit(patients[i].mobile[j])) {
                            validMobile = 0;  // Found a non-digit character
                            printf("Mobile number should contain only digits.\n");
                            break;
                        }
                    }
                } else {
                    printf("Mobile number must be exactly 10 digits.\n");
                }
            }
            
            savePatients();
            printf("\nPatient Details Updated Successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("\nPatient With ID %d Not Found.\n", id);
    }
}

// 5. Delete Patient Function
void deletePatient(){
	system("cls");
	displayPatients();
	int id, i, j, found=0;
	printf("----------------------");
	printf("\n=== Delete Patient ===\n");
	printf("----------------------\n\n");
	printf("Enter Patient ID To Delete: ");
	scanf("%d", &id);
	
	for(i=0; i<patientCount; i++){
		if(patients[i].id == id){
			found = 1;
			for(j=0; j<patientCount; j++){
				patients[j] = patients[j+1]; // Shift Left
			}
			patientCount--;
			savePatients();
			printf("\nPatient Deleted Successfully.\n");
			break;
		}
	}
	
	if(!found){
		printf("\nPatient With ID %d Not Found.\n", id);
	}
}

// 6. Function To Save Patient Data
void savePatients(){
	FILE *file = fopen(FILENAME_PATIENTS, "w");
	if(file == NULL){
		printf("\nError Opening File To Save Patients!\n");
		return;
	}
	
	int i;
	for(i=0; i<patientCount; i++){
		fprintf(file, "%d \n%s \n%d \n%s \n%s \n%s\n",
		patients[i].id,
		patients[i].name,
		patients[i].age,
		patients[i].disease,
		patients[i].address,
		patients[i].mobile);
	}
	fclose(file);
	printf("\nPatient Data Saved To File.\n");
}

// 7. Add Doctor Function
void addDoctor(){
	system("cls");
	if(doctorCount >= MAX_DOCTORS){
		printf("\nCannot Add More Doctors. Maximum Limit Reached.\n");
		return;
	}
	
	struct Doctor newDoctor;
	int isUnique = 0, i;

    while (!isUnique) {
        printf("----------------------");
        printf("\n=== Add New Doctor ===\n");
        printf("----------------------\n\n");
        printf("Enter Doctor ID: ");
        scanf("%d", &newDoctor.id);

        // Check if ID already exists
        isUnique = 1;  // Assume the ID is unique
        for (i = 0; i < doctorCount; i++) {
            if (doctors[i].id == newDoctor.id) {
                printf("\nID Already Exists. Please Enter a Unique Doctor ID.\n");
                isUnique = 0;  // Set to false and ask for a new ID
                break;
            }
        }
    }
    
	int validName = 0;
    while (!validName) {
        printf("Enter Doctor Name: ");
        scanf(" %[^\n]", newDoctor.name);

        // Check if name only contains alphabetic characters and spaces
        validName = 1;  // Assume the name is valid
        for (i = 0; newDoctor.name[i] != '\0'; i++) {
            if (!isalpha(newDoctor.name[i]) && newDoctor.name[i] != ' ') {
                printf("Doctor name should only contain letters and spaces.\n");
                validName = 0;  // Invalid character found
                break;
            }
        }
    }
	
	// Validate Doctor Specialty
    int validSpecialty = 0;
    while (!validSpecialty) {
        printf("Enter Doctor Specialty: ");
        scanf(" %[^\n]", newDoctor.speciality);

        validSpecialty = 1;  // Assume the specialty is valid
        for (i = 0; newDoctor.speciality[i] != '\0'; i++) {
            if (!isalpha(newDoctor.speciality[i]) && newDoctor.speciality[i] != ' ' && newDoctor.speciality[i] != '.') {
                printf("Doctor specialty should only contain letters, spaces, and periods.\n");
                validSpecialty = 0;  // Invalid character found
                break;
            }
        }
    }
	
	doctors[doctorCount++] = newDoctor;
	saveDoctors();
	printf("\nDoctor Added Successfully.\n");
}

// 8. Display Doctor Function
void displayDoctors(){
	system("cls");
	if(doctorCount == 0){
		printf("\nNo Doctors To Display.\n");
		return;
	}
	printf("-----------------------");
	printf("\n=== List Of Doctors ===\n");
	printf("-----------------------\n\n");
	int i;
	for(i=0; i<doctorCount; i++){
		printf("ID: %d | Name: %s | Speciality: %s\n\n",
		doctors[i].id,
		doctors[i].name,
		doctors[i].speciality);
	}
}

// 9. Edit Doctor Function
void editDoctor(){
    system("cls");
    displayDoctors();
	int id, i, found=0;
	printf("-------------------");
	printf("\n=== Edit Doctor ===\n");
	printf("-------------------\n\n");
	printf("Enter Doctor ID To Edit: ");
	scanf("%d", &id);
	
	for(i=0; i<doctorCount; i++){
		if(doctors[i].id == id){
			found=1;
			printf("Editing Details For Doctor ID %d: \n", id);
			
			// Input and validate doctor name
            int isValidName = 0, j;
            while (!isValidName) {
                printf("Enter Doctor's New Name: ");
                scanf(" %[^\n]", doctors[i].name);  // Allows spaces in the name
                
                // Check if name contains only letters and spaces
                isValidName = 1;
                for (j = 0; doctors[i].name[j] != '\0'; j++) {
                    if (!isalpha(doctors[i].name[j]) && doctors[i].name[j] != ' ' && doctors[i].name[j] != '.') {
                        isValidName = 0;
                        printf("Name should only contain alphabetic characters and spaces.\n");
                        break;
                    }
                }
            }
            
			// Input and validate doctor specialty
            int isValidSpeciality = 0;
            while (!isValidSpeciality) {
                printf("Enter Doctor's New Specialty: ");
                scanf(" %[^\n]", doctors[i].speciality);
                
                // Check if specialty contains only letters, spaces, and periods
                isValidSpeciality = 1;
                for (j = 0; doctors[i].speciality[j] != '\0'; j++) {
                    if (!isalpha(doctors[i].speciality[j]) && doctors[i].speciality[j] != ' ' && doctors[i].speciality[j] != '.') {
                        isValidSpeciality = 0;
                        printf("Specialty should only contain alphabetic characters, spaces, and periods.\n");
                        break;
                    }
                }
            }
            
			saveDoctors();
			printf("\nDoctor Details Updated Successfully.\n");
			break;
		}
	}
	
	if(!found){
		printf("\nDoctor With ID %d Not Found.\n", id);
	}
}

// 10. Delete Doctor Function
void deleteDoctor(){
	system("cls");
	displayDoctors();
	int id, i, j, found=0;
	printf("---------------------");
	printf("\n=== Delete Doctor ===\n");
	printf("---------------------\n\n");
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
			printf("\nDoctor Deleted Successfully.\n");
			break;
		}
	}
	
	if(!found){
		printf("\nDoctor With ID %d Not Found.\n", id);
	}
}

// 11. Function To Save Doctor Data
void saveDoctors(){
	FILE *file = fopen(FILENAME_DOCTORS, "w");
	if(file == NULL){
		printf("\nError Opening File To Save Doctors!\n");
		return;
	}
	
	int i;
	for(i=0; i<doctorCount; i++){
		fprintf(file, "%d \n%s \n%s\n",
		doctors[i].id,
		doctors[i].name,
		doctors[i].speciality);
	}
	fclose(file);
	printf("\nDoctor Data Saved To File.\n");
}

// 12. Add Receptionist Function(Admin Only)
void addReceptionist(){
	system("cls");
	if(receptionistCount >= MAX_RECEPTIONISTS){
		printf("\nCannot Add More Receptionists. Maximum Lmit Reached.\n");
		return;
	}
	
	struct Receptionist newReceptionist;
	int isUnique = 0, i;
	
	while (!isUnique) {
	printf("----------------------------");
	printf("\n=== Add New Receptionist ===\n");
	printf("----------------------------\n\n");
	printf("Enter Receptionist ID: ");
    scanf("%d", &newReceptionist.id);
    // Check if ID already exists
        isUnique = 1;  // Assume the ID is unique
        for (i = 0; i < receptionistCount; i++) {
        	if (receptionists[i].id == newReceptionist.id) {
                printf("ID Already Exists. Please Enter a Unique Receptionists ID.\n");
                isUnique = 0;  // Set to false and ask for a new ID
                break;
            }
        }
    }
        
	int validName = 0;
    while (!validName) {
        printf("Enter Receptionist Name: ");
        scanf(" %[^\n]", newReceptionist.name); // Read input with spaces

        validName = 1; // Assume the name is valid
        for (i = 0; newReceptionist.name[i] != '\0'; i++) {
            if (!isalpha(newReceptionist.name[i]) && newReceptionist.name[i] != ' ') {
                validName = 0; // Invalid character found
                printf("Name must contain only alphabetic characters and spaces.\n");
                break;
            }
        }
    }
    
	printf("Enter Receptionist User Name: ");
	scanf("%s", newReceptionist.username);
	printf("Enter Receptionist Password: ");
	scanf("%s", newReceptionist.password);
	
	receptionists[receptionistCount++] = newReceptionist;
	saveReceptionists();
	printf("\nReceptionists Added Successfully.\n");
}

// 13. Display Receptionist Function(Admin Only)
void displayReceptionists(){
	system("cls");
	if(receptionistCount == 0){
		printf("\nNo Receptionists To Display.\n");
		return;
	}
	printf("-----------------------------");
	printf("\n=== List Of Receptionists ===\n");
	printf("-----------------------------\n\n");
	
	int i;
	for(i=0; i<receptionistCount; i++){
		printf("Id: %d| Name: %s| Username: %s\n\n",
		receptionists[i].id,
		receptionists[i].name,
		receptionists[i].username); //Display Name And Username
	}
}

// 14. Edit Receptionist Function (Admin Only)
void editReceptionists() {
    system("cls");
    displayReceptionists();
    if (receptionistCount == 0) {
        printf("\nNo Receptionists To Edit.\n");
        return;
    }

    int id, found = 0, i;
    printf("---------------------------");
    printf("\n=== Edit Receptionist ===\n");
    printf("---------------------------\n\n");
    printf("Enter Receptionist ID To Edit: ");
    scanf("%d", &id);

    for (i = 0; i < receptionistCount; i++) {
        if (receptionists[i].id == id) {
            found = 1;
            printf("Editing Details For Receptionist ID %d\n", id);

            // Name validation: Only alphabetic characters and spaces allowed
            int validName = 0, j;
            while (!validName) {
                printf("Enter New Name: ");
                scanf(" %[^\n]", receptionists[i].name); // Read name with spaces
                validName = 1;
                for (j = 0; receptionists[i].name[j] != '\0'; j++) {
                    if (!isalpha(receptionists[i].name[j]) && receptionists[i].name[j] != ' ') {
                        validName = 0;
                        printf("Name must contain only alphabetic characters and spaces.\n");
                        break;
                    }
                }
            }
            
            //Username Input
            printf("Enter New Username: ");
            scanf("%s", receptionists[i].username);
                
            // Password input
            printf("Enter New Password: ");
            scanf("%s", receptionists[i].password);

            saveReceptionists();
            printf("\nReceptionist Details Updated Successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("\nReceptionist With ID %d Not Found.\n", id);
    }
}

// 15. Delete Receptionist Function(Admin Only)
void deleteReceptionist(){
	displayReceptionists();
	if(receptionistCount == 0){
		printf("\nNo receptionists To Delete.\n");
		return;
	}
	
	int id;
	printf("---------------------------");
	printf("\n=== Delete Receptionist ===\n");
	printf("---------------------------\n\n");
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
			printf("\nReceptionists Deleted Successfully.\n");
			break;
		}
	}
	
	if(!found){
		printf("\nReceptionist With ID %d Not Found.\n", id);
	}
}

// 16. Function To Save Receptionist Data
void saveReceptionists(){
	FILE *file = fopen(FILENAME_RECEPTIONISTS, "w");
	if(file == NULL){
		printf("\nError Opening File To Save Receptionists!\n");
		return;
	}
	
	int i;
	for(i=0; i<receptionistCount; i++){
		fprintf(file, "%d\n%s\n%s\n%s\n",
		receptionists[i].id,
		receptionists[i].name,
		receptionists[i].username,
		receptionists[i].password);
	}
	fclose(file);
	printf("\nReceptionist Data Saved To File.\n");
}

// 17. Login Function For Admin
int login(){
	system("cls");
	char username[20], password[20];
	printf("-------------------");
	printf("\n=== Admin Login ===\n");
	printf("-------------------\n\n");
	printf("Enter Admin Username: ");
	scanf("%s", username);
	printf("Enter Admin Password: ");
	scanf("%s", password);
	
	if(strcmp(username, adminUsername) == 0 && strcmp(password, adminPassword) == 0){
		printf("\nAdmin Login Successful!\n");
		return 1;
	}
	else{
		printf("\nInvalid Admin Credentials!\n");
		return 0;
	}
}

// 18. Login Function For Receptionist
int receptionistLogin() {
    system("cls");
    char username[20], password[20];
    int i;
    printf("--------------------------");
    printf("\n=== Receptionist Login ===\n");
    printf("--------------------------\n\n");
    printf("Enter Receptionist Username: ");
    scanf("%s", username);
    printf("Enter Receptionist Password: ");
    scanf("%s", password);
    
    // Loop through each receptionist to check credentials
    for (i = 0; i < receptionistCount; i++) {
        if (strcmp(receptionists[i].username, username) == 0 && strcmp(receptionists[i].password, password) == 0) {
            printf("\nReceptionist Login Successful!\n");
            return 1; // Login successful
        }
    }
    
    // If no match was found after the loop, credentials are invalid
    printf("\nInvalid Receptionist Credentials!\n");
    return 0; // Login failed
}

// 19. Display Patient Menu
void patientMenu(){
	system("cls");
	int choice;
	do{
		printf("--------------------");
		printf("\n=== Patient Menu ===\n");
		printf("--------------------\n\n");
		printf("1. Add Patient\n");
		printf("2. Display Patients\n");
		printf("3. Edit Patient\n");
		printf("4. Delete Patient\n");
		printf("5. Go Back\n\n");
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
				printf("\nReturning To Previous Menu...\n ");
				break;
			default:
				printf("\nInvalid Choice! Please Try Again.\n");
		}
	}while(choice != 5);			// Repeat until the user chooses to go back
}

// 20. Display Doctor Menu
void doctorMenu(){
	system("cls");
	int choice;
	do{
		printf("-------------------");
		printf("\n=== Doctor Menu ===\n");
		printf("-------------------\n\n");
        printf("1. Add Doctor\n");
        printf("2. Display Doctors\n");
        printf("3. Edit Doctor\n");
        printf("4. Delete Doctor\n");
        printf("5. Go Back\n\n");
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
                printf("\nReturning To Previous Menu...\n");
                break;
            default:
                printf("\nInvalid Choice! Please Try Again.\n");
        }
    } while (choice != 5);  		// Repeat until the user chooses to go back
}

// 21. Display Receptionist Menu For Manage Only Patients
void receptionistMenu() {
	system("cls");
    int choice;
    do {
    	printf("-------------------------");
        printf("\n=== Receptionist Menu ===\n");
        printf("-------------------------\n\n");
        printf("1. Add Patient\n");
        printf("2. Display Patients\n");
        printf("3. Edit Patient\n");
        printf("4. Delete Patient\n");
        printf("5. Logout\n\n");
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
                printf("\nReceptionist Successfully Logout...\n");
                break;
            default:
                printf("\nInvalid Choice! Please Try Again.\n");
        }
    } while (choice != 5);
}

// 22. Display Receptionist Menu Manages All Data
void adminMenu() {
	system("cls");
    int choice;
    do {
    	printf("------------------");
        printf("\n=== Admin Menu ===\n");
        printf("------------------\n\n");
        printf("1. Manage Patients\n");
        printf("2. Manage Doctors\n");
        printf("3. Manage Receptionists\n");
        printf("4. Logout\n\n");
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
                printf("\nAdmin Successfully Logout...\n");
                break;
            default:
                printf("\nInvalid Choice! Please Try Again.\n");
        }
    } while (choice != 4);
}

// 23. Display Receptionist Menu Manage By Admin
void manageReceptionists() {
    system("cls");
    int choice;
    do {
    	printf("----------------------------");
        printf("\n=== Manage Receptionists ===\n");
        printf("----------------------------\n\n");
        printf("1. Add Receptionist\n");
        printf("2. Display Receptionists\n");
        printf("3. Edit Receptionists\n");
        printf("4. Delete Receptionist\n");
        printf("5. Go Back\n\n");
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
                printf("\nReturning To Previous Menu...\n");
                break;
            default:
                printf("\nInvalid Choice! Please Try Again.\n");
        }
    } while (choice != 5);
}