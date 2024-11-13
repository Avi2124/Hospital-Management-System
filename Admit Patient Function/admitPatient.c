#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 100
#define MAX_ROOMS 20

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

// Function to add a new patient
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

// Function to add a room
void addRoom(int roomNumber) {
    struct Room newRoom;
    newRoom.roomNumber = roomNumber;
    newRoom.isOccupied = 0; // Room initially available
    rooms[roomCount++] = newRoom;
}

// Function to delete a patient
void deletePatient() {
    int patientID;
    printf("Enter patient ID to delete: ");
    scanf("%d", &patientID);
    int i, j;
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

// Function to edit patient information
void editPatient() {
    int patientID;
    printf("Enter patient ID to edit: ");
    scanf("%d", &patientID);
    int i;
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

// Function to admit a patient to a room
void admitPatientToRoom() {
    int patientID, roomNumber, i, j;

    printf("Enter patient ID to admit: ");
    scanf("%d", &patientID);
    printf("Enter room number to assign: ");
    scanf("%d", &roomNumber);

    // Check if room is available
    for (i = 0; i < roomCount; i++) {
        if (rooms[i].roomNumber == roomNumber) {
            if (rooms[i].isOccupied == 1) {
                printf("Room is already occupied.\n");
                return;
            }
            // Assign room to patient
            for (j = 0; j < patientCount; j++) {
                if (patients[j].id == patientID) {
                    patients[j].roomNumber = roomNumber;
                    rooms[i].isOccupied = 1;
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

// Function to discharge a patient from a room
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
            // Find the room and mark it as unoccupied
            for (j = 0; j < roomCount; j++) {
                if (rooms[j].roomNumber == patients[i].roomNumber) {
                    rooms[j].isOccupied = 0;
                    printf("Patient %s discharged from room %d.\n", patients[i].name, rooms[j].roomNumber);
                    patients[i].roomNumber = -1; // Set patient room to -1 to show they are not admitted
                    return;
                }
            }
        }
    }
    printf("Patient ID not found.\n");
}

// Function to display all patients
void displayPatients() {
    int i;
    printf("\nList of Patients:\n");
    printf("ID\tName\t\tAge\tAilment\t\tRoom\n");
    printf("--------------------------------------------------\n");
    for (i = 0; i < patientCount; i++) {
        printf("%d\t%s\t\t%d\t%s\t\t%d\n",
               patients[i].id,
               patients[i].name,
               patients[i].age,
               patients[i].ailment,
               patients[i].roomNumber == -1 ? 0 : patients[i].roomNumber);
    }
}

// Function to list available rooms
void listAvailableRooms() {
    int i;
    printf("\nAvailable Rooms:\n");
    for (i = 0; i < roomCount; i++) {
        if (rooms[i].isOccupied == 0) {
            printf("Room %d is available.\n", rooms[i].roomNumber);
        }
    }
}

int main() {
    // Initialize some rooms
    int i;
    for (i = 1; i <= 5; i++) {
        addRoom(i); // Adds rooms numbered 1 to 5
    }

    // Main program loop
    int choice;
    while (1) {
        printf("\nHospital Management System\n");
        printf("1. Add Patient\n");
        printf("2. Admit Patient to Room\n");
        printf("3. Edit Patient\n");
        printf("4. Delete Patient\n");
        printf("5. Display Patients\n");
        printf("6. List Available Rooms\n");
        printf("7. Discharge Patient\n");
        printf("8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPatient();
                break;
            case 2:
                admitPatientToRoom();
                break;
            case 3:
                editPatient();
                break;
            case 4:
                deletePatient();
                break;
            case 5:
                displayPatients();
                break;
            case 6:
                listAvailableRooms();
                break;
            case 7:
                dischargePatient();
                break;
            case 8:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

