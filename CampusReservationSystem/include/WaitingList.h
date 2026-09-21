#ifndef WAITING_LIST_H
#define WAITING_LIST_H

#include "Reservation.h"
#include <string>

class WaitingList {
private:
    struct Node {
        Reservation request;
        Node* next;
        explicit Node(const Reservation& reservation)
            : request(reservation), next(nullptr) {}
    };

    Node* front;
    Node* rear;
    int size;

public:
    WaitingList();
    ~WaitingList();

    WaitingList(const WaitingList&) = delete;
    WaitingList& operator=(const WaitingList&) = delete;

    void enqueue(const Reservation& request);
    bool dequeueForResource(const std::string& resourceID, Reservation& nextRequest);
    bool containsReservationID(int reservationID) const;
    void displayAll() const;
    int getCount() const;
    int getCountForResource(const std::string& resourceID) const;
    bool isEmpty() const;

    template <typename Func>
    void forEach(Func func) const {
        Node* current = front;
        while (current != nullptr) {
            func(current->request);
            current = current->next;
        }
    }
};

#endif
