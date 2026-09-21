#include "ReservationManager.h"
#include "ResourceManager.h"
#include <iostream>
#include <limits>
#include <string>

using namespace std;

namespace {
int readPositiveInt(const string& prompt) {
    int value;

    while (true) {
        cout << prompt;
        if (cin >> value && value > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid input. Please enter a positive integer.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int readMenuChoice() {
    int choice;

    while (true) {
        cout << "Enter Choice: ";
        if (cin >> choice) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }

        cout << "Invalid input. Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readRequiredLine(const string& prompt) {
    string value;
    do {
        cout << prompt;
        getline(cin, value);
        if (value.empty()) {
            cout << "This field cannot be empty.\n";
        }
    } while (value.empty());
    return value;
}

void printMainMenu() {
    cout << "\n===== Campus Resource Reservation System =====\n"
         << "1. View Resources\n"
         << "2. Search Resource\n"
         << "3. Create Reservation\n"
         << "4. Cancel Reservation\n"
         << "5. View Active Reservations\n"
         << "6. View Waiting Lists\n"
         << "7. Undo Cancellation\n"
         << "8. Search Reservations\n"
         << "9. Sort Resources\n"
         << "10. Generate Report\n"
         << "11. View Cancellation History\n"
         << "12. Exit\n";
}
}

int main() {
    const string resourceFile = "data/resources.txt";
    const string reservationFile = "data/reservations.txt";
    const string reservationOutputFile = "data/reservations_output.txt";

    ResourceManager resourceManager;
    if (!resourceManager.loadFromFile(resourceFile)) {
        cout << "The program cannot continue without a valid resource file.\n";
        return 1;
    }

    ReservationManager reservationManager(resourceManager);
    reservationManager.loadReservationsFromFile(reservationFile);

    bool running = true;

    while (running) {
        printMainMenu();
        const int choice = readMenuChoice();

        switch (choice) {
            case 1: {
                resourceManager.displayAll();
                break;
            }

            case 2: {
                const string resourceID = readRequiredLine("Enter Resource ID: ");
                Resource* resource = resourceManager.searchByID(resourceID);
                if (resource == nullptr) {
                    cout << "Resource not found.\n";
                } else {
                    resource->display();
                }
                break;
            }

            case 3: {
                const int reservationID = readPositiveInt("Reservation ID: ");
                const int studentID = readPositiveInt("Student ID: ");
                const string studentName = readRequiredLine("Student Name: ");
                const string resourceID = readRequiredLine("Resource ID: ");
                const string reservationDate = readRequiredLine(
                    "Reservation Date (MM/DD/YYYY): ");

                reservationManager.createReservation(
                    reservationID, studentID, studentName,
                    resourceID, reservationDate);
                break;
            }

            case 4: {
                const int reservationID = readPositiveInt("Reservation ID to cancel: ");
                reservationManager.cancelReservation(reservationID);
                break;
            }

            case 5: {
                reservationManager.displayActiveReservations();
                break;
            }

            case 6: {
                reservationManager.displayWaitingLists();
                break;
            }

            case 7: {
                reservationManager.undoLastCancellation();
                break;
            }

            case 8: {
                cout << "1. Search by Reservation ID\n"
                     << "2. Search by Student ID\n";
                const int searchChoice = readMenuChoice();

                if (searchChoice == 1) {
                    const int reservationID = readPositiveInt("Reservation ID: ");
                    reservationManager.searchReservationByID(reservationID);
                } else if (searchChoice == 2) {
                    const int studentID = readPositiveInt("Student ID: ");
                    reservationManager.searchReservationsByStudentID(studentID);
                } else {
                    cout << "Invalid search selection.\n";
                }
                break;
            }

            case 9: {
                cout << "1. Sort Resources by Name (Quick Sort)\n"
                     << "2. Sort Resources by Popularity (Quick Sort)\n";
                const int sortChoice = readMenuChoice();

                if (sortChoice == 1) {
                    resourceManager.sortByName();
                    cout << "Resources sorted by name.\n";
                    resourceManager.displayAll();
                } else if (sortChoice == 2) {
                    resourceManager.sortByPopularity();
                    cout << "Resources sorted by popularity.\n";
                    resourceManager.displayAll();
                } else {
                    cout << "Invalid sort selection.\n";
                }
                break;
            }

            case 10: {
                reservationManager.generateReport();
                break;
            }

            case 11: {
                reservationManager.displayCancellationHistory();
                break;
            }

            case 12: {
                reservationManager.saveReservationsToFile(reservationOutputFile);
                cout << "Session reservations saved to " << reservationOutputFile << ". Goodbye!\n";
                running = false;
                break;
            }

            default:
                cout << "Invalid menu choice. Please select 1 through 12.\n";
        }
    }

    return 0;
}
