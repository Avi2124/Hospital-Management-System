#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 100
#define MAX_ROOMS 20

#define PATIENTS_FILE "patients.txt"
#define ROOMS_FILE "rooms.txt"

// Structure to hold patient information
struct Patient {
    int id;
    char name[50];
    int age;
    char ailment[100];
    int roomNumber; // room assigned to the patient (-1 if not assigned)
};

// Structure to hold room information
struct Room {
    int roomNumber;
    int isOccupied; // 0 if available, 1 if occupied
};

// Arrays to hold patients and rooms
struct Patient patients[MAX_PATIENTS];
struct Room rooms[MAX_ROOMS];
int patientCount = 0;
int roomCount = 0;

void addRoomByUser();  //1
void saveRoomsToFile();//2
void savePatientsToFile();
void loadPatientsFromFile();
void loadRoomsFromFile();//3
void listAvailableRooms();//4
void addPatient();
void deletePatient();
void editPatient();
void admitPatientToRoom();//5
void dischargePatient();//6
void displayPatients();

void saveRoomsToFile() {
    int i;
    FILE *file = fopen(ROOMS_FILE, "w");
    if (!file) {
        printf("Error saving rooms data.\n");
        return;
    }

    for (i = 0; i < roomCount; i++) {
        fprintf(file, "%d %d\n", rooms[i].roomNumber, rooms[i].isOccupied);
    }
    fclose(file);
}

void savePatientsToFile() {
    int i;
    FILE *file = fopen(PATIENTS_FILE, "w");
    if (!file) {
        printf("Error saving patients data.\n");
        return;
    }

    for (i = 0; i < patientCount; i++) {
        fprintf(file, "%d %s %d %s %d\n",
                patients[i].id,
                patients[i].name,
                patients[i].age,
                patients[i].ailment,
                patients[i].roomNumber);
    }
    fclose(file);
}

void loadRoomsFromFile() {
    int i;
    FILE *file = fopen(ROOMS_FILE, "r");
    if (!file) {
        printf("No room data file found. Creating an empty rooms.txt file.\n");
        // Create an empty rooms.txt file
        file = fopen(ROOMS_FILE, "w");
        if (file) {
            fclose(file);  // Close the empty file immediately after creation
        }
        return;  // Simply return without loading any rooms
    }

    roomCount = 0; // Reset room count before loading
    while (fscanf(file, "%d %d\n", &rooms[roomCount].roomNumber, &rooms[roomCount].isOccupied) != EOF) {
        roomCount++;
    }
    fclose(file);
    printf("Room data loaded successfully.\n");
}


void loadPatientsFromFile() {
    FILE *file = fopen(PATIENTS_FILE, "r");
    if (!file) {
        printf("No patient data file found. Starting fresh.\n");
        return;
    }

    patientCount = 0; // Reset patient count
    while (fscanf(file, "%d %s %d %s %d\n",
                  &patients[patientCount].id,
                  patients[patientCount].name,
                  &patients[patientCount].age,
                  patients[patientCount].ailment,
                  &patients[patientCount].roomNumber) != EOF) {
        patientCount++;
    }
    fclose(file);
}

void addRoomByUser() {
    int roomNumber, i;
    printf("Enter the room number you want to add: ");
    scanf("%d", &roomNumber);

    // Ensure the room number is not already taken
    for (i = 0; i < roomCount; i++) {
        if (rooms[i].roomNumber == roomNumber) {
            printf("Room number %d is already in use. Choose a different room number.\n", roomNumber);
            return;
        }
    }

    // Add the room
    struct Room newRoom;
    newRoom.roomNumber = roomNumber;
    newRoom.isOccupied = 0; // Room initially available
    rooms[roomCount++] = newRoom;

    saveRoomsToFile(); // Save the newly added room
    printf("Room number %d added successfully.\n", roomNumber);
}

void listAvailableRooms() {
    int i;
    printf("\nAvailable Rooms:\n");
    for (i = 0; i < roomCount; i++) {
        if (rooms[i].isOccupied == 0) {
            printf("Room %d is available.\n", rooms[i].roomNumber);
        }
    }
}

void addPatient() {
    struct Patient newPatient;
    newPatient.id = patientCount + 1;
    printf("Enter patient's name: ");
    scanf("%s", newPatient.name);
    printf("Enter patient's age: ");
    scanf("%d", &newPatient.age);
    printf("Enter patient's ailment: ");
    scanf("%s", newPatient.ailment);
    newPatient.roomNumber = -1; // Patient initially has no room assigned

    patients[patientCount++] = newPatient;
    printf("Patient added with ID: %d\n", newPatient.id);
}

void deletePatient() {
    int patientID, i, j;
    printf("Enter patient ID to delete: ");
    scanf("%d", &patientID);
    for (i = 0; i < patientCount; i++) {
        if (patients[i].id == patientID) {
            if (patients[i].roomNumber != -1) {
                // Free up the room if assigned
                for (j = 0; j < roomCount; j++) {
                    if (rooms[j].roomNumber == patients[i].roomNumber) {
                        rooms[j].isOccupied = 0;
                        break;
                    }
                }
            }
            // Shift all patients after the deleted one
            for (j = i; j < patientCount - 1; j++) {
                patients[j] = patients[j + 1];
            }
            patientCount--;
            printf("Patient with ID %d deleted.\n", patientID);
            return;
        }
    }
    printf("Patient ID not found.\n");
}

void editPatient() {
    int patientID, i;
    printf("Enter patient ID to edit: ");
    scanf("%d", &patientID);
    for (i = 0; i < patientCount; i++) {
        if (patients[i].id == patientID) {
            printf("Editing patient %s\n", patients[i].name);
            printf("Enter new name: ");
            scanf("%s", patients[i].name);
            printf("Enter new age: ");
            scanf("%d", &patients[i].age);
            printf("Enter new ailment: ");
            scanf("%s", patients[i].ailment);
            printf("Patient information updated.\n");
            return;
        }
    }
    printf("Patient ID not found.\n");
}

void admitPatientToRoom() {
    listAvailableRooms();
    int patientID, roomNumber;
    printf("Enter patient ID to admit: ");
    scanf("%d", &patientID);
    printf("Enter room number to assign: ");
    scanf("%d", &roomNumber);

    int found = 0, i, j;
    for (i = 0; i < roomCount; i++) {
        if (rooms[i].roomNumber == roomNumber) {
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Invalid room number. Please try again.\n");
        return;
    }

    for (i = 0; i < roomCount; i++) {
        if (rooms[i].roomNumber == roomNumber) {
            if (rooms[i].isOccupied == 1) {
                printf("Room is already occupied.\n");
                return;
            }
            for (j = 0; j < patientCount; j++) {
                if (patients[j].id == patientID) {
                    patients[j].roomNumber = roomNumber;
                    rooms[i].isOccupied = 1;
                    saveRoomsToFile(); // Save immediately
                    printf("Patient %s admitted to room %d.\n", patients[j].name, roomNumber);
                    return;
                }
            }
            printf("Patient ID not found.\n");
            return;
        }
    }
    printf("Room not found.\n");
}

void dischargePatient() {
    int patientID, i, j;
    printf("Enter patient ID to discharge: ");
    scanf("%d", &patientID);

    for (i = 0; i < patientCount; i++) {
        if (patients[i].id == patientID) {
            if (patients[i].roomNumber == -1) {
                printf("Patient is not currently admitted to any room.\n");
                return;
            }
            for (j = 0; j < roomCount; j++) {
                if (rooms[j].roomNumber == patients[i].roomNumber) {
                    rooms[j].isOccupied = 0;
                    saveRoomsToFile(); // Save immediately
                    printf("Patient %s discharged from room %d.\n", patients[i].name, rooms[j].roomNumber);
                    patients[i].roomNumber = -1;
                    return;
                }
            }
        }
    }
    printf("Patient ID not found.\n");
}

void displayPatients() {
    int i;
    printf("\nList of Patients:\n");
    for (i = 0; i < patientCount; i++) {
        printf("ID: %d, Name: %s, Age: %d, Ailment: %s, Room: %d\n",
                patients[i].id,
                patients[i].name,
                patients[i].age,
                patients[i].ailment,
                patients[i].roomNumber);
    }
}

int main() {
    loadPatientsFromFile();
    loadRoomsFromFile();

    int choice;
    while (1) {
        printf("\nHospital Management System\n");
        printf("1. Add Patient\n");
        printf("2. Admit Patient to Room\n");
        printf("3. Edit Patient\n");
        printf("4. Delete Patient\n");
        printf("5. Display Patients\n");
        printf("6. List Available Rooms\n");
        printf("7. Add Room\n");  // Option to add a room
        printf("8. Discharge Patient\n");
        printf("9. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPatient();
                savePatientsToFile(); // Save patients after adding
                break;
            case 2:
                admitPatientToRoom();
                savePatientsToFile(); // Save patients after admitting
                break;
            case 3:
                editPatient();
                savePatientsToFile(); // Save after edit
                break;
            case 4:
                deletePatient();
                savePatientsToFile(); // Save after deletion
                break;
            case 5:
                displayPatients();
                break;
            case 6:
                listAvailableRooms();
                break;
            case 7:
                addRoomByUser();  // Add room by user input
                break;
            case 8:
                dischargePatient();
                savePatientsToFile(); // Save after discharge
                break;
            case 9:
                printf("Exiting...\n");
                savePatientsToFile(); // Final save before exiting
                saveRoomsToFile(); // Save rooms data
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
