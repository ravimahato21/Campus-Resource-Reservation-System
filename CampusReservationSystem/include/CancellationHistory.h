#ifndef CANCELLATION_HISTORY_H
#define CANCELLATION_HISTORY_H

#include "Reservation.h"

// Managing CancellationHistory operations and maintaining their associated data.
class CancellationHistory {
private:
    struct Node {
        Reservation data;
        Node* next;
        explicit Node(const Reservation& reservation)
            : data(reservation), next(nullptr) {}
    };

    Node* top;
    int size;

public:
    // Handling cancellation history.
    CancellationHistory();
    // Handling cancellation history.
    ~CancellationHistory();

    // Handling cancellation history.
    CancellationHistory(const CancellationHistory&) = delete;
    CancellationHistory& operator=(const CancellationHistory&) = delete;

    // Pushing a cancelled reservation onto the linked stack.
    void push(const Reservation& reservation);
    // Reading the latest cancelled reservation without removing it.
    bool peek(Reservation& reservation) const;
    // Removing the most recently cancelled reservation from the linked stack.
    bool pop(Reservation& reservation);
    // Checking reservation id.
    bool containsReservationID(int reservationID) const;
    // Displaying each entry stored in the current collection.
    void displayAll() const;
    // Counting the stored state.
    int getSize() const;
    // Checking empty.
    bool isEmpty() const;
};

#endif
