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

// Preventing reservation ID reuse while maintaining cancellation history.
bool ReservationManager::reservationIDExists(int reservationID) const {
    return activeReservations.containsID(reservationID) ||
           waitingList.containsReservationID(reservationID) ||
           cancellationHistory.containsReservationID(reservationID);
}

bool ReservationManager::resourceHasReservations(const string& resourceID) const {
    bool found = waitingList.getCountForResource(resourceID) > 0;
    activeReservations.forEach([&](const Reservation& r) {
        if (r.getResourceID() == resourceID) found = true;
    });
    return found;
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
               resourceID, reservationDate, status;
        stringstream ss(line);
        if (!getline(ss, reservationIDText, '|') ||
            !getline(ss, studentIDText, '|') ||
            !getline(ss, studentName, '|') ||
            !getline(ss, resourceID, '|') ||
            !getline(ss, reservationDate, '|')) {
            cerr << "Warning: Skipping invalid reservation line " << lineNumber << ".\n";
            continue;
        }
        getline(ss, status);
        status = trim(status);
        if (!status.empty() && status != "ACTIVE" && status != "WAITING") {
            cerr << "Warning: Unknown status on line " << lineNumber << ".\n";
            continue;
        }
        try {
            size_t n1 = 0, n2 = 0;
            const int reservationID = stoi(trim(reservationIDText), &n1);
            const int studentID = stoi(trim(studentIDText), &n2);
            if (n1 != trim(reservationIDText).size() || n2 != trim(studentIDText).size() ||
                reservationID <= 0 || studentID <= 0 ||
                trim(studentName).empty() || trim(resourceID).empty() ||
                trim(reservationDate).empty() || reservationIDExists(reservationID)) {
                cerr << "Warning: Invalid or duplicate reservation on line " << lineNumber << ".\n";
                continue;
            }
            Resource* resource = resourceManager.searchByID(trim(resourceID));
            if (!resource) {
                cerr << "Warning: Unknown resource on reservation line " << lineNumber << ".\n";
                continue;
            }
            Reservation reservation(reservationID, studentID, trim(studentName),
                                    trim(resourceID), trim(reservationDate));
            resource->incrementRequestCount();
            // Loading legacy records by assigning the first request and queuing later requests.
            // Preserving explicit WAITING records when reloading saved data.
            bool hasActive = false;
            activeReservations.forEach([&](const Reservation& existing) {
                if (existing.getResourceID() == trim(resourceID)) hasActive = true;
            });
            if (status == "WAITING" || hasActive) {
                waitingList.enqueue(reservation);
                if (hasActive && status == "ACTIVE")
                    cerr << "Warning: Conflicting ACTIVE record " << reservationID
                         << " moved to waiting list.\n";
            } else {
                activeReservations.add(reservation);
                resource->setAvailable(false);
                resource->incrementReservationCount();
            }
        } catch (const exception&) {
            cerr << "Warning: Invalid numeric value on line " << lineNumber << ".\n";
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
    file << "# reservationID|studentID|studentName|resourceID|reservationDate|status\n";
    auto write = [&file](const Reservation& r, const char* status) {
        file << r.getReservationID() << '|' << r.getStudentID() << '|'
             << r.getStudentName() << '|' << r.getResourceID() << '|'
             << r.getReservationDate() << '|' << status << '\n';
    };
    activeReservations.forEach([&](const Reservation& r) { write(r, "ACTIVE"); });
    waitingList.forEach([&](const Reservation& r) { write(r, "WAITING"); });
    return static_cast<bool>(file);
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

    // Recording cancellations in the LIFO history for undoing the latest cancellation.
    cancellationHistory.push(removed);

    Resource* resource = resourceManager.searchByID(removed.getResourceID());
    if (resource != nullptr) {
        resource->setAvailable(true);
    }

    cout << "Reservation cancelled successfully and added to cancellation history.\n";

    // Promoting the earliest matching waiting request after releasing a resource.
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

    // Preventing double-booking when restoring a cancelled reservation.
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

    // Restoring an existing reservation without counting another request.
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
