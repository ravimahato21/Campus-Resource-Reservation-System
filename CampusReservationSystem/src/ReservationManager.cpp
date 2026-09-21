#include "ReservationManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

namespace {
string trim(const string& value) {
    const size_t first = value.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    const size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}
}

ReservationManager::ReservationManager(ResourceManager& manager)
    : resourceManager(manager) {}

// Reservation IDs remain unique even after cancellation so an undo cannot
// conflict with a newly-created reservation that reused the same ID.
bool ReservationManager::reservationIDExists(int reservationID) const {
    return activeReservations.containsID(reservationID) ||
           waitingList.containsReservationID(reservationID) ||
           cancellationHistory.containsReservationID(reservationID);
}

bool ReservationManager::loadReservationsFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Warning: Could not open reservation file: " << filename
             << ". Starting with no reservations.\n";
        return false;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        ++lineNumber;
        if (trim(line).empty() || trim(line)[0] == '#') continue;

        string reservationIDText, studentIDText, studentName,
               resourceID, reservationDate;
        stringstream ss(line);

        if (!getline(ss, reservationIDText, '|') ||
            !getline(ss, studentIDText, '|') ||
            !getline(ss, studentName, '|') ||
            !getline(ss, resourceID, '|') ||
            !getline(ss, reservationDate)) {
            cerr << "Warning: Skipping invalid reservation line "
                 << lineNumber << ".\n";
            continue;
        }

        try {
            const int reservationID = stoi(trim(reservationIDText));
            const int studentID = stoi(trim(studentIDText));
            studentName = trim(studentName);
            resourceID = trim(resourceID);
            reservationDate = trim(reservationDate);

            if (reservationID <= 0 || studentID <= 0 || studentName.empty() ||
                resourceID.empty() || reservationDate.empty()) {
                cerr << "Warning: Incomplete/invalid reservation on line "
                     << lineNumber << " was skipped.\n";
                continue;
            }

            Resource* resource = resourceManager.searchByID(resourceID);
            if (resource == nullptr) {
                cerr << "Warning: Reservation " << reservationID
                     << " uses invalid resource ID and was skipped.\n";
                continue;
            }

            if (reservationIDExists(reservationID)) {
                cerr << "Warning: Duplicate reservation ID "
                     << reservationID << " skipped.\n";
                continue;
            }

            Reservation reservation(reservationID, studentID, studentName,
                                    resourceID, reservationDate);

            // The instructor-provided reservations file contains existing
            // reservation records. Load every valid record into the active
            // reservation linked list exactly as supplied. Resource availability
            // is kept from resources.txt instead of being reinterpreted here.
            activeReservations.add(reservation);
            resource->incrementRequestCount();
            resource->incrementReservationCount();
        } catch (...) {
            cerr << "Warning: Invalid numeric value on reservation line "
                 << lineNumber << ".\n";
        }
    }

    return true;
}

bool ReservationManager::saveReservationsToFile(const string& filename) const {
    ofstream file(filename);
    if (!file) {
        cerr << "Error: Could not write reservation file: " << filename << '\n';
        return false;
    }

    file << "# reservationID|studentID|studentName|resourceID|reservationDate\n";

    // Save active reservations first. On reload, the first request for each
    // resource becomes active again.
    activeReservations.forEach([&file](const Reservation& reservation) {
        file << reservation.getReservationID() << '|'
             << reservation.getStudentID() << '|'
             << reservation.getStudentName() << '|'
             << reservation.getResourceID() << '|'
             << reservation.getReservationDate() << '\n';
    });

    // Save waiting requests too so normal program exit does not lose the queue.
    waitingList.forEach([&file](const Reservation& reservation) {
        file << reservation.getReservationID() << '|'
             << reservation.getStudentID() << '|'
             << reservation.getStudentName() << '|'
             << reservation.getResourceID() << '|'
             << reservation.getReservationDate() << '\n';
    });

    return true;
}

void ReservationManager::createReservation(int reservationID,
                                           int studentID,
                                           const string& studentName,
                                           const string& resourceID,
                                           const string& reservationDate) {
    if (reservationID <= 0 || studentID <= 0 || trim(studentName).empty() ||
        trim(resourceID).empty() || trim(reservationDate).empty()) {
        cout << "Invalid input. All reservation fields are required, and IDs must be positive.\n";
        return;
    }

    if (studentName.find('|') != string::npos ||
        resourceID.find('|') != string::npos ||
        reservationDate.find('|') != string::npos) {
        cout << "Invalid input. The '|' character is reserved for the data-file format.\n";
        return;
    }

    if (reservationIDExists(reservationID)) {
        cout << "Error: Reservation ID " << reservationID
             << " already exists or is present in cancellation history.\n";
        return;
    }

    Resource* resource = resourceManager.searchByID(trim(resourceID));
    if (resource == nullptr) {
        cout << "Error: Resource ID " << resourceID << " does not exist.\n";
        return;
    }

    Reservation reservation(reservationID, studentID, trim(studentName),
                            trim(resourceID), trim(reservationDate));
    resource->incrementRequestCount();

    if (resource->isAvailable()) {
        activeReservations.add(reservation);
        resource->setAvailable(false);
        resource->incrementReservationCount();
        cout << "Reservation created successfully.\n";
    } else {
        waitingList.enqueue(reservation);
        cout << "Resource is currently unavailable. Request added to the waiting list.\n";
    }
}

void ReservationManager::assignNextWaitingUser(const string& resourceID) {
    Resource* resource = resourceManager.searchByID(resourceID);
    if (resource == nullptr || !resource->isAvailable()) {
        return;
    }

    Reservation nextRequest;
    if (waitingList.dequeueForResource(resourceID, nextRequest)) {
        activeReservations.add(nextRequest);
        resource->setAvailable(false);
        resource->incrementReservationCount();

        cout << "Resource " << resourceID
             << " automatically assigned to waiting student "
             << nextRequest.getStudentName()
             << " (Reservation ID " << nextRequest.getReservationID() << ").\n";
    }
}

void ReservationManager::cancelReservation(int reservationID) {
    Reservation removed;

    if (!activeReservations.removeByID(reservationID, removed)) {
        cout << "Reservation ID " << reservationID
             << " was not found among active reservations.\n";
        return;
    }

    // LIFO cancellation history supports undo of the most recent cancellation.
    cancellationHistory.push(removed);

    Resource* resource = resourceManager.searchByID(removed.getResourceID());
    if (resource != nullptr) {
        resource->setAvailable(true);
    }

    cout << "Reservation cancelled successfully and added to cancellation history.\n";

    // Required automatic FIFO assignment when the resource becomes free.
    assignNextWaitingUser(removed.getResourceID());
}

void ReservationManager::undoLastCancellation() {
    Reservation reservation;

    if (!cancellationHistory.peek(reservation)) {
        cout << "No cancelled reservations to restore.\n";
        return;
    }

    Resource* resource = resourceManager.searchByID(reservation.getResourceID());
    if (resource == nullptr) {
        cout << "Cannot restore reservation because its resource no longer exists.\n";
        return;
    }

    // If a waiting user was automatically assigned this resource after the
    // cancellation, restoring the old reservation would double-book it.
    if (!resource->isAvailable()) {
        cout << "Cannot restore the most recent cancellation because resource "
             << reservation.getResourceID()
             << " is currently unavailable.\n";
        return;
    }

    if (activeReservations.containsID(reservation.getReservationID())) {
        cout << "Cannot restore because the reservation ID already exists.\n";
        return;
    }

    Reservation popped;
    cancellationHistory.pop(popped);
    activeReservations.add(popped);
    resource->setAvailable(false);

    // Undo restores an existing reservation; it does not count as a new request.
    cout << "Reservation restored successfully.\n";
}

void ReservationManager::displayActiveReservations() const {
    activeReservations.displayAll();
}

void ReservationManager::searchReservationByID(int reservationID) const {
    const Reservation* reservation = activeReservations.searchByID(reservationID);
    if (reservation == nullptr) {
        cout << "Active reservation ID " << reservationID << " was not found.\n";
        return;
    }

    cout << "\nReservation found:\n";
    reservation->display();
}

void ReservationManager::searchReservationsByStudentID(int studentID) const {
    cout << "\n===== RESERVATIONS FOR STUDENT " << studentID << " =====\n";
    activeReservations.displayByStudentID(studentID);
}

void ReservationManager::displayWaitingLists() const {
    waitingList.displayAll();
}

void ReservationManager::displayCancellationHistory() const {
    cancellationHistory.displayAll();
}

void ReservationManager::generateReport() const {
    cout << "\n===== SYSTEM REPORT =====\n"
         << "Total Resources: " << resourceManager.getResourceCount() << '\n'
         << "Available Resources: " << resourceManager.getAvailableCount() << '\n'
         << "Unavailable Resources: " << resourceManager.getUnavailableCount() << '\n'
         << "Active Reservations: " << activeReservations.getSize() << '\n'
         << "Waiting List Requests: " << waitingList.getCount() << '\n'
         << "Cancellation History Size: " << cancellationHistory.getSize() << '\n';

    cout << "\n===== RESOURCE UTILIZATION / WAITING STATISTICS =====\n";
    for (const Resource& resource : resourceManager.getResources()) {
        cout << resource.getResourceID() << " - "
             << resource.getResourceName()
             << " | Status: " << (resource.isAvailable() ? "Available" : "Unavailable")
             << " | Reservations: " << resource.getReservationCount()
             << " | Requests: " << resource.getRequestCount()
             << " | Waiting: " << waitingList.getCountForResource(resource.getResourceID())
             << '\n';
    }

    const Resource* mostReserved = nullptr;
    const Resource* mostRequested = nullptr;
    for (const Resource& resource : resourceManager.getResources()) {
        if (mostReserved == nullptr ||
            resource.getReservationCount() > mostReserved->getReservationCount()) {
            mostReserved = &resource;
        }
        if (mostRequested == nullptr ||
            resource.getRequestCount() > mostRequested->getRequestCount()) {
            mostRequested = &resource;
        }
    }

    cout << "\nMost Frequently Reserved Resource: ";
    if (mostReserved == nullptr || mostReserved->getReservationCount() == 0) {
        cout << "No reservations yet.\n";
    } else {
        cout << mostReserved->getResourceName()
             << " (" << mostReserved->getResourceID() << ") - "
             << mostReserved->getReservationCount() << " reservation(s)\n";
    }

    cout << "Most Requested Resource: ";
    if (mostRequested == nullptr || mostRequested->getRequestCount() == 0) {
        cout << "No requests yet.\n";
    } else {
        cout << mostRequested->getResourceName()
             << " (" << mostRequested->getResourceID() << ") - "
             << mostRequested->getRequestCount() << " request(s)\n";
    }
}
