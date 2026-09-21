#ifndef RESERVATION_MANAGER_H
#define RESERVATION_MANAGER_H

#include "CancellationHistory.h"
#include "ReservationList.h"
#include "ResourceManager.h"
#include "WaitingList.h"
#include <string>

class ReservationManager {
private:
    ResourceManager& resourceManager;
    ReservationList activeReservations;
    WaitingList waitingList;
    CancellationHistory cancellationHistory;

    void assignNextWaitingUser(const std::string& resourceID);

public:
    explicit ReservationManager(ResourceManager& resourceManager);

    bool loadReservationsFromFile(const std::string& filename);
    bool saveReservationsToFile(const std::string& filename) const;

    void createReservation(int reservationID,
                           int studentID,
                           const std::string& studentName,
                           const std::string& resourceID,
                           const std::string& reservationDate);

    void cancelReservation(int reservationID);
    void undoLastCancellation();

    void displayActiveReservations() const;
    void searchReservationByID(int reservationID) const;
    void searchReservationsByStudentID(int studentID) const;
    void displayWaitingLists() const;
    void displayCancellationHistory() const;
    void generateReport() const;

    bool reservationIDExists(int reservationID) const;
};

#endif
