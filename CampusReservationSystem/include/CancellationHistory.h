#ifndef CANCELLATION_HISTORY_H
#define CANCELLATION_HISTORY_H

#include "Reservation.h"

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
    CancellationHistory();
    ~CancellationHistory();

    CancellationHistory(const CancellationHistory&) = delete;
    CancellationHistory& operator=(const CancellationHistory&) = delete;

    void push(const Reservation& reservation);
    bool peek(Reservation& reservation) const;
    bool pop(Reservation& reservation);
    bool containsReservationID(int reservationID) const;
    void displayAll() const;
    int getSize() const;
    bool isEmpty() const;
};

#endif
