#ifndef RESERVATION_MANAGER_H
#define RESERVATION_MANAGER_H

#include "CancellationHistory.h"
#include "ReservationList.h"
#include "ResourceManager.h"
#include "WaitingList.h"
#include <string>

// Managing ReservationManager operations and maintaining their associated data.
class ReservationManager {
private:
    ResourceManager& resourceManager;
    ReservationList activeReservations;
    WaitingList waitingList;
    CancellationHistory cancellationHistory;

    // Promoting the earliest waiting request for the newly available resource.
    void assignNextWaitingUser(const std::string& resourceID);

public:
    // Handling reservation manager.
    explicit ReservationManager(ResourceManager& resourceManager);

    // Loading saved reservations while preventing conflicting active bookings and preserving waiting requests.
    bool loadReservationsFromFile(const std::string& filename);
    // Saving active and waiting reservations with explicit status values.
    bool saveReservationsToFile(const std::string& filename) const;

    void createReservation(int reservationID,
                           int studentID,
                           const std::string& studentName,
                           const std::string& resourceID,
                           const std::string& reservationDate);

    // Cancelling an active reservation, tracking its history, and promoting a waiting request.
    void cancelReservation(int reservationID);
    // Restoring the last cancelled reservation only when avoiding a conflicting booking.
    void undoLastCancellation();

    // Displaying active reservations.
    void displayActiveReservations() const;
    // Searching reservation by id.
    void searchReservationByID(int reservationID) const;
    // Searching reservations by student id.
    void searchReservationsByStudentID(int studentID) const;
    // Displaying waiting lists.
    void displayWaitingLists() const;
    // Displaying cancellation history.
    void displayCancellationHistory() const;
    // Generating report.
    void generateReport() const;

    // Preventing duplicate reservation identifiers across active, waiting, and cancellation records.
    bool reservationIDExists(int reservationID) const;
    // Checking both active and waiting requests before permitting resource removal.
    bool resourceHasReservations(const std::string& resourceID) const;
};

#endif
